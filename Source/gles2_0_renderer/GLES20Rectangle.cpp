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
GLES20Rectangle::GLES20Rectangle( /*Scene* parent, Model* model, ModelType type, std::string objectName*/)
{
}

/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
GLES20Rectangle::~GLES20Rectangle()
{
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

	positionAttribHandle   = GetAttribute(ProgramID,(char*)"VertexPosition");
	positionTextureHandle  = GetAttribute(ProgramID,(char*)"VertexTexCoord");
	glUniform4fv( col, 1, color );
	int sizeT = 8*sizeof(float);
	int sizeV = 12*sizeof(float);

	glGenBuffers(1, &vid);
	glBindBuffer(GL_ARRAY_BUFFER, vid);
	glBufferData(GL_ARRAY_BUFFER, sizeT+sizeV, 0, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeT, texcor);
	glBufferSubData(GL_ARRAY_BUFFER, sizeT, sizeV, vertices);
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vid);
	if(positionAttribHandle >= 0){
		glEnableVertexAttribArray(positionAttribHandle);
		glVertexAttribPointer(positionAttribHandle, 3, GL_FLOAT, GL_FALSE, 0, (void*)sizeT);
	}

	if(positionTextureHandle >= 0){
		glEnableVertexAttribArray(positionTextureHandle);
		glVertexAttribPointer(positionTextureHandle, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
    glBindVertexArray(vao);
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

//void GLES20Rectangle::SetMVP(float* MVPM)
//{
//   MVPMatrix = MVPM;
//}

void GLES20Rectangle::SetVertices(glm::vec3* vertex)
{
   vertices = (float*)&vertex[0];
}

void GLES20Rectangle::SetTexCoords(glm::vec2* texCrds)
{
   texcor = (float*)&texCrds[0];
}

void GLES20Rectangle::SetColor(glm::vec4* colors)
{
   color = (float*)&colors[0];
   glUseProgram(ProgramID);
   glUniform4fv( col, 1, color );
}
