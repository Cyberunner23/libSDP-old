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
#include <stdint.h>
#include <streambuf>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "sodium.h"

#include "HexBinTool.hpp"
#include "RawFileIO.hpp"
#include "SDPVer.hpp"
#include "typedefs.hpp"

//FIXME: use a better way to detect eof while parsing SDP
//TODO: implement eof detection for hash compute loop
//TODO: finish SDP open sequence
//TODO: implement virtual streambuf functions
//TODO: implement seeking
//TODO?: Remove disable read/write stuff?
//TODO?: add capability to add subfiles to existing SDP

class SDPStreamBuf : public std::streambuf{

public:

    //Vars

	struct SDPFileHeaderStruct{
		uint32 				magicNumber;
		uint8  				SDPSpecRev;
		uint64 				extraFieldSize;
		std::vector<uchar> 	extraField;
		std::string 		expectedHeaderHash;
		std::string 		actualHeaderHash;
	};

	struct SDPSubContainerHeaderStruct{
		uint64                                       fileNameLength;
		std::string                                  fileName;
		bool 		                                 isContainingSubContainer;
		//For when theres another sub-container inside this one.
		std::shared_ptr<SDPSubContainerHeaderStruct> subContainerHeader;

		enum subContainerTypeEnum : uint8{
			RAW_DATA		= 0x01,
			COMPRESSED_DATA = 0x02,
			ENCRYPTED_DATA  = 0x03
		} subContainerType;

		enum compressionAlgorithmIDEnum : uint8{
			COMPRESSION_DETERMINED_BY_API_CALL = 0x00,
			COMPRESSION_ZLIB				   = 0x01,
			COMPRESSION_LZ4					   = 0x02
		} compressionAlgorithmID;

		enum encryptionAlgorithmIDEnum : uint8{
			ENCRYPTION_DETERMINED_BY_API_CALL = 0x00,
			ENCRYPTION_XSALSA20	   		   	  = 0x01,
			ENCRYPTION_XSALSA20_WITH_POLY1305 = 0x02,
			ENCRYPTION_SALSA20				  = 0x03,
			ENCRYPTION_CHACHA20               = 0x04,
			ENCRYPTION_AES_128_CTR            = 0x05,
		}encryptionAlgorithmID;

		uint64 				extraFieldSize;
		std::vector<uchar> 	extraField;
		uint64 				subContainerDataSize;
		std::string 		expectedSubContainerDataHash;
		std::string 		actualSubContainerDataHash;
		std::string 		expectedSubContainerHeaderHash;
		std::string 		actualSubContainerHeaderHash;
	};

	struct SDPSubContainerInfoStruct{
		std::string 				               subContainerFileName;
		SDPSubContainerHeaderStruct                subContainerHeader;
		bool                                       isContainingSubContainer;
		std::shared_ptr<SDPSubContainerInfoStruct> subContainerInfo; //for when the SubContainer contains another Subcontainer.
		uint64                                     begDataPos;
		uint64                                     endDataPos;
	};

	struct SDPFileInfoStruct{
		SDPFileHeaderStruct 								 	   	SDPFileHeader;
		std::unordered_map<std::string, SDPSubContainerInfoStruct> 	subContainersInSDPFile;
		uint64 												 		numOfSubContainers;
		SDPSubContainerInfoStruct 						     		currentSubContainerInUse;
	};

    enum SDPStreamBufErrEnum{
        SDP_NO_ERROR,
        SDP_INVALID_FILE_HEADER
	};


    //Funcs

    //SDP reading funcs

	SDPStreamBufErrEnum openSDP(std::shared_ptr<std::iostream> inOutStream);
	SDPFileInfoStruct*  getSDPFileInfo();

	SDPStreamBufErrEnum       createSDP();
	SDPSubContainerInfoStruct addSubContainerToSDPFile(std::string subContainerFileName);

	bool 					   setSubContainer(std::string &SubContainerFileName);
	SDPSubContainerInfoStruct* getCurrentSubContainerInfo();

    //SDP creation


    //SDPSubFilePropsStruct addFileToSDP(std::string SDPSubFileName);


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

	static const uint32 magicWord = 0x53445041;
	SDPVer SDPVersion;

	SDPFileInfoStruct SDPFileInfo;

	std::shared_ptr<std::istream> SDPInStream;
	std::shared_ptr<std::ostream> SDPOutStream;
    RawFileIO rawFileIO;


    //Funcs

	//Read the header at current stream position
	//returns true if header is valid, false otherwise.
	bool getSDPFileHeader(std::shared_ptr<std::istream> inStream, SDPFileHeaderStruct &SDPFileHeader);
	//Read the sub-container's header at current stream position
	//returns true if header is valid, false otherwise.
	bool getSDPSubContainerHeader(std::shared_ptr<std::istream> inStream, SDPSubContainerHeaderStruct &SDPSubContainerHeader);


};

#endif // SDPSTREAMBUF_H




















