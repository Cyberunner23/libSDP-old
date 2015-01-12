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

#ifndef RAWREAD_H
#define RAWREAD_H

#include <fstream>
#include <memory>
#include <stdint.h>

#include "src/Endianness.hpp"

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

    //Reads a value from a file and converts it to the system's
    //endianness if necessary
    size_t read(uint8_t &val, std::istream *inStream);
    size_t read(uint_least16_t &val, std::istream *inStream, Endian fileEndian);
    size_t read(uint_least32_t &val, std::istream *inStream, Endian fileEndian);
    size_t read(uint_least64_t &val, std::istream *inStream, Endian fileEndian);

    //Writes a value from a file and converts it to the system's
    //endianness if necessary
    void write(uint8_t val, std::ostream *outStream);
    void write(uint_least16_t val, std::ostream *outStream, Endian fileEndian);
    void write(uint_least32_t &val, std::ostream *outStream, Endian fileEndian);
    void write(uint_least64_t &val, std::ostream *outStream, Endian fileEndian);


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

#endif // RAWREAD_H
