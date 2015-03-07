#ifndef SDPCOMPRESSIONALGORITHMBASE_H
#define SDPCOMPRESSIONALGORITHMBASE_H

#include "typedefs.hpp"

class SDPCompressionAlgorithmBase{

public:

    //vars

    //funcs

    SDPCompressionAlgorithmBase();
    ~SDPCompressionAlgorithmBase();

	//These functions will only be used if isStreamCompression = false
    virtual uint64 decompress(unsigned char* compressedBuffer,   unsigned char* decompressedBuffer, uint64 compressedBufferSize) = 0;
    virtual uint64 compress  (unsigned char* uncompressedBuffer, unsigned char* compressedBuffer,   uint64 uncompressedBufferSize) = 0;

	//These functions will only be used if isStreamCompression = true
	virtual bool decompressStream(unsigned char compressedChar, unsigned char decompressedChar) = 0;
	virtual bool compressStream  (unsigned char uncompressedChar, unsigned char compressedChar) = 0;

    virtual void onInit() = 0;
    virtual void onExit() = 0;
    virtual void onSync() = 0;

    bool		   getIsStreamCompression();
	uint64 getBufferSize();
	uint64 getBufferSizeWithOverhead();


protected:

	//Vars

	//Funcs

	//Should be set somewhere in the algorithm's constructor or onInit?
	//true = stream compression, false = block compression.
	void setIsStreamCompression(bool isStreamCompression);

	//Should be set somewhere in the algorithm's constructor or onInit?
	//Sets the maximum buffer size supported by the algorithm.
	//Used when isStreamCompression = false
	void setMaxBufferSize(uint64 maxBufferSize);

	//Should be set somewhere in the algorithm's constructor or onInit? after setMaxBufferSize(true/false)
	//and setMaxBufferSize(uint64)
	//Sets the buffer size to be used when isStreamCipher = false (block compression)
	bool setPreferedBufferSize(uint64 preferedBufferSize);

	//Buffer size including possible overhead on compression to be used when isStreamCompression = false
	//This is in case the data is incompressible and that the algorithm introduces an overhead.
	//This is to prevent an overflow and a SIGSEGV.
	void setPreferedBufferSizeWithOverhead(uint64 preferedBufferSizeWithOverhead);


private:

    //vars

	//true = stream compression, false = block compression
	bool isStreamCompression;

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
