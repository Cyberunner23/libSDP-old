CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)
PROJECT(libSDP C CXX)

#TODO: MAJOR cleanup.
#TODO: using ${CMAKE_CURRENT_SOURCE_DIR} -> using ${SOULENGINE_SOURCE_DIR} for lib path finding.
#TODO: Add include dirs of externallibs to SDP's include dir.





#FIXME: LIBSODIUM_CMAKE_DIR option setting stuff

#----------------------------------------------------------
####################### OPTIONS ###########################
#----------------------------------------------------------
OPTION(LIBSDP_BUILD_SHARED_LIBRARIES    "Build libSDP as a shared lib (setting this to OFF will compile statically)" ON)
OPTION(LIBSODIUM_BUILD_SHARED_LIBRARIES "Build libsodium-CMake as a shared lib (setting this to OFF will compile statically)")

SET(LIBSODIUM_CMAKE_DIR "" CACHE STRING "Directory where libsodium-CMake is placed. (Don't set this if libsodium-CMake is in extlibs/)")
SET(ZLIB_CMAKE_DIR      "" CACHE STRING "Directory where zlib is placed. (Don't set this if zlib is in extlibs/)")

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
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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
)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/AlgorithmBase)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/DefaultAlgorithms)


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
#IF(LIBSODIUM_CMAKE_DIR STREQUAL "")
#    MESSAGE("Building libsodium-Cmake from internal directories.")
#    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake)
#    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake/include)
#    ADD_DEPENDENCIES(libSDP sodium)
#    TARGET_LINK_LIBRARIES(libSDP ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake/lib/${LIBSODIUM_LIB_NAME})
#ELSEIF(LIBSODIUM_CMAKE_DIR STREQUAL "")
#    MESSAGE("Building libsodium-Cmake from exterior directories.")
#    ADD_SUBDIRECTORY(${LIBSODIUM_CMAKE_DIR})
#    INCLUDE_DIRECTORIES(${LIBSODIUM_CMAKE_DIR}/include)
#    ADD_DEPENDENCIES(libSDP sodium)
#    TARGET_LINK_LIBRARIES(libSDP ${LIBSODIUM_CMAKE_DIR}/lib/${LIBSODIUM_LIB_NAME})
#ENDIF(LIBSODIUM_CMAKE_DIR STREQUAL "")

IF(LIBSODIUM_CMAKE_DIR STREQUAL "")
    SET(LIBSODIUM_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake")
    MESSAGE("Building libsodium from ${LIBSODIUM_CMAKE_DIR}")
ENDIF()

ADD_SUBDIRECTORY(${LIBSODIUM_CMAKE_DIR})
INCLUDE_DIRECTORIES(${LIBSODIUM_CMAKE_DIR}/include)
TARGET_LINK_LIBRARIES(libSDP sodium)

########################## zlib ##########################
#If directory not specified, use files from extlibs folder
#IF(ZLIB_CMAKE_DIR STREQUAL "")
#    MESSAGE("Building zlib from internal directories.")
#    SET(DISABLE_EXAMPLE_BINARIES ON)
#    SET(BUILD_SHARED_LIBS OFF)
#    SET(SKIP_INSTALL_ALL ON)
#    ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extlibs/zlib)
#    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/extlibs/zlib)
#    TARGET_LINK_LIBRARIES(libSDP zlibstatic)
#ELSEIF(ZLIB_CMAKE_DIR STREQUAL "")
#    MESSAGE("Building zlib from exterior directories.")
#    SET(DISABLE_EXAMPLE_BINARIES ON)
#    SET(BUILD_SHARED_LIBS OFF)
#    SET(SKIP_INSTALL_ALL ON)
#    ADD_SUBDIRECTORY(${ZLIB_CMAKE_DIR})
#    INCLUDE_DIRECTORIES(${ZLIB_CMAKE_DIR})
#    TARGET_LINK_LIBRARIES(libSDP zlibstatic)
#ENDIF(ZLIB_CMAKE_DIR STREQUAL "")

IF(ZLIB_CMAKE_DIR STREQUAL "")
    SET(ZLIB_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extlibs/zlib")
    MESSAGE("Building zlib from ${ZLIB_CMAKE_DIR}")
ENDIF()

SET(DISABLE_EXAMPLE_BINARIES ON)
SET(BUILD_SHARED_LIBS OFF)
SET(SKIP_INSTALL_ALL ON)
ADD_SUBDIRECTORY(${ZLIB_CMAKE_DIR})
INCLUDE_DIRECTORIES(${ZLIB_CMAKE_DIR})
TARGET_LINK_LIBRARIES(libSDP zlibstatic)

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


