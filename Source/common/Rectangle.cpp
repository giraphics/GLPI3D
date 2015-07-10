#include "Rectangle.h"
#include "GLES20Rectangle.h"

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
GRectangle::GRectangle(Scene* parent, Model* model, ModelType type, std::string objectName,
	BufferScheme bufScheme, bool isInterleaved, DrawingScheme drawScheme) :Model(parent, model, type, objectName)
{
   specificRectangle = new GLES20Rectangle(bufScheme, isInterleaved, drawScheme);
}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
GRectangle::~GRectangle()
{
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void GRectangle::Initialize()
{
    specificRectangle->Initialize();
    Model::Initialize();
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void GRectangle::Render(bool (*customRender)())
{
    TransformObj->TransformPushMatrix(); // Parent Child Level
    ApplyModelsParentsTransformation();
    
    if(isVisible)
    {
        TransformObj->TransformPushMatrix(); // Local Level
        ApplyModelsLocalTransformation();
        //specificRectangle->SetMVP(( float * )TransformObj->TransformGetModelViewProjectionMatrix());
		specificRectangle->SetModelMat(TransformObj->TransformGetModelMatrix());
		specificRectangle->SetViewMat(TransformObj->TransformGetViewMatrix());
		specificRectangle->SetProjectionMat(TransformObj->TransformGetProjectionMatrix());

        specificRectangle->Render(customRender);
        TransformObj->TransformPopMatrix(); // Local Level
    }

    Model::Render();
    TransformObj->TransformPopMatrix();  // Parent Child Level
}

void GRectangle::SetVertices(std::vector<glm::vec3>* verticesList){
    specificRectangle->SetVertices(verticesList);
}

void GRectangle::SetTexCoords(std::vector<glm::vec2>* texCoordList)
{
	specificRectangle->SetTexCoords(texCoordList);
}

void GRectangle::SetColor(glm::vec4* color){
   memcpy(&rectColor, color, sizeof(glm::vec4));
   specificRectangle->SetColor(&rectColor);
}

void GRectangle::SetProgram(unsigned int ID)
{
   specificRectangle->SetProgram(ID);
}
