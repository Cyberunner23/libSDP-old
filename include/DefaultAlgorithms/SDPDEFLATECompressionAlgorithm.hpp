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

#ifndef _LIBSDP_SDPDEFLATECOMPRESSIONALGORITHM_H_
#define _LIBSDP_SDPDEFLATECOMPRESSIONALGORITHM_H_

#include "SDPCompressionAlgorithmBase.hpp"

#include "zlib.h"

namespace libSDP{
namespace DefaultAlgorithms{

    using namespace libSDP::AlgorithmBase;
    using namespace libSDP::Utils;

    class SDPDEFLATECompressionAlgorithm : public SDPCompressionAlgorithmBase{

    public:

        //Vars

        //Funcs

        SDPDEFLATECompressionAlgorithm();

        ~SDPDEFLATECompressionAlgorithm();

        virtual uint64 decompressBuffer(uchar *compressedBuffer, uchar *decompressedBuffer, uint64 compressedBufferSize,
                                        uint64 decompressedDataSize, uint64 chunkNum);

        virtual uint64 compressBuffer(uchar *uncompressedBuffer, uchar *compressedBuffer, uint64 uncompressedBufferSize,
                                      uint64 chunkNum);

        virtual void onInit();

        virtual void onExit();

        virtual void onSync();


    private:
//Vars
        //Not the actual maximum buffer size supported by zlib but
        //its fine for us.
        static const uint64 maxBufferSize=64 * 1024;
        static const uint64 preferedBufferSize=32 * 1024;
        //Not actual size with overhead but its nice to have a safety margin.
        static const uint64 preferedBufferSizeWithOverhead=(preferedBufferSize * 1.2) + 12;

        //Funcs

    };
}
}

#endif //_LIBSDP_SDPDEFLATECOMPRESSIONALGORITHM_H_