//
//  GLES20FBO.h
//

// Parminder: This class need to modified and changes needs to be done in the FrameBufferObject class, the latter then need to
// be moved to Common/Generic section.

#ifndef __GLES20FBO_H__
#define __GLES20FBO_H__

#include "glutils.h"
#include "Interfaces/ILifeCycle.h"

class GLES20FBO : public ILifeCycle{
public:
	GLES20FBO();

	~GLES20FBO();
	
	void Initialize();

	void Render(bool (*customRender)()=NULL);
	
	void setFBOID(GLuint ID);

private:
	/**
	Current frame buffer ID.
	*/
	GLuint fboID;
};

class GLES20DefaultBuffer : public ILifeCycle{

public:
	GLES20DefaultBuffer();

	~GLES20DefaultBuffer();
	
	virtual void Initialize();

	virtual void Render(bool (*customRender)()=NULL);

	void setDefaultBufferID(GLuint ID);

private:
	/**
	Current frame buffer id before generator ID.
	*/
	GLint previousFboID;
};

#endif // GLES20FBO_H
 
