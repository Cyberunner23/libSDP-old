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

	//These functions will only be used if isStreamCipher = false
	virtual uint_least64_t decrypt(unsigned char* encryptedBuffer,   unsigned char* decryptedBuffer, uint_least64_t encryptedBufferSize);
	virtual uint_least64_t encrypt(unsigned char* unencryptedBuffer, unsigned char* encryptedBuffer, uint_least64_t unencryptedBufferSize);

	//These functions will only be used if isStreamCipher = true
	virtual bool decryptStream(unsigned char *encryptedChar,   unsigned char *unencryptedChar);
	virtual bool encryptStream(unsigned char *unencryptedChar, unsigned char *encryptedChar);

	virtual void onInit();
	virtual void onExit();
	virtual void onSync();


protected:

	//Vars

	//Funcs

private:

	//Vars

	//Funcs


};

#endif //SDPXSALSA20ENCRYPTIONALGORITHM_HPP