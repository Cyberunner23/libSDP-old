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

#ifndef SDPSTREAMBUF_H
#define SDPSTREAMBUF_H

#include <fstream>
#include <memory>
#include <streambuf>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

//#include "cryptlib.h"
//#include "hex.h"
//#include "sha3.h"

#include "RawFileIO.hpp"
#include "SDPVer.hpp"

//FIXME: use a better way to detect eof while parsing SDP
//TODO: implement eof detection for hash compute loop
//TODO: use istream/ostream in constructor/class
//TODO: finish SDP open sequence
//TODO: implement virtual streambuf functions
//TODO: implement seeking
//TODO?: Remove disable read/write stuff?
//TODO?: add capability to add subfiles to existing SDP

class SDPStreamBuf : public std::streambuf{

public:

    //Vars

    enum SDPStreamBufErrEnum{
        SDP_NO_ERROR,
        SDP_BAD_MAGIC_WORD,
        SDP_UNSUPPORTED_SPEC,
        SDP_ALREADY_CREATING_SDP,
        SDP_ALREADY_READING_SDP
    };

    struct SDPFilePropsStruct{
        SDPVer::SDPSpecVerStruct SDPSpecVerInFile;
        int numSubFiles;
    };

    enum SDPSubFileDataPropsEnum{
        SDP_NONE                     = 1,
        SDP_COMPRESSED               = 2,
        SDP_ENCRYPTED                = 4,
        SDP_COMPRESSED_AND_ENCRYPTED = 8,
        SDP_NULL                     = 32
    };

    struct SDPSubFilePropsStruct{
        std::string             SDPSubFileName;
        std::string             SDPSubFileHash;
        uint_least64_t          SDPSubFileSize; //of raw data
        uint_least64_t          dataBegPos;     //of raw data
        uint_least64_t          dataEndPos;     //of raw data
        SDPSubFileDataPropsEnum SDPSubFileDataProps;
    };


    //Funcs


#ifndef SDP_DISABLE_READ //Why would any one want this? Anyways the option is here, also, probably the bad way to do this...

    //SDP reading funcs

    SDPStreamBufErrEnum openSDP(std::istream &inStream, bool skipHashCheck);
    void                closeSDP();

    void SDPSetSubFile(std::string &SDPSubFileName);

    SDPFilePropsStruct      getSDPFileProps();
    SDPSubFilePropsStruct   getSDPSubFileProps(std::string SDPSubFileName);

#endif //SDP_DISABLE_READ


#ifndef SDP_DISABLE_WRITE //Again, the option is here...

    //Add sub files to existing sdp
    //SDPStreamBufErrEnum openSDP(std::iostream &outStream, bool skipHashCheck);
    //void                closeSDP();

    //SDP creation
    SDPStreamBufErrEnum createSDP(std::string SDPFileName);
    SDPFilePropsStruct  packSDP();

    SDPSubFilePropsStruct addFileToSDP(std::string SDPSubFileName);

#endif //SDP_DISABLE_WRITE


protected:

    virtual std::streamsize xsputn(const char* charArray, std::streamsize numChars);
    virtual int_type        overflow(int_type ch = traits_type::eof());

    virtual std::streamsize showmanyc();
    virtual std::streamsize xsgetn(char* charArray, std::streamsize numChars);
    virtual int_type        underflow();
    virtual int_type        uflow();
    virtual int_type        pbackfail(int_type ch);

    virtual int sync();
    virtual std::streampos seekpos(std::streampos streamPos, std::ios_base::openmode mode);


private:

    //Vars

    SDPVer SDPVersion;
    RawFileIO rawFileIO;

    SDPFilePropsStruct SDPFileProps;
    std::string currentSubFileInUse;

    std::ifstream inStream;
    std::ofstream *SDPWriteStream;

    bool isReadingSDP  = false;
    bool isWritingToSDP = false;

    std::unordered_map<std::string, SDPSubFilePropsStruct> SDPSubFileList;

    const char magicWord[4] {'S', 'D', 'P', 'A'};

    //Funcs



};

#endif // SDPSTREAMBUF_H




















