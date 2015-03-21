/*
Copyright 2015 Alex Frappier Lachapelle

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


SDPCompressionStreamBuf::SDPCompressionStreamBuf(std::shared_ptr<std::istream> compressedIn, std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm){

    hasFirstBlockBeenRead = false;

    inStream  = compressedIn;
    outStream = nullptr;

    if(compressionAlgorithm){
        currentCompressionAlgorithmInfo = makeCompressionAlgorithmInfo(compressionAlgorithm);
        currentCompressionAlgorithmInfo.compressionAlgorithm.get()->onInit();
        uncompressedBuffer.resize(currentCompressionAlgorithmInfo.bufferSizeWithOverhead);
        compressedBuffer.resize(currentCompressionAlgorithmInfo.bufferSize);
    }else{
        currentCompressionAlgorithmInfo.compressionAlgorithm = nullptr;
    }

    currentChunkNum = 0;

    nextChar = traits_type::eof();

}

SDPCompressionStreamBuf::SDPCompressionStreamBuf(std::shared_ptr<std::ostream> compressedOut, std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm){

    inStream  = nullptr;
    outStream = compressedOut;

    if(compressionAlgorithm){
        currentCompressionAlgorithmInfo = makeCompressionAlgorithmInfo(compressionAlgorithm);
        currentCompressionAlgorithmInfo.compressionAlgorithm.get()->onInit();
        uncompressedBuffer.reserve(currentCompressionAlgorithmInfo.bufferSize);
        compressedBuffer.reserve(currentCompressionAlgorithmInfo.bufferSizeWithOverhead);
    }else{
        currentCompressionAlgorithmInfo.compressionAlgorithm = nullptr;
    }

    currentChunkNum = 0;

}

SDPCompressionStreamBuf::~SDPCompressionStreamBuf(){
    if(currentCompressionAlgorithmInfo.compressionAlgorithm)
        currentCompressionAlgorithmInfo.compressionAlgorithm.get()->onExit();
}


void SDPCompressionStreamBuf::setCompressionAlgorithm(std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm, bool resetPosCounter){

    if(currentCompressionAlgorithmInfo.compressionAlgorithm != nullptr){
        currentCompressionAlgorithmInfo.compressionAlgorithm.get()->onSync();
        currentCompressionAlgorithmInfo.compressionAlgorithm.get()->onExit();
    }
    if(resetPosCounter){
        currentChunkNum = 0;
    }
    currentCompressionAlgorithmInfo = makeCompressionAlgorithmInfo(compressionAlgorithm);
    currentCompressionAlgorithmInfo.compressionAlgorithm.get()->onInit();

}

SDPCompressionStreamBuf::SDPCompressionAlgorithmInfoStruct SDPCompressionStreamBuf::getCompressionAlgorithmInfo(){
    return currentCompressionAlgorithmInfo;
}


SDPCompressionStreamBuf::int_type SDPCompressionStreamBuf::overflow(int_type ch){

    return setNextChar(ch);
}


SDPCompressionStreamBuf::int_type SDPCompressionStreamBuf::underflow(){

    return (int_type)getNextChar(false);
}

SDPCompressionStreamBuf::int_type SDPCompressionStreamBuf::uflow(){

    return (int_type)getNextChar(true);
}

SDPCompressionStreamBuf::int_type SDPCompressionStreamBuf::pbackfail(int_type ch){

    return traits_type::eof();
}


int SDPCompressionStreamBuf::sync(){

    //If there's anything left in the buffer, compress and write it.
    if(outStream && uncompressedBuffer.size() != 0){
        compressAndWriteNextChunk();
        currentCompressionAlgorithmInfo.compressionAlgorithm.get()->onSync();
    }

    outStream->flush();

    return 0;
}


SDPCompressionStreamBuf::SDPCompressionAlgorithmInfoStruct SDPCompressionStreamBuf::makeCompressionAlgorithmInfo(std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm){

    SDPCompressionAlgorithmInfoStruct compressionAlgorithmInfoStruct = {};
    compressionAlgorithmInfoStruct.compressionAlgorithm              = compressionAlgorithm;
    compressionAlgorithmInfoStruct.bufferSize                        = compressionAlgorithm.get()->getBufferSize();
    compressionAlgorithmInfoStruct.bufferSizeWithOverhead            = compressionAlgorithm.get()->getBufferSizeWithOverhead();

    return compressionAlgorithmInfoStruct;
}


SDPCompressionStreamBuf::int_type SDPCompressionStreamBuf::getNextChar(bool doAdvance){

    if(!doAdvance){
        return nextChar;
    }

    //fill the uncompressed buffer if we're at the end of it
    //or if the first block has not been read yet
    if(bufferIterator == uncompressedBuffer.end() || !hasFirstBlockBeenRead){

        if(!readAndDecompressNextChunk()){
            return traits_type::eof();
        }

        bufferIterator = uncompressedBuffer.begin();
        nextChar       = (*bufferIterator);
        //bufferIterator++;
        std::advance(bufferIterator, 1);

        hasFirstBlockBeenRead = true;

        return nextChar;

    }else{

        nextChar = *bufferIterator;
        //bufferIterator++;
        std::advance(bufferIterator, 1);

        return nextChar;
    }
}

SDPCompressionStreamBuf::int_type SDPCompressionStreamBuf::setNextChar(int_type ch){

    uncompressedBuffer.emplace_back(ch);

    if (uncompressedBuffer.size() == currentCompressionAlgorithmInfo.bufferSize){
        compressAndWriteNextChunk();
    }

    return traits_type::to_int_type(ch);
}


bool SDPCompressionStreamBuf::readAndDecompressNextChunk(){

    uint64 compressedDataSizeInFile;
    uint64 uncompressedDataSizeInFile;
    bool   isDataCompressed;

    uncompressedBuffer.clear();
    compressedBuffer.clear();

    //Read compressed data size.
    if(rawFileIO.read(compressedDataSizeInFile, inStream, RawFileIO::BIG__ENDIAN) != sizeof(compressedDataSizeInFile))
        return false;
    //Read uncompressed data size.
    if(rawFileIO.read(uncompressedDataSizeInFile, inStream, RawFileIO::BIG__ENDIAN) != sizeof(uncompressedDataSizeInFile))
        return false;

    //Read is data compressed tag.
    uint8 isDataCompressedTag;
    if(rawFileIO.read(isDataCompressedTag, inStream) != sizeof(isDataCompressed))
        return false;
    if(isDataCompressedTag == 0x00)
        isDataCompressed = false;
    else if(isDataCompressedTag == 0xFF)
        isDataCompressed = true;
    else
        return false; //error

    //Read compressed/uncompressed data.
    if(isDataCompressed){

        //read compressed data
        compressedBuffer.resize(compressedDataSizeInFile);
        inStream.get()->read((char*)compressedBuffer.data(), compressedDataSizeInFile);
        if(inStream.get()->gcount() != compressedDataSizeInFile)
            return false; //Assume eos/error.

        //Decompress
        uncompressedBuffer.resize(uncompressedDataSizeInFile);
        uint64 actualUncompressedDataSize;
        actualUncompressedDataSize = currentCompressionAlgorithmInfo.compressionAlgorithm.get()->decompressBuffer(compressedBuffer.data(), uncompressedBuffer.data(), compressedBuffer.size(), uncompressedDataSizeInFile, currentChunkNum);
        if(actualUncompressedDataSize != uncompressedDataSizeInFile)
            return false; //Uncompressed size marked in file not consistent with achtual size, error.

    }else{

        //read uncompressed data.
        uncompressedBuffer.resize(uncompressedDataSizeInFile);
        inStream.get()->read((char*)uncompressedBuffer.data(), uncompressedDataSizeInFile);
        if(inStream.get()->gcount() != uncompressedDataSizeInFile)
            return false;

    }

    currentChunkNum++;

    return true;
}

void SDPCompressionStreamBuf::compressAndWriteNextChunk(){

    uint64 compressedDataSize;
    uint64 uncompressedDataSize;

    compressedBuffer.clear();
    compressedBuffer.resize(currentCompressionAlgorithmInfo.bufferSizeWithOverhead);
    uncompressedDataSize = uncompressedBuffer.size();

    //Compress
    compressedDataSize = currentCompressionAlgorithmInfo.compressionAlgorithm.get()->compressBuffer(uncompressedBuffer.data(), compressedBuffer.data(), uncompressedDataSize, currentChunkNum);

    //Write compressed data size (value will be ignored if data is incompressible.)
    rawFileIO.write(compressedDataSize, outStream, RawFileIO::BIG__ENDIAN);
    //Write uncompressed data size.
    rawFileIO.write(uncompressedDataSize, outStream, RawFileIO::BIG__ENDIAN);

    //If the compressed data is of equal size or larger than the uncompressed data.
    //Assume incompressible chunk and write uncompressed data
    if(compressedDataSize >= uncompressedDataSize){

        //Write is data compressed tag (false).
        uint8 falseTag = 0x00;
        rawFileIO.write(falseTag, outStream);
        //Write uncompressed data.
        outStream.get()->write((char*)uncompressedBuffer.data(), uncompressedDataSize);

    }else{

        //Write is data compressed tag (true).
        uint8 falseTag = 0xFF;
        rawFileIO.write(falseTag, outStream);
        //Write compressed data.
        outStream.get()->write((char*)compressedBuffer.data(), compressedDataSize);

    }

    uncompressedBuffer.clear();
    currentChunkNum++;

}


















