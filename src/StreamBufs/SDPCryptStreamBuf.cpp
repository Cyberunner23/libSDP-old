/*
Copyright 2014 Alex Frappier Lachapelle

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "SDPCryptStreamBuf.hpp"


SDPCryptStreamBuf::SDPCryptStreamBuf(std::istream *cryptIn){

    hasFirstBlockBeenRead = false;

    inStream  = cryptIn;
    outStream = nullptr;

    cryptOptions.cryptAlgorithm = &defaultCryptAlgorithm;
    cryptOptions.encryptionKeyDecoded = "0000000000000000"
                                        "0000000000000000"
                                        "0000000000000000"
                                        "0000000000000000";
    cryptOptions.IVDecoded            = "0000000000000000"
                                        "00000000";
    cryptOptions.blockSize            = 1024*64;

    unencryptedBuffer.resize(this->cryptOptions.blockSize);
    encryptedBuffer.resize(this->cryptOptions.blockSize);

    cryptOptions.cryptAlgorithm->onInit();

    nextChar = traits_type::eof();

}

SDPCryptStreamBuf::SDPCryptStreamBuf(std::ostream *cryptOut){

    hasFirstBlockBeenRead = false;

    inStream  = nullptr;
    outStream = cryptOut;

    cryptOptions.cryptAlgorithm = &defaultCryptAlgorithm;
    cryptOptions.encryptionKeyDecoded = "0000000000000000"
                                        "0000000000000000"
                                        "0000000000000000"
                                        "0000000000000000";
    cryptOptions.IVDecoded            = "0000000000000000"
                                        "00000000";
    cryptOptions.blockSize            = 1024*64;

    unencryptedBuffer.resize(cryptOptions.blockSize);
    encryptedBuffer.resize(cryptOptions.blockSize);

    defaultCryptAlgorithm.onInit();

}

SDPCryptStreamBuf::~SDPCryptStreamBuf() throw(){
    cryptOptions.cryptAlgorithm->onExit();
}


SDPCryptStreamBuf::int_type SDPCryptStreamBuf::overflow(int_type ch){

    return setNextChar(ch);
}


SDPCryptStreamBuf::int_type SDPCryptStreamBuf::underflow(){

    return getNextChar(false);
}

SDPCryptStreamBuf::int_type SDPCryptStreamBuf::uflow(){

    return getNextChar(true);
}

SDPCryptStreamBuf::int_type SDPCryptStreamBuf::pbackfail(int_type ch){

    return traits_type::eof();
}


int SDPCryptStreamBuf::sync(){

    //TODO?: add chunk size checking
    //send whats left in the buffer to be encrypted

    if(outStream && unencryptedBuffer.size() != 0){

        RawFileIO rawFileIO;

        //encrypt whats left
        encryptedBuffer.resize(cryptOptions.blockSize);
        uint_least64_t numBytesEncrypted = cryptOptions.cryptAlgorithm->encrypt((char*) unencryptedBuffer.data(), (char*) encryptedBuffer.data(), unencryptedBuffer.size());
        encryptedBuffer.resize(numBytesEncrypted);

        //write block size
        rawFileIO.write(numBytesEncrypted, outStream, RawFileIO::BIG__ENDIAN);
        //write block data
        outStream->write((char*) encryptedBuffer.data(), encryptedBuffer.size());

    }

    outStream->flush();

    cryptOptions.cryptAlgorithm->onSync();

    return 0;
}


void SDPCryptStreamBuf::encodeHex(std::string decoded, std::string &encoded){

    CryptoPP::HexEncoder encoder;
    encoder.Put((byte*)decoded.data(), decoded.size());
    encoder.MessageEnd();

    encoded.resize(encoder.MaxRetrievable());
    encoder.Get((byte*)encoded.data(), encoded.size());

}

void SDPCryptStreamBuf::decodeHex(std::string encoded, std::string &decoded){

    CryptoPP::HexDecoder decoder;
    decoder.Put((byte*)encoded.data(), encoded.size());
    decoder.MessageEnd();

    decoded.resize(decoder.MaxRetrievable());
    decoder.Get((byte*)decoded.data(), decoded.size());

}


int SDPCryptStreamBuf::getNextChar(bool doAdvance){

    //if nextChar was not reset (in underflow), return the buffered char.
    if(!doAdvance){
        return nextChar;
    }

    if(bufferIterator == unencryptedBuffer.end() || !hasFirstBlockBeenRead){

        RawFileIO rawFileIO;

        unencryptedBuffer.clear();
        encryptedBuffer.clear();

        uint_least64_t numEncryptedBytes;

        //check encrypted data size
        if(rawFileIO.read(numEncryptedBytes, inStream, RawFileIO::BIG__ENDIAN) != sizeof numEncryptedBytes){
            return traits_type::eof();
        }

        //check if numEncryptedBytes makes sense, if so, read the data
        if(numEncryptedBytes > cryptOptions.blockSize){
            return traits_type::eof();
        }else{

            encryptedBuffer.resize(numEncryptedBytes);
            inStream->read((char*) encryptedBuffer.data(), numEncryptedBytes);

        }

        //check how many bytes were really read, if
        //that number is not what we expect, something
        //went wrong
        int gCount = inStream->gcount();
        if(gCount != numEncryptedBytes){
            return traits_type::eof();
        }

        //do decryption

    }



    return nextChar;
}

SDPCryptStreamBuf::int_type SDPCryptStreamBuf::setNextChar(int_type ch){

    //TODO: Implement this


    return traits_type::to_int_type(ch);
}


