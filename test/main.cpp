#include <iostream>
#include <string>
#include <iostream>
#include <sstream>

#include "SDPCompressionStreamBuf.hpp"
#include "SDPDEFLATECompressionAlgorithm.hpp"

#include "sodium.h"

void decompressFile(const char* sourceFileName, const char* destFileName){

    std::shared_ptr<std::ifstream> inStream = std::make_shared<std::ifstream>();
    std::shared_ptr<std::ofstream> outStream = std::make_shared<std::ofstream>();
    inStream.get()->open(sourceFileName, std::ios_base::in);
    outStream.get()->open(destFileName, std::ios::out);

    if (inStream.get()->is_open()) {
        if (outStream.get()->is_open()) {

            std::shared_ptr<SDPDEFLATECompressionAlgorithm> compressionAlgorithm = std::make_shared<SDPDEFLATECompressionAlgorithm>();
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
        }
    }else {
        std::cerr << "Failed to open file " << sourceFileName << "'." << std::endl;
    }
}

void compressFile(const char* sourceFileName, const char* destFileName){

    std::shared_ptr<std::ifstream> inStream = std::make_shared<std::ifstream>();
    std::shared_ptr<std::ofstream> outStream = std::make_shared<std::ofstream>();
    inStream.get()->open(sourceFileName, std::ios_base::in);
    outStream.get()->open(destFileName, std::ios::out);

    if(inStream.get()->is_open()){
        if(outStream.get()->is_open()){

            std::shared_ptr<SDPDEFLATECompressionAlgorithm> compressionAlgorithm = std::make_shared<SDPDEFLATECompressionAlgorithm>();
            SDPCompressionStreamBuf compressBuf(outStream, compressionAlgorithm);

            std::ostream encrypt(&compressBuf);

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

	//SDPXSalsa20EncryptionAlgorithm test;

    std::string uncompressedFile = "TEST";
    std::string compressedFile   = "TEST_COMPRESSED";
    std::string decompressedFile = "TEST_DECOMPRESSED";

    compressFile(uncompressedFile.c_str(), compressedFile.c_str());
    decompressFile(compressedFile.c_str(), decompressedFile.c_str());

    //<editor-fold desc="Old Encryption Test">
    /*test.setEncryptionKeyAndNonce(
        "FFFFFFFFFFFFFFFF"
        "FFFFFFFFFFFFFFFF"
        "FFFFFFFF0FFFFFFF"
        "FFFFFFFFFFFFFFFF", true,
        "FFFFFFABFFFFFFFF"
        "FFFFFFFFFFFFFFFF"
        "FFFFFFFFFFFFFFFF", true);

    std::vector<unsigned char> unencryptedBytes{'1', '1', '1', '4', '5', '6', '7', '8', '9'};
    std::vector<unsigned char> encryptedBytes;
    std::vector<unsigned char> decryptedBytes;

    encryptedBytes.resize(9);
    decryptedBytes.resize(9);

    std::cout << "Unencrypted bytes:" << std::endl;
    std::cout << unencryptedBytes[0] << std::endl;
    std::cout << unencryptedBytes[1] << std::endl;
    std::cout << unencryptedBytes[2] << std::endl;
    std::cout << unencryptedBytes[3] << std::endl;
    std::cout << unencryptedBytes[4] << std::endl;
    std::cout << unencryptedBytes[5] << std::endl;
    std::cout << unencryptedBytes[6] << std::endl;
    std::cout << unencryptedBytes[7] << std::endl;
    std::cout << unencryptedBytes[8] << std::endl;

    test.encryptStream(&unencryptedBytes[0], &encryptedBytes[0]);
    test.encryptStream(&unencryptedBytes[1], &encryptedBytes[1]);
    test.encryptStream(&unencryptedBytes[2], &encryptedBytes[2]);
    test.encryptStream(&unencryptedBytes[3], &encryptedBytes[3]);
    test.encryptStream(&unencryptedBytes[4], &encryptedBytes[4]);
    test.encryptStream(&unencryptedBytes[5], &encryptedBytes[5]);
    test.encryptStream(&unencryptedBytes[6], &encryptedBytes[6]);
    test.encryptStream(&unencryptedBytes[7], &encryptedBytes[7]);
    test.encryptStream(&unencryptedBytes[8], &encryptedBytes[8]);

    std::cout << "Encrypted bytes:" << std::endl;
    std::cout << encryptedBytes[0] << std::endl;
    std::cout << encryptedBytes[1] << std::endl;
    std::cout << encryptedBytes[2] << std::endl;
    std::cout << encryptedBytes[3] << std::endl;
    std::cout << encryptedBytes[4] << std::endl;
    std::cout << encryptedBytes[5] << std::endl;
    std::cout << encryptedBytes[6] << std::endl;
    std::cout << encryptedBytes[7] << std::endl;
    std::cout << encryptedBytes[8] << std::endl;

    test.decryptStream(&encryptedBytes[0], &decryptedBytes[0]);
    test.decryptStream(&encryptedBytes[1], &decryptedBytes[1]);
    test.decryptStream(&encryptedBytes[2], &decryptedBytes[2]);
    test.decryptStream(&encryptedBytes[3], &decryptedBytes[3]);
    test.decryptStream(&encryptedBytes[4], &decryptedBytes[4]);
    test.decryptStream(&encryptedBytes[5], &decryptedBytes[5]);
    test.decryptStream(&encryptedBytes[6], &decryptedBytes[6]);
    test.decryptStream(&encryptedBytes[7], &decryptedBytes[7]);
    test.decryptStream(&encryptedBytes[8], &decryptedBytes[8]);

    std::cout << "Decrypted bytes:" << std::endl;
    std::cout << decryptedBytes[0] << std::endl;
    std::cout << decryptedBytes[1] << std::endl;
    std::cout << decryptedBytes[2] << std::endl;
    std::cout << decryptedBytes[3] << std::endl;
    std::cout << decryptedBytes[4] << std::endl;
    std::cout << decryptedBytes[5] << std::endl;
    std::cout << decryptedBytes[6] << std::endl;
    std::cout << decryptedBytes[7] << std::endl;
    std::cout << decryptedBytes[8] << std::endl;*/
    //</editor-fold>


	return 0;
}