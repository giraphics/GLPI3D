#include "Rectangle.h"
#include "GLES20Rectangle.h"

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
GRectangle::GRectangle(Scene* parent, Model* model, ModelType type, std::string objectName,
	BufferScheme bufScheme, DrawingScheme drawScheme) :Model(parent, model, type, objectName)
{
   specificRectangle = new GLES20Rectangle(bufScheme, drawScheme);
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
	if (scene()->getRenderer()->getRendererType() == PluginType::OPENGLES20_STATIC_PLUGIN){
		transformationMethod = &GRectangle::setTransformationForOpenGLES20Pipeline;
	}
	else{
		// Handle other pipelines 
	}

    //specificRectangle->Initialize();
	scene()->getRenderer()->initFlatList.push_back(specificRectangle);
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
		// Remove this if check using the function ptr in the initialization itself, rendering must be 
		//if (this->scene()->getRenderer()->getRendererType() == PluginType::OPENGLES20_STATIC_PLUGIN){
		//	GLES20Rectangle* gles20Rect = ((GLES20Rectangle*)specificRectangle);
		//	gles20Rect->SetModelMat(TransformObj->TransformGetModelMatrix());
		//	gles20Rect->SetViewMat(TransformObj->TransformGetViewMatrix());
		//	gles20Rect->SetProjectionMat(TransformObj->TransformGetProjectionMatrix());
		//}
		(this->*transformationMethod)();
        //specificRectangle->Render(customRender);
		scene()->getRenderer()->renderFlatList.push_back(specificRectangle);
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

void GRectangle::SetIndices(std::vector<unsigned short>* indicesList){
	specificRectangle->SetIndices(indicesList);
}

void GRectangle::SetProgram(unsigned int ID)
{
   specificRectangle->SetProgram(ID);
}

void GRectangle::setTransformationForOpenGLES20Pipeline(){
	GLES20Rectangle* gles20Rect = ((GLES20Rectangle*)specificRectangle);
	*gles20Rect->getTempMatrix() = *TransformObj->TransformGetProjectionMatrix() * *TransformObj->TransformGetViewMatrix() * *TransformObj->TransformGetModelMatrix();
	//gles20Rect->SetModelMat(TransformObj->TransformGetModelMatrix());
	//gles20Rect->SetViewMat(TransformObj->TransformGetViewMatrix());
	//gles20Rect->SetProjectionMat(TransformObj->TransformGetProjectionMatrix());
}

void GRectangle::setDrawingPrimitive(GLenum drawPrimitive)
{
	specificRectangle->setDrawingPrimitive(drawPrimitive);
}
