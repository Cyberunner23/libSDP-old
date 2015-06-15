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

#include "SDPParser.hpp"

SDPParser::SDPParser(){}

SDPParser::~SDPParser(){}


SDPErrEnum SDPParser::parseSDPFile(std::shared_ptr<std::istream> inStream, SDPFileInfoStruct &SDPFileInfo){

    SDPFileInfo = {};

    //Check file header.
    if(parseSDPFileHeader(inStream, SDPFileInfo.SDPFileHeader) != SDPErrEnum::NO_ERR)
        return SDPErrEnum::INVALID_FILE_HEADER;

    //Loop through the sub-content headers.
    while(true){

        SDPSubContainerInfoStruct tmpSubContainerInfo = {};
        SDPErrEnum retVal                        = parseSDPSubContainer(inStream, tmpSubContainerInfo);
        if(retVal != EOS_REACHED){
            //Add Sub-Container info to SDPFileInfo even in its invalid (internally marked as invalid)
            SDPFileInfo.subContainersInSDPFile.insert(std::pair<std::string, SDPSubContainerInfoStruct>(tmpSubContainerInfo.subContainerFileName, tmpSubContainerInfo));
            SDPFileInfo.numOfSubContainers++;
            //Skip to the next Sub-Container header (end of the current Sub-Container's data section.)
            inStream.get()->seekg(tmpSubContainerInfo.endDataPos, inStream.get()->beg);
        }else{
            //If EOS reached then ignore the incomplete header and finish with parsing headers.
            break;
        }
    }

    return SDPErrEnum::NO_ERR;
}


SDPErrEnum SDPParser::parseSDPFileHeader(std::shared_ptr<std::istream> inStream, SDPParser::SDPFileHeaderStruct &SDPFileHeader){

    bool isHeaderValid = true;
    HexBinTool hexBinTool;

    SDPFileHeader = {};

    //Check magic word.
    if(rawFileIO.read(SDPFileHeader.magicNumber, inStream, RawFileIO::BIG__ENDIAN) != sizeof(SDPFileHeader.magicNumber))
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.
    if(SDPFileHeader.magicNumber != magicWord)
        isHeaderValid = false;

    //Check spec rev.
    //major
    if(rawFileIO.read(SDPFileHeader.SDPSpecRev.major, inStream) != sizeof(SDPFileHeader.SDPSpecRev.major))
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.
    if(SDPFileHeader.SDPSpecRev.major != SDPVersion.SDPSpecRev.major)
        isHeaderValid = false;
    //minor
    if(rawFileIO.read(SDPFileHeader.SDPSpecRev.minor, inStream) != sizeof(SDPFileHeader.SDPSpecRev.minor))
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.
    if(SDPFileHeader.SDPSpecRev.minor != SDPVersion.SDPSpecRev.minor)
        isHeaderValid = false;

    //check size of and read extra field
    if(rawFileIO.read(SDPFileHeader.extraFieldSize, inStream, RawFileIO::BIG__ENDIAN) != sizeof(SDPFileHeader.extraFieldSize))
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.
    SDPFileHeader.extraField.resize(SDPFileHeader.extraFieldSize);
    inStream.get()->read((char*)SDPFileHeader.extraField.data(), SDPFileHeader.extraFieldSize);
    if(inStream.get()->gcount() != SDPFileHeader.extraFieldSize)
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.

    //Get expected header hash.
    std::string expectedHashInFile;
    expectedHashInFile.resize(crypto_hash_sha256_BYTES);
    inStream.get()->read((char*)expectedHashInFile.data(), crypto_hash_sha256_BYTES);
    if(inStream.get()->gcount() != crypto_hash_sha256_BYTES)
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.
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

    return isHeaderValid ? SDPErrEnum::NO_ERR : SDPErrEnum::INVALID_HEADER;
}

