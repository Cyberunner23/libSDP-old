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


SDP::SDP(){}
SDP::~SDP(){}


SDPErrEnum SDP::openSDP(std::string &SDPFileName){

    std::shared_ptr<std::fstream> tmpStream = std::make_shared<std::fstream>();
    tmpStream.get()->open(SDPFileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary);
    if(!tmpStream.get()->is_open())
        return SDPErrEnum::CANNOT_OPEN_FILE;

    SDPInOutStream = tmpStream;
    return parser.parseSDPFile(SDPInOutStream, SDPFileInfo);
}

SDPErrEnum SDP::openSDP(std::shared_ptr<std::iostream> inOutStream){
    SDPInOutStream = inOutStream;
    return parser.parseSDPFile(SDPInOutStream, SDPFileInfo);
}


SDPParser::SDPFileInfoStruct* SDP::getSDPFileInfo(){
    return &SDPFileInfo;
}

SDPParser::SDPSubContainerInfoStruct* SDP::getCurrentSubContainerInfo(){
    return &SDPFileInfo.currentSubContainerInUse;
}

SDPParser::SDPSubContainerInfoStruct* SDP::getSubContainerInfo(std::string &subContainerFileName){

    if(SDPFileInfo.subContainersInSDPFile.find(subContainerFileName) == SDPFileInfo.subContainersInSDPFile.end())
        return nullptr;
    return &SDPFileInfo.subContainersInSDPFile[subContainerFileName];
}


void SDP::createSDP(std::string &SDPFileName, const std::vector<uchar> &extraField){

}

void SDP::createSDP(std::shared_ptr<std::iostream> inOutStream, const std::vector<uchar> &extraField){

}


void SDP::addSubContainerToSDP(std::string &SubContainerFileName, compressionAlgorithmsEnum compAlg, encryptionAlgorithmsEnum encAlg, const std::vector<uchar> &extraField){

}

void SDP::addSubContainerToSDP(std::string &SubContainerFileName, std::shared_ptr<std::iostream> algorithm, const std::vector<uchar> &extraField){

}

void SDP::finalizeSubContainer(){

}

void SDP::removeSubContainerFromSDP(std::string &subContainerName){

}


bool SDP::setSDPSubContainer(std::string &subContainerFileName, std::shared_ptr<std::iostream> algorithm){

    if(SDPFileInfo.subContainersInSDPFile.find(subContainerFileName) == SDPFileInfo.subContainersInSDPFile.end())
        return false;
    else
        SDPFileInfo.currentSubContainerInUse = SDPFileInfo.subContainersInSDPFile[subContainerFileName];

    if(algorithm)
        setAlgorithm(algorithm);

    return true;
}


std::shared_ptr<std::iostream> SDP::getInternalStream(){
    return SDPInOutStream;
}

void SDP::setAlgorithm(std::shared_ptr<std::iostream> algorithm){
    SDPFileInfo.currentAlgorithmInUse = algorithm;
}


bool SDP::read(uchar *data, uint64 readSize){

}

SDP& SDP::operator>>(uchar &c){

}

uint64 SDP::getReadCount(){

}


bool SDP::write(uchar *data, uint64 writeSize){

}

SDP& SDP::operator<<(uchar &c){

}

uint64 SDP::getWriteCount(){

}




