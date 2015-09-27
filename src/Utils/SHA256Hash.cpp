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

#include "Utils/SHA256Hash.hpp"

using namespace libSDP::Utils;

SHA256Hash::SHA256Hash(){
    digest.resize(crypto_hash_sha256_BYTES);
    crypto_hash_sha256_init(&sha256State);
}

SHA256Hash::~SHA256Hash(){}

template<typename... Args>
std::string SHA256Hash::getHashStr(Args... args){
    updateHash(args...);
    crypto_hash_sha256_final(&sha256State, digest.data());
    return std::string(reinterpret_cast<char*>(digest.data()), digest.size());
}

template<typename... Args>
std::vector<uchar> SHA256Hash::getHashVec(Args... args){
    updateHash(args...);
    crypto_hash_sha256_final(&sha256State, digest.data());
    return digest;
}


template<typename... Args>
void SHA256Hash::updateHash(uchar val, Args... args){
    crypto_hash_sha256_update(&sha256State,
                              &val,
                              sizeof(val));
    updateHash(args...);
}

template<typename... Args>
void SHA256Hash::updateHash(uchar* val, uint64 size, Args... args){
    crypto_hash_sha256_update(&sha256State,
                              val,
                              size);
    updateHash(args...);
}

template<typename... Args>
void SHA256Hash::updateHash(uint16 val, Args... args){////////////////////////
    crypto_hash_sha256_update(&sha256State,
                              reinterpret_cast<uchar*>(val),
                              sizeof(reinterpret_cast<uchar*>(val)));
    updateHash(args...);
}

template<typename... Args>
void SHA256Hash::updateHash(uint32 val, Args... args){/////////////////////
    crypto_hash_sha256_update(&sha256State,
                              reinterpret_cast<uchar*>(val),
                              sizeof(reinterpret_cast<uchar*>(val)));
    updateHash(args...);
}

template<typename... Args>
void SHA256Hash::updateHash(uint64 val, Args... args){/////////////////////////
    crypto_hash_sha256_update(&sha256State,
                              reinterpret_cast<uchar*>(val),
                              sizeof(reinterpret_cast<uchar*>(val)));
    updateHash(args...);
}

template<typename... Args>
void SHA256Hash::updateHash(std::string val, Args... args){///////////////////
    crypto_hash_sha256_update(&sha256State,
                              reinterpret_cast<const uchar*>(val.c_str()),
                              sizeof(reinterpret_cast<const uchar*>(val.c_str())));
    updateHash(args...);
}

template<typename... Args>
void SHA256Hash::updateHash(std::vector<uchar> val, Args... args){
    crypto_hash_sha256_update(&sha256State,
                              val.data(),
                              val.size());
    updateHash(args...);
}

template<typename... Args>
void SHA256Hash::updateHash(std::vector<uint16> val, Args... args){//////////////////
    crypto_hash_sha256_update(&sha256State,
                              reinterpret_cast<uchar*>(val.data()),
                              sizeof(reinterpret_cast<uchar*>(val.data())));
    updateHash(args...);
}

template<typename... Args>
void SHA256Hash::updateHash(std::vector<uint32> val, Args... args){///////////////////////
    crypto_hash_sha256_update(&sha256State,
                              reinterpret_cast<uchar*>(val.data()),
                              sizeof(reinterpret_cast<uchar*>(val.data())));
    updateHash(args...);
}

template<typename... Args>
void SHA256Hash::updateHash(std::vector<uint64> val, Args... args){///////////////////////
    crypto_hash_sha256_update(&sha256State,
                              reinterpret_cast<uchar*>(val.data()),
                              sizeof(reinterpret_cast<uchar*>(val.data())));
    updateHash(args...);
}




