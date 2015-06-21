#include "Rectangle.h"
#include "GLES20Rectangle.h"

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
GRectangle::GRectangle(Scene* parent, Model* model, ModelType type, std::string objectName) :Model(parent, model, type, objectName)
{
   specificRectangle = new GLES20Rectangle(/*parent, model, type,objectName*/);
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

void GRectangle::SetVertices(glm::vec3* vtx)
{
    memcpy(verticesRectangle, vtx, sizeof(glm::vec3)*4);
    specificRectangle->SetVertices(verticesRectangle);
}

void GRectangle::SetTexCoords(glm::vec2* tex){
   memcpy(texCoordinatesRectangle, tex, sizeof(glm::vec2)*4);
   specificRectangle->SetTexCoords(texCoordinatesRectangle);
}

void GRectangle::SetColor(glm::vec4* color){
   memcpy(&rectColor, color, sizeof(glm::vec4));
   specificRectangle->SetColor(&rectColor);
}

void GRectangle::SetProgram(unsigned int ID)
{
   specificRectangle->SetProgram(ID);
}
////////////////////////////////////////////////////////

//Pixmap::Pixmap(const char* imagePath, Scene* parent, Model* model, ModelType type, TextureTypeEnum textureType, std::string objectName):Model(parent, model, type)
//{
//   // Load the image here and pass the Image pointer to the Pixmap
//   Image* imageItem = new PngImage(); // This hardcoding need to be fixed, the image should be loaded on the basis of fileextension.
//   imageItem->loadImage("../Resource/Icons/Volkswagen1500.png");
//
//   specificPixmap = new GLES20Pixmap( imageItem, parent, model, type, textureType, objectName);
//}
//
//Pixmap::~Pixmap()
//{
//}
//
///*!
//	Initialize the scene, reserve shaders, compile and chache program
//
//	\param[in] None.
//	\return None
//
//*/
//void Pixmap::Initialize()
//{
//    specificPixmap->Initialize();
//    Model::Initialize();
//    return;
//}
//
///*!
//	Initialize the scene, reserve shaders, compile and chache program
//
//	\param[in] None.
//	\return None
//
//*/
//void Pixmap::Render()
//{
//    TransformObj->TransformPushMatrix(); // Parent Child Level
//    ApplyModelsParentsTransformation();
//    
//    if(isVisible)
//    {
//        TransformObj->TransformPushMatrix(); // Local Level
//        ApplyModelsLocalTransformation();
//        specificPixmap->SetMVP(( float * )TransformObj->TransformGetModelViewProjectionMatrix());
//        specificPixmap->Render();
//        TransformObj->TransformPopMatrix(); // Local Level
//    }
//
//    Model::Render();
//    TransformObj->TransformPopMatrix();  // Parent Child Level
//}
//
//void Pixmap::SetVertices(glm::vec3* vtx)
//{
//    memcpy(verticesRectangle, vtx, sizeof(glm::vec3)*4);
//    specificPixmap->SetVertices(verticesRectangle);
//}
//
//void Pixmap::SetTexCoords(glm::vec2* tex){
//   glm::vec2 texCoords[4] = {
//      glm::vec2(0.0f, 1.0f),
//      glm::vec2(1.0f, 1.0f),
//      glm::vec2(0.0f, 0.0f),
//      glm::vec2(1.0f, 0.0f),
//   };
//   memcpy(texCoordinatesRectangle, texCoords, sizeof(glm::vec2)*4);
//   specificPixmap->SetTexCoords(texCoordinatesRectangle);
//}
//
//void Pixmap::SetColor(glm::vec4* color){
//   memcpy(&rectColor, color, sizeof(glm::vec4));
//   specificPixmap->SetColor(&rectColor);
//}
//
