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

#include "SDPChainBlock.hpp"

using namespace libSDP;

SDPChainBlock::SDPChainBlock(std::shared_ptr<SDPChainBlock> childChainBlock)
    : childChainBlock(childChainBlock),
      chainBlockType(NORMAL_CHAINBLOCK){

}

SDPChainBlock::SDPChainBlock(std::shared_ptr<std::iostream> fileStream)
    : fileStream(fileStream),
      chainBlockType(BOTTOM_CHAINBLOCK){

}

SDPChainBlock::~SDPChainBlock(){
    //new used in algorithm clone()
    delete readAlgBufPack.algorithm;
    delete writeAlgBufPack.algorithm;
}


bool SDPChainBlock::setAlgorithm(std::unique_ptr<SDPAlgorithmBase> &algorithm){

    bool isReadAlgInitSuccess;
    bool isWriteAlgInitSuccess;

    //If the pack is set up (aka we're already using it)
    //deinitialize algorithm, switch it, reinitialize algorithm
    //If not, just set the algorithm for now
    if(readAlgBufPack.isPackSetUp){
        readAlgBufPack.algorithm->onSync();
        readAlgBufPack.algorithm->onExit();
        readAlgBufPack.algorithm = algorithm.get()->clone();
        isReadAlgInitSuccess     = readAlgBufPack.algorithm->onInit();
    }else{
        readAlgBufPack.algorithm = algorithm.get()->clone();
        isReadAlgInitSuccess     = true;
    }

    //If the pack is set up (aka we're already using it)
    //deinitialize algorithm, switch it, reinitialize algorithm
    //If not, just set the algorithm for now
    if(writeAlgBufPack.isPackSetUp){
        writeAlgBufPack.algorithm->onSync();
        writeAlgBufPack.algorithm->onExit();
        writeAlgBufPack.algorithm = algorithm.get()->clone();
        isWriteAlgInitSuccess     = writeAlgBufPack.algorithm->onInit();
    }else{
        writeAlgBufPack.algorithm = algorithm.get()->clone();
        isWriteAlgInitSuccess     = true;
    }

    return isReadAlgInitSuccess && isWriteAlgInitSuccess;
}


bool SDPChainBlock::read(uchar *data, uint64 size){
    STUB_FUNC(__FILE__, __LINE__)
}

SDPChainBlock& SDPChainBlock::operator>>(const uchar &c){
    STUB_FUNC(__FILE__, __LINE__)
}


bool SDPChainBlock::write(uchar *data, uint64 size){
    STUB_FUNC(__FILE__, __LINE__)
}

SDPChainBlock& SDPChainBlock::operator<<(const uchar &c){
    STUB_FUNC(__FILE__, __LINE__)
}


uint64 SDPChainBlock::getBottomBlockReadChunkNum(){

    INCOMPLETE_FUNC(__FILE__, __LINE__)
    if(chainBlockType == NORMAL_CHAINBLOCK){
        assert(childChainBlock && "One of the chain blocks is invalid.");
        return childChainBlock.get()->getBottomBlockReadChunkNum();
    }else{
        //Get current block num from BufferManager
    }

}

uint64 SDPChainBlock::getBottomBlockWriteChunkNum(){

    INCOMPLETE_FUNC(__FILE__, __LINE__)
    if(chainBlockType == NORMAL_CHAINBLOCK){
        assert(childChainBlock && "One of the chain blocks is invalid.");
        return childChainBlock.get()->getBottomBlockWriteChunkNum();
    }else{
        //Get current block num from BufferManager
    }

}