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


ADD_SUBDIRECTORY("compressionTests/DEFLATECompressionTest")
ADD_TEST(NAME DEFLATECompressionTest
    COMMAND DEFLATECompressionTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/compressionTests/DEFLATECompressionTest
)

ADD_SUBDIRECTORY("compressionTests/lz4CompressionTest")
ADD_TEST(NAME lz4CompressionTest
    COMMAND lz4CompressionTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/compressionTests/lz4CompressionTest
)


ADD_SUBDIRECTORY("encryptionTests/XSalsa20EncryptionTest")
ADD_TEST(NAME XSalsa20EncryptionTest
    COMMAND XSalsa20EncryptionTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/encryptionTests/XSalsa20EncryptionTest
)


