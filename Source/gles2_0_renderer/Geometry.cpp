#include "Geometry.h"
#include "ProgramManager.h"

#define GetAttribute ProgramManager::GetInstance()->ProgramGetVertexAttribLocation
#define GetUniform ProgramManager::GetInstance()->ProgramGetUniformLocation

GeometryBuffer::GeometryBuffer(IModel* prnt, BufferScheme scheme, DrawingScheme drawScheme, bool isInterleavedForm)
{
	parent			= prnt;
	schemeBuf		= scheme;
	schemeDraw		= drawScheme;
	vbo				= NULL;
	ibo				= NULL;
	vao				= NULL;
	indexList		= NULL;
	primitiveType	= GL_TRIANGLE_STRIP;

	// Initialized interleave form of data
	isInterleaved				= (int)isInterleavedForm;
	interleaveBuffer.dataBuffer = NULL;
	interleaveBuffer.size		= 0;

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
			if(schemeDraw == DRAW_ELEMENT){
				ibo = new VBO(GL_ELEMENT_ARRAY_BUFFER);
			}
			else{
				ibo = NULL;
			}
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

	if(ibo){
		delete ibo;
		ibo = NULL;
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

void GeometryBuffer::setInterleavedBuffer(void* dataArray, size_t size){
	interleaveBuffer.dataBuffer = dataArray;
	interleaveBuffer.size		= size;
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

void GeometryBuffer::populateIBO()
{
	if(schemeDraw == DRAW_ELEMENT){ // Use the ibo if drawing scheme is DrawElement
		ibo->bind();
		unsigned short indexSize = indexList->size * sizeof(indexList->type);
		ibo->bufferData(indexSize, 0, GL_STATIC_DRAW);
		unsigned short* temp = (unsigned short*)indexList->dataArray;
		ibo->bufferSubData(0, indexSize, indexList->dataArray);
		ibo->unbind();
	}
}

void GeometryBuffer::enableAttributes()
{
	unsigned int ProgramID = parent->GetProgram();
	glUseProgram(ProgramID);
	for(int i=0; i<attributeList.size(); i++){
		attributeList[i]->attributeLocation = GetAttribute(ProgramID,(char*)attributeList[i]->name.c_str());
		if(attributeList[i]->attributeLocation >= 0){
			glEnableVertexAttribArray(attributeList[i]->attributeLocation);
		}
	}
}

void GeometryBuffer::vertexAttribPointerVBO()
{
	for(int i=0; i<attributeList.size(); i++){
		if(attributeList[i]->attributeLocation >= 0){
			glVertexAttribPointer(attributeList[i]->attributeLocation, attributeList[i]->itemNum, attributeList[i]->type, GL_FALSE, attributeList[i]->stride, (void*)attributeList[i]->index);
		}
	}
}

void GeometryBuffer::vertexAttribPointerVA()
{
	for(int i=0; i<attributeList.size(); i++){
		if(attributeList[i]->attributeLocation >= 0){
			glVertexAttribPointer(attributeList[i]->attributeLocation, attributeList[i]->itemNum, attributeList[i]->type, GL_FALSE, 0, (void*)attributeList[i]->dataArray);
		}
	}
}

// There are two situtation for VBO, the data can be of two forms
//	1. Interleaved: Interleaved data is for case like Rectangle and Pixmap which the end user supplies the attribute in different lists.
//                  In this case the incoming data exist in the form of discreate list.
/*

Non-Interleaved                                  VBO with one data buffer             
===============                                  ========================             
+----------+  +----------+  +----------+       +-----------------------------+        
|VVVVVVVVVV|  |UUUUUUUUUU|  |CCCCCCCCCC|       |VVVVVVVVVVUUUUUUUUUCCCCCCCCCC|        
+----------+  +----------+  +----------+       +-----------------------------+        
 SetVertices   SetTextures   SetColors          ^         ^        ^             STRIDE = 0       
                                                                                      
*/                                                                                      
                                                                                      
//	2. Non-Interleaved: Cases like meshes could be complex and may contain the data in the list or processed interleaved form.
/*                                                                                      
     Interleaved                                  VBO with one data buffer            
     ===========                                  ========================            
                                                                                      
 +------------------------------------+         +------------------------------------+
 |VUCVUCVUCVUCVUCVUCVUCVUCVUCVUCVUCVUC|         |VUCVUCVUCVUCVUCVUCVUCVUCVUCVUCVUCVUC|
 +------------------------------------+         +------------------------------------+
                                                 ^^^                                   STRIDE != 0 
*/
void GeometryBuffer::populateVBO()
{
	if(!this->isInterleaved){
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
	}
	else{
		char* base = (char*)this->interleaveBuffer.dataBuffer;
		for(int i=0; i<attributeList.size(); i++){
			attributeList[i]->index = (char*)attributeList[i]->dataArray - base;
		}

		vbo->bind();
		vbo->bufferData(interleaveBuffer.size, interleaveBuffer.dataBuffer, GL_STATIC_DRAW);		
		vbo->unbind();
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
			populateVBO();
			
			populateIBO();

			vao->bind();
				vbo->bind();
					enableAttributes();
					vertexAttribPointerVBO();
					if(schemeDraw == DRAW_ELEMENT)
						{ ibo->bind(); }
				vbo->unbind();
			vao->unbind();
		}
		break;
		case BUFFER_VBO:
		{
			populateVBO();
			
			populateIBO();

			vbo->bind();
				enableAttributes();
				vertexAttribPointerVBO();
				if(schemeDraw == DRAW_ELEMENT){
					ibo->bind();
				}
			vbo->unbind();
		}
		break;
		default:
		{
			schemeDraw == DRAW_ELEMENT ? indexList->indexOrDataPtr = indexList->dataArray : NULL;
			enableAttributes();
		}
		break;
	}

	// HANDLE UNIFORMS
	initUniforms();

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
			vertexAttribPointerVBO();
		}
		break;

		default:
		{
			vertexAttribPointerVA();
		}
	};
}

void GeometryBuffer::unbind()
{
	//schemeBuf == BUFFER_VAO ? vao->unbind() : (schemeBuf == BUFFER_VBO ? vbo->unbind() : NULL);
	switch(schemeBuf){
		case BUFFER_VAO:
			vao->unbind();
			break;

		case BUFFER_VBO:
			vbo->unbind();
			break;
		
		default:
			break;
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
	glDrawArrays(primitiveType, 0, geometryData.positions.size);
	return;

	if(this->isInterleaved){
//		glDrawArrays(primitiveType, 0, 5046);
		glDrawArrays(primitiveType, 0, geometryData.positions.size);
	}
	else{
		glDrawArrays(primitiveType, 0, geometryData.positions.size);
	}
}

void GeometryBuffer::drawElement(){
	glDrawElements(primitiveType, indexList->size, indexList->type, indexList->indexOrDataPtr);
}
