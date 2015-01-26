CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)
PROJECT(libSDP C CXX)

#TODO: Major cleanup.
#TODO: using ${CMAKE_CURRENT_SOURCE_DIR} -> using ${SOULENGINE_SOURCE_DIR} for lib path finding.
#TODO: make an include dir for people to use.

#----------------------------------------------------------
####################### OPTIONS ###########################
#----------------------------------------------------------
OPTION(COMPILE_LIBSDP_LIB_SHARED   "Build libSDP as a shared lib (setting this to OFF will compile statically)" ON)
OPTION(BUILD_SODIUM_SHARED         "Build libsodium-CMake as a shared lib (setting this to OFF will compile statically)")
#SET(SOULENGINE_SOURCE_DIR "" PATH "Top level directory of Soul Engine (Will be set by Soul Engine's CMakeLists.txt, setting this will cause ${PROJECT_NAME} to use Soul Engine's libraries.)")


#----------------------------------------------------------
#################### INTERNAL/DEV VARS ####################
#----------------------------------------------------------
SET(IS_DEV_BUILD                ON CACHE BOOL  "Set to ON if its a development build for libSDP, set to OFF otherwise.")
SET(CVT_ALREADY_CLONED          OFF CACHE BOOL "Sets if CVT was already cloned")# FORCE) #Add FORCE to reset if needed.
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
    src/RawFileIO.cpp
    src/SDPCompressionStreamBuf.cpp
    src/SDPCryptStreamBuf.cpp
    src/SDPStreamBuf.cpp
    src/AlgorithmBase/SDPCompressionAlgorithmBase.cpp
    src/AlgorithmBase/SDPEncryptionAlgorithmBase.cpp

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
    include/RawFileIO.hpp
    include/SDPCompressionStreamBuf.hpp
    include/SDPCryptStreamBuf.hpp
    include/SDPStreamBuf.hpp
    include/SDPVer.hpp
    include/AlgorithmBase/SDPCompressionAlgorithmBase.hpp
    include/AlgorithmBase/SDPEncryptionAlgorithmBase.hpp
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




IF(COMPILE_LIBSDP_LIB_SHARED)
    ADD_LIBRARY(SDP SHARED ${SOURCE_FILES} ${HEADER_FILES})
ELSE(COMPILE_LIBSDP_LIB_SHARED)
    ADD_LIBRARY(SDP STATIC ${SOURCE_FILES} ${HEADER_FILES})
ENDIF(COMPILE_LIBSDP_LIB_SHARED)

SET_TARGET_PROPERTIES(SDP PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    PROJECT_LABEL "libSDP"
)

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

        #Download libsodium-Cmake
        ADD_CUSTOM_TARGET(LIBSODIUM-CMAKE_DOWNLOAD_STEP
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/Cyberunner23/libsodium-CMake.git
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/
        )

        #Configure libsodium-Cmake
        ADD_CUSTOM_TARGET(LIBSODIUM-CMAKE_CONFIGURE_STEP
            COMMAND ${CMAKE_COMMAND} -DBUILD_SODIUM_SHARED=${BUILD_SODIUM_SHARED} ..
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake/build
            DEPENDS LIBSODIUM-CMAKE_DOWNLOAD_STEP
        )

        #Build libsodium-Cmake
        ADD_CUSTOM_TARGET(LIBSODIUM-CMAKE_BUILD_STEP
            COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake/build/
            DEPENDS LIBSODIUM-CMAKE_CONFIGURE_STEP
        )
        ADD_DEPENDENCIES(SDP LIBSODIUM-CMAKE_BUILD_STEP)

        SET(EXTERNAL_LIB_ALREADY_CLONED ON CACHE BOOL "Sets if the external libraries were already cloned" FORCE)

    ENDIF(EXTERNAL_LIB_ALREADY_CLONED)

    #Include CryptoPP-Cmake even if download and compile is not needed.
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/extlibs/libsodium-CMake/include)

    #If libSDP is being developped then get CVT and run it.
    IF(IS_DEV_BUILD)

        #If CVT is already downloaded and compiled then just run it.
        IF(CVT_ALREADY_CLONED)

            #Run CVT
            ADD_CUSTOM_TARGET(CVT_RUN
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/CVT/build/CVT -ib -sop ../../include/SDPVer.hpp -stp ../template.hpp -spp ../progress.txt
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools/CVT/
            )
            ADD_DEPENDENCIES(SDP CVT_RUN)

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
            ADD_CUSTOM_TARGET(CVT_RUN
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/CVT/build/CVT -ib -sop ../../src/SDPVer.hpp -stp ../template.hpp -spp ../progress.txt
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tools/CVT/
                DEPENDS CVT_BUILD_STEP
            )
            ADD_DEPENDENCIES(SDP CVT_RUN)

            SET(CVT_ALREADY_CLONED ON CACHE BOOL "Sets if CVT was already cloned" FORCE)

        ENDIF(CVT_ALREADY_CLONED)

    ENDIF(IS_DEV_BUILD)


    #Library linking:

    #Link CryptoPP
    IF(COMPILE_CRYPTOPP_LIB_SHARED)
        TARGET_LINK_LIBRARIES(SDP ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/CryptoPP-Cmake/build/libcryptopp.so)
    ELSE(COMPILE_CRYPTOPP_LIB_SHARED)
        TARGET_LINK_LIBRARIES(SDP ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/CryptoPP-Cmake/build/libcryptopp.a)
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