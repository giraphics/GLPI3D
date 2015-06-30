#include "GLES20Rectangle.h"
#include "Rectangle.h"

#define GetAttribute ProgramManager::GetInstance()->ProgramGetVertexAttribLocation
#define GetUniform ProgramManager::GetInstance()->ProgramGetUniformLocation

// Namespace used
/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
GLES20Rectangle::GLES20Rectangle()
{
	vbo = new VBO(GL_ARRAY_BUFFER);
	vao = new VAO();
	geoBuffer = new GeometryBuffer(this);
	geoBuffer->vbo = vbo;
	geoBuffer->vao = vao;
}

/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
GLES20Rectangle::~GLES20Rectangle()
{
	delete vbo;
	vbo = NULL;

	delete vao;
	vao = NULL;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void GLES20Rectangle::Initialize()
{
	glUseProgram( ProgramID );

	mvp     = GetUniform( ProgramID, ( char* )"ModelViewProjectionMatrix" );
	col     = GetUniform( ProgramID, (char *) "RectColor" );

	//int a = offsetof(GeometryMesh, positions);
	//int b = offsetof(GeometryMesh, normals);
	//int c = offsetof(GeometryMesh, tangents);
	//int d = offsetof(GeometryMesh, texCoords);
	//int e = offsetof(GeometryMesh, texCoordsSec);

	geoBuffer->addAttribute(AttributeInfo("VertexPosition", 3, gm.positions->size() , GL_FLOAT, &gm.positions[0]));
	geoBuffer->addAttribute(AttributeInfo("VertexTexCoord", 2, gm.texCoords->size() , GL_FLOAT, &gm.texCoords[0]));
	geoBuffer->init();
	positionAttribHandle   = GetAttribute(ProgramID,(char*)"VertexPosition");
	positionTextureHandle  = GetAttribute(ProgramID,(char*)"VertexTexCoord");
	glUniform4fv( col, 1, color );

	int sizeV = gm.positions->size() * sizeof(glm::vec3);// * sizeof(float);
	int sizeT = gm.texCoords->size() * sizeof(glm::vec2);// * sizeof(float);
	vbo->bind();
	vbo->bufferData(sizeT+sizeV, 0, GL_STATIC_DRAW);
	vbo->bufferSubData(0, sizeT, &(*gm.texCoords)[0]);
	vbo->bufferSubData(sizeT, sizeV, &(*gm.positions)[0]);
	/*glGenBuffers(1, &vid);
	glBindBuffer(GL_ARRAY_BUFFER, vid);
	glBufferData(GL_ARRAY_BUFFER, sizeT+sizeV, 0, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeT, &(*gm.texCoords)[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeT, sizeV, &(*gm.positions)[0]);*/
	vao->bind();
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vid);
	vbo->bind();
	if(positionAttribHandle >= 0){
		glEnableVertexAttribArray(positionAttribHandle);
		glVertexAttribPointer(positionAttribHandle, 3, GL_FLOAT, GL_FALSE, 0, (void*)sizeT);
	}

	if(positionTextureHandle >= 0){
		glEnableVertexAttribArray(positionTextureHandle);
		glVertexAttribPointer(positionTextureHandle, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	//glBindVertexArray(0);
	vao->unbind();
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	vbo->unbind();

	return;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void GLES20Rectangle::Render(bool (*customRender)())
{
	tempMatrix = *ProjectionMatrix * *ViewMatrix * *ModelMatrix;

	glUseProgram(ProgramID);
    //glBindVertexArray(vao);
	vao->bind();
    glUniformMatrix4fv(mvp, 1, GL_FALSE, (float*)&tempMatrix[0]);
    glUniform4fv( col, 1, color );

    // Draw triangle
    if(!customRender){
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
   else{
	   (*customRender)();
   }
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLES20Rectangle::SetVertices(std::vector<glm::vec3>* verticesList)
{
//	int a = offsetof(GeometryMesh, positions);
	//char* base = (char*)&gm;
	//(std::vector<glm::vec3>*)(base+a) = verticesList;
	gm.positions = verticesList;
}

void GLES20Rectangle::SetTexCoords(std::vector<glm::vec2>* texCoordList)
{
	int d = offsetof(GeometryMesh, texCoords);
	gm.texCoords = texCoordList;
}

void GLES20Rectangle::SetColor(glm::vec4* colors)
{
   color = (float*)&colors[0];
   glUseProgram(ProgramID);
   glUniform4fv( col, 1, color );
}
