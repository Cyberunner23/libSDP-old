#include "SDPCompressionAlgorithmBase.hpp"

SDPCompressionAlgorithmBase::SDPCompressionAlgorithmBase(){

}

SDPCompressionAlgorithmBase::~SDPCompressionAlgorithmBase(){

}


bool SDPCompressionAlgorithmBase::getIsStreamCompression(){
	return isStreamCompression;
}

uint_least64_t SDPCompressionAlgorithmBase::getBufferSize(){
	return bufferSize;
}

uint_least64_t SDPCompressionAlgorithmBase::getBufferSizeWithOverhead(){
	return bufferSizeWithOverhead;
}


void SDPCompressionAlgorithmBase::setIsStreamCompression(bool isStreamCompression){
	this->isStreamCompression = isStreamCompression;
}

void SDPCompressionAlgorithmBase::setMaxBufferSize(uint_least64_t maxBufferSize){
	this->maxBufferSize = maxBufferSize;
}

bool SDPCompressionAlgorithmBase::setPreferedBufferSize(uint_least64_t preferedBufferSize){

	if(preferedBufferSize <= maxBufferSize){
		bufferSize = preferedBufferSize;
		return true;
	}

	return false;
}

void SDPCompressionAlgorithmBase::setPreferedBufferSizeWithOverhead(uint_least64_t preferedBufferSizeWithOverhead){
	this->bufferSizeWithOverhead = preferedBufferSizeWithOverhead;
}




