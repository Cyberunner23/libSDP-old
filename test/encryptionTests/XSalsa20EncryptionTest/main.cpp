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

#include <iostream>
#include <sstream>

#include "common.hpp"
#include "SDPCryptStreamBuf.hpp"
#include "SDPXSalsa20EncryptionAlgorithm.hpp"

int main(){

    std::string unencryptedFile = "../../common/data/randomData64k";
    std::string encryptedFile   = "randomData64k_ENCRYPTED";
    std::string decryptedFile   = "randomData64k_DECRYPTED";

    std::shared_ptr<SDPXSalsa20EncryptionAlgorithm> encryptionAlgorithm = std::make_shared<SDPXSalsa20EncryptionAlgorithm>();

    encryptFile(unencryptedFile.c_str(), encryptedFile.c_str(), encryptionAlgorithm);
    decryptFile(encryptedFile.c_str(),   decryptedFile.c_str(), encryptionAlgorithm);

    if(!areFilesSame(unencryptedFile.c_str(), decryptedFile.c_str()))
        return -1;

    return 0;
}