#include "SDPXSalsa20EncryptionAlgorithm.hpp"

SDPXSalsa20EncryptionAlgorithm::SDPXSalsa20EncryptionAlgorithm(){

    setEncryptionAlgorithmID(encryptionAlgorithmID);
    setIsStreamCipher(isStreamCipher);
    setEncryptionKeyWidthInBits(crypto_stream_KEYBYTES * 8);
    setNonceWidthInBits(crypto_stream_NONCEBYTES * 8);
    setMaxBufferSize(bufferSize);
    setPreferedBufferSize(bufferSize);
    setPreferedBufferSizeWithOverhead(bufferSizeWithOverhead);

}

SDPXSalsa20EncryptionAlgorithm::~SDPXSalsa20EncryptionAlgorithm(){

}


uint64 SDPXSalsa20EncryptionAlgorithm::decryptBuffer(uchar *encryptedBuffer, uchar *decryptedBuffer, uint64 encryptedBufferSize, uint64 chunkNum){

    crypto_stream_xsalsa20_xor_ic(decryptedBuffer, encryptedBuffer, encryptedBufferSize, (uchar*)nonceInBin.c_str(), chunkNum * blocksPerBuffer, (uchar*)encryptionKeyInBin.c_str());

    return encryptedBufferSize; //1 byte in, 1 byte out
}

uint64 SDPXSalsa20EncryptionAlgorithm::encryptBuffer(uchar *unencryptedBuffer, uchar *encryptedBuffer, uint64 unencryptedBufferSize, uint64 chunkNum){

    crypto_stream_xsalsa20_xor_ic(encryptedBuffer, unencryptedBuffer, unencryptedBufferSize, (uchar*)nonceInBin.c_str(), chunkNum * blocksPerBuffer, (uchar*)encryptionKeyInBin.c_str());

    return unencryptedBufferSize; //1 byte in, 1 byte out
}


void SDPXSalsa20EncryptionAlgorithm::onInit(){

}

void SDPXSalsa20EncryptionAlgorithm::onExit(){

}

void SDPXSalsa20EncryptionAlgorithm::onSync(){

}