SDPErrEnum SDPParser::parseSDPSubContainer(std::shared_ptr<std::istream> inStream, SDPParser::SDPSubContainerInfoStruct &SDPSubContainerInfo){

    //Check Sub-Container header.
    SDPSubContainerInfo = {};

    bool isHeaderValid = true;
    HexBinTool hexBinTool;

    //Get size of the name for the file stored in the sub-container.
    if(rawFileIO.read(SDPSubContainerInfo.subContainerHeader.fileNameLength, inStream, RawFileIO::BIG__ENDIAN) != sizeof(SDPSubContainerInfo.subContainerHeader.fileNameLength))
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.

    //Get the name for the file stored in the sub-container.
    SDPSubContainerInfo.subContainerHeader.fileName.resize(SDPSubContainerInfo.subContainerHeader.fileNameLength);
    inStream.get()->read((char*)SDPSubContainerInfo.subContainerHeader.fileName.data(), SDPSubContainerInfo.subContainerHeader.fileNameLength);
    if(inStream.get()->gcount() != SDPSubContainerInfo.subContainerHeader.fileNameLength)
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.

    //Get sub-container type
    uint8 subContainerTypeChar;
    if(rawFileIO.read(subContainerTypeChar, inStream) != sizeof(subContainerTypeChar))
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.
    if(subContainerTypeChar < 0x01 || subContainerTypeChar >= 0x04)
        isHeaderValid = false;
    SDPSubContainerInfo.subContainerHeader.subContainerType = static_cast<SDPSubContainerHeaderStruct::subContainerTypeEnum> (subContainerTypeChar);

    //Get compression algorithm ID.
    uint8 compressionAlgorithmIDChar;
    if(rawFileIO.read(compressionAlgorithmIDChar, inStream) != sizeof(compressionAlgorithmIDChar))
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.
    SDPSubContainerInfo.subContainerHeader.compressionAlgorithmID = static_cast<SDPSubContainerHeaderStruct::compressionAlgorithmIDEnum> (compressionAlgorithmIDChar);

    //Get Encryption Algorithm ID.
    uint8 encryptionAlgorithmIDChar;
    if(rawFileIO.read(encryptionAlgorithmIDChar, inStream) != sizeof(encryptionAlgorithmIDChar))
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.
    SDPSubContainerInfo.subContainerHeader.encryptionAlgorithmID = static_cast<SDPSubContainerHeaderStruct::encryptionAlgorithmIDEnum> (encryptionAlgorithmIDChar);

    //Get size of extra field and its contents
    if(rawFileIO.read(SDPSubContainerInfo.subContainerHeader.extraFieldSize, inStream, RawFileIO::BIG__ENDIAN) != sizeof(SDPSubContainerInfo.subContainerHeader.extraFieldSize))
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.
    SDPSubContainerInfo.subContainerHeader.extraField.resize(SDPSubContainerInfo.subContainerHeader.extraFieldSize);
    inStream.get()->read((char*)SDPSubContainerInfo.subContainerHeader.extraField.data(), SDPSubContainerInfo.subContainerHeader.extraFieldSize);
    if(inStream.get()->gcount() != SDPSubContainerInfo.subContainerHeader.extraFieldSize)
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.

    //Get size of sub-container data
    if(rawFileIO.read(SDPSubContainerInfo.subContainerHeader.subContainerDataSize, inStream, RawFileIO::BIG__ENDIAN) != sizeof(SDPSubContainerInfo.subContainerHeader.subContainerType))
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.

    //Get expected hash of sub-container data.
    std::string expectedSubContainerDataHash;
    expectedSubContainerDataHash.resize(crypto_hash_sha256_BYTES);
    inStream.get()->read((char*)expectedSubContainerDataHash.data(), crypto_hash_sha256_BYTES);
    if(inStream.get()->gcount() != crypto_hash_sha256_BYTES)
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.
    hexBinTool.binToHex(expectedSubContainerDataHash, SDPSubContainerInfo.subContainerHeader.expectedSubContainerDataHash);

    //Get expected hash of sub-container header.
    std::string expectedSubContainerHeaderHash;
    expectedSubContainerHeaderHash.resize(crypto_hash_sha256_BYTES);
    inStream.get()->read((char*)expectedSubContainerHeaderHash.data(), crypto_hash_sha256_BYTES);
    if(inStream.get()->gcount() != crypto_hash_sha256_BYTES)
        return SDPErrEnum::EOS_REACHED; //Assume end of stream.
    hexBinTool.binToHex(expectedSubContainerHeaderHash, SDPSubContainerInfo.subContainerHeader.expectedSubContainerHeaderHash);

    //Compute actual hash of sub-container header.
    uchar digest[crypto_hash_sha256_BYTES];
    crypto_hash_sha256_state sha256Hash;
    //NOTE: This reinterpret_cast mess it to avoid "cast to pointer from integer of different size" warnings.
    crypto_hash_sha256_init(&sha256Hash);
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(SDPSubContainerInfo.subContainerHeader.fileNameLength),                            sizeof(reinterpret_cast<uchar*>(SDPSubContainerInfo.subContainerHeader.fileNameLength)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<const uchar*>(SDPSubContainerInfo.subContainerHeader.fileName.data()),                     sizeof(reinterpret_cast<const uchar*>(SDPSubContainerInfo.subContainerHeader.fileName.data())));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(subContainerTypeChar),                                                             sizeof(reinterpret_cast<uchar*>(subContainerTypeChar)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(compressionAlgorithmIDChar),                                                       sizeof(reinterpret_cast<uchar*>(compressionAlgorithmIDChar)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(encryptionAlgorithmIDChar),                                                        sizeof(reinterpret_cast<uchar*>(encryptionAlgorithmIDChar)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(SDPSubContainerInfo.subContainerHeader.extraFieldSize),                            sizeof(reinterpret_cast<uchar*>(SDPSubContainerInfo.subContainerHeader.extraFieldSize)));
    crypto_hash_sha256_update(&sha256Hash, SDPSubContainerInfo.subContainerHeader.extraField.data(),                                                   sizeof(SDPSubContainerInfo.subContainerHeader.extraField.data()));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<uchar*>(SDPSubContainerInfo.subContainerHeader.subContainerDataSize),                      sizeof(reinterpret_cast<uchar*>(SDPSubContainerInfo.subContainerHeader.subContainerDataSize)));
    crypto_hash_sha256_update(&sha256Hash, reinterpret_cast<const uchar*>(SDPSubContainerInfo.subContainerHeader.expectedSubContainerDataHash.data()), sizeof(reinterpret_cast<const uchar*>(SDPSubContainerInfo.subContainerHeader.expectedSubContainerDataHash.data())));
    crypto_hash_sha256_final(&sha256Hash,  digest);

    //Compare expected and actual sub-container header hash.
    hexBinTool.binToHex(std::string(reinterpret_cast<char*>(digest), crypto_hash_sha256_BYTES), SDPSubContainerInfo.subContainerHeader.actualSubContainerHeaderHash);
    if(SDPSubContainerInfo.subContainerHeader.actualSubContainerHeaderHash != SDPSubContainerInfo.subContainerHeader.expectedSubContainerHeaderHash)
        isHeaderValid = false;

    //Compute actual hash for sub-container data
    crypto_hash_sha256_state sha256HashData;
    uchar digestData[crypto_hash_sha256_BYTES];
    std::ios::pos_type posBeforeHashCheck = inStream.get()->tellg();

    for(int i = 0; i < SDPSubContainerInfo.subContainerHeader.subContainerDataSize; i++){
        uchar tmp = 0;
        inStream.get()->read(reinterpret_cast<char*>(tmp), sizeof(reinterpret_cast<char*>(tmp)));
        crypto_hash_sha256_update(&sha256HashData, &tmp, sizeof(&tmp));
        if(inStream.get()->gcount() != sizeof(reinterpret_cast<char*>(tmp)))
            return SDPErrEnum::EOS_REACHED; //Assume end of stream.
    }
    crypto_hash_sha256_final(&sha256HashData, digestData);
    hexBinTool.binToHex(std::string(reinterpret_cast<char*>(digestData)), SDPSubContainerInfo.subContainerHeader.actualSubContainerDataHash);

    //Compare expected and actual sub-container data hash.
    if(SDPSubContainerInfo.subContainerHeader.actualSubContainerDataHash != SDPSubContainerInfo.subContainerHeader.expectedSubContainerDataHash)
        isHeaderValid = false;

    //Return to the end of the top sub-container's header.
    inStream.get()->seekg(posBeforeHashCheck, inStream.get()->beg);


    //Fill the rest of sub-container info
    SDPSubContainerInfo.subContainerFileName = SDPSubContainerInfo.subContainerHeader.fileName;
    SDPSubContainerInfo.isHeaderValid        = isHeaderValid;
    SDPSubContainerInfo.begDataPos           = (uint64)inStream.get()->tellg();
    SDPSubContainerInfo.endDataPos           = (uint64)inStream.get()->tellg() + SDPSubContainerInfo.subContainerHeader.subContainerDataSize;

    return isHeaderValid ? SDPErrEnum::NO_ERR : SDPErrEnum::INVALID_HEADER;
}