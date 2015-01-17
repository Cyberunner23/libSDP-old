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

#ifndef SDPCRYPTSTREAMBUF_H
#define SDPCRYPTSTREAMBUF_H

#include <iostream>

#include "aes.h"
#include "gcm.h"
#include "hex.h"
#include "modes.h"
#include "filters.h"
#include "files.h"

#include "src/RawFileIO.hpp"
#include "src/StreamBufs/AlgorithmBase/SDPEncryptionAlgorithmBase.hpp"
#include "src/StreamBufs/DefaultAlgorithms/SDPAES256GCMAlgorithm.hpp"

//TODO: implement seeking

class SDPCryptStreamBuf : public std::streambuf{

public:

    //Vars

    //Funcs

    SDPCryptStreamBuf(std::istream *cryptIn);
    SDPCryptStreamBuf(std::ostream *cryptOut);
    ~SDPCryptStreamBuf() throw(); //dirty fix to remove "exception specification in declaration does not match previous declaration" error

    void setEncryptionAlgorithm(SDPEncryptionAlgorithmBase *cryptAlgorithm);

    void setEncryptionKeyEncoded();
    void setIVEncoded();
    void setEncryptionKeyDecoded();
    void setIVDecoded();


protected:

    virtual int_type overflow(int_type ch = traits_type::eof());

    virtual int_type underflow();
    virtual int_type uflow();
    virtual int_type pbackfail(int_type ch);

    virtual int sync();

private:

    //Vars

    struct SDPCryptOptions{
        SDPEncryptionAlgorithmBase *cryptAlgorithm;
        std::string encryptionKeyDecoded;
        std::string IVDecoded;
        uint_least64_t blockSize;
    }cryptOptions;

    int  nextChar;

    bool hasFirstBlockBeenRead;

    std::vector<unsigned char>::iterator bufferIterator;
    std::vector<unsigned char>           unencryptedBuffer;
    std::vector<unsigned char>           encryptedBuffer;

    std::istream *inStream;
    std::ostream *outStream;

    SDPAES256GCMAlgorithm defaultCryptAlgorithm;

    //Funcs

    //turns AABBCC to 0xAA oxBB 0xCC
    void encodeHex(std::string decoded, std::string &encoded);
    //turns 0xAA 0xBB 0xCC to AABBCC
    void decodeHex(std::string encoded, std::string &decoded);

    int getNextChar(bool doAdvance);
    int_type setNextChar(int_type ch);


};

#endif // SDPCRYPTSTREAMBUF_H



























