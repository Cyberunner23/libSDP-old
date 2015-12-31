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

#include "SDP.hpp"

using namespace libSDP;

// metadata
// all contained in SDP
// bootstrap file spec
// SDP file;
// file.fromFilePath(string);
// file fromCharVector(vec);
// file.fromCustomSourceSink(std::streambuf);

SDP::SDP(){}
SDP::~SDP(){}


SDPErrEnum SDP::openSDP(std::string &SDPFileName){

    STUB_FUNC(__FILE, __LINE__)
    //Setup default Source/Sink
    //Get Spec version.
    //Set Spec class.
    //create stack with empty algorithm chain.
    //parse headers.
    //return status;
}

SDPErrEnum SDP::openSDP(std::shared_ptr<SDPSourceSinkBase> inOutStream){

    STUB_FUNC(__FILE__, __LINE__)
    //Get Spec verison.
    //Set Spec class.
    //create stack with empty algorithm chain.
    //parse headers.
    //return status;
}


std::shared_ptr<SDPStack> setStack(std::shared_ptr<SDPStack> stack){
STUB_FUNC(__FILE__, __LINE__)
}

void setAlgorithmChain(std::shared_ptr<SDPChainBlockBase> algorithmChain){
STUB_FUNC(__FILE__, __LINE__)
}

std::shared_ptr<SDPSourceSinkBase> getSourceSink(){
STUB_FUNC(__FILE__, __LINE__)
}



SDPFileInfoStruct* SDP::getSDPFileInfo(){
    STUB_FUNC(__FILE__, __LINE__)
}

SDPSubContainerInfoStruct* SDP::getCurrentSubContainerInfo(){
    STUB_FUNC(__FILE__, __LINE__)
}

SDPSubContainerInfoStruct* SDP::getSubContainerInfo(std::string &subContainerFileName){
    STUB_FUNC(__FILE__, __LINE__)
}


void SDP::createSDP(std::string &SDPFileName, const std::vector<uchar> &extraField){
    STUB_FUNC(__FILE__, __LINE__)
}

void SDP::createSDP(std::shared_ptr<SDPSourceSinkBase> inOutStream, const std::vector<uchar> &extraField){
    STUB_FUNC(__FILE__, __LINE__)
}


void SDP::addSubContainerToSDP(std::string &SubContainerFileName, std::shared_ptr<std::iostream> algorithm, const std::vector<uchar> &extraField){
    STUB_FUNC(__FILE__, __LINE__)
}

void SDP::finalizeSubContainer(){
    STUB_FUNC(__FILE__, __LINE__)
}

void SDP::removeSubContainerFromSDP(std::string &subContainerName){
    STUB_FUNC(__FILE__, __LINE__)
}


bool SDP::setSDPSubContainer(std::string &subContainerFileName, std::shared_ptr<std::iostream> algorithm){
    STUB_FUNC(__FILE__, __LINE__)
}


bool SDP::read(uchar *data, uint64 readSize){
    STUB_FUNC(__FILE__, __LINE__)
}

SDP& SDP::operator>>(uchar &c){
    STUB_FUNC(__FILE__, __LINE__)
}

uint64 SDP::getReadCount(){
    STUB_FUNC(__FILE__, __LINE__)
}


bool SDP::write(uchar *data, uint64 writeSize){
    STUB_FUNC(__FILE__, __LINE__)
}

SDP& SDP::operator<<(uchar &c){
    STUB_FUNC(__FILE__, __LINE__)
}

uint64 SDP::getWriteCount(){
    STUB_FUNC(__FILE__, __LINE__)
}




