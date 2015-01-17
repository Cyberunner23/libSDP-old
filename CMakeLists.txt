CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)
PROJECT(libSDP)

#TODO: using ${CMAKE_CURRENT_SOURCE_DIR} -> using ${SOULENGINE_SOURCE_DIR} for lib path finding.
#TODO: mane an include dir for people to use.

#----------------------------------------------------------
#Set options here.
#----------------------------------------------------------
OPTION(COMPILE_LIBSDP_LIB_SHARED   "Build libSDP as a shared lib (setting this to OFF will compile statically)" ON)
OPTION(COMPILE_CRYPTOPP_LIB_SHARED "Build cryptopp as a shared lib (setting this to OFF will compile statically)" ON)
#SET(SOULENGINE_SOURCE_DIR "" PATH "Top level directory of Soul Engine (Will be set by Soul Engine's CMakeLists.txt, setting this will cause ${PROJECT_NAME} to use Soul Engine's libraries.)")

#----------------------------------------------------------
#Internal/Dev vars.
#----------------------------------------------------------
SET(IS_DEV_BUILD                ON CACHE BOOL  "Set to ON if its a development build for libSDP, set to OFF otherwise.")
SET(CVT_ALREADY_CLONED          OFF CACHE BOOL "Sets if CVT was already cloned") #Add FORCE to reset if needed.
SET(EXTERNAL_LIB_ALREADY_CLONED OFF CACHE BOOL "Sets if the external libraries were already cloned") #Add FORCE to reset if needed.

#----------------------------------------------------------
#Compiler settings.
#----------------------------------------------------------
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#----------------------------------------------------------
#Source and Include Files.
#----------------------------------------------------------
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
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

#----------------------------------------------------------
#Main Library
#----------------------------------------------------------
IF(COMPILE_LIBSDP_LIB_SHARED)
    ADD_LIBRARY(libSDP SHARED ${SOURCE_FILES})
ELSE(COMPILE_LIBSDP_LIB_SHARED)
    ADD_LIBRARY(libSDP STATIC ${SOURCE_FILES})
ENDIF(COMPILE_LIBSDP_LIB_SHARED)

#----------------------------------------------------------
#Is the library being compiled inside SoulEngine checker.
#----------------------------------------------------------

