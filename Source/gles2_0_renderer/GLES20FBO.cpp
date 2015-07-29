#include "GLES20FBO.h"

GLES20FBO::GLES20FBO()
{
}

GLES20FBO::~GLES20FBO()
{
}
	
void GLES20FBO::Initialize(){
}

// Render the Model class, the function pointer here as an argument is not in use
// The purpose of this argument is to override the default rendering behaviour thus giving much more flexibility to upper layer.
void GLES20FBO::Render(bool (*customRender)())
{
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
}

void GLES20FBO::setFBOID(GLuint ID) 
{ 
	fboID = ID; 
}

GLES20DefaultBuffer::GLES20DefaultBuffer()
{
}
	
GLES20DefaultBuffer::~GLES20DefaultBuffer()
{
}
	
void GLES20DefaultBuffer::Initialize(){
}

// Render the Model class, the function pointer here as an argument is not in use
// The purpose of this argument is to override the default rendering behaviour thus giving much more flexibility to upper layer.
void GLES20DefaultBuffer::Render(bool (*customRender)())
{
	glBindFramebuffer(GL_FRAMEBUFFER, previousFboID);
}

void GLES20DefaultBuffer::setDefaultBufferID(GLuint ID) 
{ 
	previousFboID = ID; 
}
