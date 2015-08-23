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

#ifndef LIBSDP_SDPCHAINBLOCK_H
#define LIBSDP_SDPCHAINBLOCK_H

#include <assert.h>
#include <iostream>
#include <memory>
#include <vector>

#include "DevMacros.hpp"
#include "SDPAlgorithmBase.hpp"
#include "Typedefs.hpp"

namespace libSDP{

    using namespace libSDP::Utils;

    class SDPChainBlock{

    public:

        //Vars

        enum ChainBlockType{
            NORMAL_CHAINBLOCK,
            BOTTOM_CHAINBLOCK
        };

        //Funcs

        SDPChainBlock()=delete;

        //Add child chain block.
        SDPChainBlock(std::shared_ptr<SDPChainBlock> childChainBlock);

        //Used if last block in the chain.
        SDPChainBlock(std::shared_ptr<std::iostream> fileStream);

        ~SDPChainBlock();

        bool setAlgorithm(std::unique_ptr<SDPAlgorithmBase> &algorithm);

        bool read(uchar *data, uint64 size);

        SDPChainBlock &operator>>(const uchar &c);

        bool write(uchar *data, uint64 size);

        SDPChainBlock &operator<<(const uchar &c);

        uint64 getBottomBlockReadChunkNum();

        uint64 getBottomBlockWriteChunkNum();


    private:

        //Vars

        struct AlgorithmBufferPack{
            std::vector<uchar> buffer;
            SDPAlgorithmBase *algorithm;
            //BufferManager
            bool isPackSetUp;
        };

        std::shared_ptr<std::iostream> fileStream;
        std::shared_ptr<SDPChainBlock> childChainBlock;

        AlgorithmBufferPack readAlgBufPack;
        AlgorithmBufferPack writeAlgBufPack;

        //Funcs

        ChainBlockType chainBlockType;

    };
}

#endif //LIBSDP_SDPCHAINBLOCK_H
