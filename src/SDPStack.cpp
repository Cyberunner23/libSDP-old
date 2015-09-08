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

#include "SDPStack.hpp"

using namespace libSDP;

SDPStack::SDPStack(std::shared_ptr<SDPChainBlockBase> algorithmBlockChain, std::shared_ptr<SDPSpecBase> specReadWrite, std::shared_ptr<SDPSourceSinkBase> sourceSink, uint64 stackBufferSize)
    : algorithmBlockChain(algorithmBlockChain)
    , specReadWrite(specReadWrite)
    , sourceSink(sourceSink)
    , stackBufferSize(stackBufferSize){

    algorithmBlockChain->init();
    //NOTE: readStackBuffer will be set by the algorithm chain.
    writeStackBuffer->resize(stackBufferSize);

}

SDPStack::SDPStack(std::shared_ptr<SDPSpecBase> specReadWrite, std::shared_ptr<SDPSourceSinkBase> sourceSink, uint64 stackBufferSize)
        : specReadWrite(specReadWrite)
        , sourceSink(sourceSink)
        , stackBufferSize(stackBufferSize){

    //NOTE: readStackBuffer will be set by the algorithm chain.
    writeStackBuffer->resize(stackBufferSize);

}

SDPStack::~SDPStack() {

}


void SDPStack::setAlgorithmBlockChain(std::shared_ptr<SDPChainBlockBase> algorithmBlockChain){
    if(this->algorithmBlockChain != nullptr){
        this->algorithmBlockChain->exit();
    }
    this->algorithmBlockChain = algorithmBlockChain;
    this->algorithmBlockChain->init();
}


void SDPStack::read(uint8* data, uint64 aize){
    STUB_FUNC(__FILE__, __LINE__)
}

void SDPStack::read(std::vector<uint8> data){
    STUB_FUNC(__FILE__, __LINE__)
}

SDPStack &SDPStack::operator>>(const uchar &c){
    STUB_FUNC(__FILE__, __LINE__)
}


void SDPStack::write(uint8* data, uint64 aize){
    STUB_FUNC(__FILE__, __LINE__)
}

void SDPStack::write(std::vector<uint8> data){
    STUB_FUNC(__FILE__, __LINE__)
}

SDPStack &SDPStack::operator<<(const uchar &c){
    STUB_FUNC(__FILE__, __LINE__)
}


void SDPStack::seek(uint64 pos){
    STUB_FUNC(__FILE__, __LINE__)
}