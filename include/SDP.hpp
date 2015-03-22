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

#ifndef _LIBSDP_SDP_H_
#define _LIBSDP_SDP_H_

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "RawFileIO.hpp"
#include "SDPCompressionAlgorithmBase.hpp"
#include "SDPEncryptionAlgorithmBase.hpp"
#include "SDPVer.hpp"
#include "Typedefs.hpp"


//TODO:  Error reporting on createSDP()
//TODO:  Error reporting on setAlgorithm()
//TODO?: Use shared_ptr/unique_ptr in info functions?

class SDP{

public:

    //Vars

    struct SDPSubContainerHeaderStruct{
        uint64      fileNameLength;
        std::string fileName;

        enum subContainerTypeEnum : uint8{
            RAW_DATA        = 0x01,
            COMPRESSED_DATA = 0x02,
            ENCRYPTED_DATA  = 0x03
        } subContainerType;

        enum compressionAlgorithmIDEnum : uint8{
            COMPRESSION_DETERMINED_BY_API_CALL = 0x00,
            COMPRESSION_ZLIB                   = 0x01,
            COMPRESSION_LZ4                    = 0x02
        } compressionAlgorithmID;

        enum encryptionAlgorithmIDEnum : uint8{
            ENCRYPTION_DETERMINED_BY_API_CALL = 0x00,
            ENCRYPTION_XSALSA20               = 0x01,
            ENCRYPTION_XSALSA20_WITH_POLY1305 = 0x02,
            ENCRYPTION_SALSA20                = 0x03,
            ENCRYPTION_CHACHA20               = 0x04,
            ENCRYPTION_AES_128_CTR            = 0x05,
        }encryptionAlgorithmID;

        uint64             extraFieldSize;
        std::vector<uchar> extraField;
        uint64             subContainerDataSize;
        std::string        expectedSubContainerDataHash;
        std::string        actualSubContainerDataHash;
        std::string        expectedSubContainerHeaderHash;
        std::string        actualSubContainerHeaderHash;
    };

    struct SDPSubContainerInfoStruct{
        std::string                 subContainerFileName;
        SDPSubContainerHeaderStruct subContainerHeader;
        bool                        isHeaderValid;
        uint64                      begDataPos;
        uint64                      endDataPos;
    };

    struct SDPFileHeaderStruct{
        uint32                   magicNumber;
        SDPVer::SDPSpecRevStruct SDPSpecRev;
        uint64                   extraFieldSize;
        std::vector<uchar>       extraField;
        std::string              expectedHeaderHash;
        std::string              actualHeaderHash;
    };

    struct SDPFileInfoStruct{
        SDPFileHeaderStruct                                        SDPFileHeader;
        bool                                                       isHeaderValid;
        std::unordered_map<std::string, SDPSubContainerInfoStruct> subContainersInSDPFile;
        uint64                                                     numOfSubContainers;
        SDPSubContainerInfoStruct                                  currentSubContainerInUse;
    };

    enum SDPErrEnum{
        SDP_NO_ERROR,
        SDP_CANNOT_OPEN_FILE,
        SDP_INVALID_FILE_HEADER,
        SDP_INVALID_HEADER,
        SDP_EOS_REACHED
    };

    //Funcs

    SDP();
    ~SDP();

    SDPErrEnum openSDP(std::string &SDPFileName,                   bool stopOnParseErr = false);
    SDPErrEnum openSDP(std::shared_ptr<std::iostream> inOutStream, bool stopOnParseErr = false);

    SDPFileInfoStruct*         getSDPFileInfo();
    SDPSubContainerInfoStruct* getCurrentSubContainerInfo();
    SDPSubContainerInfoStruct* getSubContainerInfo(std::string &subContainerFileName);
    bool                       setSDPSubContainer(std::string &subContainerFileName, std::shared_ptr<std::iostream> algorithm = {});

    //Return type void for now. Must change later.
    void createSDP(std::string                    &SDPFileName,          std::vector<uchar> &extraField);
    void createSDP(std::shared_ptr<std::iostream> inOutStream,           std::vector<uchar> &extraField);
    void addSubContainerToSDP(std::string         &SubContainerFileName, std::vector<uchar> &extraField);

    std::shared_ptr<std::iostream> getInternalStream();
    void                           setAlgorithm(std::shared_ptr<std::iostream> algorithm);

    bool   read(uchar* data, uint64 readSize = 1);
    SDP&   operator>>(uchar &c);
    uint64 getReadCount();

    bool   write(uchar* data, uint64 writeSize = 1);
    SDP&   operator<<(uchar &c);
    uint64 getWriteCount();

    bool squeezeIn(uchar* data, uint64 writeSize = 1);
    SDP& operator<<=(uchar &c);


private:

    //Vars

    static const uint32 magicWord = 0x53445041; //"SDPA"
    SDPVer              SDPVersion;

    SDPFileInfoStruct SDPFileInfo;

    std::shared_ptr<std::iostream> SDPInOutStream;
    RawFileIO                      rawFileIO;

    //Funcs


    //Parse the header at current stream position
    SDPErrEnum parseSDPFileInfo(std::shared_ptr<std::istream> inStream, SDPFileHeaderStruct &SDPFileHeader);
    //Parse the sub-container's header at current stream position
    SDPErrEnum parseSubContainerInfo(std::shared_ptr<std::istream> inStream, SDPSubContainerInfoStruct &SDPSubContainerInfo);
    //Parse the whole SDP file at current stream porision.
    SDPErrEnum parseSDPFile(std::shared_ptr<std::istream> inStream, SDPFileInfoStruct &SDPFileInfo);


};

#endif //_LIBSDP_SDP_H_

