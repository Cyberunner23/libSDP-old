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


SDPCryptStreamBuf::SDPCryptStreamBuf(std::istream *cryptIn, std::shared_ptr<SDPEncryptionAlgorithmBase> cryptAlgorithm)
	: hasFirstBlockBeenRead(false),
	  inStream(cryptIn),
	  outStream(nullptr),
	  cryptAlgorithm(cryptAlgorithm),
	  nextChar(traits_type::eof())
	{

	this->cryptAlgorithm->onInit();

	isStreamCipher 		   = this->cryptAlgorithm->getIsStreamCipher();
	bufferSize     		   = this->cryptAlgorithm->getBufferSize();
	bufferSizeWithOverhead = this->cryptAlgorithm->getBufferSizeWithOverhead();

	if(!isStreamCipher){
		encryptedBuffer.resize(bufferSize);
		unencryptedBuffer.resize(bufferSize);
	}

}

SDPCryptStreamBuf::SDPCryptStreamBuf(std::ostream *cryptOut, std::shared_ptr<SDPEncryptionAlgorithmBase> cryptAlgorithm)
	: hasFirstBlockBeenRead(false),
	  inStream(nullptr),
	  outStream(cryptOut),
	  cryptAlgorithm(cryptAlgorithm),
	  nextChar(traits_type::eof())
	{

	this->cryptAlgorithm->onInit();

	isStreamCipher 		   = this->cryptAlgorithm->getIsStreamCipher();
	bufferSize     		   = this->cryptAlgorithm->getBufferSize();
	bufferSizeWithOverhead = this->cryptAlgorithm->getBufferSizeWithOverhead();

	if(!isStreamCipher){
		encryptedBuffer.resize(bufferSize);
		unencryptedBuffer.resize(bufferSize);
	}

}

SDPCryptStreamBuf::~SDPCryptStreamBuf(){
    cryptAlgorithm->onExit();
}


void SDPCryptStreamBuf::setEncryptionAlgorithm(std::shared_ptr<SDPEncryptionAlgorithmBase> cryptAlgorithm){

	this->cryptAlgorithm->onSync();
	this->cryptAlgorithm->onExit();

	this->cryptAlgorithm = cryptAlgorithm;
	this->cryptAlgorithm->onInit();

}


void SDPCryptStreamBuf::setEncryptionKeyAndNonce(std::string encryptionKey, bool isEncryptionKeyInHex, std::string nonce, bool isNonceInHex){
	cryptAlgorithm->setEncryptionKeyAndNonce(encryptionKey, isEncryptionKeyInHex, nonce, isNonceInHex);
}

void SDPCryptStreamBuf::setEncryptionKey(std::string encryptionKey, bool isEncryptionKeyInHex){
	cryptAlgorithm->setEncryptionKey(encryptionKey, isEncryptionKeyInHex);
}

void SDPCryptStreamBuf::setNonce(std::string nonce, bool isNonceInHex){
	cryptAlgorithm->setNonce(nonce, isNonceInHex);
}


SDPCryptStreamBuf::int_type SDPCryptStreamBuf::overflow(int_type ch){

    return setNextChar(ch);
}


SDPCryptStreamBuf::int_type SDPCryptStreamBuf::underflow(){

    return (int_type)getNextChar(false);
}

SDPCryptStreamBuf::int_type SDPCryptStreamBuf::uflow(){

    return (int_type)getNextChar(true);
}

SDPCryptStreamBuf::int_type SDPCryptStreamBuf::pbackfail(int_type ch){

    return traits_type::eof();
}


