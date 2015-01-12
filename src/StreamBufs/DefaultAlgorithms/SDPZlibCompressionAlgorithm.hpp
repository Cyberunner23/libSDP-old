#ifndef SDPZLIBCOMPRESSIONALGORITHM_H
#define SDPZLIBCOMPRESSIONALGORITHM_H

#include "zlib.h"

#include "src/StreamBufs/AlgorithmBase/SDPCompressionAlgorithmBase.hpp"

class SDPZlibCompressionAlgorithm : public SDPCompressionAlgorithmBase{

public:

    //vars

    //funcs
    SDPZlibCompressionAlgorithm();
    ~SDPZlibCompressionAlgorithm();

    virtual uint_least64_t decompress(char* compressedBuffer,   char* decompressedBuffer, uint_least64_t compressedBufferSize);
    virtual uint_least64_t compress(char*   uncompressedBuffer, char* compressedBuffer,   uint_least64_t uncompressedBufferSize);

    virtual void onInit();
    virtual void onExit();
    virtual void onSync();


private:

    //vars

    //funcs


};

#endif // SDPZLIBCOMPRESSIONALGORITHM_H
