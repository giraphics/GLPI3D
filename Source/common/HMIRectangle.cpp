#include "HMIRectangle.h"

#ifdef __IPHONE_4_0
#define VERTEX_SHADER_PRG			( char * )"SimpleTextureVertex.glsl"
#define FRAGMENT_SHADER_PRG			( char * )"SimpleTextureFragment.glsl"
#else
#define VERTEX_SHADER_PRG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleVertex.glsl"
#define FRAGMENT_SHADER_PRG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleFragment.glsl"
#define TEX_VERTEX_SHADER_PRG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\SimpleTextureVertex.glsl"
#define TEX_FRAGMENT_SHADER_PRG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\SimpleTextureFragment.glsl"
#endif

HMIRectangle::HMIRectangle( Scene* parent, Model* model, ModelType type,std::string objectName ):GRectangle(parent, model, type, objectName)
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

