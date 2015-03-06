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

#include <sodium/crypto_hash_sha256.h>
#include <string.h>
#include "SDPStreamBuf.hpp"


SDPStreamBuf::SDPStreamBufErrEnum SDPStreamBuf::openSDP(std::shared_ptr<std::iostream> inOutStream){

    memset(&SDPFileInfo, 0, sizeof(SDPFileInfoStruct));

    //Check file header.
    if(!getSDPFileHeader(inOutStream, SDPFileInfo.SDPFileHeader))
        return SDP_INVALID_FILE_HEADER;

    while(true){



        break;
    }

    return SDP_NO_ERROR;
}

/*void SDPStreamBuf::closeSDP(){ //put this king of stuff in flush()

    SDPFileProps.numSubFiles            = 0;
    //SDPFileProps.SDPFileName          = "";
    SDPFileProps.SDPSpecVerInFile.major = 0;
    SDPFileProps.SDPSpecVerInFile.minor = 0;
    SDPFileProps.SDPSpecVerInFile.patch = 0;

    inStream.close();
    isReadingSDP = false;

}*/

SDPStreamBuf::SDPFileInfoStruct* SDPStreamBuf::getSDPFileInfo(){
    return &SDPFileInfo;
}


SDPStreamBuf::SDPStreamBufErrEnum SDPStreamBuf::createSDP(){

}

SDPStreamBuf::SDPSubContainerInfoStruct SDPStreamBuf::addSubContainerToSDPFile(std::string subContainerFileName){

}


bool SDPStreamBuf::setSubContainer(std::string &SubContainerFileName){

}

SDPStreamBuf::SDPSubContainerInfoStruct* SDPStreamBuf::getCurrentSubContainerInfo(){
    return &SDPFileInfo.currentSubContainerInUse;
}



std::streamsize SDPStreamBuf::xsputn(const char* charArray, std::streamsize numChars){

}

SDPStreamBuf::int_type SDPStreamBuf::overflow(SDPStreamBuf::int_type ch /*= SDPStreamBuf::traits_type::eof()*/){

}

std::streamsize SDPStreamBuf::showmanyc(){

}

std::streamsize SDPStreamBuf::xsgetn(char* charArray, std::streamsize numChars){

}

SDPStreamBuf::int_type SDPStreamBuf::underflow(){

}

SDPStreamBuf::int_type SDPStreamBuf::uflow(){

}


SDPStreamBuf::int_type SDPStreamBuf::pbackfail(int_type ch){

}

int SDPStreamBuf::sync(){

}


std::streampos SDPStreamBuf::seekpos(std::streampos streamPos, std::ios_base::openmode mode){

}


