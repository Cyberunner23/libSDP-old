#include "SDPAES256GCMAlgorithm.hpp"

SDPAES256GCMAlgorithm::SDPAES256GCMAlgorithm(){

}

SDPAES256GCMAlgorithm::~SDPAES256GCMAlgorithm(){

}


uint_least64_t SDPAES256GCMAlgorithm::decrypt(char* encryptedBuffer, char* decryptedBuffer, uint_least64_t encryptedBufferSize){

    encryptedBufferProxy     = std::make_shared<byte*> ((byte*) encryptedBuffer);
    encryptedBufferSizeProxy = std::make_shared<uint_least64_t> (encryptedBufferSize);

    authenticatedDecryptionFilter->ChannelMessageEnd(CryptoPP::DEFAULT_CHANNEL);

    uint_least64_t numBytesRetrived = authenticatedDecryptionFilter->Get((byte*) decryptedBuffer, authenticatedDecryptionFilter->MaxRetrievable());

    return numBytesRetrived;
}

uint_least64_t SDPAES256GCMAlgorithm::encrypt(char* unencryptedBuffer, char* encryptedBuffer, uint_least64_t unencryptedBufferSize){

    std::string encryptedBufferString(encryptedBuffer);
    encryptedBufferStringProxy = std::make_shared<std::string> (encryptedBufferString);

    authenticatedEncryptionFilter->Put((byte*) unencryptedBuffer, unencryptedBufferSize);

    return sizeof(encryptedBuffer);
}


void SDPAES256GCMAlgorithm::onInit(){

    authenticatedEncryptionFilter = nullptr;
    authenticatedDecryptionFilter = nullptr;
    stringSource = nullptr;
    stringSink   = nullptr;

    isMessageEnded = false; //this part needs some testing to see what happens then
                            //only one of the encrypt/decrypt sides is is used.

    stringSink                    = new CryptoPP::StringSink(*encryptedBufferStringProxy);
    authenticatedEncryptionFilter = new CryptoPP::AuthenticatedEncryptionFilter(encryptor, stringSink);

    authenticatedDecryptionFilter = new CryptoPP::AuthenticatedDecryptionFilter(decryptor);
    stringSource                  = new CryptoPP::StringSource((byte*) *encryptedBufferProxy, *encryptedBufferSizeProxy, true, authenticatedDecryptionFilter);

}

void SDPAES256GCMAlgorithm::onExit(){

    if(!isMessageEnded){
        authenticatedEncryptionFilter->MessageEnd();
    }

    if(stringSink){
        delete authenticatedEncryptionFilter;
    }

    if(stringSource){
        delete stringSource;
    }

}

void SDPAES256GCMAlgorithm::onSync(){

    if(stringSink){

        authenticatedEncryptionFilter->MessageEnd();
        isMessageEnded = true;

    }
}


void SDPAES256GCMAlgorithm::setEncryptionKeyAndIV(std::string encryptionKey, bool isEncryptionKeyEncoded, std::string IV, bool isIVEncoded){

    if(isEncryptionKeyEncoded){
        decodeHex(encryptionKey, this->encryptionKeyDecoded);
    }else{
        this->encryptionKeyDecoded = encryptionKey;
    }

    if(isIVEncoded){
        decodeHex(IV, this->IVDecoded);
    }else{
        this->IVDecoded = IV;
    }

    encryptor.SetKeyWithIV((byte*) encryptionKeyDecoded.data(), encryptionKeyDecoded.size(), (byte*) IVDecoded.data(), IVDecoded.size());

}

















