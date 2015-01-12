#include "SDPZlibCompressionAlgorithm.hpp"

SDPZlibCompressionAlgorithm::SDPZlibCompressionAlgorithm(){
return;
}

SDPZlibCompressionAlgorithm::~SDPZlibCompressionAlgorithm(){

}


uint_fast64_t SDPZlibCompressionAlgorithm::compress(char *uncompressedBuffer, char* compressedBuffer, uint_least64_t uncompressedBufferSize){

    CryptoPP::ZlibCompressor compressor;
    compressor.Put((byte*) uncompressedBuffer, uncompressedBufferSize);
    compressor.MessageEnd();

    uint_least64_t numCompressedBytes;
    numCompressedBytes = compressor.MaxRetrievable();

    compressor.Get((byte*) compressedBuffer, numCompressedBytes);

    return numCompressedBytes;
}

uint_least64_t SDPZlibCompressionAlgorithm::decompress(char *compressedBuffer, char* decompressedBuffer,  uint_least64_t compressedBufferSize){

    CryptoPP::ZlibDecompressor decompressor;
    decompressor.Put((byte*) compressedBuffer, compressedBufferSize);
    decompressor.MessageEnd();

    uint_least64_t numBytesUncompressed;
    numBytesUncompressed = decompressor.MaxRetrievable();

    decompressor.Get((byte*) decompressedBuffer, numBytesUncompressed);

    return numBytesUncompressed;
}


void SDPZlibCompressionAlgorithm::onInit(){

}

void SDPZlibCompressionAlgorithm::onExit(){

}

void SDPZlibCompressionAlgorithm::onSync(){

}
