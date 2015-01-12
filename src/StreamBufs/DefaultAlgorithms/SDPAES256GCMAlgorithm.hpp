#ifndef SDPAES256GCMALGORITHM_H
#define SDPAES256GCMALGORITHM_H

#include <memory>

#include "aes.h"
#include "filters.h"
#include "gcm.h"

#include "src/StreamBufs/AlgorithmBase/SDPEncryptionAlgorithmBase.hpp"

class SDPAES256GCMAlgorithm : public SDPEncryptionAlgorithmBase{

public:

    //vars

    //funcs

    SDPAES256GCMAlgorithm();
    ~SDPAES256GCMAlgorithm();

    virtual uint_least64_t decrypt(char* encryptedBuffer,   char* decryptedBuffer, uint_least64_t encryptedBufferSize);
    virtual uint_least64_t encrypt(char* unencryptedBuffer, char* encryptedBuffer, uint_least64_t unencryptedBufferSize);

    virtual void onInit();
    virtual void onExit();
    virtual void onSync();

    virtual void setEncryptionKeyAndIV(std::string encryptionKey, bool isEncryptionKeyEncoded, std::string IV, bool isIVEncoded);

private:

    //vars

    CryptoPP::AuthenticatedEncryptionFilter *authenticatedEncryptionFilter;
    CryptoPP::AuthenticatedDecryptionFilter *authenticatedDecryptionFilter;

    CryptoPP::GCM<CryptoPP::AES>::Encryption encryptor;
    CryptoPP::GCM<CryptoPP::AES>::Decryption decryptor;

    CryptoPP::StringSource *stringSource;
    CryptoPP::StringSink   *stringSink;

    bool isMessageEnded;

    //decrypt side

    std::shared_ptr<byte*>          encryptedBufferProxy;
    std::shared_ptr<uint_least64_t> encryptedBufferSizeProxy;

    //encrype side

    std::shared_ptr<std::string> encryptedBufferStringProxy;

    //funcs


};

#endif // SDPAES256GCMALGORITHM_H


















