#include "SDPXSalsa20EncryptionAlgorithm.hpp"

SDPXSalsa20EncryptionAlgorithm::SDPXSalsa20EncryptionAlgorithm(){

	setIsStreamCipher(true);
	setEncryptionKeyWidthInBits(crypto_stream_KEYBYTES);
	setNonceWidthInBits(crypto_stream_NONCEBYTES);

}

SDPXSalsa20EncryptionAlgorithm::~SDPXSalsa20EncryptionAlgorithm(){

}


uint_least64_t SDPXSalsa20EncryptionAlgorithm::decrypt(unsigned char *encryptedBuffer, unsigned char *decryptedBuffer, uint_least64_t encryptedBufferSize){

}

uint_least64_t SDPXSalsa20EncryptionAlgorithm::encrypt(unsigned char *unencryptedBuffer, unsigned char *encryptedBuffer, uint_least64_t unencryptedBufferSize){

}


bool SDPXSalsa20EncryptionAlgorithm::decryptStream(unsigned char *encryptedChar, unsigned char *unencryptedChar){
	crypto_stream_xor(unencryptedChar, encryptedChar, 1, (const unsigned char*)nonceInBin.data(), (const unsigned char*)encryptionKeyInBin.data());
}

bool SDPXSalsa20EncryptionAlgorithm::encryptStream(unsigned char *unencryptedChar, unsigned char *encryptedChar){
	crypto_stream_xor(encryptedChar, unencryptedChar, 1, (const unsigned char*)nonceInBin.data(), (const unsigned char*)encryptionKeyInBin.data());
}


void SDPXSalsa20EncryptionAlgorithm::onInit(){

}

void SDPXSalsa20EncryptionAlgorithm::onExit(){

}

void SDPXSalsa20EncryptionAlgorithm::onSync(){

}