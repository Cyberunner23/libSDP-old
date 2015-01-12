/*
Copyright 2014 Alex Frappier Lachapelle

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "SDPCompressionStreamBuf.hpp"


SDPCompressionStreamBuf::SDPCompressionStreamBuf(std::istream *compressedIn){

    hasFirstBlockBeenRead = false;

    inStream  = compressedIn;
    outStream = nullptr;

    this->compressionOptions.blockSize             = 1024*64;
    this->compressionOptions.blockSizeWithOverhead = 1024*64*1.5;//useless to set this here?
    this->compressionOptions.compressionAlgorithm  = &defaultCompressionAlgorithm;

    uncompressedBuffer.resize(this->compressionOptions.blockSize);
    compressedBuffer.resize(this->compressionOptions.blockSize);

    this->defaultCompressionAlgorithm.onInit();

    nextChar = traits_type::eof();

}

SDPCompressionStreamBuf::SDPCompressionStreamBuf(std::ostream *compressedOut){

    inStream  = nullptr;
    outStream = compressedOut;

    this->compressionOptions.blockSize             = 1024*64;
    this->compressionOptions.blockSizeWithOverhead = 1024*64*1.5;
    this->compressionOptions.compressionAlgorithm  = &defaultCompressionAlgorithm;

    uncompressedBuffer.reserve(this->compressionOptions.blockSize);
    compressedBuffer.reserve(this->compressionOptions.blockSize);

    this->defaultCompressionAlgorithm.onInit();

}

SDPCompressionStreamBuf::~SDPCompressionStreamBuf(){
    compressionOptions.compressionAlgorithm->onExit();
}


void SDPCompressionStreamBuf::SDPSetCompressionOptions(SDPCompressionOptions compressionOptions){

    this->compressionOptions = compressionOptions;
    this->compressionOptions.compressionAlgorithm->setBlockSize(this->compressionOptions.blockSize);

    this->compressionOptions.compressionAlgorithm->onInit();

}


SDPCompressionStreamBuf::int_type SDPCompressionStreamBuf::overflow(int_type ch){

    return setNextChar(ch);
}


SDPCompressionStreamBuf::int_type SDPCompressionStreamBuf::underflow(){

    return getNextChar(false);
}

SDPCompressionStreamBuf::int_type SDPCompressionStreamBuf::uflow(){

    return getNextChar(true);
}

SDPCompressionStreamBuf::int_type SDPCompressionStreamBuf::pbackfail(int_type ch){

    return traits_type::eof();
}


int SDPCompressionStreamBuf::sync(){

    //Compress whats left
    if(outStream && uncompressedBuffer.size() != 0){

        RawFileIO rawFileIO;

        compressedBuffer.clear();
        compressedBuffer.resize(this->compressionOptions.blockSizeWithOverhead);//add overhead comprensation

        uint_least64_t numCompressedBytes;
        numCompressedBytes = this->compressionOptions.compressionAlgorithm->compress((char*) uncompressedBuffer.data(), (char*) compressedBuffer.data(), uncompressedBuffer.size());

        this->compressionOptions.compressionAlgorithm->onSync();

        //check if we encountered an unompressible block.
        //if the num of compressed bytes is bigger or equal than the
        //than the input size (blockSize) then might as
        //well write the uncompressed block.
        if(numCompressedBytes >= this->compressionOptions.blockSize){
            //write block size
            rawFileIO.write(this->compressionOptions.blockSize, outStream, RawFileIO::BIG__ENDIAN);
            //Write is compressed tag (false)
            rawFileIO.write(0x00, outStream);
            //Write block data
            outStream->write((char*) uncompressedBuffer.data(), this->compressionOptions.blockSize);
        }else{
            //write compressed block size
            rawFileIO.write(numCompressedBytes, outStream, RawFileIO::BIG__ENDIAN);
            //write is compressed block (true)
            rawFileIO.write(0xFF, outStream);
            //Write block data
            outStream->write((char*) compressedBuffer.data(), numCompressedBytes);
        }
    }

    outStream->flush();


    return 0;
}

int SDPCompressionStreamBuf::getNextChar(bool doAdvance){

    if(!doAdvance){
        return nextChar;
    }

    //fill the uncompressed buffer if we're at the end of it
    //or if the first block has not been read yet
    if(bufferIterator == uncompressedBuffer.end() || !hasFirstBlockBeenRead){

        RawFileIO rawFileIO;

        unsigned int gCount;

        uncompressedBuffer.clear();
        compressedBuffer.clear();

        uint_least64_t numCompressedBytes;


        //Read compressed data size
        if(rawFileIO.read(numCompressedBytes, inStream, RawFileIO::BIG__ENDIAN) != sizeof numCompressedBytes ){
            return traits_type::eof();
        }

        if(numCompressedBytes > compressionOptions.blockSize){
            return traits_type::eof();
        }

        //Read is data compressed tag
        uint_least8_t isDataCompressedTag;
        if(rawFileIO.read(isDataCompressedTag, inStream) != sizeof isDataCompressedTag){
            return traits_type::eof();
        }else if (isDataCompressedTag != 0x00 && isDataCompressedTag != 0xFF){
            return traits_type::eof(); //put better error reporting...
        }

        if(isDataCompressedTag == 0xFF){ //if block is compressed

            compressedBuffer.resize(numCompressedBytes);
            inStream->read((char*) compressedBuffer.data(), numCompressedBytes);

            gCount = (int) inStream->gcount();

            //do some better error reporting...
            if(gCount != numCompressedBytes){
                return traits_type::eof();
            }

            //if we try to read and and it doesn't, assume we're @ EOF
            if(gCount == 0){
                return traits_type::eof();
            }

            uncompressedBuffer.resize(this->compressionOptions.blockSize);

            uint_least64_t numBytesUncompressed;
            numBytesUncompressed = this->compressionOptions.compressionAlgorithm->decompress((char*) compressedBuffer.data(), (char*) uncompressedBuffer.data(), compressedBuffer.size());

            uncompressedBuffer.resize(numBytesUncompressed);

        }else{ //if block is not compressed

            uncompressedBuffer.resize(this->compressionOptions.blockSize);
            inStream->read((char*) uncompressedBuffer.data(), this->compressionOptions.blockSize);

        }

        bufferIterator = uncompressedBuffer.begin();
        nextChar       = *bufferIterator;
        bufferIterator++;

        hasFirstBlockBeenRead = true;

        return nextChar;

    }else{

        nextChar = *bufferIterator;
        bufferIterator++;

        return nextChar;
    }
}

SDPCompressionStreamBuf::int_type SDPCompressionStreamBuf::setNextChar(int_type ch){

    uncompressedBuffer.emplace_back(ch);

    if (uncompressedBuffer.size() == this->compressionOptions.blockSize){

        RawFileIO rawFileIO;

        //compressedBuffer.clear();
        compressedBuffer.resize(this->compressionOptions.blockSizeWithOverhead);

        uint_least64_t numCompressedBytes;
        numCompressedBytes = this->compressionOptions.compressionAlgorithm->compress((char*) uncompressedBuffer.data(), (char*) compressedBuffer.data(), uncompressedBuffer.size());

        //check if we encountered an unompressible block.
        //if the num of compressed bytes is bigger or equal than the
        //than the input size (blockSize) then might as
        //well write the uncompressed block.
        if(numCompressedBytes >= this->compressionOptions.blockSize){
            //write block size
            rawFileIO.write(this->compressionOptions.blockSize, outStream, RawFileIO::BIG__ENDIAN);
            //Write is compressed tag (false)
            rawFileIO.write(0x00, outStream);
            //Write block data
            outStream->write((char*) uncompressedBuffer.data(), this->compressionOptions.blockSize);
        }else{
            //write compressed block size
            rawFileIO.write(numCompressedBytes, outStream, RawFileIO::BIG__ENDIAN);
            //write is compressed block (true)
            rawFileIO.write(0xFF, outStream);
            //Write block data
            outStream->write((char*) compressedBuffer.data(), numCompressedBytes);
        }

        uncompressedBuffer.clear();
        compressedBuffer.clear();

    }

    return traits_type::to_int_type(ch);
}






















