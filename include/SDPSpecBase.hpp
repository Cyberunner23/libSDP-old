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

#ifndef LIBSDP_SDPSPECBASE_HPP
#define LIBSDP_SDPSPECBASE_HPP

#include <memory>
#include <unordered_map>
#include <vector>

#include "SDPSourceSinkBase.hpp"
#include "SDPVer.hpp"
#include "Typedefs.hpp"

namespace libSDP {

    using namespace libSDP::Utils;

    struct SDPSubContainerHeaderStruct{
        std::string        fileName;
        uint64             extraFieldSize;
        std::vector<uchar> extraField;
        uint64             subContainerDataSize;
        //TODO?: use a vector<uchar> for hashes?
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
        std::shared_ptr<std::iostream>                             currentAlgorithmInUse;
    };

    class SDPSpecBase {

    public:

        //Vars

        //Funcs
        SDPSpecBase(std::shared_ptr<SDPSourceSinkBase> sourceSink);
        ~SDPSpecBase();

        bool parseSDPFile();

        SDPFileInfoStruct* getFileInfo();

        void readChunk();
        void writeChunk();


    private:

        //Vars
        SDPFileInfoStruct                  fileInfo;
        std::shared_ptr<SDPSourceSinkBase> sourceSink;
        uint64                             readChunkNum;
        uint64                             writeChunkNum;

        //Funcs

    };
}

#endif //LIBSDP_SDPSPECBASE_HPP
