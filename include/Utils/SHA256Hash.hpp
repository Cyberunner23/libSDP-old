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

#ifndef LIBSDP_SHA256HASH_HPP
#define LIBSDP_SHA256HASH_HPP

#include <cassert>
#include <string>
#include <vector>

#include "sodium.h"

#include "Typedefs.hpp"

namespace libSDP{
namespace Utils{

    using namespace libSDP::Utils;

    class SHA256Hash{

    public:

        //Vars

        //Funcs

        SHA256Hash();
        ~SHA256Hash();

        template<typename... Args>
        std::string getHashStr(Args... args);

        template<typename... Args>
        std::vector<uchar> getHashVec(Args... args);


    private:

        //Vars
        crypto_hash_sha256_state sha256State;
        std::vector<uchar>       digest;

        //Funcs

        /*
         * Not sure if this one will be called
         * instead of the overloaded ones.
         * //NOTE: Remove this function if it
         *         asserts even when supplying
         *         it with valid data types.
         *         User will have to interpret
         *         the cryptic? compiler error...
         */
        template<typename... Args>
        void updateHash(Args... args);

        template<typename... Args>
        void updateHash(uchar val, Args... args);

        template<typename... Args>
        void updateHash(uchar* val, uint64 size, Args... args);

        template<typename... Args>
        void updateHash(uint16 val, Args... args);

        template<typename... Args>
        void updateHash(uint32 val, Args... args);

        template<typename... Args>
        void updateHash(uint64 val, Args... args);

        template<typename... Args>
        void updateHash(std::string val, Args... args);

        template<typename... Args>
        void updateHash(std::vector<uchar> val, Args... args);

        template<typename... Args>
        void updateHash(std::vector<uint16> val, Args... args);

        template<typename... Args>
        void updateHash(std::vector<uint32> val, Args... args);

        template<typename... Args>
        void updateHash(std::vector<uint64> val, Args... args);

    };
}
}

#endif //LIBSDP_SHA256HASH_HPP















