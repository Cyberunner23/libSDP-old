CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)
PROJECT(libSDP C CXX)

#TODO: MAJOR cleanup.
#TODO: using ${CMAKE_CURRENT_SOURCE_DIR} -> using ${SOULENGINE_SOURCE_DIR} for lib path finding.
#TODO: Add include dirs of externallibs to SDP's include dir.

#----------------------------------------------------------
####################### OPTIONS ###########################
#----------------------------------------------------------
OPTION(LIBSDP_BUILD_SHARED_LIBRARIES    "Build libSDP as a shared lib (setting this to OFF will compile statically)" ON)
OPTION(LIBSODIUM_BUILD_SHARED_LIBRARIES "Build libsodium-CMake as a shared lib (setting this to OFF will compile statically)")
OPTION(LIBSODIUM_CMAKE_DIR "Directory where libsodium-CMake is placed. (Don't set this if libsodium-CMake is in extlibs/)" "")

SET(LIBSODIUM_CMAKE_DIR "")
#SET(SOULENGINE_SOURCE_DIR "" PATH "Top level directory of Soul Engine (Will be set by Soul Engine's CMakeLists.txt, setting this will cause ${PROJECT_NAME} to use Soul Engine's libraries.)")
#SET(LIBSODIUM_BUILD_SHARED_LIBRARIES ON)
#----------------------------------------------------------
#################### INTERNAL/DEV VARS ####################
#----------------------------------------------------------
SET(IS_DEV_BUILD                ON CACHE BOOL  "Set to ON if its a development build for libSDP, set to OFF otherwise.")
SET(EXTERNAL_LIB_ALREADY_CLONED OFF CACHE BOOL "Sets if the external libraries were already cloned")# FORCE) #Add FORCE to reset if needed.

#----------------------------------------------------------
##################### COMPILER FLAGS ######################
#----------------------------------------------------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

#----------------------------------------------------------
#################### SOURCE / HEADERS #####################
#----------------------------------------------------------
SET(SOURCE_FILES
    src/Endianness.cpp
    src/HexBinTool.cpp
    src/RawFileIO.cpp
    src/SDPCompressionStreamBuf.cpp
    src/SDPCryptStreamBuf.cpp
    src/SDPStreamBuf.cpp
    src/AlgorithmBase/SDPCompressionAlgorithmBase.cpp
    src/AlgorithmBase/SDPEncryptionAlgorithmBase.cpp
    src/DefaultAlgorithms/SDPXSalsa20EncryptionAlgorithm.cpp
    src/DefaultAlgorithms/lz4/lz4.c
    src/DefaultAlgorithms/lz4/lz4frame.c
    src/DefaultAlgorithms/lz4/lz4hc.c
    src/DefaultAlgorithms/lz4/xxhash.c
    src/DefaultAlgorithms/zlib/adler32.c
    src/DefaultAlgorithms/zlib/compress.c
    src/DefaultAlgorithms/zlib/crc32.c
    src/DefaultAlgorithms/zlib/deflate.c
    src/DefaultAlgorithms/zlib/gzclose.c
    src/DefaultAlgorithms/zlib/gzlib.c
    src/DefaultAlgorithms/zlib/gzread.c
    src/DefaultAlgorithms/zlib/gzwrite.c
    src/DefaultAlgorithms/zlib/infback.c
    src/DefaultAlgorithms/zlib/inffast.c
    src/DefaultAlgorithms/zlib/inflate.c
    src/DefaultAlgorithms/zlib/inftrees.c
    src/DefaultAlgorithms/zlib/trees.c
    src/DefaultAlgorithms/zlib/uncompr.c
    src/DefaultAlgorithms/zlib/zutil.c
)
SET(HEADER_FILES
    include/Endianness.hpp
    include/HexBinTool.hpp
    include/RawFileIO.hpp
    include/SDPCompressionStreamBuf.hpp
    include/SDPCryptStreamBuf.hpp
    include/SDPStreamBuf.hpp
    include/SDPVer.hpp
    include/AlgorithmBase/SDPCompressionAlgorithmBase.hpp
    include/AlgorithmBase/SDPEncryptionAlgorithmBase.hpp
    include/DefaultAlgorithms/SDPXSalsa20EncryptionAlgorithm.hpp
    include/DefaultAlgorithms/lz4/lz4.h
    include/DefaultAlgorithms/lz4/lz4frame.h
    include/DefaultAlgorithms/lz4/lz4frame_static.h
    include/DefaultAlgorithms/lz4/lz4hc.h
    include/DefaultAlgorithms/lz4/xxhash.h
    include/DefaultAlgorithms/zlib/crc32.h
    include/DefaultAlgorithms/zlib/deflate.h
    include/DefaultAlgorithms/zlib/gzguts.h
    include/DefaultAlgorithms/zlib/inffast.h
    include/DefaultAlgorithms/zlib/inffixed.h
    include/DefaultAlgorithms/zlib/inflate.h
    include/DefaultAlgorithms/zlib/inftrees.h
    include/DefaultAlgorithms/zlib/trees.h
    include/DefaultAlgorithms/zlib/zconf.h
    include/DefaultAlgorithms/zlib/zlib.h
    include/DefaultAlgorithms/zlib/zutil.h
)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/AlgorithmBase)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultAlgorithms)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultAlgorithms/lz4)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultAlgorithms/zlib)


