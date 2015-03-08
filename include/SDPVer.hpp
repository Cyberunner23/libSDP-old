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

#ifndef SDPVER_HPP
#define SDPVER_HPP


#include "typedefs.hpp"

#ifdef USE_IN_SOUL_ENGINE
namespace SE{
namespace IO{
#endif

class SDPVer{

public:

    //Vars
    const struct SDPLibVerStruct{
        uint8 major = 0;
        uint8 minor = 1;
        uint8 patch = 3;
    }SDPLibVer;

    const struct SDPSpecRevStruct{
        uint8 major = 2;
        uint8 minor = 3;
    }SDPSpecRev;

};

#ifdef USE_IN_SOUL_ENGINE
}
}
#endif

#endif // SDPVER_HPP

