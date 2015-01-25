#ifndef SDPCOMPRESSIONALGORITHMBASE_H
#define SDPCOMPRESSIONALGORITHMBASE_H

#include <stdint.h>

class SDPCompressionAlgorithmBase{

public:

    //vars

    //funcs

    SDPCompressionAlgorithmBase();
    ~SDPCompressionAlgorithmBase();

    virtual uint_least64_t decompress(char* compressedBuffer,   char* decompressedBuffer, uint_least64_t compressedBufferSize) = 0;
    virtual uint_least64_t compress  (char* uncompressedBuffer, char* compressedBuffer,   uint_least64_t uncompressedBufferSize) = 0;

    virtual void onInit() = 0;
    virtual void onExit() = 0;
    virtual void onSync() = 0;

    void setBlockSize(uint_least64_t blockSize);


private:

    //vars

    uint_least64_t blockSize;

    //funcs


};

#endif // SDPCOMPRESSIONALGORITHMBASE_H
