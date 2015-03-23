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

#ifndef _LIBSDP_SDPSTREAMBUFBASE_H_
#define _LIBSDP_SDPSTREAMBUFBASE_H_

#include <iostream>
#include <vector>

#include "Typedefs.hpp"

class SDPStreamBufBase : public std::streambuf{

public:

    //Vars

    //Funcs

    SDPStreamBufBase();
    ~SDPStreamBufBase();


private:

    //Vars

    std::vector<uchar>::iterator bufferIterator;
    std::vector<uchar>           unprocessedBuffer;
    std::vector<uchar>           processedBuffer;

    uint64 processBuffer(uchar* unprocessedBuffer, uint64 &unprocessedBufferSize, uchar* processedBuffer,   uint64 &processedBufferSize,   uint64 &chunkNum);
    uint64 unprocessBuffer(uchar* processedBuffer, uint64 &processedBufferSize,   uchar* unprocessedBuffer, uint64 &unprocessedBufferSize, uint64 &chunkNum);

    uint64 processChar();
    uint64 unprocessChar();

    //Funcs

};

#endif //_LIBSDP_SDPSTREAMBUFBASE_H_
