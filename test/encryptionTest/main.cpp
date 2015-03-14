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

#include <iostream>
#include <sstream>

#include "SDPCryptStreamBuf.hpp"
#include "SDPXSalsa20EncryptionAlgorithm.hpp"





void decryptFile(const char* sourceFileName, const char* destFileName){

    std::shared_ptr<std::ifstream> inStream = std::make_shared<std::ifstream>();
    std::shared_ptr<std::ofstream> outStream = std::make_shared<std::ofstream>();
    inStream.get()->open(sourceFileName, std::ios_base::in);
    outStream.get()->open(destFileName, std::ios::out);

    if (inStream.get()->is_open()) {
        if (outStream.get()->is_open()) {

            std::shared_ptr<SDPXSalsa20EncryptionAlgorithm> encryptionAlgorithm = std::make_shared<SDPXSalsa20EncryptionAlgorithm>();
            SDPCryptStreamBuf cryptBuf(inStream, encryptionAlgorithm);
            cryptBuf.setEncryptionKeyAndNonce(
                "FFFFFFFFFFFFFFFF"
                "FFFFFFFFFFFFFFFF"
                "FFFFFFFF0FFFFFFF"
                "FFFFFFFFFFFFFFFF", true,
                "FFFFFFABFFFFFFFF"
                "FFFFFFFFFFFFFFFF"
                "FFFFFFFFFFFFFFFF", true);

            int c;
            while (true) {
                c = cryptBuf.sbumpc();
                if(c == EOF){
                    break;
                }
                outStream.get()->put(c);
            }

            outStream.get()->flush();

        }else {
            std::cerr << "Failed to open file " << destFileName << "'." << std::endl;
        }
    }else {
        std::cerr << "Failed to open file " << sourceFileName << "'." << std::endl;
    }

}

void encryptFile(const char* sourceFileName, const char* destFileName){

    std::shared_ptr<std::ifstream> inStream = std::make_shared<std::ifstream>();
    std::shared_ptr<std::ofstream> outStream = std::make_shared<std::ofstream>();
    inStream.get()->open(sourceFileName, std::ios_base::in);
    outStream.get()->open(destFileName, std::ios::out);

    if(inStream.get()->is_open()){
        if(outStream.get()->is_open()){

            std::shared_ptr<SDPXSalsa20EncryptionAlgorithm> compressionAlgorithm = std::make_shared<SDPXSalsa20EncryptionAlgorithm>();
            SDPCryptStreamBuf cryptBuf(outStream, compressionAlgorithm);
            cryptBuf.setEncryptionKeyAndNonce(
                "FFFFFFFFFFFFFFFF"
                "FFFFFFFFFFFFFFFF"
                "FFFFFFFF0FFFFFFF"
                "FFFFFFFFFFFFFFFF", true,
                "FFFFFFABFFFFFFFF"
                "FFFFFFFFFFFFFFFF"
                "FFFFFFFFFFFFFFFF", true);

            std::ostream encrypt(&cryptBuf);

            int c;
            while(EOF != (c = inStream.get()->get())){
                encrypt << (unsigned char)c;
            }

            encrypt.flush();

        } else{
            std::cerr << "Failed to open file '" << destFileName << "'." << std::endl;
        }
    } else{
        std::cerr << "Failed to open file '" << sourceFileName << "'." << std::endl;
    }

}


int main(){

    std::string unencryptedFile = "ENCRYPTION_TEST";
    std::string encryptedFile   = "ENCRYPTION_TEST_ENCRYPTED";
    std::string decryptedFile   = "ENCRYPTION_TEST_DECRYPTED";

    encryptFile(unencryptedFile.c_str(), encryptedFile.c_str());
    decryptFile(encryptedFile.c_str(), decryptedFile.c_str());


}