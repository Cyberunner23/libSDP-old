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

#ifndef LIBSDP_SDPPARSER_H
#define LIBSDP_SDPPARSER_H

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#include "Utils/HexBinTool.hpp"
#include "Utils/RawFileIO.hpp"
#include "Utils/SHA256Hash.hpp"
#include "Utils/Typedefs.hpp"
#include "SDPError.hpp"
#include "SDPVer.hpp"

namespace libSDP{

    using namespace libSDP::Utils;

    class SDPParser{

    public:

        //Vars

        struct SDPSubContainerHeaderStruct{
            uint64 fileNameLength;
            std::string fileName;

            enum subContainerTypeEnum : uint8{
                DETERMINED_BY_API_CALL=0x00,
                RAW_DATA=0x01,
                COMPRESSED_DATA=0x02,
                ENCRYPTED_DATA=0x03
            } subContainerType;

            enum compressionAlgorithmIDEnum : uint8{
                COMPRESSION_DETERMINED_BY_API_CALL=0x00,
                COMPRESSION_ZLIB=0x01,
                COMPRESSION_LZ4=0x02
            } compressionAlgorithmID;

            enum encryptionAlgorithmIDEnum : uint8{
                ENCRYPTION_DETERMINED_BY_API_CALL=0x00,
                ENCRYPTION_XSALSA20=0x01,
                ENCRYPTION_XSALSA20_WITH_POLY1305=0x02,
                ENCRYPTION_SALSA20=0x03,
                ENCRYPTION_CHACHA20=0x04,
                ENCRYPTION_AES_128_CTR=0x05,
            } encryptionAlgorithmID;

            uint64 extraFieldSize;
            std::vector<uchar> extraField;
            uint64 subContainerDataSize;
            std::string expectedSubContainerDataHash;
            std::string actualSubContainerDataHash;
            std::string expectedSubContainerHeaderHash;
            std::string actualSubContainerHeaderHash;
        };

        struct SDPSubContainerInfoStruct{
            std::string subContainerFileName;
            SDPSubContainerHeaderStruct subContainerHeader;
            bool isHeaderValid;
            uint64 begDataPos;
            uint64 endDataPos;
        };

        struct SDPFileHeaderStruct{
            uint32 magicNumber;
            SDPVer::SDPSpecRevStruct SDPSpecRev;
            uint64 extraFieldSize;
            std::vector<uchar> extraField;
            std::string expectedHeaderHash;
            std::string actualHeaderHash;
        };

        struct SDPFileInfoStruct{
            SDPFileHeaderStruct SDPFileHeader;
            bool isHeaderValid;
            std::unordered_map<std::string, SDPSubContainerInfoStruct> subContainersInSDPFile;
            uint64 numOfSubContainers;
            SDPSubContainerInfoStruct currentSubContainerInUse;
            std::shared_ptr<std::iostream> currentAlgorithmInUse;
        };

        //Funcs

        SDPParser();
        ~SDPParser();

        SDPErrEnum parseSDPFile(std::shared_ptr<std::istream> inStream, SDPFileInfoStruct &SDPFileInfo);


    private:

        //Vars

        static const uint32 magicWord=0x53445041; //"SDPA"
        SDPVer              SDPVersion;

        RawFileIO rawFileIO;

        //Funcs

        SDPErrEnum parseSDPFileHeader(std::shared_ptr<std::istream> inStream,
                                      SDPParser::SDPFileHeaderStruct &SDPFileHeader);

        SDPErrEnum parseSDPSubContainer(std::shared_ptr<std::istream> inStream,
                                        SDPParser::SDPSubContainerInfoStruct &SDPSubContainerInfo);

    };
}

#endif //LIBSDP_SDPPARSER_H
