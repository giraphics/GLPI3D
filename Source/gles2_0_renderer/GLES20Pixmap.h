//
//  GLES20Pixmap.h
//

#ifndef __GLES20PIXMAP_H__
#define __GLES20PIXMAP_H__

#include <iostream>

#include "ModelEx.h"
#include "glutils.h"
#include "Renderer.h"
#include "Scene.h"
#include "Rectangle.h"
#include "Texture.h"
#include "Geometry.h"

using namespace std;

class GLES20Pixmap : public IRenderable, public IGeoBuffer, public TransformationMatrix{

public:
   GLES20Pixmap( Image* imageItem, TextureTypeEnum textureType = TWO_DIMENSIONAL_TEXTURE);

   GLES20Pixmap( unsigned int ID, TextureTypeEnum textureType );
   
   virtual ~GLES20Pixmap();

    void Initialize();
    
    // Render the Model class
    void Render(bool (*customRender)()=NULL);

	void Update( float t ){}

	void SetVertices(std::vector<glm::vec3>* verticesList);

	void SetTexCoords(std::vector<glm::vec2>* texCoordList);

	void SetColor(glm::vec4* colors);

	void SetIndices(std::vector<unsigned short>* indicesList);
	
	GLint getInternalFormat(Image* imageItem);

	GLenum getTarget(TextureTypeEnum textureType);

private:
	// Keep the member variables multiple of 32 as much 
	// as possible to avoid explicit packing and alignment 
	UniformMatrix4fv*	mvpUniform;
	Uniform4fv*			colUniform;
	Uniform1i*			texUniform;
	float*				color;
	GLint				textureUnit;
	Texture				textureObj;
};

class GLES20FBO : public ILifeCycle{
public:
	GLES20FBO(){}

	~GLES20FBO(){}
	
	virtual void Initialize(){
	}

	// Render the Model class, the function pointer here as an argument is not in use
	// The purpose of this argument is to override the default rendering behaviour thus giving much more flexibility to upper layer.
	virtual void Render(bool (*customRender)()=NULL)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	}

	void setFBOID(GLuint ID) { fboID = ID; }

	/**
	Current frame buffer ID.
	*/
	GLuint fboID;
};

class GLES20DefaultBuffer : public ILifeCycle{
public:
	GLES20DefaultBuffer(){}
	~GLES20DefaultBuffer(){}
	virtual void Initialize(){
	}

	// Render the Model class, the function pointer here as an argument is not in use
	// The purpose of this argument is to override the default rendering behaviour thus giving much more flexibility to upper layer.
	virtual void Render(bool (*customRender)()=NULL)
	{
	    glBindFramebuffer(GL_FRAMEBUFFER, previousFboID);
	}

	void setDefaultBufferID(GLuint ID) { previousFboID = ID; }

	/**
	Current frame buffer id before generator ID.
	*/
	GLint previousFboID;
};


class GLES20Viewport : public ILifeCycle{
public:
	GLES20Viewport(){
		scissor					= true;
		viewPortParam.x			= 0;
		viewPortParam.y			= 0;
		viewPortParam.width		= 500;
		viewPortParam.height	= 500;
		scissor ? glEnable(GL_SCISSOR_TEST): glDisable(GL_SCISSOR_TEST);
	}

	~GLES20Viewport(){}

	virtual void Initialize(){
		// Enable/Disable the Scissor test
		scissor ? glEnable(GL_SCISSOR_TEST): glDisable(GL_SCISSOR_TEST);
	}

   // Render the Model class, the function pointer here as an argument is not in use
   // The purpose of this argument is to override the default rendering behaviour thus giving much more flexibility to upper layer.
   virtual void Render(bool (*customRender)()=NULL)
   {
		glViewport( viewPortParam.x, viewPortParam.y, viewPortParam.width, viewPortParam.height );
		glScissor ( viewPortParam.x, viewPortParam.y, viewPortParam.width, viewPortParam.height );
		if(clearFlag){
			glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w );
			glClear(clearBitFieldMask);
		}
   }

   void Resize()
   {
	  //glGetIntegerv( GL_VIEWPORT, viewport_matrix );
   }
public:
    ViewPort            viewPortParam;
    CameraViewParams    cameraViewParameters;
    glm::vec4           clearColor;
    GLbitfield          clearBitFieldMask;
	void setScissorTest(bool flag){scissor = flag;}
	void setClearFlag(bool flag){clearFlag = flag;}
private:
	bool scissor;
	bool clearFlag;
};

#endif // GLES20PIXMAP_H
 
