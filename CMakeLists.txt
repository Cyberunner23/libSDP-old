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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.7)
PROJECT(libSDP C CXX)

#TODO: MAJOR cleanup.
#TODO: Add include dirs of externallibs to SDP's include dir.
#TODO: unit tests, TEST EVERYTHING
#FIXME: LIBSODIUM_CMAKE_DIR option setting stuff



#----------------------------------------------------------
####################### OPTIONS ###########################
#----------------------------------------------------------
OPTION(LIBSDP_ENABLE_TESTS              "Enable the building of tests.")
OPTION(LIBSDP_BUILD_SHARED_LIBRARIES    "Build libSDP as a shared lib (setting this to OFF will compile statically)" ON)
OPTION(LIBSODIUM_BUILD_SHARED_LIBRARIES "Build libsodium-CMake as a shared lib (setting this to OFF will compile statically)")

SET(LIBSODIUM_CMAKE_DIR "" CACHE STRING "Directory where libsodium-CMake is placed. (Don't set this if libsodium-CMake is in extlibs/)")
SET(ZLIB_CMAKE_DIR      "" CACHE STRING "Directory where zlib is placed. (Don't set this if zlib is in extlibs/)")
SET(LZ4_CMAKE_DIR       "" CACHE STRING "Directory where lz4 is placed. (Don't set this if lz4 is in extlibs/)")

#----------------------------------------------------------
##################### COMPILER FLAGS ######################
#----------------------------------------------------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    OUTPUT_NAME "SDP"
)


#----------------------------------------------------------
################## EXTERNAL DEPENDENCIES ##################
#----------------------------------------------------------

######################## libsodium #######################
#If directory not specified, use files from extlibs folder

IF(LIBSODIUM_CMAKE_DIR STREQUAL "")
    SET(LIBSODIUM_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake")
    MESSAGE("Building libsodium from ${LIBSODIUM_CMAKE_DIR}")
ELSE()
    MESSAGE("Building libsodium from ${LIBSODIUM_CMAKE_DIR}")
ENDIF()

ADD_SUBDIRECTORY(${LIBSODIUM_CMAKE_DIR} EXCLUDE_FROM_ALL)
INCLUDE_DIRECTORIES(${LIBSODIUM_CMAKE_DIR}/src/libsodium)
INCLUDE_DIRECTORIES(${LIBSODIUM_CMAKE_DIR}/src/libsodium/include)
TARGET_LINK_LIBRARIES(libSDP sodium)

########################## zlib ##########################
#If directory not specified, use files from extlibs folder

IF(ZLIB_CMAKE_DIR STREQUAL "")
    SET(ZLIB_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extlibs/zlib")
    MESSAGE("Building zlib from ${ZLIB_CMAKE_DIR}")
ENDIF()

SET(DISABLE_EXAMPLE_BINARIES ON CACHE BOOL "")
SET(BUILD_SHARED_LIBS OFF CACHE BOOL "")
SET(SKIP_INSTALL_ALL ON CACHE BOOL "")
SET(COMPILE_STATIC_LIB_WITH_FPIC ON CACHE BOOL "")
ADD_SUBDIRECTORY(${ZLIB_CMAKE_DIR} EXCLUDE_FROM_ALL)
INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(libSDP zlibstatic)

########################## lz4 ###########################
#If directory not specified, use files from extlibs folder

IF(LZ4_CMAKE_DIR STREQUAL "")
    SET(LZ4_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extlibs/lz4")
    MESSAGE("Building lz4 from ${LZ4_CMAKE_DIR}")
ENDIF()

SET(BUILD_TOOLS OFF CACHE BOOL "")
SET(BUILD_LIBS  ON  CACHE BOOL "")
ADD_SUBDIRECTORY(${LZ4_CMAKE_DIR}/cmake_unofficial EXCLUDE_FROM_ALL)
INCLUDE_DIRECTORIES (${LZ4_CMAKE_DIR}/lib)
ADD_DEPENDENCIES(libSDP liblz4)
TARGET_LINK_LIBRARIES(libSDP liblz4)


#Testing stuff
if(LIBSDP_ENABLE_TESTS)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()