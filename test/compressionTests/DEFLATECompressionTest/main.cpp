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

#include "common.hpp"
#include "SDPCompressionStreamBuf.hpp"
#include "SDPDEFLATECompressionAlgorithm.hpp"

int main(){

    std::string uncompressedFileCompressible = "../../common/data/data64k";
    std::string compressedFileCompressible   = "data64k_COMPRESSED";
    std::string decompressedFileCompressible = "data64k_DECOMPRESSED";

    std::string uncompressedFileIncompressible = "../../common/data/randomData64k";
    std::string compressedFileIncompressible   = "randomData64k_COMPRESSED";
    std::string decompressedFileIncompressible = "randomData64k_DECOMPRESSED";

    std::shared_ptr<SDPDEFLATECompressionAlgorithm> compressionAlgorithm = std::make_shared<SDPDEFLATECompressionAlgorithm>();


    compressFile(uncompressedFileCompressible.c_str(), compressedFileCompressible.c_str(),   compressionAlgorithm);
    decompressFile(compressedFileCompressible.c_str(), decompressedFileCompressible.c_str(), compressionAlgorithm);

    compressFile(uncompressedFileIncompressible.c_str(), compressedFileIncompressible.c_str(),   compressionAlgorithm);
    decompressFile(compressedFileIncompressible.c_str(), decompressedFileIncompressible.c_str(), compressionAlgorithm);

    if(!areFilesSame(uncompressedFileCompressible.c_str(), decompressedFileCompressible.c_str()))
        return -1;
    if(!areFilesSame(uncompressedFileIncompressible.c_str(), decompressedFileIncompressible.c_str()))
        return -2;

	return 0;
}