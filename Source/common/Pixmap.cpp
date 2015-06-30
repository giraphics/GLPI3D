#include "Pixmap.h"

#include <cstdio>
#include <cstdlib>

#include "GLES20Pixmap.h"
#include "ImageManager.h"

Pixmap::Pixmap(const char* imagePath, Scene* parent, Model* model, ModelType type, TextureTypeEnum textureType, std::string objectName) :Model(parent, model, type, objectName)
{
   Image* imageItem = ImageManager::GetInstance()->GetImage(imagePath);
   specificPixmap = new GLES20Pixmap(imageItem, textureType/*, parent, model, type, textureType, objectName*/);
}

Pixmap::Pixmap(Image* imageItem, Scene* parent, Model* model, ModelType type, TextureTypeEnum textureType, std::string objectName):Model(parent, model, type, objectName)
{
   specificPixmap = new GLES20Pixmap(imageItem, textureType);
}

Pixmap::Pixmap(unsigned int ID, Scene* parent, Model* model, ModelType type, TextureTypeEnum textureType, std::string objectName):Model(parent, model, type, objectName)
{
   specificPixmap = new GLES20Pixmap(ID, textureType);
}

Pixmap::~Pixmap()
{
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void Pixmap::Initialize()
{
    specificPixmap->Initialize();
    Model::Initialize();
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void Pixmap::Render(bool (*customRender)())
{
    TransformObj->TransformPushMatrix(); // Parent Child Level
    ApplyModelsParentsTransformation();
    
    if(isVisible)
    {
        TransformObj->TransformPushMatrix(); // Local Level
        ApplyModelsLocalTransformation();
		specificPixmap->SetModelMat(TransformObj->TransformGetModelMatrix());
		specificPixmap->SetViewMat(TransformObj->TransformGetViewMatrix());
		specificPixmap->SetProjectionMat(TransformObj->TransformGetProjectionMatrix());
        specificPixmap->Render();
        TransformObj->TransformPopMatrix(); // Local Level
    }

    Model::Render();
    TransformObj->TransformPopMatrix();  // Parent Child Level
}

void Pixmap::SetVertices(std::vector<glm::vec3>* verticesList){
    specificPixmap->SetVertices(verticesList);
}

void Pixmap::SetTexCoords(std::vector<glm::vec2>* texCoordList){
	specificPixmap->SetTexCoords(texCoordList);
}

void Pixmap::SetColor(glm::vec4* color){
   memcpy(&rectColor, color, sizeof(glm::vec4));
   specificPixmap->SetColor(&rectColor);
}

void Pixmap::SetProgram(unsigned int ID)
{
   specificPixmap->SetProgram(ID);
}