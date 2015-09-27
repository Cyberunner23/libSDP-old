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

#include "SDPError.hpp"
#include "SDPParser.hpp"
#include "SDPStack.hpp"
#include "SDPVer.hpp"
#include "SourceSink/SDPSourceSinkBase.hpp"
#include "Utils/DevMacros.hpp"
#include "Utils/RawFileIO.hpp"
#include "Utils/Typedefs.hpp"

//TODO!: WRITE DOCUMENTATION FFS!!!
//TODO:  Redo tests.
//TODO:  Use UTF-8 within API and in file (for the file names.)
//TODO:  Dont forget to add checks so that the bottom layer buffers don't overlap.
//TODO:  Error reporting on createSDP()
//TODO:  Error reporting on setAlgorithm()
//TODO?: Use shared_ptr/unique_ptr in info functions?

namespace libSDP{

    using namespace libSDP::Utils;

    class SDP{

    public:

        //Vars

        //Funcs
        SDP();
        ~SDP();

        SDPErrEnum openSDP(std::string &SDPFileName);
        SDPErrEnum openSDP(std::shared_ptr<SDPSourceSinkBase> inOutStream);

        //Stack Modifiers.
        std::shared_ptr<SDPStack>          setStack(std::shared_ptr<SDPStack> stack);
        void                               setAlgorithmChain(std::shared_ptr<SDPChainBlockBase> algorithmChain);
        std::shared_ptr<SDPSourceSinkBase> getSourceSink();

        //File Info.
        SDPFileInfoStruct         *getSDPFileInfo();
        SDPSubContainerInfoStruct *getCurrentSubContainerInfo();
        SDPSubContainerInfoStruct *getSubContainerInfo(std::string &subContainerFileName);

        bool setSDPSubContainer(std::string &subContainerFileName, std::shared_ptr<std::iostream> algorithm = {});

#ifndef SDP_DISABLE_WRITE
        //Return type void for now. Must change later.
        void createSDP(std::string &SDPFileName, const std::vector<uchar> &extraField={});

        void createSDP(std::shared_ptr<SDPSourceSinkBase> inOutStream, const std::vector<uchar> &extraField = {});

        //void addSubContainerToSDP(std::string &SubContainerFileName,
        //                          compressionAlgorithmsEnum compAlg=NO_COMPERSSION_ALG,
        //                          encryptionAlgorithmsEnum encAlg=NO_ENCRYPTION_ALG,
        //                          const std::vector<uchar> &extraField={});

        void addSubContainerToSDP(std::string &SubContainerFileName, std::shared_ptr<std::iostream> algorithm,
                                  const std::vector<uchar> &extraField={});

        void finalizeSubContainer();

        void removeSubContainerFromSDP(std::string &subContainerName);
#endif

        //Read Write
#ifndef SDP_DISABLE_READ
        bool read(uchar *data, uint64 readSize);
        SDP &operator>>(uchar &c);
        uint64 getReadCount();
#endif

#ifndef SDP_DISABLE_WRITE
        bool write(uchar *data, uint64 writeSize);
        SDP &operator<<(uchar &c);
        uint64 getWriteCount();
#endif


    private:

        //Vars

        SDPParser parser;
        SDPParser::SDPFileInfoStruct SDPFileInfo;

        std::shared_ptr<std::iostream> SDPInOutStream;

        //Funcs


    };
}

#endif //_LIBSDP_SDP_H_

