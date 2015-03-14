
ADD_SUBDIRECTORY("compressionTests/DEFLATECompressionTest")
ADD_TEST(NAME DEFLATECompressionTest
    COMMAND DEFLATECompressionTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/compressionTests/DEFLATECompressionTest
)

ADD_SUBDIRECTORY("encryptionTests/XSalsa20EncryptionTest")
ADD_TEST(NAME XSalsa20EncryptionTest
    COMMAND XSalsa20EncryptionTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/encryptionTests/XSalsa20EncryptionTest
)


