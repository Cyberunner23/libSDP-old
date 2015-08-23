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
#include "SDPError.hpp"
#include "SDPParser.hpp"
#include "SDPVer.hpp"
#include "Typedefs.hpp"

//TODO!: WRITE DOCUMENTATION FFS!!!
//TODO:  Redo tests.
//TODO:  Use UTF-8 within API and in file (for the file names.)
//TODO:  Implement new algorithm stack idea (with double Buffers for squeezeIn features)
//TODO:  Add a switch for algorithms to determine if its a compression or encryption algorithm
//           so that the right block header is read/written.
//TODO:  Dont forget copy constructors on the unified algorithm block.
//TODO:  Dont forget to add checks so that the bottom layer buffers don't overlap.
//TODO:  Error reporting on createSDP()
//TODO:  Error reporting on setAlgorithm()
//TODO?: Use shared_ptr/unique_ptr in info functions?
//TODO?: Use SDP to recreate SDPStreamBuf? (SqueezeIn functions will be missing though...)

namespace libSDP{

    using namespace libSDP::Utils;

    class SDP{

    public:

        //Vars

        enum compressionAlgorithmsEnum{
            NO_COMPERSSION_ALG,
            LZ4,
            DEFLATE
        };

        enum encryptionAlgorithmsEnum{
            NO_ENCRYPTION_ALG,
            CHACHA20,
            SALSA20,
            XSALSA20,
            AES_128_CTR
        };

        //Funcs

        SDP();

        ~SDP();

        SDPErrEnum openSDP(std::string &SDPFileName);

        SDPErrEnum openSDP(std::shared_ptr<std::iostream> inOutStream);

        SDPParser::SDPFileInfoStruct *getSDPFileInfo();

        SDPParser::SDPSubContainerInfoStruct *getCurrentSubContainerInfo();

        SDPParser::SDPSubContainerInfoStruct *getSubContainerInfo(std::string &subContainerFileName);

        bool setSDPSubContainer(std::string &subContainerFileName, std::shared_ptr<std::iostream> algorithm={});

        //Return type void for now. Must change later.
        void createSDP(std::string &SDPFileName, const std::vector<uchar> &extraField={});

        void createSDP(std::shared_ptr<std::iostream> inOutStream, const std::vector<uchar> &extraField={});

        void addSubContainerToSDP(std::string &SubContainerFileName,
                                  compressionAlgorithmsEnum compAlg=NO_COMPERSSION_ALG,
                                  encryptionAlgorithmsEnum encAlg=NO_ENCRYPTION_ALG,
                                  const std::vector<uchar> &extraField={});

        void addSubContainerToSDP(std::string &SubContainerFileName, std::shared_ptr<std::iostream> algorithm,
                                  const std::vector<uchar> &extraField={});

        void finalizeSubContainer();

        void removeSubContainerFromSDP(std::string &subContainerName);

        std::shared_ptr<std::iostream> getInternalStream();

        void setAlgorithm(std::shared_ptr<std::iostream> algorithm);

        bool read(uchar *data, uint64 readSize=1);

        SDP &operator>>(uchar &c);

        uint64 getReadCount();

        bool write(uchar *data, uint64 writeSize=1);

        SDP &operator<<(uchar &c);

        uint64 getWriteCount();


    private:

        //Vars

        SDPParser parser;
        SDPParser::SDPFileInfoStruct SDPFileInfo;

        std::shared_ptr<std::iostream> SDPInOutStream;

        //Funcs


    };
}

#endif //_LIBSDP_SDP_H_

