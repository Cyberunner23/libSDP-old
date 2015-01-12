cmake_minimum_required(VERSION 2.8.11)
project(libSDP)

#Set option shere
OPTION(COMPILE_LIBSDP_LIB_SHARED   "Build libSDP as a shared lib (setting this to OFF will compile statically)" ON)
OPTION(IS_BUILD_ISOLATED  "Whether libSDP is compiled from inside SoulEngine (OFF) or isolated/on its own (ON)" ON)
OPTION(IS_DEV_BUILD "Set to ON if its a development build for libSDP, set to OFF otherwise." ON)

OPTION(COMPILE_CRYPTOPP_LIB_SHARED "Build cryptopp as a shared lib (setting this to OFF will compile statically)" ON)

SET(EXTERNAL_LIB_ALREADY_CLONED OFF CACHE BOOL "Sets if the external libraries were already cloned")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

SET(SOURCE_FILES
    src/SDPVer.hpp
    src/SDP.hpp
    src/SDP.cpp
    src/Endianness.hpp
    src/Endianness.cpp
    src/RawFileIO.hpp
    src/RawFileIO.cpp
    src/StreamBufs/SDPCompressionStreamBuf.hpp
    src/StreamBufs/SDPCompressionStreamBuf.cpp
    src/StreamBufs/SDPCryptStreamBuf.hpp
    src/StreamBufs/SDPCryptStreamBuf.cpp
    src/StreamBufs/SDPStreamBuf.hpp
    src/StreamBufs/SDPStreamBuf.cpp
    src/StreamBufs/AlgorithmBase/SDPCompressionAlgorithmBase.hpp
    src/StreamBufs/AlgorithmBase/SDPCompressionAlgorithmBase.cpp
    src/StreamBufs/AlgorithmBase/SDPEncryptionAlgorithmBase.hpp
    src/StreamBufs/AlgorithmBase/SDPEncryptionAlgorithmBase.cpp
    src/StreamBufs/DefaultAlgorithms/SDPAES256GCMAlgorithm.hpp
    src/StreamBufs/DefaultAlgorithms/SDPAES256GCMAlgorithm.cpp
    src/StreamBufs/DefaultAlgorithms/SDPZlibCompressionAlgorithm.hpp
    src/StreamBufs/DefaultAlgorithms/SDPZlibCompressionAlgorithm.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

ADD_LIBRARY(libSDP ${SOURCE_FILES})


#If it's an isolated build then install libs locally
if(IS_BUILD_ISOLATED)

    INCLUDE(FindGit)
    find_package(Git)

    IF(GIT_FOUND)
        MESSAGE("Cloning and compiling internal libraries.")
    ELSE(GIT_FOUND)
        MESSAGE(FATAL_ERROR "Git needs to be installes in order to continue.")
    ENDIF(GIT_FOUND)

    IF(EXTERNAL_LIB_ALREADY_CLONED)
        MESSAGE("Internal libraries already cloned, skipping.")
    ELSE(EXTERNAL_LIB_ALREADY_CLONED)

        #Download CryptoPP-Cmake
        ADD_CUSTOM_TARGET(CRYPTOPP-CMAKE_DOWNLOAD_STEP
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/Cyberunner23/CryptoPP-Cmake.git
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/
        )

        #Configure CryptoPP-Cmake
        ADD_CUSTOM_TARGET(CRYPTOPP-CMAKE_CONFIGURE_STEP
            COMMAND ${CMAKE_COMMAND} . -DCOMPILE_CRYPTOPP_LIB_SHARED=${COMPILE_CRYPTOPP_LIB_SHARED}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/CryptoPP-Cmake/
            DEPENDS CRYPTOPP-CMAKE_DOWNLOAD_STEP
        )

        #Build
        ADD_CUSTOM_TARGET(CRYPTOPP-CMAKE_BUILD_STEP
            COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/CryptoPP-Cmake/
            DEPENDS CRYPTOPP-CMAKE_CONFIGURE_STEP
        )
        ADD_DEPENDENCIES(libSDP CRYPTOPP-CMAKE_BUILD_STEP)

        #CryptoPP-Cmake includes
        INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib/CryptoPP-Cmake/include)

        #Download CVT
        ADD_CUSTOM_TARGET(CVT_DOWNLOAD_STEP
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/Cyberunner23/CVT.git
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tools/
        )

        #Configure CryptoPP-Cmake
        ADD_CUSTOM_TARGET(CVT_CONFIGURE_STEP
            COMMAND ${CMAKE_COMMAND} .
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tools/CVT/
            DEPENDS CVT_DOWNLOAD_STEP
        )

        #Build
        ADD_CUSTOM_TARGET(CVT_BUILD_STEP
            COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tools/CVT/
            DEPENDS CVT_CONFIGURE_STEP
        )
        ADD_DEPENDENCIES(libSDP CVT_BUILD_STEP)

        SET(EXTERNAL_LIB_ALREADY_CLONED ON CACHE BOOL "Sets if the external libraries were already cloned" FORCE)

    ENDIF(EXTERNAL_LIB_ALREADY_CLONED)


    IF(IS_DEV_BUILD)
        #Run CVT
        ADD_CUSTOM_TARGET(CVT_RUN
            COMMAND ${CMAKE_SOURCE_DIR}/tools/CVT/CVT -ib -sop ../../src/SDPVer.hpp -stp ../template.hpp -spp ../progress.txt
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tools/CVT/
        )
        ADD_DEPENDENCIES(libSDP CVT_RUN)
    ENDIF(IS_DEV_BUILD)


    #Library linking:

    #Link CryptoPP
    IF(COMPILE_CRYPTOPP_LIB_SHARED)
        TARGET_LINK_LIBRARIES(libSDP ${CMAKE_SOURCE_DIR}/lib/CryptoPP-Cmake/build/libcryptopp.so)
    ELSE(COMPILE_CRYPTOPP_LIB_SHARED)
        TARGET_LINK_LIBRARIES(libSDP ${CMAKE_SOURCE_DIR}/lib/CryptoPP-Cmake/build/libcryptopp.a)
    ENDIF(COMPILE_CRYPTOPP_LIB_SHARED)


#If it's not an isolated build then use libs from SoulEngine dir
ELSE(IS_BUILD_ISOLATED)

    IF(IS_DEV_BUILD)
        #Run CVT
        ADD_CUSTOM_TARGET(CVT_RUN
            COMMAND ${CMAKE_SOURCE_DIR}/tools/CVT/CVT -ib -sop ${CMAKE_SOURCE_DIR}/src/SDPVer.hpp -stp ${CMAKE_SOURCE_DIR}/tools/template.hpp -spp ${CMAKE_SOURCE_DIR}/tools/progress.txt
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/../../../tools/CVT/
        )
        ADD_DEPENDENCIES(libSDP CVT_RUN)
    ENDIF(IS_DEV_BUILD)


    #Link CryptoPP
    IF(COMPILE_CRYPTOPP_LIB_SHARED)
        TARGET_LINK_LIBRARIES(libSDP ${CMAKE_SOURCE_DIR}/../../external/CryptoPP-Cmake/build/libcryptopp.so)
    ELSE(COMPILE_CRYPTOPP_LIB_SHARED)
        TARGET_LINK_LIBRARIES(libSDP ${CMAKE_SOURCE_DIR}/../../external/CryptoPP-Cmake/build/libcryptopp.a)
    ENDIF(COMPILE_CRYPTOPP_LIB_SHARED)

ENDIF(IS_BUILD_ISOLATED)