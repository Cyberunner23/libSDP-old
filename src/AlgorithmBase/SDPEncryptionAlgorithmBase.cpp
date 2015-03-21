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

#include "SDPEncryptionAlgorithmBase.hpp"

SDPEncryptionAlgorithmBase::SDPEncryptionAlgorithmBase(){

}

SDPEncryptionAlgorithmBase::~SDPEncryptionAlgorithmBase(){

}


int SDPEncryptionAlgorithmBase::setEncryptionKeyAndNonce(std::string encryptionKey, bool isEncryptionKeyInHex, std::string nonce, bool isNonceInHex){

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

int SDPEncryptionAlgorithmBase::setEncryptionKey(std::string encryptionKey, bool isEncryptionKeyInHex){

    HexBinTool hexBinTool;

    if(isEncryptionKeyInHex){
        hexBinTool.hexToBin(encryptionKey, encryptionKeyInBin);
    }else{
        encryptionKeyInBin = encryptionKey;
    }

    return encryptionKeyInBin.size() != encryptionKeyWidthInBits/8 ? -1 : 0;
}

int SDPEncryptionAlgorithmBase::setNonce(std::string nonce, bool isNonceInHex){

    HexBinTool hexBinTool;

    if(isNonceInHex){
        hexBinTool.hexToBin(nonce, nonceInBin);
    }else{
        nonceInBin = nonce;
    }

    return nonceInBin.size() != nonceWidthInBits/8 ? -1 : 0;
}


uint8 SDPEncryptionAlgorithmBase::getEncryptionAlgorithmID(){
    return encryptionAlgorithmID;
}

bool SDPEncryptionAlgorithmBase::getIsStreamCipher(){
    return isStreamCipher;
}


uint_least64_t SDPEncryptionAlgorithmBase::getBufferSize(){
    return bufferSize;
}

uint_least64_t SDPEncryptionAlgorithmBase::getBufferSizeWithOverhead(){
    return bufferSizeWithOverhead;
}


void SDPEncryptionAlgorithmBase::setEncryptionAlgorithmID(uint8 algorithmID){
    encryptionAlgorithmID = algorithmID;
}

void SDPEncryptionAlgorithmBase::setIsStreamCipher(bool isStreamCipher){
    this->isStreamCipher = isStreamCipher;
}

void SDPEncryptionAlgorithmBase::setEncryptionKeyWidthInBits(unsigned int encryptionKeyWidthInBits){
    this->encryptionKeyWidthInBits = encryptionKeyWidthInBits;
}

void SDPEncryptionAlgorithmBase::setNonceWidthInBits(unsigned int nonceWidthInBits){
    this->nonceWidthInBits = nonceWidthInBits;
}

void SDPEncryptionAlgorithmBase::setMaxBufferSize(uint64 maxBufferSize){
    this->maxBufferSize = maxBufferSize;
}

bool SDPEncryptionAlgorithmBase::setPreferedBufferSize(uint64 preferedBufferSize){

    if(preferedBufferSize <= maxBufferSize){
        bufferSize = preferedBufferSize;
        return true;
    }

    return false;
}

bool SDPEncryptionAlgorithmBase::setPreferedBufferSizeWithOverhead(uint64 preferedBufferSizeWithOverhead){

    if(preferedBufferSizeWithOverhead <= maxBufferSize){
        bufferSizeWithOverhead = preferedBufferSizeWithOverhead;
        return true;
    }

    return false;
}

