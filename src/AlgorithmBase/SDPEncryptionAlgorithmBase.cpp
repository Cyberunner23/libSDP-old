#include "SDPEncryptionAlgorithmBase.hpp"

SDPEncryptionAlgorithmBase::SDPEncryptionAlgorithmBase(){

}

SDPEncryptionAlgorithmBase::~SDPEncryptionAlgorithmBase(){

}


void SDPEncryptionAlgorithmBase::setEncryptionKeyAndIV(std::string encryptionKey, bool isEncryptionKeyEncoded, std::string IV, bool isIVEncoded){

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

}


void SDPEncryptionAlgorithmBase::decodeHex(std::string encoded, std::string &decoded){

    CryptoPP::HexDecoder decoder;
    decoder.Put((byte*)encoded.data(), encoded.size());
    decoder.MessageEnd();

    decoded.resize(decoder.MaxRetrievable());
    decoder.Get((byte*)decoded.data(), decoded.size());

}
