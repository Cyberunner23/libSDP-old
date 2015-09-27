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

#include "Utils/HexBinTool.hpp"

using namespace libSDP;
using namespace libSDP::Utils;

int HexBinTool::hexToBin(std::string hex, std::string &bin) {
	bin.resize(hex.size()/2);
	return sodium_hex2bin((unsigned char*)bin.data(), bin.size(), hex.c_str(), hex.size(), NULL, nullptr, nullptr);
}

char* HexBinTool::binToHex(std::string bin, std::string &hex) {
	hex.resize(bin.size() * 2 + 1);
	return sodium_bin2hex((char*)hex.data(), hex.size(), (unsigned char*)bin.c_str(), bin.size());
}