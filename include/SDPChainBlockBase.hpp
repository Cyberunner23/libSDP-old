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
#include "SDPSpecBase.hpp"
#include "Typedefs.hpp"

using namespace libSDP::Utils;

namespace libSDP{

    class SDPChainBlockBase{

    public:

        //Vars

        //Funcs
        SDPChainBlockBase() = delete;
        //Add child chain block.
        SDPChainBlockBase(std::shared_ptr<SDPChainBlockBase> childChainBlock);
        //Used if last block in the chain.
        SDPChainBlockBase(std::shared_ptr<SDPSpecBase> specReadWrite);
        ~SDPChainBlockBase();

        bool init();
        void exit();
        void reset();

        bool read(std::vector<uchar>  &data, uint64 size);
        bool write(std::vector<uchar> &data, uint64 size);

        //NOTE: Really need this? can access that data directly from specReadWrite (shared ptr)
        uint64 getBottomBlockReadChunkNum();
        uint64 getBottomBlockWriteChunkNum();


    protected:

        //Vars
        uint8 algorithmID;
        bool  canAlgorithmBackwardSeek;

        //Funcs
        virtual uint64 encodeBuffer() = 0;
        virtual uint64 decodeBuffer() = 0;

        virtual bool onInit()  = 0;
        virtual void onExit()  = 0;
        virtual void onReset() = 0;


    private:

        //Vars
        std::shared_ptr<SDPChainBlockBase> childChainBlock;
        std::shared_ptr<SDPSpecBase>       specReadWrite;

        //Funcs

    };
}

#endif //LIBSDP_SDPCHAINBLOCK_H
