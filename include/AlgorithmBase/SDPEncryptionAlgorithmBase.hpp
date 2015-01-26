#ifndef SDPENCRYPTIONALGORITHMBASE_H
#define SDPENCRYPTIONALGORITHMBASE_H

#include <stdint.h>
#include <string>

#include "sodium.h"

class SDPEncryptionAlgorithmBase{


public:

    //vars

    //funcs

    SDPEncryptionAlgorithmBase();
    ~SDPEncryptionAlgorithmBase();

    virtual uint_least64_t decrypt(char* encryptedBuffer,   char* decryptedBuffer, uint_least64_t encryptedBufferSize) = 0;
    virtual uint_least64_t encrypt(char* unencryptedBuffer, char* encryptedBuffer, uint_least64_t unencryptedBufferSize) = 0;

    virtual void onInit() = 0;
    virtual void onExit() = 0;
    virtual void onSync() = 0;

    virtual void setEncryptionKeyAndIV(std::string encryptionKey, bool isEncryptionKeyEncoded, std::string IV, bool isIVEncoded);


protected:

    //vars

    std::string encryptionKeyDecoded;
    std::string IVDecoded;

    //funcs

    void decodeHex(std::string encoded, std::string &decode);


private:

    //vars

    //funcs


};

#endif // SDPENCRYPTIONALGORITHMBASE_H
