#ifndef ATTRIBUTE_h
#define ATTRIBUTE_h
#include "glutils.h"
#include <string>

class Attribute{
public:
	Attribute(std::string name, GLint itemPerElement, size_t size, GLenum typeInfo, void* arr, int strideSize = 0);
	~Attribute();
	std::string name;
	void* dataArray;
	int index;
	int attributeLocation;
	int stride;
	GLenum type;
	GLint itemNum;
	size_t size;
};


#endif
