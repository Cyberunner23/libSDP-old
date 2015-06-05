#Copyright 2015 Alex Frappier Lachapelle
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.


#TODO: unit tests, TEST EVERYTHING
#TODO?: Add include dirs of externallibs to SDP's include dir.

CMAKE_MINIMUM_REQUIRED(VERSION 3.1.3)
PROJECT(libSDP C CXX)

#----------------------------------------------------------
####################### OPTIONS ###########################
#----------------------------------------------------------

OPTION(LIBSDP_ENABLE_TESTS              "Enable the building of tests.")
OPTION(LIBSDP_BUILD_SHARED_LIBRARIES    "Build libSDP as a shared lib (setting this to OFF will compile statically)"          ON)
OPTION(LIBSODIUM_BUILD_SHARED_LIBRARIES "Build libsodium-CMake as a shared lib (setting this to OFF will compile statically)" OFF)

SET(EXTLIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extlibs")

#----------------------------------------------------------
##################### COMPILER FLAGS ######################
#----------------------------------------------------------
SET(CMAKE_CXX_STANDARD 11)

#----------------------------------------------------------
#################### SOURCE / HEADERS #####################
#----------------------------------------------------------
SET(SOURCE_FILES
    src/SDP.cpp
    src/SDPAlgorithmBase.cpp
    src/SDPChainBlock.cpp
    src/AlgorithmBase/SDPCompressionAlgorithmBase.cpp
    src/AlgorithmBase/SDPEncryptionAlgorithmBase.cpp
    src/DefaultAlgorithms/SDPDEFLATECompressionAlgorithm.cpp
    src/DefaultAlgorithms/SDPlz4CompressionAlgorithm.cpp
    src/DefaultAlgorithms/SDPXSalsa20EncryptionAlgorithm.cpp
    src/StreamBufs/SDPCompressionStreamBuf.cpp
    src/StreamBufs/SDPCryptStreamBuf.cpp
    src/StreamBufs/SDPStreamBufBase.cpp
    src/Tools/Endianness.cpp
    src/Tools/HexBinTool.cpp
    src/Tools/RawFileIO.cpp
)
SET(HEADER_FILES
    include/SDP.hpp
    include/SDPAlgorithmBase.hpp
    include/SDPChainBlock.hpp
    include/SDPVer.hpp
    include/AlgorithmBase/SDPCompressionAlgorithmBase.hpp
    include/AlgorithmBase/SDPEncryptionAlgorithmBase.hpp
    include/DefaultAlgorithms/SDPDEFLATECompressionAlgorithm.hpp
    include/DefaultAlgorithms/SDPlz4CompressionAlgorithm.hpp
    include/DefaultAlgorithms/SDPXSalsa20EncryptionAlgorithm.hpp
    include/StreamBufs/SDPCompressionStreamBuf.hpp
    include/StreamBufs/SDPCryptStreamBuf.hpp
    include/StreamBufs/SDPStreamBufBase.hpp
    include/Tools/DevMacros.hpp
    include/Tools/Endianness.hpp
    include/Tools/HexBinTool.hpp
    include/Tools/RawFileIO.hpp
    include/Tools/Typedefs.hpp
)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/AlgorithmBase)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultAlgorithms)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/StreamBufs)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/Tools)


#----------------------------------------------------------
######################### LIBRARY #########################
#----------------------------------------------------------

IF(LIBSDP_BUILD_SHARED_LIBRARIES)
    ADD_LIBRARY(libSDP SHARED ${SOURCE_FILES} ${HEADER_FILES})
ELSE(LIBSDP_BUILD_SHARED_LIBRARIES)
    ADD_LIBRARY(libSDP STATIC ${SOURCE_FILES} ${HEADER_FILES})
ENDIF(LIBSDP_BUILD_SHARED_LIBRARIES)

SET_TARGET_PROPERTIES(libSDP PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
    OUTPUT_NAME "SDP"
)


#----------------------------------------------------------
################## EXTERNAL DEPENDENCIES ##################
#----------------------------------------------------------

######################## libsodium #######################
#If directory not specified, use files from extlibs folder

SET(LIBSODIUM_DIR ${EXTLIBS_DIR}/libsodium-CMake)

MESSAGE("\nBuilding libsodium from ${LIBSODIUM_DIR}/")

ADD_SUBDIRECTORY(${LIBSODIUM_DIR} EXCLUDE_FROM_ALL)
INCLUDE_DIRECTORIES(${LIBSODIUM_DIR}/src/libsodium)
INCLUDE_DIRECTORIES(${LIBSODIUM_DIR}/src/libsodium/include)
TARGET_LINK_LIBRARIES(libSDP sodium)

IF(LIBSODIUM_BUILD_SHARED_LIBRARIES)
    MESSAGE("libsodium will be built as a shared library, library file will be placed in libSDP/bin/")
    SET_TARGET_PROPERTIES(sodium PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
        PROJECT_LABEL "libsodium"
    )
ENDIF()


########################## zlib ##########################
#If directory not specified, use files from extlibs folder

SET(ZLIB_DIR ${EXTLIBS_DIR}/zlib)

MESSAGE("\nBuilding zlib from ${ZLIB_DIR}/")

SET(DISABLE_EXAMPLE_BINARIES ON CACHE BOOL "")
SET(BUILD_SHARED_LIBS OFF CACHE BOOL "")
SET(SKIP_INSTALL_ALL ON CACHE BOOL "")
SET(COMPILE_STATIC_LIB_WITH_FPIC ON CACHE BOOL "")

ADD_SUBDIRECTORY(${ZLIB_DIR} EXCLUDE_FROM_ALL)
INCLUDE_DIRECTORIES(${ZLIB_PUBLIC_HDRS})
TARGET_LINK_LIBRARIES(libSDP zlibstatic)


########################## lz4 ###########################
#If directory not specified, use files from extlibs folder

SET(LZ4_DIR ${EXTLIBS_DIR}/lz4)

MESSAGE("\nBuilding lz4 from ${LZ4_DIR}/")

SET(BUILD_TOOLS OFF CACHE BOOL "")
SET(BUILD_LIBS  ON  CACHE BOOL "")

ADD_SUBDIRECTORY(${LZ4_DIR}/cmake_unofficial EXCLUDE_FROM_ALL)
INCLUDE_DIRECTORIES (${LZ4_DIR}/lib)
TARGET_LINK_LIBRARIES(libSDP liblz4)


#----------------------------------------------------------
###################### LIBSDP TESTS #######################
#----------------------------------------------------------

if(LIBSDP_ENABLE_TESTS)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()