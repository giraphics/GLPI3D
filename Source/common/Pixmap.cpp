#include "Pixmap.h"

#include <cstdio>
#include <cstdlib>

#include "GLES20Pixmap.h"
#include "ImageManager.h"

// Move this map solution to better place, global is not a good solution, its a temporary solution for now.
map<std::string, namespaceimage::Image*>::const_iterator it;

Pixmap::Pixmap(const char* imagePath, Scene* parent, Model* model, ModelType type, TextureTypeEnum textureType, std::string objectName) :Model(parent, model, type, objectName)
{
   Image* imageItem = NULL;
   ImageManager* imageManagerObject = ImageManager::GetInstance();
   it = imageManagerObject->imageMap.find(imagePath);
   if( it != imageManagerObject->imageMap.end()){
      imageItem = (*it).second;
   }
   else{   // Load the image here and pass the Image pointer to the Pixmap
      imageItem = new PngImage(); // This hardcoding need to be fixed, the image should be loaded on the basis of fileextension.
      imageItem->loadImage(imagePath);
      imageManagerObject->imageMap[imagePath] = imageItem;
   }
   specificPixmap = new GLES20Pixmap(imageItem, textureType/*, parent, model, type, textureType, objectName*/);

   //glDisable(GL_CULL_FACE);
   //glEnable(GL_CULL_FACE);
   //glCullFace(GL_FRONT);
   //glFrontFace(GL_CCW);
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

void Pixmap::SetVertices(glm::vec3* vtx)
{
    memcpy(verticesRectangle, vtx, sizeof(glm::vec3)*4);
    specificPixmap->SetVertices(verticesRectangle);
}

void Pixmap::SetTexCoords(glm::vec2* tex){
   memcpy(texCoordinatesRectangle, tex, sizeof(glm::vec2)*4);
   specificPixmap->SetTexCoords(texCoordinatesRectangle);
}

void Pixmap::SetColor(glm::vec4* color){
   memcpy(&rectColor, color, sizeof(glm::vec4));
   specificPixmap->SetColor(&rectColor);
}

void Pixmap::SetProgram(unsigned int ID)
{
   specificPixmap->SetProgram(ID);
}