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

#include "Specs/SDPSpecRev_1_0.hpp"

using namespace libSDP::Spec;

SDPSpecRev_1_0::SDPSpecRev_1_0(){}

SDPSpecRev_1_0::~SDPSpecRev_1_0(){}

bool SDPSpecRev_1_0::parseSDPFile(bool skipHashCheck){



}


void SDPSpecRev_1_0::parseSDPFileHeader(bool skipHashCheck){

    //TODO: dont forget to seek back to 0 after initial header parsing.

    //Check magic word.
    if(rawFileIO.read(fileInfo.SDPFileHeader.magicNumber, sourceSink, RawFileIO::BIG__ENDIAN) != sizeof(fileInfo.SDPFileHeader.magicNumber))
        throw SDPParseException("Invalid file header. (Unable to read Majic Word.)");
    if(fileInfo.SDPFileHeader.magicNumber != magicWord)
        throw SDPParseException("Invalid file header. (Magic Word mismatch.)");

    //Check spec rev.
    //major
    if(rawFileIO.read(fileInfo.SDPFileHeader.SDPSpecRev.major, sourceSink) != sizeof(fileInfo.SDPFileHeader.SDPSpecRev.major))
        throw SDPParseException("Invalid file header. (Unable to read Major Version.)");
    if(fileInfo.SDPFileHeader.SDPSpecRev.major != SDPSpecVerMajor)
        throw SDPParseException("Major behavioral bug: wrong parser was selected during initial parsing.");

    //minor
    if(rawFileIO.read(fileInfo.SDPFileHeader.SDPSpecRev.minor, sourceSink) != sizeof(fileInfo.SDPFileHeader.SDPSpecRev.minor))
        throw SDPParseException("Invalid file header. (Unable to read Major Version.)");
    if(fileInfo.SDPFileHeader.SDPSpecRev.minor != SDPSpecVerMinor)
        throw SDPParseException("Major behavioral bug: wrong parser was selected during initial parsing.");

    //check size of and read extra field
    if(rawFileIO.read(fileInfo.SDPFileHeader.extraFieldSize, sourceSink, RawFileIO::BIG__ENDIAN) != sizeof(fileInfo.SDPFileHeader.extraFieldSize))
        throw SDPParseException("Invalid file header. (Unable to read Extra Field size.)");
    fileInfo.SDPFileHeader.extraField.resize(fileInfo.SDPFileHeader.extraFieldSize);
    sourceSink.get()->read(fileInfo.SDPFileHeader.extraField.data(), fileInfo.SDPFileHeader.extraFieldSize);
    if(sourceSink.get()->getReadCount() != fileInfo.SDPFileHeader.extraFieldSize)
        throw SDPParseException("Invalid file header. (Unable to read Extra Field.)");

    //Get expected header hash.
    std::string expectedHashInFile;
    expectedHashInFile.resize(crypto_hash_sha256_BYTES);
    sourceSink.get()->read((uchar*)expectedHashInFile.data(), crypto_hash_sha256_BYTES);
    if(sourceSink.get()->getReadCount() != crypto_hash_sha256_BYTES)
        throw SDPParseException("Invalid file header. (Unable to read File Header hash.)");
    fileInfo.SDPFileHeader.expectedHeaderHash = expectedHashInFile;

    if(!skipHashCheck){
        //Compute actual header file.
        fileInfo.SDPFileHeader.actualHeaderHash = SHA256Hash().getHashStr(fileInfo.SDPFileHeader.magicNumber,
                                                                          fileInfo.SDPFileHeader.SDPSpecRev.major,
                                                                          fileInfo.SDPFileHeader.SDPSpecRev.minor,
                                                                          fileInfo.SDPFileHeader.extraFieldSize,
                                                                          fileInfo.SDPFileHeader.extraField);

        //Compare expected and actual header hash.
        if(fileInfo.SDPFileHeader.actualHeaderHash != fileInfo.SDPFileHeader.expectedHeaderHash)
            throw SDPParseException("Invalid file header. (Unable to read Extra Field size.)");
    }
}
void SDPSpecRev_1_0::parseSDPSubContainer(){

}