bool SDPStreamBuf::getSDPFileHeader(std::shared_ptr<std::istream> inStream, SDPFileHeaderStruct &SDPFileHeader){

    bool isHeaderValid = true;
    HexBinTool hexBinTool;

    memset(&SDPFileHeader, 0, sizeof(SDPFileHeaderStruct));

    //Check magic word.
    if(rawFileIO.read(SDPFileHeader.magicNumber, inStream, RawFileIO::BIG__ENDIAN) != sizeof(SDPFileHeader.magicNumber) || SDPFileHeader.magicNumber != magicWord)
        isHeaderValid = false;

    //Check spec rev.
    if(rawFileIO.read(SDPFileHeader.SDPSpecRev.major, inStream) != sizeof(SDPFileHeader.SDPSpecRev.major) || SDPFileHeader.SDPSpecRev.major != SDPVersion.SDPSpecRev.major)
        isHeaderValid = false;
    if(rawFileIO.read(SDPFileHeader.SDPSpecRev.minor, inStream) != sizeof(SDPFileHeader.SDPSpecRev.minor) || SDPFileHeader.SDPSpecRev.minor != SDPVersion.SDPSpecRev.minor)
        isHeaderValid = false;

    //check size of and read extra field
    if(rawFileIO.read(SDPFileHeader.extraFieldSize, inStream, RawFileIO::BIG__ENDIAN) != sizeof(SDPFileHeader.extraFieldSize))
        isHeaderValid = false;
    SDPFileHeader.extraField.resize(SDPFileHeader.extraFieldSize);
    inStream.get()->read((char*)SDPFileHeader.extraField.data(), SDPFileHeader.extraFieldSize);
    if(inStream.get()->gcount() != SDPFileHeader.extraFieldSize)
        isHeaderValid = false;

    //Get expected header hash.
    std::string expectedHashInFile;
    expectedHashInFile.resize(crypto_hash_sha256_BYTES);
    inStream.get()->read((char*)expectedHashInFile.data(), crypto_hash_sha256_BYTES);
    if(inStream.get()->gcount() != crypto_hash_sha256_BYTES)
        isHeaderValid = false;
    hexBinTool.binToHex(expectedHashInFile, SDPFileHeader.expectedHeaderHash);

    //Compute actual header file.
    uchar digest[crypto_hash_sha256_BYTES];
    crypto_hash_sha256_state sha256Hash;
    //NOTE: This reinterpret_cast mess it to avoid "cast to pointer from integer of different size" warnings.
    crypto_hash_sha256_init(&sha256Hash);
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(SDPFileHeader.magicNumber),          sizeof(reinterpret_cast<uchar*>(SDPFileHeader.magicNumber)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(SDPFileHeader.SDPSpecRev.major),     sizeof(reinterpret_cast<uchar*>(SDPFileHeader.SDPSpecRev.major)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(SDPFileHeader.SDPSpecRev.minor),     sizeof(reinterpret_cast<uchar*>(SDPFileHeader.SDPSpecRev.minor)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(SDPFileHeader.extraFieldSize),       sizeof(reinterpret_cast<uchar*>(SDPFileHeader.extraFieldSize)));
    crypto_hash_sha256_update(&sha256Hash, SDPFileHeader.extraField.data(),                              sizeof(SDPFileHeader.extraField.data()));
    crypto_hash_sha256_final(&sha256Hash,  digest);

    //Compare expected and actual header hash.
    hexBinTool.binToHex(std::string(reinterpret_cast<char*>(digest), crypto_hash_sha256_BYTES), SDPFileHeader.actualHeaderHash);
    if(SDPFileHeader.actualHeaderHash != SDPFileHeader.expectedHeaderHash)
        isHeaderValid = false;

    return isHeaderValid;
}

bool SDPStreamBuf::getSDPSubContainerHeader(std::shared_ptr<std::istream> inStream, SDPSubContainerHeaderStruct &SDPSubContainerHeader){

    bool isHeaderValid = true;
    HexBinTool hexBinTool;

    memset(&SDPSubContainerHeader, 0, sizeof(SDPSubContainerHeaderStruct)); // hacky struct reset.

    //Get size of the name for the file stored in the sub-container.
    if(rawFileIO.read(SDPSubContainerHeader.fileNameLength, inStream, RawFileIO::BIG__ENDIAN) != sizeof(SDPSubContainerHeader.fileNameLength))
        isHeaderValid = false;

    //Get the name for the file stored in the sub-container.
    SDPSubContainerHeader.fileName.resize(SDPSubContainerHeader.fileNameLength);
    inStream.get()->read((char*)SDPSubContainerHeader.fileName.data(), SDPSubContainerHeader.fileNameLength);
    if(inStream.get()->gcount() != SDPSubContainerHeader.fileNameLength)
        isHeaderValid = false;

    //Does this sub-container contain another one?
    //uint8 isContainingSubContainerChar;
    //if(rawFileIO.read(isContainingSubContainerChar, inStream) != sizeof(isContainingSubContainerChar))
    //    isHeaderValid = false;
    //if(isContainingSubContainerChar == 0x00)
    //    SDPSubContainerHeader.isContainingSubContainer = false;
    //else if(isContainingSubContainerChar == 0xFF)
    //    SDPSubContainerHeader.isContainingSubContainer = true;
    //else
    //    isHeaderValid = false;

    //Get sub-container type
    uint8 subContainerTypeChar;
    if(rawFileIO.read(subContainerTypeChar, inStream) != sizeof(subContainerTypeChar))
        isHeaderValid = false;
    if(subContainerTypeChar < 0x01 || subContainerTypeChar > 0x03)
        isHeaderValid = false;
    SDPSubContainerHeader.subContainerType = static_cast<SDPSubContainerHeaderStruct::subContainerTypeEnum> (subContainerTypeChar);

    //Get compression algorithm ID.
    uint8 compressionAlgorithmIDChar;
    if(rawFileIO.read(compressionAlgorithmIDChar, inStream) != sizeof(compressionAlgorithmIDChar))
        isHeaderValid = false;
    if(compressionAlgorithmIDChar < 0x00 || compressionAlgorithmIDChar > 0x02)
        isHeaderValid = false;
    SDPSubContainerHeader.compressionAlgorithmID = static_cast<SDPSubContainerHeaderStruct::compressionAlgorithmIDEnum> (compressionAlgorithmIDChar);

    //Get Encryption Algorithm ID.
    uint8 encryptionAlgorithmIDChar;
    if(rawFileIO.read(encryptionAlgorithmIDChar, inStream) != sizeof(encryptionAlgorithmIDChar))
        isHeaderValid = false;
    if(encryptionAlgorithmIDChar < 0x00 || encryptionAlgorithmIDChar > 0x05)
        isHeaderValid = false;
    SDPSubContainerHeader.encryptionAlgorithmID = static_cast<SDPSubContainerHeaderStruct::encryptionAlgorithmIDEnum> (encryptionAlgorithmIDChar);

    //Get size of extra field and its contents
    if(rawFileIO.read(SDPSubContainerHeader.extraFieldSize, inStream, RawFileIO::BIG__ENDIAN) != sizeof(SDPSubContainerHeader.extraFieldSize))
        isHeaderValid = false;
    SDPSubContainerHeader.extraField.resize(SDPSubContainerHeader.extraFieldSize);
    inStream.get()->read((char*)SDPSubContainerHeader.extraField.data(), SDPSubContainerHeader.extraFieldSize);
    if(inStream.get()->gcount() != SDPSubContainerHeader.extraFieldSize)
        isHeaderValid = false;

    //Get size of sub-container data
    if(rawFileIO.read(SDPSubContainerHeader.subContainerDataSize, inStream, RawFileIO::BIG__ENDIAN) != sizeof(SDPSubContainerHeader.subContainerType))
        isHeaderValid = false;

    //Get expected hash of sub-container data.
    std::string expectedSubContainerDataHash;
    expectedSubContainerDataHash.resize(crypto_hash_sha256_BYTES);
    inStream.get()->read((char*)expectedSubContainerDataHash.data(), crypto_hash_sha256_BYTES);
    if(inStream.get()->gcount() != crypto_hash_sha256_BYTES)
        isHeaderValid = false;
    hexBinTool.binToHex(expectedSubContainerDataHash, SDPSubContainerHeader.expectedSubContainerDataHash);

    //Get expected hash of sub-container header.
    std::string expectedSubContainerHeaderHash;
    expectedSubContainerHeaderHash.resize(crypto_hash_sha256_BYTES);
    inStream.get()->read((char*)expectedSubContainerHeaderHash.data(), crypto_hash_sha256_BYTES);
    if(inStream.get()->gcount() != crypto_hash_sha256_BYTES)
        isHeaderValid = false;
    hexBinTool.binToHex(expectedSubContainerHeaderHash, SDPSubContainerHeader.expectedSubContainerHeaderHash);

    //Compute actual hash of sub-container header.
    uchar digest[crypto_hash_sha256_BYTES];
    crypto_hash_sha256_state sha256Hash;
    //NOTE: This reinterpret_cast mess it to avoid "cast to pointer from integer of different size" warnings.
    crypto_hash_sha256_init(&sha256Hash);
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(SDPSubContainerHeader.fileNameLength),                            sizeof(reinterpret_cast<uchar*>(SDPSubContainerHeader.fileNameLength)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<const uchar*>(SDPSubContainerHeader.fileName.data()),                     sizeof(reinterpret_cast<const uchar*>(SDPSubContainerHeader.fileName.data())));
    //crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(isContainingSubContainerChar),                                    sizeof(reinterpret_cast<uchar*>(isContainingSubContainerChar)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(subContainerTypeChar),                                            sizeof(reinterpret_cast<uchar*>(subContainerTypeChar)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(compressionAlgorithmIDChar),                                      sizeof(reinterpret_cast<uchar*>(compressionAlgorithmIDChar)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(encryptionAlgorithmIDChar),                                       sizeof(reinterpret_cast<uchar*>(encryptionAlgorithmIDChar)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(SDPSubContainerHeader.extraFieldSize),                            sizeof(reinterpret_cast<uchar*>(SDPSubContainerHeader.extraFieldSize)));
    crypto_hash_sha256_update(&sha256Hash, SDPSubContainerHeader.extraField.data(),                                                   sizeof(SDPSubContainerHeader.extraField.data()));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(SDPSubContainerHeader.subContainerDataSize),                      sizeof(reinterpret_cast<uchar*>(SDPSubContainerHeader.subContainerDataSize)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<const uchar*>(SDPSubContainerHeader.expectedSubContainerDataHash.data()), sizeof(reinterpret_cast<const uchar*>(SDPSubContainerHeader.expectedSubContainerDataHash.data())));
    crypto_hash_sha256_final(&sha256Hash,  digest);

    //Compare expected and actual sub-container header hash.
    hexBinTool.binToHex(std::string(reinterpret_cast<char*>(digest), crypto_hash_sha256_BYTES), SDPSubContainerHeader.actualSubContainerHeaderHash);
    if(SDPSubContainerHeader.actualSubContainerHeaderHash != SDPSubContainerHeader.expectedSubContainerHeaderHash)
        isHeaderValid = false;

    //Compute actual hash for sub-container data
    crypto_hash_sha256_state sha256HashData;
    uchar digestData[crypto_hash_sha256_BYTES];
    pos_type posBeforeHashCheck = inStream.get()->tellg();

    for(int i = 0; i < SDPSubContainerHeader.subContainerDataSize; i++){
        uchar tmp = 0;
        inStream.get()->read(reinterpret_cast<char*>(tmp), sizeof(reinterpret_cast<char*>(tmp)));
        crypto_hash_sha256_update(&sha256HashData, &tmp, sizeof(&tmp));
    }
    crypto_hash_sha256_final(&sha256HashData, digestData);
    hexBinTool.binToHex(std::string(reinterpret_cast<char*>(digestData)), SDPSubContainerHeader.actualSubContainerDataHash);

    //Compare expected and actual sub-container data hash.
    if(SDPSubContainerHeader.actualSubContainerDataHash != SDPSubContainerHeader.expectedSubContainerDataHash)
        isHeaderValid = false;

    inStream.get()->seekg(posBeforeHashCheck, inStream.get()->beg);//Return to the end of the top sub-container's header.

    return isHeaderValid;
}






















