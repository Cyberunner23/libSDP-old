#include <iostream>
#include <string>
#include <iostream>

#include "sodium.h"

int hexToBin(std::string hex, std::string &bin){
	bin.resize(hex.size()/2);
	return sodium_hex2bin((unsigned char*)bin.data(), (hex.size()/2), hex.c_str(), hex.size(), NULL, nullptr, nullptr);
}

char* binToHex(std::string bin, std::string &hex){
	hex.resize(bin.size() *2 + 1);
	return sodium_bin2hex((char*)hex.data(), (bin.size() *2 + 1), (unsigned char*)bin.c_str(), bin.size());
}

int main(){

	std::string hexToBinHEX {"4142"};
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
	std::cout << "Hex: " << "\"" << binToHexHEX << "\"" << std::endl << std::endl;

	return 0;
}