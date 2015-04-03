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

#ifndef LIBSDP_SDPALGORITHMBASE_H
#define LIBSDP_SDPALGORITHMBASE_H

#include <string>

#include "HexBinTool.hpp"
#include "Typedefs.hpp"

//TODO: Add derivable copy constructor.

class SDPAlgorithmBase{

public:

    //Vars

    enum AlgorithmType{
        SDP_COMPRESSION,
        SDP_ENCRYPTION
    };

    //Funcs

    SDPAlgorithmBase();
    ~SDPAlgorithmBase();

    //Compress/Encrypt
    virtual uint64 processBuffer(uchar* unprocessedBuffer, uint64 unprocessedBufferSize, uchar* processedBuffer, uint64 expectedProcessedBufferSize, uint64 chunkNum);
    //Decompress/Decrypt
    virtual uint64 unprocessBuffer(uchar* processedBuffer, uint64 processedBufferSize,   uchar* unprocessedBuffer, uint64 expectedUnprocessedBufferSize, uint64 chunkNum);

    //Compress/Encrypt
    virtual bool processChar(uchar &unprocessedChar, uchar &processedChar, uint64 charNum);
    //Decompress/Decrypt
    virtual bool unprocessChar(uchar &processedChar, uchar &deprocessedChar, uint64 charNum);

    virtual void onInit() = 0;
    virtual void onSync() = 0;
    virtual void onExit() = 0;

    AlgorithmType getAlgorithmType();
    uint8         getAlgorithmID();
    bool          getIsStreamAlgorithm();
    bool          getIsAlgorithmSeekable();

    uint64 getBufferSize();
    uint64 getBufferSizeWithOverhead();

    //Only effective when algorithmType is set to SDP_ENCRYPTION
    virtual int setEncryptionKeyAndNonce(std::string encryptionKey, bool isEncryptionKeyInHex, std::string nonce, bool isNonceInHex);
    virtual int setEncryptionKey(std::string encryptionKey,         bool isEncryptionKeyInHex);
    virtual int setNonce(std::string nonce,                         bool isNonceInHex);



protected:

    //Vars

    std::string encryptionKeyInBin;
    std::string nonceInBin;


    //Funcs

    void setAlgorithmType(AlgorithmType algorithmType);
    void setAlgorithmID(uint8           algorithmID);
    void setIsStreamAlgorithm(bool      isStreamAlgorithm);
    void setIsAlgorithmSeekable(bool    isAlgorithmSeekable);

    void setMaxBufferSize(uint64                  maxBufferSize);
    bool setPreferedBufferSize(uint64             preferedBufferSize);
    bool setPreferedBufferSizeWithOverhead(uint64 preferedBufferSizeWithOverhead);

    //Only used when algorithmType is set to SDP_COMPRESSION
    void setCompressionLevel(uint64 compressionLevel);

    //Only used when algorithmType is set to SDP_ENCRYPTION

    //Should be set somewhere in the algorithm's constructor or onInit
    //Sets the supported key length in bits for the algorithm.
    void setEncryptionKeyWidthInBits(uint64 encryptionKeyWidthInBits);

    //Should be set somewhere in the algorithm's constructor or onInit
    //Sets the supported nonce length in bits for the algorithm.
    void setNonceWidthInBits(uint64 nonceWidthInBits);


private:

    //Vars

    AlgorithmType algorithmType;
    uint8         algorithmID;

    bool isStreamAlgorithm;
    bool isAlgorithmSeekable;

    //Maximum buffer size supported by the algorithm,
    //Used when isStreamCompression = false
    //and when algorithmType = SDP_COMPRESSION
    uint64 maxBufferSize;

    //Buffer size that will be used when isStreamCompression = false
    //This may be smaller than the prefered buffer size if it is set to be larger than
    //what the algorithm can handle.
    uint64 bufferSize = 0;

    //Buffer size including possible overhead on compression to be used when isStreamCompression = false
    //This is in case the data is incompressible and that the algorithm introduces an overhead.
    //This is to prevent an overflow and a SIGSEGV.
    uint64 bufferSizeWithOverhead = 0;

    uint64 compresseionLevel;

    uint64 encryptionKeyWidthInBits;
    uint64 nonceWidthInBits;

    //Funcs

};

#endif //LIBSDP_SDPALGORITHMBASE_H
