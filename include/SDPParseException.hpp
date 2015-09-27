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

#ifndef LIBSDP_SDPPARSEEXCEPTION_HPP
#define LIBSDP_SDPPARSEEXCEPTION_HPP

#include <exception>
#include <string>

namespace libSDP{

    class SDPParseException : public std::exception{

    public:

        //Vars

        //Funcs

        SDPParseException(const char* whatArg){whatString = whatArg;}
        SDPParseException(std::string whatArg){whatString = whatArg.c_str();}
        ~SDPParseException(){}

        virtual const char* what() noexcept final{
            return whatString;
        }

    private:

        //Vars
        const char* whatString;

        //Funcs

    };
}

#endif //LIBSDP_SDPPARSEEXCEPTION_HPP
