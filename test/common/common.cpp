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

#include "common.hpp"


bool compressFile(const char* sourceFileName, const char* destFileName, std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm){

    std::shared_ptr<std::ifstream> inStream = std::make_shared<std::ifstream>();
    std::shared_ptr<std::ofstream> outStream = std::make_shared<std::ofstream>();
    inStream.get()->open(sourceFileName, std::ios_base::in);
    outStream.get()->open(destFileName, std::ios::out);

    if(inStream.get()->is_open()){
        if(outStream.get()->is_open()){

            SDPCompressionStreamBuf compressBuf(outStream, compressionAlgorithm);

            std::ostream encrypt(&compressBuf);

            int c;
            while(EOF != (c = inStream.get()->get())){
                encrypt << (unsigned char)c;
            }

            encrypt.flush();

        } else{
            std::cerr << "Failed to open file '" << destFileName << "'." << std::endl;
            return false;
        }
    } else{
        std::cerr << "Failed to open file '" << sourceFileName << "'." << std::endl;
        return false;
    }

    return true;
}

bool decompressFile(const char* sourceFileName, const char* destFileName, std::shared_ptr<SDPCompressionAlgorithmBase> compressionAlgorithm){

    std::shared_ptr<std::ifstream> inStream = std::make_shared<std::ifstream>();
    std::shared_ptr<std::ofstream> outStream = std::make_shared<std::ofstream>();
    inStream.get()->open(sourceFileName, std::ios_base::in);
    outStream.get()->open(destFileName, std::ios::out);

    if (inStream.get()->is_open()) {
        if (outStream.get()->is_open()) {

            SDPCompressionStreamBuf compressBuf(inStream, compressionAlgorithm);

            int c;
            while (true) {
                c = compressBuf.sbumpc();
                if(c == EOF){
                    break;
                }
                outStream.get()->put(c);
            }

            outStream.get()->flush();

        }else {
            std::cerr << "Failed to open file " << destFileName << "'." << std::endl;
            return false;
        }
    }else {
        std::cerr << "Failed to open file " << sourceFileName << "'." << std::endl;
        return false;
    }

    return true;
}


bool encryptFile(const char* sourceFileName, const char* destFileName, std::shared_ptr<SDPEncryptionAlgorithmBase> encryptionAlgorithm){

    std::shared_ptr<std::ifstream> inStream = std::make_shared<std::ifstream>();
    std::shared_ptr<std::ofstream> outStream = std::make_shared<std::ofstream>();
    inStream.get()->open(sourceFileName, std::ios_base::in);
    outStream.get()->open(destFileName, std::ios::out);

    if(inStream.get()->is_open()){
        if(outStream.get()->is_open()){

            SDPCryptStreamBuf cryptBuf(outStream, encryptionAlgorithm);
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
            return false;
        }
    } else{
        std::cerr << "Failed to open file '" << sourceFileName << "'." << std::endl;
        return false;
    }

    return true;
}

bool decryptFile(const char* sourceFileName, const char* destFileName, std::shared_ptr<SDPEncryptionAlgorithmBase> encryptionAlgorithm){


    std::shared_ptr<std::ifstream> inStream = std::make_shared<std::ifstream>();
    std::shared_ptr<std::ofstream> outStream = std::make_shared<std::ofstream>();
    inStream.get()->open(sourceFileName, std::ios_base::in);
    outStream.get()->open(destFileName, std::ios::out);

    if (inStream.get()->is_open()) {
        if (outStream.get()->is_open()) {

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
            return false;
        }
    }else {
        std::cerr << "Failed to open file " << sourceFileName << "'." << std::endl;
        return false;
    }

    return true;
}


bool areFilesSame(const char* file1, const char* file2){

    std::ifstream inFile1;
    std::ifstream inFile2;
    inFile1.open(file1, std::ios_base::in);
    inFile2.open(file2, std::ios_base::in);

    if(!inFile1.is_open() || !inFile2.is_open()){
    std::cout << "Cannot open files to compare them..." << std::endl;
        return false;
    }

    while(!inFile1.eof() || !inFile2.eof()){

        uchar c1;
        uchar c2;
        inFile1 >> c1;
        inFile2 >> c2;

        if(c1 != c2)
            return false;
    }

    //Check if one file is shorter than the other.
    return (inFile1.eof() && inFile2.eof());
}
