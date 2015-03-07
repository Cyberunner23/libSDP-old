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
#include <memory>
#include <vector>

#include "RawFileIO.hpp"
#include "SDPEncryptionAlgorithmBase.hpp"

//TODO: implement seeking

class SDPCryptStreamBuf : public std::streambuf{

public:

    //Vars

    //Funcs

    SDPCryptStreamBuf(std::istream *cryptIn,  std::shared_ptr<SDPEncryptionAlgorithmBase> cryptAlgorithm);
    SDPCryptStreamBuf(std::ostream *cryptOut, std::shared_ptr<SDPEncryptionAlgorithmBase> cryptAlgorithm);
    ~SDPCryptStreamBuf();

    void setEncryptionAlgorithm(std::shared_ptr<SDPEncryptionAlgorithmBase> cryptAlgorithm);

    void setEncryptionKeyAndNonce(std::string encryptionKey, bool isEncryptionKeyInHex, std::string nonce, bool isNonceInHex);
	void setEncryptionKey(std::string encryptionKey, bool isEncryptionKeyInHex);
	void setNonce(std::string nonce, bool isNonceInHex);


protected:

    virtual int_type overflow(int_type ch = traits_type::eof());

    virtual int_type underflow();
    virtual int_type uflow();
    virtual int_type pbackfail(int_type ch);

    virtual int sync();

private:

    //Vars


    std::shared_ptr<SDPEncryptionAlgorithmBase> cryptAlgorithm;
	bool 										isStreamCipher;
	uint_least64_t 								bufferSize;
	uint_least64_t 								bufferSizeWithOverhead;


    int  nextChar;

    bool hasFirstBlockBeenRead;

    std::vector<unsigned char>::iterator bufferIterator;
    std::vector<unsigned char>           unencryptedBuffer;
    std::vector<unsigned char>           encryptedBuffer;

    std::shared_ptr<std::istream> inStream;
    std::shared_ptr<std::ostream> outStream;

    //SDPAES256GCMAlgorithm defaultCryptAlgorithm;

    //Funcs

    int getNextChar(bool doAdvance);
	bool fillAndDecryptBuffer();

    int_type setNextChar(int_type ch);


};

#endif // SDPCRYPTSTREAMBUF_H



























