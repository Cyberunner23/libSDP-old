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
namespace SE::IO{
#endif

class SDPVer{

public:

    struct SDPVerStruct{
        int major;
        int minor;
        int patch;
        int build;
    };

    struct SDPSpecVerStruct{
        unsigned char major;
        unsigned char minor;
        unsigned char patch;
    };


    SDPVer(){

        SDPLibVer.major = 0;
        SDPLibVer.minor = 0;
        SDPLibVer.patch = 1;
        SDPLibVer.build = 395;

        SDPSpecVer.major = 0;
        SDPSpecVer.minor = 0;
        SDPSpecVer.patch = 5;

    }

    SDPVerStruct getSDPVer(){
        return SDPLibVer;
    }

    SDPSpecVerStruct getSDPSpecVer(){
        return SDPSpecVer;
    }

private:

    SDPVerStruct SDPLibVer;
    SDPSpecVerStruct SDPSpecVer;

};

#ifdef USE_IN_SOUL_ENGINE
};
#endif

#endif // SDPVER_HPP