#If library not used inside SoulEngine (on its own)
#Download and compile dependencies.
if(NOT DEFINED SOULENGINE_SOURCE_DIR)

    #Find Git
    INCLUDE(FindGit)
    find_package(Git)
    IF(GIT_FOUND)
        MESSAGE("Git found, attempting to clone and compile internal libraries and tools.")
    ELSE(GIT_FOUND)
        MESSAGE(FATAL_ERROR "Git needs to be installes in order to continue.")
    ENDIF(GIT_FOUND)

    #If dependencies already downloaded and compiled then skip.
    IF(EXTERNAL_LIB_ALREADY_CLONED)
        MESSAGE("Internal libraries already cloned, skipping.")
    ELSE(EXTERNAL_LIB_ALREADY_CLONED)

        #Download CryptoPP-Cmake
        ADD_CUSTOM_TARGET(CRYPTOPP-CMAKE_DOWNLOAD_STEP
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/Cyberunner23/CryptoPP-Cmake.git
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/
        )

        #Configure CryptoPP-Cmake
        ADD_CUSTOM_TARGET(CRYPTOPP-CMAKE_CONFIGURE_STEP
            COMMAND ${CMAKE_COMMAND} . -DCOMPILE_CRYPTOPP_LIB_SHARED=${COMPILE_CRYPTOPP_LIB_SHARED}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/CryptoPP-Cmake/
            DEPENDS CRYPTOPP-CMAKE_DOWNLOAD_STEP
        )

        #Build CryptoPP-Cmake
        ADD_CUSTOM_TARGET(CRYPTOPP-CMAKE_BUILD_STEP
            COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/CryptoPP-Cmake/
            DEPENDS CRYPTOPP-CMAKE_CONFIGURE_STEP
        )
        ADD_DEPENDENCIES(libSDP CRYPTOPP-CMAKE_BUILD_STEP)

        SET(EXTERNAL_LIB_ALREADY_CLONED ON CACHE BOOL "Sets if the external libraries were already cloned" FORCE)

    ENDIF(EXTERNAL_LIB_ALREADY_CLONED)

    #Include CryptoPP-Cmake even if download and compile is not needed.
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib/CryptoPP-Cmake/include)

    #If libSDP is being developped then get CVT and run it.
    IF(IS_DEV_BUILD)

        #If CVT is already downloaded and compiled then just run it.
        IF(CVT_ALREADY_CLONED)

            #Run CVT
            MESSAGE("CVT")
            ADD_CUSTOM_TARGET(CVT_RUN
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/CVT/build/CVT -ib -sop ../../src/SDPVer.hpp -stp ../template.hpp -spp ../progress.txt
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools/CVT/
            )
            ADD_DEPENDENCIES(libSDP CVT_RUN)

        #If CVT is not already downloaded and compiled then
        #download, compile and run it.
        ELSE(CVT_ALREADY_CLONED)

            #Download CVT
            ADD_CUSTOM_TARGET(CVT_DOWNLOAD_STEP
                COMMAND ${GIT_EXECUTABLE} clone https://github.com/Cyberunner23/CVT.git
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools/
            )

            #Configure CVT
            ADD_CUSTOM_TARGET(CVT_CONFIGURE_STEP
                COMMAND ${CMAKE_COMMAND} .
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools/CVT/
                DEPENDS CVT_DOWNLOAD_STEP
            )

            #Build CVT
            ADD_CUSTOM_TARGET(CVT_BUILD_STEP
                COMMAND ${CMAKE_COMMAND} --build .
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools/CVT/
                DEPENDS CVT_CONFIGURE_STEP
            )

            #Run CVT
            MESSAGE("CVT")
            ADD_CUSTOM_TARGET(CVT_RUN
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/CVT/build/CVT -ib -sop ../../src/SDPVer.hpp -stp ../template.hpp -spp ../progress.txt
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools/CVT/
                DEPENDS CVT_BUILD_STEP
            )
            ADD_DEPENDENCIES(libSDP CVT_RUN)

            SET(CVT_ALREADY_CLONED ON CACHE BOOL "Sets if CVT was already cloned" FORCE)

        ENDIF(CVT_ALREADY_CLONED)

    ENDIF(IS_DEV_BUILD)


    #Library linking:

    #Link CryptoPP
    IF(COMPILE_CRYPTOPP_LIB_SHARED)
        TARGET_LINK_LIBRARIES(libSDP ${CMAKE_CURRENT_SOURCE_DIR}/lib/CryptoPP-Cmake/build/libcryptopp.so)
    ELSE(COMPILE_CRYPTOPP_LIB_SHARED)
        TARGET_LINK_LIBRARIES(libSDP ${CMAKE_CURRENT_SOURCE_DIR}/lib/CryptoPP-Cmake/build/libcryptopp.a)
    ENDIF(COMPILE_CRYPTOPP_LIB_SHARED)


#TODO: Put a better comment here...
ELSE(NOT DEFINED SOULENGINE_SOURCE_DIR)

    IF(IS_DEV_BUILD)
        #Run CVT
        ADD_CUSTOM_TARGET(CVT_RUN
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/CVT/CVT -ib -sop ${CMAKE_CURRENT_SOURCE_DIR}/src/SDPVer.hpp -stp ${CMAKE_CURRENT_SOURCE_DIR}/tools/template.hpp -spp ${CMAKE_CURRENT_SOURCE_DIR}/tools/progress.txt
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../../tools/CVT/
        )
        ADD_DEPENDENCIES(libSDP CVT_RUN)
    ENDIF(IS_DEV_BUILD)


    #TODO: put includes here


    #Link CryptoPP
    IF(COMPILE_CRYPTOPP_LIB_SHARED)
        TARGET_LINK_LIBRARIES(libSDP ${CMAKE_CURRENT_SOURCE_DIR}/../../external/CryptoPP-Cmake/build/libcryptopp.so)
    ELSE(COMPILE_CRYPTOPP_LIB_SHARED)
        TARGET_LINK_LIBRARIES(libSDP ${CMAKE_CURRENT_SOURCE_DIR}/../../external/CryptoPP-Cmake/build/libcryptopp.a)
    ENDIF(COMPILE_CRYPTOPP_LIB_SHARED)

ENDIF(NOT DEFINED SOULENGINE_SOURCE_DIR)