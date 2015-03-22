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


SDP::SDP(){

}

SDP::~SDP(){

}


SDP::SDPErrEnum SDP::openSDP(std::string &SDPFileName, bool stopOnParseErr){

}

SDP::SDPErrEnum SDP::openSDP(std::shared_ptr<std::iostream> inOutStream, bool stopOnParseErr){

}


SDP::SDPFileInfoStruct* SDP::getSDPFileInfo(){

}

SDP::SDPSubContainerInfoStruct* SDP::getCurrentSubContainerInfo(){

}

SDP::SDPSubContainerInfoStruct* SDP::getSubContainerInfo(std::string &subContainerFileName){

}


void SDP::createSDP(std::string &SDPFileName, std::vector<uchar> &extraField){

}

void SDP::createSDP(std::shared_ptr<std::iostream> inOutStream, std::vector<uchar> &extraField){

}

void SDP::addSubContainerToSDP(std::string &SubContainerFileName, std::vector<uchar> &extraField){

}


bool SDP::setSDPSubContainer(std::string &subContainerFileName, std::shared_ptr<std::iostream> algorithm){

}


std::shared_ptr<std::iostream> SDP::getInternalStream(){

}

void SDP::setAlgorithm(std::shared_ptr<std::iostream> algorithm){

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


bool SDP::squeezeIn(uchar *data, uint64 writeSize){

}

SDP& SDP::operator<<=(uchar &c){

}


SDP::SDPErrEnum SDP::parseSDPFileInfo(std::shared_ptr<std::istream> inStream, SDPFileHeaderStruct &SDPFileHeader){

}

SDP::SDPErrEnum SDP::parseSubContainerInfo(std::shared_ptr<std::istream> inStream, SDPSubContainerInfoStruct &SDPSubContainerInfo){

}

SDP::SDPErrEnum SDP::parseSDPFile(std::shared_ptr<std::istream> inStream, SDPFileInfoStruct &SDPFileInfo){

}

















