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

#include "SDPCryptStreamBuf.hpp"


SDPCryptStreamBuf::SDPCryptStreamBuf(std::istream *cryptIn, std::shared_ptr<SDPEncryptionAlgorithmBase> cryptAlgorithm)
        : hasFirstBlockBeenRead(false),
          inStream(cryptIn),
          outStream(nullptr),
          nextChar(traits_type::eof())
{

    currentEncryptionAlgorithmInfo = makeEncryptionAlgorithmInfo(cryptAlgorithm);
    currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->onInit();

    //Is this even needed?
    if(!currentEncryptionAlgorithmInfo.isStreamCipher){
        encryptedBuffer.resize(currentEncryptionAlgorithmInfo.bufferSize);
        unencryptedBuffer.resize(currentEncryptionAlgorithmInfo.bufferSize);
    }

}

SDPCryptStreamBuf::SDPCryptStreamBuf(std::ostream *cryptOut, std::shared_ptr<SDPEncryptionAlgorithmBase> cryptAlgorithm)
        : hasFirstBlockBeenRead(false),
          inStream(nullptr),
          outStream(cryptOut),
          nextChar(traits_type::eof())
{

    currentEncryptionAlgorithmInfo = makeEncryptionAlgorithmInfo(cryptAlgorithm);
    currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->onInit();

    //Is this even necessary?
    if(!currentEncryptionAlgorithmInfo.isStreamCipher){
        encryptedBuffer.resize(currentEncryptionAlgorithmInfo.bufferSize);
        unencryptedBuffer.resize(currentEncryptionAlgorithmInfo.bufferSize);
    }

}

SDPCryptStreamBuf::~SDPCryptStreamBuf(){
    currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->onExit();
}


void SDPCryptStreamBuf::setEncryptionAlgorithm(std::shared_ptr<SDPEncryptionAlgorithmBase> cryptAlgorithm){
    currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->onSync();
    currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->onExit();
    currentEncryptionAlgorithmInfo = makeEncryptionAlgorithmInfo(cryptAlgorithm);
    currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->onInit();
}


void SDPCryptStreamBuf::setEncryptionKeyAndNonce(std::string encryptionKey, bool isEncryptionKeyInHex, std::string nonce, bool isNonceInHex){
    currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->setEncryptionKeyAndNonce(encryptionKey, isEncryptionKeyInHex, nonce, isNonceInHex);
}

void SDPCryptStreamBuf::setEncryptionKey(std::string encryptionKey, bool isEncryptionKeyInHex){
    currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->setEncryptionKey(encryptionKey, isEncryptionKeyInHex);
}

void SDPCryptStreamBuf::setNonce(std::string nonce, bool isNonceInHex){
    currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->setNonce(nonce, isNonceInHex);
}


SDPCryptStreamBuf::int_type SDPCryptStreamBuf::overflow(int_type ch){

    return setNextChar(ch);
}


SDPCryptStreamBuf::int_type SDPCryptStreamBuf::underflow(){

    return (int_type)getNextChar(false);
}

SDPCryptStreamBuf::int_type SDPCryptStreamBuf::uflow(){

    return (int_type)getNextChar(true);
}

SDPCryptStreamBuf::int_type SDPCryptStreamBuf::pbackfail(int_type ch){

    return traits_type::eof();
}


int SDPCryptStreamBuf::sync(){

    //send whats left in the buffer to be encrypted
    if(!currentEncryptionAlgorithmInfo.isStreamCipher){

        if(outStream && unencryptedBuffer.size() != 0){
            encryptAndWriteNextChunk();
        }
    }

    currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->onSync();
    outStream->flush();

    return 0;
}


SDPCryptStreamBuf::SDPEncryptionAlgorithmInfoStruct SDPCryptStreamBuf::makeEncryptionAlgorithmInfo(std::shared_ptr<SDPEncryptionAlgorithmBase> encryptionAlgorithm){

    SDPEncryptionAlgorithmInfoStruct encryptionAlgorithmInfo = {};
    encryptionAlgorithmInfo.encryptionAlgorithm              = encryptionAlgorithm;
    encryptionAlgorithmInfo.encryptionAlgorithmID            = encryptionAlgorithm.get()->getEncryptionAlgorithmID();
    encryptionAlgorithmInfo.isStreamCipher                   = encryptionAlgorithm.get()->getIsStreamCipher();
    encryptionAlgorithmInfo.bufferSize                       = encryptionAlgorithm.get()->getBufferSize();
    encryptionAlgorithmInfo.bufferSizeWithOverhead           = encryptionAlgorithm.get()->getBufferSizeWithOverhead();

    return encryptionAlgorithmInfo;
}


