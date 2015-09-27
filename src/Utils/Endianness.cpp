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

#include "Utils/Endianness.hpp"

using namespace libSDP;
using namespace libSDP::Utils;

bool Endianness::isSysBigEndian(){

    union {
            uint32_t num;
            char c[4];
        }testUnion = {0x01020304};

        return testUnion.c[0] == 1;
}

uint_least16_t Endianness::byteSwap(uint_least16_t uint16){

    union{
        uint_least16_t num;
        char c[2];
    }src, dest;

    src.num = uint16;

    dest.c[0] = src.c[1];
    dest.c[1] = src.c[0];

    return dest.num;
}

uint_least32_t Endianness::byteSwap(uint_least32_t uint32){

    union{
        uint_least32_t num;
        char c[4];
    }src, dest;

    src.num = uint32;

    dest.c[0] = src.c[3];
    dest.c[1] = src.c[2];
    dest.c[2] = src.c[1];
    dest.c[3] = src.c[0];

    return dest.num;
}

uint_least64_t Endianness::byteSwap(uint_least64_t uint64){

    union{
        uint_least64_t num;
        char c[8];
    }src, dest;

    src.num = uint64;

    dest.c[0] = src.c[7];
    dest.c[1] = src.c[6];
    dest.c[2] = src.c[5];
    dest.c[3] = src.c[4];
    dest.c[4] = src.c[3];
    dest.c[5] = src.c[2];
    dest.c[6] = src.c[1];
    dest.c[7] = src.c[0];

    return dest.num;
}

