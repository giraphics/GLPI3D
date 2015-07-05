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
	geoBuffer = new GeometryBuffer(this);
}

/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
GLES20Rectangle::~GLES20Rectangle()
{
	delete geoBuffer;
	geoBuffer = NULL;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void GLES20Rectangle::Initialize()
{
	glUseProgram( ProgramID );

	//mvp     = GetUniform( ProgramID, ( char* )"ModelViewProjectionMatrix" );
	col     = GetUniform( ProgramID, (char *) "RectColor" );

	geoBuffer->addAttribute(new Attribute("VertexTexCoord", 2, geoBuffer->geometry()->texCoords->size() , GL_FLOAT, &(*geoBuffer->geometry()->texCoords)[0]));
	geoBuffer->addAttribute(new Attribute("VertexPosition", 3, geoBuffer->geometry()->positions->size() , GL_FLOAT, &(*geoBuffer->geometry()->positions)[0]));
	geoBuffer->addUniform(mvpUniform = new UniformMatrix4fv("ModelViewProjectionMatrix"));
	geoBuffer->init();
	glUniform4fv( col, 1, color );
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

	geoBuffer->bind();
    //glUniformMatrix4fv(mvp, 1, GL_FALSE, (float*)&tempMatrix[0]);
	mvpUniform->SetValue((GLfloat*)&tempMatrix[0]);
    glUniform4fv( col, 1, color );

    // Draw triangle
    if(!customRender){
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
   else{
	   (*customRender)();
   }
   geoBuffer->unbind();
}

void GLES20Rectangle::SetVertices(std::vector<glm::vec3>* verticesList)
{
	geoBuffer->geometry()->positions = verticesList;
}

void GLES20Rectangle::SetTexCoords(std::vector<glm::vec2>* texCoordList)
{
	geoBuffer->geometry()->texCoords = texCoordList;
}

void GLES20Rectangle::SetColor(glm::vec4* colors)
{
   color = (float*)&colors[0];
   glUseProgram(ProgramID);
   glUniform4fv( col, 1, color );
}
