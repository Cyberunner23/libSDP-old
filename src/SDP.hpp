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

#ifndef SDP_H
#define SDP_H

#include <stdint.h>
#include <string>

#include "SDPVer.hpp"


#ifdef USE_IN_SOUL_ENGINE
namespace SE::IO::SDP{
#endif

class SDP{

    //TODO: Endian checking
    //TODO: Finish SDPStreamBuf
    //TODO: Make definitions for SDPCryptStreamBuf
    //TODO: MAke definitions for SDPCompressionStreamBuf

public:

    //Vars


    //Funcs

    void setEncryptKey(std::string AES256Key, std::string IV);//key and IV sent to CryptoStreamBuf


private:

    //Vars

    //Funcs

};

#ifdef USE_IN_SOUL_ENGINE
}
#endif

#endif // SDP_H
