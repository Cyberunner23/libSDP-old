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

#ifndef LIBSDP_SDPSOURCESINKBASE_HPP
#define LIBSDP_SDPSOURCESINKBASE_HPP

#include "Utils/Typedefs.hpp"

namespace libSDP {
namespace SourceSink{

    using namespace Utils;

    class SDPSourceSinkBase{

    public:

        //Vars

        //Funcs
        SDPSourceSinkBase(){}

        ~SDPSourceSinkBase(){}

        //Read Write
        virtual bool read(uchar *data, uint64 readSize) = 0;
        virtual SDPSourceSinkBase &operator>>(uchar &c) = 0;
        virtual uint64 getReadCount() = 0;

        virtual bool write(const uchar *data, uint64 writeSize) = 0;
        virtual SDPSourceSinkBase &operator<<(uchar &c) = 0;
        virtual uint64 getWriteCount() = 0;

        virtual void flush() = 0;
        virtual void seek(uint64 pos) = 0;
        virtual void close() = 0;

    private:

        //Vars

        //Funcs

    };
}
}

#endif //LIBSDP_SDPSOURCESINKBASE_HPP
