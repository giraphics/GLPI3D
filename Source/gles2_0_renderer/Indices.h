#ifndef INDICES_h
#define INDICES_h

class Indices{
public:
	Indices(size_t indexCount, GLenum typeInfo, void* arr){ size = indexCount; type = typeInfo; dataArray = arr; indexOrDataPtr = NULL;}
	
	~Indices(){}
	
	void* dataArray;

	// The indexOrDataPtr is common interface for drawing API for either (VA) or (VBO and VAO) 
	void* indexOrDataPtr;
	
	GLenum type;
	
	size_t size;
};

struct InterleavedBufferInfo{
	void* dataBuffer;
	size_t size;
};

#endif
