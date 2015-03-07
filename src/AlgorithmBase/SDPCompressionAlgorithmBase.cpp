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

#include "SDPCompressionAlgorithmBase.hpp"

SDPCompressionAlgorithmBase::SDPCompressionAlgorithmBase(){

}

SDPCompressionAlgorithmBase::~SDPCompressionAlgorithmBase(){

}


bool SDPCompressionAlgorithmBase::getIsStreamCompression(){
	return isStreamCompression;
}

uint64 SDPCompressionAlgorithmBase::getBufferSize(){
	return bufferSize;
}

uint64 SDPCompressionAlgorithmBase::getBufferSizeWithOverhead(){
	return bufferSizeWithOverhead;
}


void SDPCompressionAlgorithmBase::setIsStreamCompression(bool isStreamCompression){
	this->isStreamCompression = isStreamCompression;
}

void SDPCompressionAlgorithmBase::setMaxBufferSize(uint64 maxBufferSize){
	this->maxBufferSize = maxBufferSize;
}

bool SDPCompressionAlgorithmBase::setPreferedBufferSize(uint64 preferedBufferSize){

	if(preferedBufferSize <= maxBufferSize){
		bufferSize = preferedBufferSize;
		return true;
	}

	return false;
}

void SDPCompressionAlgorithmBase::setPreferedBufferSizeWithOverhead(uint64 preferedBufferSizeWithOverhead){
	this->bufferSizeWithOverhead = preferedBufferSizeWithOverhead;
}




