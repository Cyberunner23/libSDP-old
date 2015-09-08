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

#ifndef LIBSDP_SDPSTACK_HPP
#define LIBSDP_SDPSTACK_HPP

#include <memory>
#include <vector>

#include "DevMacros.hpp"
#include "SDPChainBlockBase.hpp"
#include "SDPSourceSinkBase.hpp"
#include "SDPSpecBase.hpp"
#include "Typedefs.hpp"

namespace libSDP {

    using namespace libSDP::Utils;

    class SDPStack {

    public:

        //Vars

        //Funcs
        SDPStack(std::shared_ptr<SDPChainBlockBase> algorithmBlockChain,
                 std::shared_ptr<SDPSpecBase>       specReadWrite,
                 std::shared_ptr<SDPSourceSinkBase> sourceSink,
                 uint64                             stackBufferSize);
        SDPStack(std::shared_ptr<SDPSpecBase>       specReadWrite,
                 std::shared_ptr<SDPSourceSinkBase> sourceSink,
                 uint64                             stackBufferSize);
        ~SDPStack();

        void setAlgorithmBlockChain(std::shared_ptr<SDPChainBlockBase> algorithmBlockChain);

        void read(uint8* data, uint64 aize);
        void read(std::vector<uint8> data);
        SDPStack &operator>>(const uchar &c);

        void write(uint8* data, uint64 aize);
        void write(std::vector<uint8> data);
        SDPStack &operator<<(const uchar &c);

        void seek(uint64 pos);


    private:

        //Vars

        uint64                             stackBufferSize;
        std::unique_ptr<std::vector<char>> readStackBuffer;
        std::unique_ptr<std::vector<char>> writeStackBuffer;

        std::shared_ptr<SDPChainBlockBase> algorithmBlockChain = nullptr;
        std::shared_ptr<SDPSpecBase>       specReadWrite       = nullptr;
        std::shared_ptr<SDPSourceSinkBase> sourceSink          = nullptr;

        //Funcs

    };
}

#endif //LIBSDP_SDPSTACK_HPP
