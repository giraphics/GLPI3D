#include "HMIRectangle.h"

HMIRectangle::HMIRectangle( Scene* parent, Model* model, ModelType type,std::string objectName,BufferScheme bufScheme, DrawingScheme drawScheme )
	:GRectangle(parent, model, type, objectName, bufScheme, drawScheme)
{
}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
HMIRectangle::~HMIRectangle()
{
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void HMIRectangle::Initialize()
{
    GRectangle::Initialize();
    return;
}

static bool triangleCall()
{
   // Draw triangle
   glDrawArrays(GL_LINE_STRIP, 0, 4);
   return true;
}
/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void HMIRectangle::Render(bool (*customRender)())
{
   this->RotateLocal(1.0, 0.0, 0.0, 1.0);

   //GRectangle::Render(&triangleCall);
   GRectangle::Render();
}

