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


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../include/AlgorithmBase)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../include/DefaultAlgorithms)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../../include/StreamBufs)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../common)

SET(COMMON_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/common.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common/common.cpp
)

ADD_EXECUTABLE(lz4CompressionTest
    ${COMMON_FILES}
    main.cpp
)
TARGET_LINK_LIBRARIES(lz4CompressionTest libSDP)
SET_TARGET_PROPERTIES(lz4CompressionTest
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
SET(additionalCleanFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/data64k_COMPRESSED
    ${CMAKE_CURRENT_SOURCE_DIR}/data64k_DECOMPRESSED
    ${CMAKE_CURRENT_SOURCE_DIR}/randomData64k_COMPRESSED
    ${CMAKE_CURRENT_SOURCE_DIR}/randomData64k_DECOMPRESSED
)
SET_DIRECTORY_PROPERTIES(PROPERTIES
ADDITIONAL_MAKE_CLEAN_FILES "${additionalCleanFiles}")