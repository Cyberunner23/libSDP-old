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

#ifndef SDPENCRYPTIONALGORITHMBASE_H
#define SDPENCRYPTIONALGORITHMBASE_H

#include <stdint.h>
#include <string>

#include "HexBinTool.hpp"
#include "typedefs.hpp"

#include "sodium.h"

class SDPEncryptionAlgorithmBase{


public:

    //vars

    //funcs

    SDPEncryptionAlgorithmBase();
    ~SDPEncryptionAlgorithmBase();

    virtual uint64 decrypt(unsigned char* encryptedBuffer,   unsigned char* decryptedBuffer, uint64 encryptedBufferSize,   uint64 chunkNum) = 0;
    virtual uint64 encrypt(unsigned char* unencryptedBuffer, unsigned char* encryptedBuffer, uint64 unencryptedBufferSize, uint64 chunkNum) = 0;

	virtual void onInit() = 0;
	virtual void onExit() = 0;
    virtual void onSync() = 0;

    virtual int setEncryptionKeyAndNonce(std::string encryptionKey, bool isEncryptionKeyInHex, std::string nonce, bool isNonceInHex);
	virtual int setEncryptionKey(std::string encryptionKey, bool isEncryptionKeyInHex);
	virtual int setNonce(std::string nonce, bool isNonceInHex);

    uint8          getEncryptionAlgorithmID();
	bool 		   getIsStreamCipher();
	uint64 getBufferSize();
	uint64 getBufferSizeWithOverhead();

protected:

    //vars
	std::string encryptionKeyInBin;
    std::string nonceInBin;

    //funcs

    //Should be set somewhere in the algorithm's constructor or onInit?
    void setEncryptionAlgorithmID(uint8 algorithmID);

	//Should be set somewhere in the algorithm's constructor or onInit?
	//true = stream cipher, false = block cipher.
	void setIsStreamCipher(bool isStreamCipher);

	//Should be set somewhere in the algorithm's constructor or onInit
	//Sets the supported key length in bits for the algorithm.
	void setEncryptionKeyWidthInBits(unsigned encryptionKeyWidthInBits);

	//Should be set somewhere in the algorithm's constructor or onInit
	//Sets the supported nonce length in bits for the algorithm.
	void setNonceWidthInBits(unsigned int nonceWidthInBits);

	//Should be set somewhere in the algorithm's constructor or onInit? after setIsStreamCipher(true/false)
	//Sets the maximum buffer size supported by the algorithm.
	//Used when isStreamCipher = false
	void setMaxBufferSize(uint64 maxBufferSize);

	//Should be set somewhere in the algorithm's constructor or onInit? after setIsStreamCipher(true/false)
	//and setMaxBufferSize(uint_least64_t)
	//Sets the buffer size to be used when isStreamCipher = false (block cipher)
	bool setPreferedBufferSize(uint64 preferedBufferSize = 1024);

	//Sets the buffer size including possible overhead. To be used when isStreamCipher = false
	//This is in case the algorithm adds extra data on encryption like authentication tags.
	//This is to prevent an overflow and a SIGSEGV.
	void setPreferedBufferSizeWithOverhead(uint64 preferedBufferSizeWithOverhead);


private:

    //vars

    uint8 encryptionAlgorithmID;

	//true = stream cipher, false = block cipher
	bool isStreamCipher;

	unsigned int encryptionKeyWidthInBits;
	unsigned int nonceWidthInBits;

	//Maximum buffer size supported by the algorithm.
	//Used when isStreamCipher = false
	uint64 maxBufferSize;

	//Buffer size to be used when isStreamCipher = false (block cipher)
	//This may be smaller than the prefered buffer size if
	//it is set to be larger than what the algorithm can handle.
	uint64 bufferSize = 0;

	//Buffer size including possible overhead. To be used when isStreamCipher = false
	//This is in case the algorithm adds extra data on encryption like authentication tags.
	//This is to prevent an overflow and a SIGSEGV.
	uint64 bufferSizeWithOverhead = 0;

    //funcs


};

#endif // SDPENCRYPTIONALGORITHMBASE_H
