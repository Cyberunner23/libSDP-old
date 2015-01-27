#include "HexBinTool.hpp"

int HexBinTool::hexToBin(std::string hex, std::string &bin) {
	bin.resize(hex.size()/2);
	return sodium_hex2bin((unsigned char*)bin.data(), bin.size(), hex.c_str(), hex.size(), NULL, nullptr, nullptr);
}

char* HexBinTool::binToHex(std::string bin, std::string &hex) {
	hex.resize(bin.size() * 2 + 1);
	return sodium_bin2hex((char*)hex.data(), hex.size(), (unsigned char*)bin.c_str(), bin.size());
}