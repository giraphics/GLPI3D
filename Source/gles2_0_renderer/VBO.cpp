#include "VBO.h"
#include "ProgramManager.h"

#define GetAttribute ProgramManager::GetInstance()->ProgramGetVertexAttribLocation
#define GetUniform ProgramManager::GetInstance()->ProgramGetUniformLocation

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

AttributeInfo::AttributeInfo(std::string nm, GLint itemPerElement, size_t sz, GLenum typeInfo, void* arr)
{
	name		= nm;
	dataArray	= arr;
	index		= 0;
	size		= sz;
	type		= typeInfo;
	itemNum		= itemPerElement;
}
	
AttributeInfo::~AttributeInfo()
{
}

GeometryBuffer::GeometryBuffer(IModel* prnt, BufferScheme scheme, bool isInterleaved)
{
	parent		= prnt;
	interleaved = isInterleaved;
	schemeBuf	= scheme;
}

GeometryBuffer::~GeometryBuffer()
{
}

void GeometryBuffer::addAttribute(AttributeInfo attr)
{
	attributeList.push_back(attr);
}

void GeometryBuffer::init()
{
	unsigned int ProgramID = parent->GetProgram();
	glUseProgram(ProgramID);
	for(int i=0; i<attributeList.size(); i++){
		attributeList[i].index = GetAttribute(ProgramID,(char*)attributeList[i].name.c_str());
	}
	switch(this->schemeBuf){
		case BUFFER_VBO:
			{
			}
			break;
		case BUFFER_VAO:
			{
			}
			break;
		default:
			{
			}
			break;
	}
}
