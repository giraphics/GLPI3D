#include "GLES20Rectangle.h"
#include "Rectangle.h"

// Namespace used
/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
GLES20Rectangle::GLES20Rectangle(BufferScheme bufScheme, DrawingScheme drawScheme)
{
	geoBuffer = new GeometryBuffer(this, bufScheme, drawScheme);
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
	GeometryMesh* mesh = geoBuffer->geometry();
	
	geoBuffer->addAttribute(new Attribute("VertexTexCoord", 2, mesh->texCoords.size , GL_FLOAT, mesh->texCoords.textureData));
	geoBuffer->addAttribute(new Attribute("VertexPosition", 3, mesh->positions.size , GL_FLOAT, mesh->positions.positionData));

	geoBuffer->addUniform(mvpUniform = new UniformMatrix4fv("ModelViewProjectionMatrix"));
	geoBuffer->addUniform(colUniform = new Uniform4fv("RectColor"));
	geoBuffer->init();
	return;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void GLES20Rectangle::Render(bool (*customRender)())
{
	mvpUniform->SetValue((GLfloat*)&tempMatrix[0]);
	colUniform->SetValue(color);

	geoBuffer->update();
	geoBuffer->bind();
	geoBuffer->draw();
	geoBuffer->unbind();
}

void GLES20Rectangle::SetVertices(std::vector<glm::vec3>* verticesList)
{
	if(geoBuffer->geometry()->positions.size = verticesList->size()){
		geoBuffer->geometry()->positions.positionData = (void*)&verticesList->at(0);
	}
}

void GLES20Rectangle::SetTexCoords(std::vector<glm::vec2>* texCoordList)
{
	if(geoBuffer->geometry()->texCoords.size = texCoordList->size()){
		geoBuffer->geometry()->texCoords.textureData = (void*)&texCoordList->at(0);
	}
}

void GLES20Rectangle::SetColor(glm::vec4* colors)
{
   color = (float*)&colors[0];
}

void GLES20Rectangle::SetIndices(std::vector<unsigned short>* indicesList){
	GeometryMesh* mesh	= geoBuffer->geometry();
	
	if(mesh->geometryIndices.size = indicesList->size()){
		mesh->geometryIndices.indexData	= (void*)&indicesList->at(0);
	}
	geoBuffer->setIndices( new Indices(mesh->geometryIndices.size, GL_UNSIGNED_SHORT, mesh->geometryIndices.indexData)); 
}

void GLES20Rectangle::setDrawingPrimitive(GLenum drawPrimitive)
{
	geoBuffer->setDrawingPrimitive(drawPrimitive);
}
