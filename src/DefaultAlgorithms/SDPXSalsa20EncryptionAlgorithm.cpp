/*
Copyright 2015 Alex Frappier Lachapelle

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

#include "SDPXSalsa20EncryptionAlgorithm.hpp"

using namespace libSDP::DefaultAlgorithms;

SDPXSalsa20EncryptionAlgorithm::SDPXSalsa20EncryptionAlgorithm(){

    setEncryptionAlgorithmID(encryptionAlgorithmID);
    setIsStreamCipher(isStreamCipher);
    setEncryptionKeyWidthInBits(crypto_stream_KEYBYTES * 8);
    setNonceWidthInBits(crypto_stream_NONCEBYTES * 8);
    setMaxBufferSize(bufferSize);
    setPreferedBufferSize(bufferSize);
    setPreferedBufferSizeWithOverhead(bufferSizeWithOverhead);

}

SDPXSalsa20EncryptionAlgorithm::~SDPXSalsa20EncryptionAlgorithm(){

}


uint64 SDPXSalsa20EncryptionAlgorithm::decryptBuffer(uchar *encryptedBuffer, uchar *decryptedBuffer, uint64 encryptedBufferSize, uint64 chunkNum){

    crypto_stream_xsalsa20_xor_ic(decryptedBuffer, encryptedBuffer, encryptedBufferSize, (uchar*)nonceInBin.c_str(), chunkNum * blocksPerBuffer, (uchar*)encryptionKeyInBin.c_str());

    return encryptedBufferSize; //1 byte in, 1 byte out
}

uint64 SDPXSalsa20EncryptionAlgorithm::encryptBuffer(uchar *unencryptedBuffer, uchar *encryptedBuffer, uint64 unencryptedBufferSize, uint64 chunkNum){

    crypto_stream_xsalsa20_xor_ic(encryptedBuffer, unencryptedBuffer, unencryptedBufferSize, (uchar*)nonceInBin.c_str(), chunkNum * blocksPerBuffer, (uchar*)encryptionKeyInBin.c_str());

    return unencryptedBufferSize; //1 byte in, 1 byte out
}


bool SDPXSalsa20EncryptionAlgorithm::decryptStream(uchar encryptedChar,   uchar* decryptedChar, uint64 charNum){

}

bool SDPXSalsa20EncryptionAlgorithm::encryptStream(uchar unencryptedChar, uchar* encryptedChar, uint64 charNum){

}


void SDPXSalsa20EncryptionAlgorithm::onInit(){

}

void SDPXSalsa20EncryptionAlgorithm::onExit(){

}

void SDPXSalsa20EncryptionAlgorithm::onSync(){

}