int SDPCryptStreamBuf::sync(){

    //send whats left in the buffer to be encrypted
	if(!isStreamCipher){

		if(outStream && unencryptedBuffer.size() != 0){

			RawFileIO rawFileIO;

			//encrypt whats left
			encryptedBuffer.resize(bufferSizeWithOverhead);
			uint_least64_t numBytesEncrypted = cryptAlgorithm->encrypt(unencryptedBuffer.data(), encryptedBuffer.data(), unencryptedBuffer.size());
			encryptedBuffer.resize(numBytesEncrypted);

			//write block size
			rawFileIO.write(numBytesEncrypted, outStream, RawFileIO::BIG__ENDIAN);
			//write block data
			outStream->write((char *)encryptedBuffer.data(), encryptedBuffer.size());

		}
	}

	cryptAlgorithm->onSync();

	outStream->flush();

    return 0;
}


int SDPCryptStreamBuf::getNextChar(bool doAdvance){

    //if nextChar was not reset (in underflow), return the buffered char.
    if(!doAdvance){
        return nextChar;
    }

	if(isStreamCipher){

		RawFileIO rawFileIO;

		unsigned char readByte;
		if(rawFileIO.read(readByte, inStream) != sizeof (unsigned char)){
			return traits_type::eof();
		}

		if(cryptAlgorithm->decryptStream(&readByte, reinterpret_cast<unsigned char*>(nextChar))){
			return nextChar;
		}else{
			return traits_type::eof();
		}

	}else{

		if(bufferIterator == unencryptedBuffer.end() || !hasFirstBlockBeenRead){

			if(!fillAndDecryptBuffer()){
				return traits_type::eof();
			}

			bufferIterator = unencryptedBuffer.begin();
			nextChar       = *bufferIterator;
			bufferIterator++;

			hasFirstBlockBeenRead = true;

			return nextChar;

		}else{

			nextChar = *bufferIterator;
			bufferIterator++;

			return nextChar;
		}
	}
}

bool SDPCryptStreamBuf::fillAndDecryptBuffer(){

	RawFileIO rawFileIO;

	unencryptedBuffer.clear();
	encryptedBuffer.clear();

	//check encrypted data size
	uint_least64_t numEncryptedBytes;
	if(rawFileIO.read(numEncryptedBytes, inStream, RawFileIO::BIG__ENDIAN) != sizeof numEncryptedBytes){
		return false;
	}

	//check if numEncryptedBytes makes sense, if so, read the data
	if(numEncryptedBytes > cryptAlgorithm->getBufferSize()){
		return false;
	}else{
		encryptedBuffer.resize(numEncryptedBytes);
		inStream->read((char*) encryptedBuffer.data(), numEncryptedBytes);
	}

	//check how many bytes were really read, if
	//that number is not what we told it to read,
	//something went wrong
	if(inStream->gcount() != numEncryptedBytes){
		return false;
	}

	//do decryption
	unencryptedBuffer.resize(bufferSizeWithOverhead);
	uint_least64_t numDecryptedBytes = cryptAlgorithm->decrypt(encryptedBuffer.data(), unencryptedBuffer.data(), numEncryptedBytes);
	unencryptedBuffer.resize(numDecryptedBytes);

	return true;
}


SDPCryptStreamBuf::int_type SDPCryptStreamBuf::setNextChar(int_type ch){

	RawFileIO rawFileIO;

	if(isStreamCipher){

		unsigned char writeByte = 0;
		cryptAlgorithm->encryptStream(reinterpret_cast<unsigned char*>(ch), &writeByte);

		rawFileIO.write(writeByte, outStream);

	}else{

		unencryptedBuffer.emplace_back(ch);

		if(unencryptedBuffer.size() == cryptAlgorithm->getBufferSize()){

			encryptedBuffer.resize(bufferSizeWithOverhead);

			uint_least64_t numEncryptedBytes = cryptAlgorithm->encrypt(unencryptedBuffer.data(), encryptedBuffer.data(), unencryptedBuffer.size());

			//write encrypted chunk size
			rawFileIO.write(numEncryptedBytes, outStream, RawFileIO::BIG__ENDIAN);
			//write encrypted data
			outStream->write((char*)encryptedBuffer.data(), numEncryptedBytes);

			unencryptedBuffer.clear();
			encryptedBuffer.clear();

		}
	}

    return traits_type::to_int_type(ch);
}


