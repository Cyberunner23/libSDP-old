/*
Copyright 2014 Alex Frappier Lachapelle

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

#ifndef _LIBSDP_SDPLZ4COMPRESSIONALGORITHM_H_
#define _LIBSDP_SDPLZ4COMPRESSIONALGORITHM_H_

#include <limits>

#include "SDPCompressionAlgorithmBase.hpp"

#include "lz4.h"

class SDPlz4CompressionAlgorithm : public SDPCompressionAlgorithmBase{

public:

    //Vars


    //Funcs

    SDPlz4CompressionAlgorithm();
    ~SDPlz4CompressionAlgorithm();

    virtual uint64 decompressBuffer(uchar* compressedBuffer,   uchar* decompressedBuffer, uint64 compressedBufferSize,   uint64 decompressedDataSize,  uint64 chunkNum);
    virtual uint64 compressBuffer(uchar*   uncompressedBuffer, uchar* compressedBuffer,   uint64 uncompressedBufferSize, uint64 chunkNum);

    virtual void onInit();
    virtual void onExit();
    virtual void onSync();


private:

    //Vars
    static const uint64 maxBufferSize                  = LZ4_MAX_INPUT_SIZE;
    static const uint64 preferedBufferSize             = 32 * 1024;
    static const uint64 preferedBufferSizeWithOverhead = LZ4_COMPRESSBOUND(preferedBufferSize);

    //Funcs


};


#endif //_LIBSDP_SDPLZ4COMPRESSIONALGORITHM_H_
