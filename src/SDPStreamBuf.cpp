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

#include <sodium/crypto_hash_sha512.h>
#include "SDPStreamBuf.hpp"



#ifndef SDP_DISABLE_READ

SDPStreamBuf::SDPStreamBufErrEnum SDPStreamBuf::openSDP(std::istream &inStream, bool skipHashCheck){

    //----------------------------------------------????
    if(isWritingToSDP){
        return SDP_ALREADY_CREATING_SDP;
    }

    //Check magic word
    for(int i = 0; i <= 3; i++){
        unsigned char tmpChar;
        rawFileIO.read(tmpChar, &inStream);
        if(tmpChar != magicWord[i]) return SDP_BAD_MAGIC_WORD;
    }

    //Get in file spec version and check compatibility
    rawFileIO.read(SDPFileProps.SDPSpecVerInFile.major, &inStream, RawFileIO::Endian::BIG__ENDIAN);
    rawFileIO.read(SDPFileProps.SDPSpecVerInFile.minor, &inStream, RawFileIO::Endian::BIG__ENDIAN);
    rawFileIO.read(SDPFileProps.SDPSpecVerInFile.patch, &inStream, RawFileIO::Endian::BIG__ENDIAN);

    if(SDPFileProps.SDPSpecVerInFile.major != SDPVersion.getSDPSpecVer().major){
        return SDP_UNSUPPORTED_SPEC;
    }else if(SDPFileProps.SDPSpecVerInFile.minor != SDPVersion.getSDPSpecVer().minor){
        return SDP_UNSUPPORTED_SPEC;
    }else if(SDPFileProps.SDPSpecVerInFile.patch != SDPVersion.getSDPSpecVer().patch){
        return SDP_UNSUPPORTED_SPEC;
    }


    int subFileCount = 0;

    //read subFile header and fill SDPSubFilePropsSytuct + SDPSubFileList
    while(true){

        SDPSubFilePropsStruct tmpSubFileProps;

        //get the sub file name
        uint_least32_t tmpSubFileNameLength;
        if(rawFileIO.read(tmpSubFileNameLength, &inStream, RawFileIO::Endian::BIG__ENDIAN) != sizeof tmpSubFileNameLength){
            break; //if sub file corrupted or incomplete, stop reading
        }

        tmpSubFileProps.SDPSubFileName.resize(tmpSubFileNameLength +1);
        inStream.read((char*) tmpSubFileProps.SDPSubFileName.data(), tmpSubFileNameLength);
        if(inStream.gcount() != tmpSubFileNameLength){
            break; //if sub file corrupted or incomplete, stop reading
        }

        //get file flag
        unsigned char tmpFlagChar;
        if(rawFileIO.read(tmpFlagChar, &inStream) != sizeof tmpFlagChar){
            break; //if sub file corrupted or incomplete, stop reading
        }

        if(tmpFlagChar == 0x01 || tmpFlagChar == 0x02 || tmpFlagChar == 0x04 || tmpFlagChar == 0x08 || tmpFlagChar == 0x20){
            tmpSubFileProps.SDPSubFileDataProps = static_cast<SDPSubFileDataPropsEnum> (tmpFlagChar);
        }else{
            tmpSubFileProps.SDPSubFileDataProps = SDPSubFileDataPropsEnum::SDP_NULL;
        }

        //get data length
        if(rawFileIO.read(tmpSubFileProps.SDPSubFileSize, &inStream, RawFileIO::Endian::BIG__ENDIAN) != sizeof tmpSubFileProps.SDPSubFileSize){
            break; //if sub file corrupted or incomplete, stop reading
        }

        //compute begpos and endpos for subfile
        tmpSubFileProps.dataBegPos = (uint_least64_t) inStream.tellg();
        tmpSubFileProps.dataEndPos = (uint_least64_t) inStream.tellg() + tmpSubFileProps.SDPSubFileSize;

        //compute sub file hash
        if(skipHashCheck){
            tmpSubFileProps.SDPSubFileHash = "";
        }else{

            crypto_hash_sha512_state hashState;
			crypto_hash_sha512_init(&hashState);

			HexBinTool hexBinTool;

            std::string digest;
			digest.resize(crypto_hash_sha512_BYTES);

            for(uint_least64_t i = 0; i < tmpSubFileProps.SDPSubFileSize; i++){
                unsigned char tmpChar;
                rawFileIO.read(tmpChar, &inStream);
				crypto_hash_sha512_update(&hashState, reinterpret_cast<const unsigned char*>(tmpChar), 1);
            }

			crypto_hash_sha512_final(&hashState, (unsigned char*)digest.data());
            hexBinTool.binToHex(digest, tmpSubFileProps.SDPSubFileHash);

        }

        SDPSubFileList.insert(std::make_pair(tmpSubFileProps.SDPSubFileName, tmpSubFileProps));

        subFileCount++;
    }

    SDPFileProps.numSubFiles = subFileCount;

    isReadingSDP = true;

    return SDP_NO_ERROR;
}

void SDPStreamBuf::closeSDP(){

    SDPFileProps.numSubFiles            = 0;
    //SDPFileProps.SDPFileName            = "";
    SDPFileProps.SDPSpecVerInFile.major = 0;
    SDPFileProps.SDPSpecVerInFile.minor = 0;
    SDPFileProps.SDPSpecVerInFile.patch = 0;

    inStream.close();
    isReadingSDP = false;

}

void SDPStreamBuf::SDPSetSubFile(std::string &SDPSubFileName){
    currentSubFileInUse = SDPSubFileName;
}

SDPStreamBuf::SDPFilePropsStruct SDPStreamBuf::getSDPFileProps(){

}

SDPStreamBuf::SDPSubFilePropsStruct SDPStreamBuf::getSDPSubFileProps(std::string SDPSubFileName){

}

#endif //SDP_DISABLE_READ


#ifndef SDP_DISABLE_WRITE

SDPStreamBuf::SDPStreamBufErrEnum SDPStreamBuf::createSDP(std::string SDPFileName){

}

SDPStreamBuf::SDPFilePropsStruct  SDPStreamBuf::packSDP(){

}


SDPStreamBuf::SDPSubFilePropsStruct SDPStreamBuf::addFileToSDP(std::string SDPSubFileName){

}

#endif //SDP_DISABLE_WRITE


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
