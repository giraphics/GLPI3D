#include "VBO.h"
//VBO::VBO() {
	//memObj = new MemData(0);
	//glGenBuffers(1, &memObj->mId);
//}

VBO::VBO(GLenum aTarget)
{
	memObj = new MemData(aTarget);
	glGenBuffers(1, &memObj->mId);
}

VBO::~VBO()
{
	delete memObj;
	memObj = NULL;
}
void VBO::bind()
{
	glBindBuffer(memObj->mTarget, memObj->mId);
}

void VBO::unbind()
{
	glBindBuffer(memObj->mTarget, 0);
}

void VBO::bufferData(size_t size, const void *data, GLenum usage)
{
	glBufferData(memObj->mTarget, size, data, usage);
}

void VBO::bufferSubData(ptrdiff_t offset, size_t size, const void *data)
{
	glBufferSubData(memObj->mTarget, offset, size, data);
}

void* VBO::map(GLenum access)
{
	return glMapBuffer( memObj->mTarget, access);
}

void* VBO::mapRange(size_t offset, size_t length, GLenum access)
{
	return glMapBufferRange( memObj->mTarget, offset, length, access);
}

void VBO::unmap()
{
	glUnmapBuffer( memObj->mTarget );
}

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
