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


#include "SDPlz4CompressionAlgorithm.hpp"

using namespace libSDP::DefaultAlgorithms;

SDPlz4CompressionAlgorithm::SDPlz4CompressionAlgorithm(){

    setMaxBufferSize(maxBufferSize);
    setPreferedBufferSize(preferedBufferSize);
    setPreferedBufferSizeWithOverhead(preferedBufferSizeWithOverhead);

}

SDPlz4CompressionAlgorithm::~SDPlz4CompressionAlgorithm(){

}


uint64 SDPlz4CompressionAlgorithm::decompressBuffer(uchar *compressedBuffer, uchar *decompressedBuffer, uint64 compressedBufferSize, uint64 decompressedDataSize, uint64 chunkNum){

    return (uint64)LZ4_decompress_safe((char*)compressedBuffer, (char*)decompressedBuffer, (int)compressedBufferSize, (int)decompressedDataSize);
}

uint64 SDPlz4CompressionAlgorithm::compressBuffer(uchar *uncompressedBuffer, uchar *compressedBuffer, uint64 uncompressedBufferSize, uint64 chunkNum){

    return (uint64)LZ4_compress((char*)uncompressedBuffer, (char*)compressedBuffer, (int)uncompressedBufferSize);
}


void SDPlz4CompressionAlgorithm::onInit(){

}

void SDPlz4CompressionAlgorithm::onSync(){

}

void SDPlz4CompressionAlgorithm::onExit(){

}
