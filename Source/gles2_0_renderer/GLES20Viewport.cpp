#include "GLES20Viewport.h"

GLES20Viewport::GLES20Viewport()
{
	scissor					= true;
	viewPortParam.x			= 0;
	viewPortParam.y			= 0;
	viewPortParam.width		= 500;
	viewPortParam.height	= 500;
	scissor ? glEnable(GL_SCISSOR_TEST): glDisable(GL_SCISSOR_TEST);
}

GLES20Viewport::~GLES20Viewport()
{
}

void GLES20Viewport::Initialize(){
	// Enable/Disable the Scissor test
	scissor ? glEnable(GL_SCISSOR_TEST): glDisable(GL_SCISSOR_TEST);
}

// Render the Model class, the function pointer here as an argument is not in use
// The purpose of this argument is to override the default rendering behaviour thus giving much more flexibility to upper layer.
void GLES20Viewport::Render(bool (*customRender)())
{
	glViewport( viewPortParam.x, viewPortParam.y, viewPortParam.width, viewPortParam.height );
	glScissor ( viewPortParam.x, viewPortParam.y, viewPortParam.width, viewPortParam.height );
	if(clearFlag){
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w );
		glClear(clearBitFieldMask);
	}
}

void GLES20Viewport::Resize()
{
}
