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

#ifndef SDPXSALSA20ENCRYPTIONALGORITHM_HPP
#define SDPXSALSA20ENCRYPTIONALGORITHM_HPP

#include "sodium.h"

#include "SDPEncryptionAlgorithmBase.hpp"

class SDPXSalsa20EncryptionAlgorithm : public SDPEncryptionAlgorithmBase{

public:

    //Vars

    //Funcs

    SDPXSalsa20EncryptionAlgorithm();
    ~SDPXSalsa20EncryptionAlgorithm();

    virtual uint64 decryptBuffer(uchar* encryptedBuffer,   uchar* decryptedBuffer, uint64 encryptedBufferSize,   uint64 chunkNum);
    virtual uint64 encryptBuffer(uchar* unencryptedBuffer, uchar* encryptedBuffer, uint64 unencryptedBufferSize, uint64 chunkNum);

    virtual bool decryptStream(uchar encryptedChar,   uchar* decryptedChar, uint64 charNum);
    virtual bool encryptStream(uchar unencryptedChar, uchar* encryptedChar, uint64 charNum);

    virtual void onInit();
    virtual void onExit();
    virtual void onSync();


protected:

    //Vars

    //Funcs

private:

    //Vars

    static const uint8  encryptionAlgorithmID  = 0x01;
    static const bool   isStreamCipher         = false;
    static const int    algorithmBlockSize     = 64;
    static const int    blocksPerBuffer        = 500;
    static const uint64 bufferSize             = algorithmBlockSize * blocksPerBuffer;
    static const uint64 bufferSizeWithOverhead = bufferSize; //1 byte in, 1 byte out.

    //Funcs


};

#endif //SDPXSALSA20ENCRYPTIONALGORITHM_HPP