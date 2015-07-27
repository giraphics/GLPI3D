#include "GLES20Pixmap.h"
#include "Pixmap.h"

#define GetAttribute ProgramManager::GetInstance()->ProgramGetVertexAttribLocation
#define GetUniform ProgramManager::GetInstance()->ProgramGetUniformLocation

GLES20Pixmap::GLES20Pixmap(Image* imageItem, TextureTypeEnum textureType)
{
   if (!imageItem){
      return;
   }
	geoBuffer = new GeometryBuffer(this, BUFFER_VA);

   glActiveTexture (GL_TEXTURE0);
   if (imageItem->getTextureID() == 0){
	   GLint texInternalFormat = getInternalFormat(imageItem);
	   textureObj.generateTexture2D(getTarget(textureType), imageItem->imageWidth(), imageItem->imageHeight(), texInternalFormat, GL_UNSIGNED_BYTE, texInternalFormat, imageItem->bits(), false);
	   imageItem->getTextureID() = textureObj.getTextureID();
   }
	else{
		textureObj.setTextureID(imageItem->getTextureID());
		textureObj.setTargetType(getTarget(textureType));
	}
   	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   if(imageItem){
	   imageItem->deleteBits();
   }
}

GLES20Pixmap::GLES20Pixmap(unsigned int ID, TextureTypeEnum textureType)
{
    geoBuffer = new GeometryBuffer(this, BUFFER_VA);
	textureObj.setTextureID((GLuint)ID);
	textureObj.setTargetType(getTarget(textureType));
	
   // Present the texture 0 overrided in future we must allow the end user a provision to select the texture unit on fly.
   // This should not be very complex I guess.
   glActiveTexture (GL_TEXTURE0);
   glBindTexture (getTarget(textureType), ID);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


GLES20Pixmap::~GLES20Pixmap()
{
	delete geoBuffer;
	geoBuffer = NULL;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void GLES20Pixmap::Initialize()
{
	geoBuffer->addAttribute(new Attribute("VertexPosition", 3, geoBuffer->geometry()->positions.size , GL_FLOAT, geoBuffer->geometry()->positions.positionData));
	geoBuffer->addAttribute(new Attribute("VertexTexCoord", 2, geoBuffer->geometry()->texCoords.size , GL_FLOAT, geoBuffer->geometry()->texCoords.textureData));
	geoBuffer->addUniform(mvpUniform = new UniformMatrix4fv("ModelViewProjectionMatrix"));
	geoBuffer->addUniform(colUniform = new Uniform4fv("RectColor"));
	geoBuffer->addUniform(texUniform = new Uniform1i("RectColor"));
	geoBuffer->init();
   
   return;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void GLES20Pixmap::Render(bool (*customRender)())
{
   // In future this calculation must resides in the update function
   //tempMatrix = *ProjectionMatrix * *ViewMatrix * *ModelMatrix;
   
   //glActiveTexture (GL_TEXTURE0);
   textureObj.BindTexture();

   mvpUniform->SetValue((GLfloat*)&tempMatrix[0]);
   //textureUnit = 0; //Need to set from a setter function.
   //texUniform->SetValue(&textureUnit);

   geoBuffer->update();

   geoBuffer->bind();
   geoBuffer->draw();
   geoBuffer->unbind();
}

void GLES20Pixmap::SetVertices(std::vector<glm::vec3>* verticesList)
{
	//geoBuffer->geometry()->positions = verticesList;
	if(geoBuffer->geometry()->positions.size = verticesList->size()){
		geoBuffer->geometry()->positions.positionData = (void*)&verticesList->at(0);
	}
}

void GLES20Pixmap::SetTexCoords(std::vector<glm::vec2>* texCoordList)
{
//	geoBuffer->geometry()->texCoords = texCoordList;
	if(geoBuffer->geometry()->texCoords.size = texCoordList->size()){
		geoBuffer->geometry()->texCoords.textureData = (void*)&texCoordList->at(0);
	}
}

void GLES20Pixmap::SetColor(glm::vec4* colors)
{
   color = (float*)&colors[0];
}

void GLES20Pixmap::SetIndices(std::vector<unsigned short>* indicesList){
	//geoBuffer->geometry()->geometryIndices = indicesList;
	if(geoBuffer->geometry()->geometryIndices.size = indicesList->size()){
		geoBuffer->geometry()->geometryIndices.indexData = (void*)&indicesList->at(0);
	}
}

GLint GLES20Pixmap::getInternalFormat(Image* imageItem)
{
	switch (imageItem->colorType()) {
		case COLOR_TYPE_GRAY:{
			return GL_LUMINANCE;
		}

		case COLOR_TYPE_GRAY_ALPHA:{
			return GL_ALPHA;
		}

		case COLOR_TYPE_RGB:{
			return GL_RGB;
		}

		case COLOR_TYPE_RGB_ALPHA:{
			return GL_RGBA;
		}

		default:{
			return GL_RGBA;
		}
	}
}

GLenum GLES20Pixmap::getTarget(TextureTypeEnum textureType){
	switch (textureType)
	{
		case TWO_DIMENSIONAL_TEXTURE:
			return GL_TEXTURE_2D;

		case THREE_DIMENSIONAL_TEXTURE:
			return GL_TEXTURE_3D;
			break;
		case TWO_DIMENSIONAL_ARRAY_TEXTURE:
			return GL_TEXTURE_2D_ARRAY;
			break;
		case CUBE_MAP_TEXTURE:
			return GL_TEXTURE_CUBE_MAP;
			break;
		default:
			return GL_TEXTURE_2D;
	}
}