SDPCryptStreamBuf::int_type SDPCryptStreamBuf::getNextChar(bool doAdvance){

    //if nextChar was not reset (in underflow), return the buffered char.
    if(!doAdvance){
        return nextChar;
    }

    if(currentEncryptionAlgorithmInfo.isStreamCipher){

        uchar readByte;
        if(rawFileIO.read(readByte, inStream) != sizeof (readByte)){
            return traits_type::eof();
        }

        if(currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->decryptStream(readByte, &nextChar, currentCharNum)){
            currentCharNum++;
            return nextChar;
        }else{
            return traits_type::eof();
        }

    }else{

        if(bufferIterator == unencryptedBuffer.end() || !hasFirstBlockBeenRead){

            if(!readAndDecompressNextChunk()){
                return traits_type::eof();
            }

            currentChunkNum++;

            bufferIterator = unencryptedBuffer.begin();
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
}

SDPCryptStreamBuf::int_type SDPCryptStreamBuf::setNextChar(int_type ch){

    RawFileIO rawFileIO;

    if(currentEncryptionAlgorithmInfo.isStreamCipher){

        uchar writeByte = 0;
        if(!currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->encryptStream((uchar)ch, &writeByte, currentCharNum))
            return traits_type::eof();
        rawFileIO.write(writeByte, outStream);
        currentCharNum++;

    }else{

        unencryptedBuffer.emplace_back(ch);

        if(unencryptedBuffer.size() == currentEncryptionAlgorithmInfo.bufferSize){
            encryptAndWriteNextChunk();
        }
    }

    return traits_type::to_int_type(ch);
}


bool SDPCryptStreamBuf::readAndDecompressNextChunk(){

    uint64 encryptedDataSizeInFile;
    uint64 unencryptedDataSizeInFile;

    unencryptedBuffer.clear();
    encryptedBuffer.clear();

    //Read encrypted data size
    if(rawFileIO.read(encryptedDataSizeInFile, inStream, RawFileIO::BIG__ENDIAN) != sizeof(encryptedDataSizeInFile))
        return false;
    if(rawFileIO.read(unencryptedDataSizeInFile, inStream, RawFileIO::BIG__ENDIAN) != sizeof(unencryptedDataSizeInFile))
        return false;

    //Read encrypted data.
    encryptedBuffer.resize(encryptedDataSizeInFile);
    inStream.get()->read((char*)encryptedBuffer.data(), encryptedDataSizeInFile);
    if(inStream.get()->gcount() != encryptedDataSizeInFile)
        return false;

    //do decryption
    unencryptedBuffer.resize(unencryptedDataSizeInFile);
    uint64 numDecryptedBytes = currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->decrypt(encryptedBuffer.data(), unencryptedBuffer.data(), encryptedBuffer.size(), currentChunkNum);
    unencryptedBuffer.resize(numDecryptedBytes);
    currentChunkNum++;

    return true;
}

void SDPCryptStreamBuf::encryptAndWriteNextChunk(){

    uint64 encryptedDataSize;
    uint64 unencryptedDataSize;

    encryptedBuffer.clear();
    encryptedBuffer.resize(currentEncryptionAlgorithmInfo.bufferSizeWithOverhead);
    unencryptedDataSize = unencryptedBuffer.size();

    //Encrypt
    encryptedDataSize = currentEncryptionAlgorithmInfo.encryptionAlgorithm.get()->encrypt(unencryptedBuffer.data(), encryptedBuffer.data(), unencryptedBuffer.size(), currentChunkNum);

    //Write encrypted data size
    rawFileIO.write(encryptedDataSize, outStream, RawFileIO::BIG__ENDIAN);
    //Write unencrypted data size
    rawFileIO.write(unencryptedDataSize, outStream, RawFileIO::BIG__ENDIAN);
    //Write encrypted data
    outStream.get()->write((char*)encryptedBuffer.data(), encryptedDataSize);

    unencryptedBuffer.clear();
    currentChunkNum++;

}
















