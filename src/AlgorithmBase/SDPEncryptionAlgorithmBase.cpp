#include "SDPEncryptionAlgorithmBase.hpp"

SDPEncryptionAlgorithmBase::SDPEncryptionAlgorithmBase(){

}

SDPEncryptionAlgorithmBase::~SDPEncryptionAlgorithmBase(){

}


void SDPEncryptionAlgorithmBase::setEncryptionKeyAndNonce(std::string encryptionKey, bool isEncryptionKeyInHex, std::string nonce, bool isNonceInHex){

	HexBinTool hexBinTool;

	if(isEncryptionKeyInHex){
		hexBinTool.hexToBin(encryptionKey, encryptionKeyInBin);
	}else{
        encryptionKeyInBin = encryptionKey;
    }

    if(isNonceInHex){
		hexBinTool.hexToBin(nonce, nonceInBin);
    }else{
        nonceInBin = nonce;
    }
}

void SDPEncryptionAlgorithmBase::setEncryptionKey(std::string encryptionKey, bool isEncryptionKeyInHex){

	HexBinTool hexBinTool;

	if(isEncryptionKeyInHex){
		hexBinTool.hexToBin(encryptionKey, encryptionKeyInBin);
	}else{
		encryptionKeyInBin = encryptionKey;
}
}

void SDPEncryptionAlgorithmBase::setNonce(std::string nonce, bool isNonceInHex){

	HexBinTool hexBinTool;

	if(isNonceInHex){
		hexBinTool.hexToBin(nonce, nonceInBin);
	}else{
		nonceInBin = nonce;
	}
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


void SDPEncryptionAlgorithmBase::setIsStreamCipher(bool isStreamCipher){
	this->isStreamCipher = isStreamCipher;
}

void SDPEncryptionAlgorithmBase::setMaxBufferSize(uint_least64_t maxBufferSize){
	this->maxBufferSize = maxBufferSize;
}

bool SDPEncryptionAlgorithmBase::setPreferedBufferSize(uint_least64_t preferedBufferSize){

	if(preferedBufferSize <= maxBufferSize){
		bufferSize = preferedBufferSize;
		return true;
	}

	return false;
}

void SDPEncryptionAlgorithmBase::setPreferedBufferSizeWithOverhead(uint_least64_t preferedBufferSizeWithOverhead){
	bufferSizeWithOverhead = preferedBufferSizeWithOverhead;
}