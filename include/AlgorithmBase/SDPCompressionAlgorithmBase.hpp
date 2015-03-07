#ifndef SDPCOMPRESSIONALGORITHMBASE_H
#define SDPCOMPRESSIONALGORITHMBASE_H

#include "typedefs.hpp"

class SDPCompressionAlgorithmBase{

public:

    //vars

    //funcs

    SDPCompressionAlgorithmBase();
    ~SDPCompressionAlgorithmBase();

    virtual uint64 decompress(unsigned char* compressedBuffer,   unsigned char* decompressedBuffer, uint64 compressedBufferSize,   uint64 chunkNum) = 0;
    virtual uint64 compress  (unsigned char* uncompressedBuffer, unsigned char* compressedBuffer,   uint64 uncompressedBufferSize, uint64 chunkNum) = 0;

    virtual void onInit() = 0;
    virtual void onExit() = 0;
    virtual void onSync() = 0;

	uint64 getBufferSize();
	uint64 getBufferSizeWithOverhead();


protected:

	//Vars

	//Funcs

	//Should be set somewhere in the algorithm's constructor
	//Sets the maximum buffer size supported by the algorithm.
	//Used when isStreamCompression = false
	void setMaxBufferSize(uint64 maxBufferSize);

	//Should be set somewhere in the algorithm's constructor after setIsStreamCompression(true/false)
	//and setMaxBufferSize(uint64)
	//Sets the buffer size to be used when isStreamCipher = false (block compression)
	bool setPreferedBufferSize(uint64 preferedBufferSize);

    //Should be set somewhere in the algorithm's constructor after setIsStreamCompression(true/false)
    //and setMaxBufferSize(uint64)
	//Buffer size including possible overhead on compression to be used when isStreamCompression = false
	//This is in case the data is incompressible and that the algorithm introduces an overhead.
	//This is to prevent an overflow and a SIGSEGV.
	bool setPreferedBufferSizeWithOverhead(uint64 preferedBufferSizeWithOverhead);


private:

    //vars

	//Maximum buffer size supported by the algorithm, used when isStreamCompression = false
	//Used when isStreamCompression = false
	uint64 maxBufferSize;

	//Buffer size that will be used when isStreamCompression = false
	//This may be smaller than the prefered buffer size if it is set to be larger than
	//what the algorithm can handle.
	uint64 bufferSize = 0;

	//Buffer size including possible overhead on compression to be used when isStreamCompression = false
	//This is in case the data is incompressible and that the algorithm introduces an overhead.
	//This is to prevent an overflow and a SIGSEGV.
	uint64 bufferSizeWithOverhead = 0;

    //funcs


};

#endif // SDPCOMPRESSIONALGORITHMBASE_H
