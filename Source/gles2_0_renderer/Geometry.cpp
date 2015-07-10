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

Attribute::Attribute(std::string nm, GLint itemPerElement, size_t sz, GLenum typeInfo, void* arr)
{
	name		= nm;
	dataArray	= arr;
	index		= 0;
	size		= sz;
	type		= typeInfo;
	itemNum		= itemPerElement;
}
	
Attribute::~Attribute()
{
}

void drawArray11(){}
GeometryBuffer::GeometryBuffer(IModel* prnt, BufferScheme scheme, bool isInterleaved, DrawingScheme drawScheme)
{
	parent		= prnt;
	interleaved = isInterleaved;
	schemeBuf	= scheme;
	schemeDraw	= drawScheme;
	vbo			= NULL;
	ibo			= NULL;
	vao			= NULL;
	primitiveType = GL_TRIANGLE_STRIP;

	switch(schemeBuf){
		case BUFFER_VAO:
		{
			vbo = new VBO(GL_ARRAY_BUFFER);
			vao = new VAO();
			if(schemeDraw == DRAW_ELEMENT){
				ibo = new VBO(GL_ELEMENT_ARRAY_BUFFER);
			}
			else{
				ibo = NULL;
			}
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

	// Here, we store the function pointer of DrawArray or DrawElement command
	// This allows to avoid 'if' checks during the  
	if(schemeDraw == DRAW_ELEMENT){
		drawMethod = &GeometryBuffer::drawElement;
	}
	else{
		drawMethod = &GeometryBuffer::drawArray;
	}
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

	for(int i=0; i<attributeList.size(); i++){
		delete attributeList.at(i);
	}

	for(int i=0; i<uniformList.size(); i++){
		delete uniformList.at(i);
	}	
}

void GeometryBuffer::addAttribute(Attribute* attributeItem)
{
	attributeList.push_back(attributeItem);
}

void GeometryBuffer::addUniform(Uniform* uniformItem){
	uniformList.push_back(uniformItem);
}

void GeometryBuffer::setIndices(Indices* indexItem){
	indexList = indexItem;
}

GLenum GeometryBuffer::GetPrimitiveMode(PrimitiveScheme primitiveMode)
{
	switch(primitiveMode)
{
		case PRIMITIVE_POINTS:
			return GL_POINTS;
			break;
	
		case PRIMITIVE_LINES:
			return GL_LINES;
			break;
	
		case PRIMITIVE_LINE_LOOP:
			return GL_LINE_LOOP;
			break;
	
		case PRIMITIVE_LINE_STRIP:
			return GL_LINE_STRIP;
			break;
	
		case PRIMITIVE_TRIANGLES:
			return GL_TRIANGLES;
			break;

		case PRIMITIVE_TRIANGLE_STRIP:
			return GL_TRIANGLE_STRIP;
		break;
	
		case PRIMITIVE_TRIANGLE_FAN:
			return GL_TRIANGLE_FAN;
		break;
		default:
			return GL_TRIANGLE_STRIP;
			break;
				}
			}
void GeometryBuffer::initUniforms()
{
	unsigned int ProgramID = parent->GetProgram();
	glUseProgram(ProgramID);
	for(int i=0; i<uniformList.size(); i++){
		uniformList[i]->SetUniformLocation(GetUniform(ProgramID,(char*)uniformList[i]->GetName().c_str()));
		}
}

void GeometryBuffer::init()
{
	unsigned int ProgramID = parent->GetProgram();
	glUseProgram(ProgramID);

	// HANDLE ATTRIBUTES
	switch(schemeBuf){
		case BUFFER_VAO:
		{
	// Treat the Attributes
	int total = 0;
	for(int i=0; i<attributeList.size(); i++){
		attributeList[i]->index = total;
		total += attributeList[i]->size * attributeList[i]->itemNum * sizeof(attributeList[i]->type);
	}

			vbo->bind();
			vbo->bufferData(total, 0, GL_STATIC_DRAW);
			size_t from = 0;
			size_t to	= 0;
			for(int i=0; i<attributeList.size(); i++){
				to = attributeList[i]->size * attributeList[i]->itemNum * sizeof(attributeList[i]->type);
				vbo->bufferSubData(from, to, attributeList[i]->dataArray);
				from = to;
			}
			vbo->unbind();

			if(schemeDraw == DRAW_ELEMENT){ // Use the ibo if drawing scheme is DrawElement
				ibo->bind();
				unsigned short indexSize = indexList->size * sizeof(indexList->type);
				ibo->bufferData(indexSize, 0, GL_STATIC_DRAW);
				ibo->bufferSubData(0, indexSize, indexList->dataArray);
				ibo->unbind();
			}

			vao->bind();
			vbo->bind();
			for(int i=0; i<attributeList.size(); i++){
				attributeList[i]->attributeLocation = GetAttribute(ProgramID,(char*)attributeList[i]->name.c_str());
				if(attributeList[i]->attributeLocation >= 0){
					glEnableVertexAttribArray(attributeList[i]->attributeLocation);
					glVertexAttribPointer(attributeList[i]->attributeLocation, attributeList[i]->itemNum, attributeList[i]->type, GL_FALSE, 0, (void*)attributeList[i]->index);
				}
			}

			if(schemeDraw == DRAW_ELEMENT){
				ibo->bind();
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
				attributeList[i]->attributeLocation = GetAttribute(ProgramID,(char*)attributeList[i]->name.c_str());
				if(attributeList[i]->attributeLocation >= 0){
					glEnableVertexAttribArray(attributeList[i]->attributeLocation);
				}
			}
		}
		break;
	}

	// HANDLE UNIFORMS
	initUniforms();
	//for(int i=0; i<uniformList.size(); i++){
	//	uniformList[i]->SetUniformLocation(GetUniform(ProgramID,(char*)uniformList[i]->GetName().c_str()));
	//}

	// HANDLE INDICES

}

// In my opinion the update should be bring inside the bind function.
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
			for(int i=0; i<attributeList.size(); i++){
				if(attributeList[i]->attributeLocation >= 0){
					glVertexAttribPointer(attributeList[i]->attributeLocation, attributeList[i]->itemNum, attributeList[i]->type, GL_FALSE, 0, (void*)attributeList[i]->dataArray);
				}
			}
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

void GeometryBuffer::update()
{
	unsigned int ProgramID = parent->GetProgram();
	glUseProgram(ProgramID);
	for(int i=0; i<uniformList.size(); i++){
		uniformList[i]->update();
	}
}

void GeometryBuffer::draw(){
	(this->*drawMethod)();
}

void GeometryBuffer::drawArray(){
	glDrawArrays(primitiveType, 0, geometryData.positions->size());
}

void GeometryBuffer::drawElement(){
	glDrawElements(primitiveType, indexList->size, indexList->type, (void*)geometryData.geometryIndices->size());
}
