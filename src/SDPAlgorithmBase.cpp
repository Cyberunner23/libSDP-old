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

#include "SDPAlgorithmBase.hpp"

SDPAlgorithmBase::SDPAlgorithmBase(){

}

SDPAlgorithmBase::~SDPAlgorithmBase(){

}


uint64 SDPAlgorithmBase::processBuffer(uchar *unprocessedBuffer, uint64 unprocessedBufferSize, uchar *processedBuffer, uint64 expectedProcessedBufferSize, uint64 chunkNum){}
uint64 SDPAlgorithmBase::unprocessBuffer(uchar *processedBuffer, uint64 processedBufferSize, uchar *unprocessedBuffer, uint64 expectedUnprocessedBufferSize, uint64 chunkNum){}

bool SDPAlgorithmBase::processChar(uchar &unprocessedChar, uchar &processedChar, uint64 charNum){}
bool SDPAlgorithmBase::unprocessChar(uchar &processedChar, uchar &deprocessedChar, uint64 charNum){}


SDPAlgorithmBase::AlgorithmType SDPAlgorithmBase::getAlgorithmType(){
    return algorithmType;
}

uint8 SDPAlgorithmBase::getAlgorithmID(){
    return algorithmID;
}

bool SDPAlgorithmBase::getIsStreamAlgorithm(){
    return isStreamAlgorithm;
}


uint64 SDPAlgorithmBase::getBufferSize(){
    return bufferSize;
}

uint64 SDPAlgorithmBase::getBufferSizeWithOverhead(){
    return bufferSizeWithOverhead;
}


int SDPAlgorithmBase::setEncryptionKeyAndNonce(std::string encryptionKey, bool isEncryptionKeyInHex, std::string nonce, bool isNonceInHex){

    HexBinTool hexBinTool;

    if(isEncryptionKeyInHex){
        hexBinTool.hexToBin(encryptionKey, encryptionKeyInBin);
    }else{
        encryptionKeyInBin = encryptionKey;
    }

    if(encryptionKeyInBin.size() != encryptionKeyWidthInBits/8){
        return -1;
    }

    if(isNonceInHex){
        hexBinTool.hexToBin(nonce, nonceInBin);
    }else{
        nonceInBin = nonce;
    }

    if(nonceInBin.size() != nonceWidthInBits/8){
        return -2;
    }

    return 0;
}

int SDPAlgorithmBase::setEncryptionKey(std::string encryptionKey, bool isEncryptionKeyInHex){

    HexBinTool hexBinTool;

    if(isEncryptionKeyInHex){
        hexBinTool.hexToBin(encryptionKey, encryptionKeyInBin);
    }else{
        encryptionKeyInBin = encryptionKey;
    }

    return encryptionKeyInBin.size() != encryptionKeyWidthInBits/8 ? -1 : 0;
}

int SDPAlgorithmBase::setNonce(std::string nonce, bool isNonceInHex){

    HexBinTool hexBinTool;

    if(isNonceInHex){
        hexBinTool.hexToBin(nonce, nonceInBin);
    }else{
        nonceInBin = nonce;
    }

    return nonceInBin.size() != nonceWidthInBits/8 ? -1 : 0;
}


void SDPAlgorithmBase::setAlgorithmType(AlgorithmType algorithmType){
    this->algorithmType= algorithmType;
}

void SDPAlgorithmBase::setAlgorithmID(uint8 algorithmID){
    this->algorithmID = algorithmID;
}

void SDPAlgorithmBase::setIsStreamAlgorithm(bool isStreamAlgorithm){
    this->isStreamAlgorithm = isStreamAlgorithm;
}


void SDPAlgorithmBase::setMaxBufferSize(uint64 maxBufferSize){
    this->maxBufferSize = maxBufferSize;
}

bool SDPAlgorithmBase::setPreferedBufferSize(uint64 preferedBufferSize){

    if(preferedBufferSize <= maxBufferSize){
        bufferSize = preferedBufferSize;
        return true;
    }

    return false;
}

bool SDPAlgorithmBase::setPreferedBufferSizeWithOverhead(uint64 preferedBufferSizeWithOverhead){

    if(preferedBufferSizeWithOverhead <= maxBufferSize){
        bufferSizeWithOverhead = preferedBufferSizeWithOverhead;
        return true;
    }

    return false;
}


void SDPAlgorithmBase::setCompressionLevel(uint64 compressionLevel){
    this->compresseionLevel = compressionLevel;
}


void SDPAlgorithmBase::setEncryptionKeyWidthInBits(uint64 encryptionKeyWidthInBits){
    this->encryptionKeyWidthInBits = encryptionKeyWidthInBits;
}

void SDPAlgorithmBase::setNonceWidthInBits(uint64 nonceWidthInBits){
    this->nonceWidthInBits = nonceWidthInBits;
}