#----------------------------------------------------------
######################### LIBRARY #########################
#----------------------------------------------------------

ADD_DEFINITIONS(-DLZ4_VERSION=r126)

#zlib stuff

#From Zlib's CMakeLists.txt
include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)

check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h    HAVE_STDINT_H)
check_include_file(stddef.h    HAVE_STDDEF_H)

#
# Check to see if we have large file support
#
set(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1)
# We add these other definitions here because CheckTypeSize.cmake
# in CMake 2.4.x does not automatically do so and we want
# compatibility with CMake 2.4.x.
if(HAVE_SYS_TYPES_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_SYS_TYPES_H)
endif()
if(HAVE_STDINT_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDINT_H)
endif()
if(HAVE_STDDEF_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDDEF_H)
endif()
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
   add_definitions(-D_LARGEFILE64_SOURCE=1)
endif()
set(CMAKE_REQUIRED_DEFINITIONS) # clear variable

#
# Check for fseeko
#
check_function_exists(fseeko HAVE_FSEEKO)
if(NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
endif()

#
# Check for unistd.h
#
check_include_file(unistd.h Z_HAVE_UNISTD_H)

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultAlgorithms/zlib/zconf.h.cmakein
	${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultAlgorithms/zlib/zconf.h @ONLY)




IF(LIBSDP_BUILD_SHARED_LIBRARIES)
    ADD_LIBRARY(libSDP SHARED ${SOURCE_FILES} ${HEADER_FILES})
ELSE(LIBSDP_BUILD_SHARED_LIBRARIES)
    ADD_LIBRARY(libSDP STATIC ${SOURCE_FILES} ${HEADER_FILES})
ENDIF(LIBSDP_BUILD_SHARED_LIBRARIES)

SET_TARGET_PROPERTIES(libSDP PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    PROJECT_LABEL "libSDP"
)


#----------------------------------------------------------
################## EXTERNAL DEPENDENCIES ##################
#----------------------------------------------------------

######################## libsodium #######################
#If directory not specified, use files from extlibs folder
IF(LIBSODIUM_CMAKE_DIR STREQUAL "")
    MESSAGE("Building libsodium-Cmake from internal directories.")
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake/include)
    ADD_DEPENDENCIES(libSDP sodium)
    TARGET_LINK_LIBRARIES(libSDP ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake/lib/${LIBSODIUM_LIB_NAME})
ELSEIF(LIBSODIUM_CMAKE_DIR STREQUAL "")
    MESSAGE("Building libsodium-Cmake from exterior directories.")
    ADD_SUBDIRECTORY(${LIBSODIUM_CMAKE_DIR})
    INCLUDE_DIRECTORIES(${LIBSODIUM_CMAKE_DIR}/include)
    ADD_DEPENDENCIES(libSDP sodium)
    TARGET_LINK_LIBRARIES(libSDP ${LIBSODIUM_CMAKE_DIR}/lib/${LIBSODIUM_LIB_NAME})
ENDIF(LIBSODIUM_CMAKE_DIR STREQUAL "")


#Testing stuff

#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake/include)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/AlgorithmBase)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultAlgorithms)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultAlgorithms/lz4)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultAlgorithms/zlib)
#
#SET(TEST_SOURCE
#    test/main.cpp
#)
#ADD_EXECUTABLE(TEST ${TEST_SOURCE})
#SET_TARGET_PROPERTIES(TEST PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test
#)
#
#TARGET_LINK_LIBRARIES(TEST ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake/lib/libsodium.so)
#TARGET_LINK_LIBRARIES(TEST ${CMAKE_CURRENT_SOURCE_DIR}/lib/libSDP.so)


