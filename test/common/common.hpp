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

#ifndef _LIBSDP_COMMON_HPP_
#define _LIBSDP_COMMON_HPP_


#include <fstream>
#include <iostream>
#include <memory>

#include "SDPCryptStreamBuf.hpp"
#include "SDPCompressionStreamBuf.hpp"
#include "SDPEncryptionAlgorithmBase.hpp"
#include "SDPCompressionAlgorithmBase.hpp"
#include "Typedefs.hpp"

bool compressFile(const char* sourceFileName,   const char* destFileName, std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm);
bool decompressFile(const char* sourceFileName, const char* destFileName, std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm);

bool encryptFile(const char* sourceFileName, const char* destFileName, std::shared_ptr<SDPEncryptionAlgorithmBase> encryptionAlgorithm);
bool decryptFile(const char* sourceFileName, const char* destFileName, std::shared_ptr<SDPEncryptionAlgorithmBase> encryptionAlgorithm);

bool areFilesSame(const char* file1, const char* file2);


#endif //_LIBSDP_COMMON_HPP_
