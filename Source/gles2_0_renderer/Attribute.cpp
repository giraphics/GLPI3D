#include "Attribute.h"

Attribute::Attribute(std::string nm, GLint itemPerElement, size_t sz, GLenum typeInfo, void* arr, int strideSize)
{
	name		= nm;
	dataArray	= arr;
	index		= 0;
	size		= sz;
	type		= typeInfo;
	itemNum		= itemPerElement;
	stride		= strideSize;
}
	
Attribute::~Attribute()
{
}
