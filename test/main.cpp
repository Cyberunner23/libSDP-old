#include <iostream>
#include <string>
#include <iostream>
#include <sstream>

#include "SDPCryptStreamBuf.hpp"
#include "SDPXSalsa20EncryptionAlgorithm.hpp"

#include "sodium.h"

int main(){

	/*std::string hexToBinHEX {"4142"};
	std::string hexToBinBIN;
	std::string binToHexHEX;
	std::string binToHexBIN {0x42, 0x43};

	std::cout << "Hex to Bin: " << std::endl;
	std::cout << "Hex: " << "\"" << hexToBinHEX << "\"" << std::endl;
	hexToBin(hexToBinHEX, hexToBinBIN);
	std::cout << "Bin: " << "\"" << hexToBinBIN << "\"" << std::endl << std::endl;

	std::cout << "Bin to Hex: " << std::endl;
	std::cout << "Bin: " << "\"" << binToHexBIN << "\"" << std::endl;
	binToHex(binToHexBIN, binToHexHEX);
	std::cout << "Hex: " << "\"" << binToHexHEX << "\"" << std::endl << std::endl;*/

	//SDPXSalsa20EncryptionAlgorithm test;

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


	return 0;
}