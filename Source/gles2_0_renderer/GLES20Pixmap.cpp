#include "GLES20Pixmap.h"
#include "Pixmap.h"

#define GetAttribute ProgramManager::GetInstance()->ProgramGetVertexAttribLocation
#define GetUniform ProgramManager::GetInstance()->ProgramGetUniformLocation

GLES20Pixmap::GLES20Pixmap(Image* imageItem/*, Scene* parent, Model* model, ModelType type*/, TextureTypeEnum textureType/*, std::string objectName*/)
{
   if (!imageItem){
      return;
   }

   glActiveTexture (GL_TEXTURE0);
   if (imageItem->getTextureID() == 0){
	   GLint texInternalFormat = getInternalFormat(imageItem);
	   textureObj.generateTexture2D(getTarget(textureType), imageItem->imageWidth(), imageItem->imageHeight(), texInternalFormat, GL_UNSIGNED_BYTE, texInternalFormat, imageItem->bits(), false);
	   imageItem->getTextureID() = textureObj.getTextureID();
   }
   	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
//   glUniform1i(tex, 0);

   //imageItem->deleteBits();
}


GLES20Pixmap::~GLES20Pixmap()
{
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void GLES20Pixmap::Initialize()
{
   glUseProgram( ProgramID );

   mvp     = GetUniform( ProgramID, ( char* )"ModelViewProjectionMatrix" );
   col     = GetUniform( ProgramID, (char *) "RectColor" );
   tex     = GetUniform( ProgramID, (char *) "Tex1" );

   positionAttribHandle   = GetAttribute(ProgramID,(char*)"VertexPosition");
   positionTextureHandle  = GetAttribute(ProgramID,(char*)"VertexTexCoord");
   glEnableVertexAttribArray(positionAttribHandle);
   glEnableVertexAttribArray(positionTextureHandle);
   glUniform4fv( col, 1, color );
   glUniform1i(tex, 0);
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
   tempMatrix = *ProjectionMatrix * *ViewMatrix * *ModelMatrix;
   glUseProgram(ProgramID);
   textureObj.BindTexture();
   glActiveTexture (GL_TEXTURE0);
   glUniform1i(tex, 0);
   glUniformMatrix4fv(mvp, 1, GL_FALSE, (float*)&tempMatrix[0]);
   glVertexAttribPointer(positionTextureHandle, 2, GL_FLOAT, GL_FALSE, 0, texcor);
   glVertexAttribPointer(positionAttribHandle, 3, GL_FLOAT, GL_FALSE, 0, vertices);
   // Draw triangle
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void GLES20Pixmap::SetVertices(glm::vec3* vertex)
{
   vertices = (float*)&vertex[0];
}

void GLES20Pixmap::SetTexCoords(glm::vec2* texCrds)
{
   texcor = (float*)&texCrds[0];
}

void GLES20Pixmap::SetColor(glm::vec4* colors)
{
   color = (float*)&colors[0];
   glUniform4fv( col, 1, color );
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