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

#ifndef LIBSDP_SDPSPECREV_1_0_HPP
#define LIBSDP_SDPSPECREV_1_0_HPP

#include "SDPParseException.hpp"
#include "Specs/SDPSpecBase.hpp"

namespace libSDP{
namespace Spec{

    class SDPSpecRev_1_0 : public SDPSpecBase{

    public:

        //Vars

        //Funcs

        SDPSpecRev_1_0();
        ~SDPSpecRev_1_0();

        virtual bool parseSDPFile(bool skipHashCheck = false) noexcept(false) final;

    private:

        //Vars

        static const uint32 magicWord = 0x53445041; //"SDPA"
        static const uint8  SDPSpecVerMajor = 1;
        static const uint8  SDPSpecVerMinor = 0;

        //Funcs

        void parseSDPFileHeader(bool skipHashCheck)   noexcept(false);
        void parseSDPSubContainer() noexcept(false);


    };
}
}


#endif //LIBSDP_SDPSPECREV_1_0_HPP
