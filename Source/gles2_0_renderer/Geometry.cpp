#include "Geometry.h"
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
	vbo			= NULL;
	vao			= NULL;

	switch(schemeBuf){
		case BUFFER_VAO:
		{
			vbo = new VBO(GL_ARRAY_BUFFER);
			vao = new VAO();
		}
		break;
		case BUFFER_VBO:
		{
			vbo = new VBO(GL_ARRAY_BUFFER);
		}
		break;
		default:
		{
		}
	};
}

GeometryBuffer::~GeometryBuffer()
{
	if(vbo){
		delete vbo;
		vbo = NULL;
	}

	if(vao){
		delete vao;
		vao = NULL;
	}
}

void GeometryBuffer::addAttribute(AttributeInfo attr)
{
	attributeList.push_back(attr);
}

void GeometryBuffer::sendAttributeData()
{
	unsigned int ProgramID = parent->GetProgram();
	glUseProgram(ProgramID);

	switch(schemeBuf){
		case BUFFER_VAO:
		{
		}
		break;
		case BUFFER_VBO:
		{
		}
		break;
		default:
		{
			for(int i=0; i<attributeList.size(); i++){
				if(attributeList[i].attributeLocation >= 0){
					glVertexAttribPointer(attributeList[i].attributeLocation, attributeList[i].itemNum, attributeList[i].type, GL_FALSE, 0, (void*)attributeList[i].dataArray);
				}
			}
		}
		break;
	}

}

void GeometryBuffer::init()
{
	unsigned int ProgramID = parent->GetProgram();
	glUseProgram(ProgramID);
	int total = 0;
	for(int i=0; i<attributeList.size(); i++){
		attributeList[i].index = total;
		total += attributeList[i].size * attributeList[i].itemNum * sizeof(attributeList[i].type);
	}

	switch(schemeBuf){
		case BUFFER_VAO:
		{
			vbo->bind();
			vbo->bufferData(total, 0, GL_STATIC_DRAW);
			size_t from = 0;
			size_t to	= 0;
			for(int i=0; i<attributeList.size(); i++){
				to = attributeList[i].size * attributeList[i].itemNum * sizeof(attributeList[i].type);
				vbo->bufferSubData(from, to, attributeList[i].dataArray);
				from = to;
			}

			vao->bind();
			vbo->bind();
			for(int i=0; i<attributeList.size(); i++){
				attributeList[i].attributeLocation = GetAttribute(ProgramID,(char*)attributeList[i].name.c_str());
				if(attributeList[i].attributeLocation >= 0){
					glEnableVertexAttribArray(attributeList[i].attributeLocation);
					glVertexAttribPointer(attributeList[i].attributeLocation, attributeList[i].itemNum, attributeList[i].type, GL_FALSE, 0, (void*)attributeList[i].index);
				}
			}
			vbo->unbind();
			vao->unbind();
		}
		break;
		case BUFFER_VBO:
		{
		}
		break;
		default:
		{
			for(int i=0; i<attributeList.size(); i++){
				attributeList[i].attributeLocation = GetAttribute(ProgramID,(char*)attributeList[i].name.c_str());
				if(attributeList[i].attributeLocation >= 0){
					glEnableVertexAttribArray(attributeList[i].attributeLocation);
				}
			}
		}
		break;
	}
}

void GeometryBuffer::bind()
{
	switch(schemeBuf){
		case BUFFER_VAO:
		{
			vao->bind();
		}
		break;
		case BUFFER_VBO:
		{
			vbo->bind();
		}
		break;
		default:
		{
		}
	};
}

void GeometryBuffer::unbind()
{
	switch(schemeBuf){
		case BUFFER_VAO:
		{
			vao->unbind();
		}
		break;
		case BUFFER_VBO:
		{
			vbo->unbind();
		}
		break;
		default:
		{
		}
	};
}

GeometryMesh* GeometryBuffer::geometry(){
	return &geometryData;
}
