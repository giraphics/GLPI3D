#include "VAO.h"

VAO::VAO()
{
	memObj = new MemData();
	glGenVertexArrays(1, &memObj->mId);
}

VAO::~VAO()
{
	delete memObj;
	memObj = NULL;
}

void VAO::bind()
{
	glBindVertexArray(memObj->mId);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}
