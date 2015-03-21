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

#ifndef SDPCOMPRESSIONSTREAMBUF_H
#define SDPCOMPRESSIONSTREAMBUF_H

#include <cstdio>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

#include "RawFileIO.hpp"
#include "SDPCompressionAlgorithmBase.hpp"

//FIXME?: Use of >> in a istream causes an error and to prematurely return eof.
//TODO:   Implement seeking.
//TODO:   Testing.
//TODO:   Include compression Algorithm ID in SDPCompressionAlgorithmInfoStruct
//TODO:   Add support for algorithms that depend on data already read.
//TODO?:  Implement << >> operator overloads.
//TODO?:  Implement xsgetn() xsputn()
//TODO?:  Use a iostream in constructor?
//TODO?:  Make lib thread safe/atomic?
//TODO?:  Use sub-container fragmentation for concurrent file write?

class SDPCompressionStreamBuf : public std::streambuf{

public:

    //Vars

    struct SDPCompressionAlgorithmInfoStruct{
        std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm;
        uint64                                       bufferSize;
        uint64                                       bufferSizeWithOverhead;
    };

    //Funcs
    SDPCompressionStreamBuf(std::shared_ptr<std::istream> compressedIn,  std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm);
    SDPCompressionStreamBuf(std::shared_ptr<std::ostream> compressedOut, std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm);
    ~SDPCompressionStreamBuf();

    //void SDPSetCompressionOptions(SDPCompressionOptions compressionOptions);
    void                              setCompressionAlgorithm(std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm, bool resetPosCounter = true);
    SDPCompressionAlgorithmInfoStruct getCompressionAlgorithmInfo();


protected:

    virtual int_type overflow(int_type ch = traits_type::eof());

    virtual int_type underflow();
    virtual int_type uflow();
    virtual int_type pbackfail(int_type ch);

    virtual int sync();


private:

    //Vars

    uchar nextChar;

    bool hasFirstBlockBeenRead;

    std::vector<uchar>::iterator bufferIterator;
    std::vector<uchar>           uncompressedBuffer;
    std::vector<uchar>           compressedBuffer;

    std::shared_ptr<std::istream> inStream;
    std::shared_ptr<std::ostream> outStream;
    RawFileIO                     rawFileIO;

    SDPCompressionAlgorithmInfoStruct currentCompressionAlgorithmInfo;
    uint64                            currentChunkNum;

    //Funcs

    SDPCompressionAlgorithmInfoStruct makeCompressionAlgorithmInfo(std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm);

    int_type getNextChar(bool doAdvance);
    int_type setNextChar(int_type ch);

    bool readAndDecompressNextChunk();
    void compressAndWriteNextChunk();


};


#endif // SDPCOMPRESSIONSTREAMBUF_H














