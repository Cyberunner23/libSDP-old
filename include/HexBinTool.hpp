#ifndef HEXBINTOOL_H
#define HEXBINTOOL_H

#include <string>

#include "sodium.h"

class HexBinTool{

public:

	//Vars

	//Funcs

	//turns AABBCC to 0xAA 0xBB 0xCC
	int hexToBin(std::string hex, std::string &bin);
	//turns 0xAA 0xBB 0xCC to AABBCC
	char* binToHex(std::string bin, std::string &hex);


};

#endif //HEXBINTOOL_H