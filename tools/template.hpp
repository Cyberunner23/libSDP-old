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


#ifdef USE_IN_SOUL_ENGINE
namespace SE{
namespace IO{
#endif

class SDPVer{

public:

    struct SDPVerStruct{
        int major = <VER_MAJOR>;
        int minor = <VER_MINOR>;
        int patch = <VER_PATCH>;
        int build = <VER_BUILD>;
    }SDPLibVer;

    struct SDPSpecVerStruct{
        uint_least64_t major = 0;
        uint_least64_t minor = 0;
        uint_least64_t patch = 5;
    }SDPSpecVer;

    SDPVerStruct getSDPVer(){
        return SDPLibVer;
    }

    SDPSpecVerStruct getSDPSpecVer(){
        return SDPSpecVer;
    }

};

#ifdef USE_IN_SOUL_ENGINE
}
}
#endif

#endif // SDPVER_HPP

