#include "SDPEncryptionAlgorithmBase.hpp"

SDPEncryptionAlgorithmBase::SDPEncryptionAlgorithmBase(){

}

SDPEncryptionAlgorithmBase::~SDPEncryptionAlgorithmBase(){

}


void SDPEncryptionAlgorithmBase::setEncryptionKeyAndIV(std::string encryptionKey, bool isEncryptionKeyEncoded, std::string IV, bool isIVEncoded){

    if(isEncryptionKeyEncoded){
        //decodeHex(encryptionKey, this->encryptionKeyDecoded);
    }else{
        this->encryptionKeyDecoded = encryptionKey;
    }

    if(isIVEncoded){
        //decodeHex(IV, this->IVDecoded);
    }else{
        this->IVDecoded = IV;
    }

}

