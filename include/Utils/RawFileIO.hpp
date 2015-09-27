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

#ifndef RAWREAD_H
#define RAWREAD_H

#include <fstream>
#include <memory>
#include <stdint.h>

#include "SourceSink/SDPSourceSinkBase.hpp"
#include "DevMacros.hpp"
#include "Utils/Endianness.hpp"
#include "Utils/Typedefs.hpp"

namespace libSDP{
namespace Utils{

    using namespace SourceSink;

    class RawFileIO{

    public:

        //Vars

        //Endianness of the source fileStream
        enum Endian{
            LITTLE__ENDIAN,
            BIG__ENDIAN
        };

        //Funcs

        RawFileIO();
        ~RawFileIO();

        //Reads a value from a file and converts it to the system's
        //endianness if necessary
        size_t read(uint8  &val, std::shared_ptr<std::istream> inStream);
        size_t read(uint16 &val, std::shared_ptr<std::istream> inStream, Endian fileEndian);
        size_t read(uint32 &val, std::shared_ptr<std::istream> inStream, Endian fileEndian);
        size_t read(uint64 &val, std::shared_ptr<std::istream> inStream, Endian fileEndian);

        size_t read(uint8  &val, std::shared_ptr<SDPSourceSinkBase> sourceSink);
        size_t read(uint16 &val, std::shared_ptr<SDPSourceSinkBase> sourceSink, Endian fileEndian);
        size_t read(uint32 &val, std::shared_ptr<SDPSourceSinkBase> sourceSink, Endian fileEndian);
        size_t read(uint64 &val, std::shared_ptr<SDPSourceSinkBase> sourceSink, Endian fileEndian);

        //Writes a value from a file and converts it to the system's
        //endianness if necessary
        void write(uint8  &val, std::shared_ptr<std::ostream> outStream);
        void write(uint16 &val, std::shared_ptr<std::ostream> outStream, Endian fileEndian);
        void write(uint32 &val, std::shared_ptr<std::ostream> outStream, Endian fileEndian);
        void write(uint64 &val, std::shared_ptr<std::ostream> outStream, Endian fileEndian);

        void write(uint8  &val, std::shared_ptr<SDPSourceSinkBase> sourceSink);
        void write(uint16 &val, std::shared_ptr<SDPSourceSinkBase> sourceSink, Endian fileEndian);
        void write(uint32 &val, std::shared_ptr<SDPSourceSinkBase> sourceSink, Endian fileEndian);
        void write(uint64 &val, std::shared_ptr<SDPSourceSinkBase> sourceSink, Endian fileEndian);


    private:

        //Vars

        bool isSysBigEndian;
        Endianness endian;

        //Funcs

        //Does the endianness of the file and the system match?
        //True:  they match
        //False: they don't match
        bool isFileAndSysEndianSame(Endian fileEndian);
    };
}
}



#endif // RAWREAD_H
