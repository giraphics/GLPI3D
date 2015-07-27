#include "Pixmap.h"

#include <cstdio>
#include <cstdlib>

#include "GLES20Pixmap.h"
#include "ImageManager.h"

Pixmap::Pixmap(const char* imagePath, Scene* parent, Model* model, ModelType type, TextureTypeEnum textureType, std::string objectName) :Model(parent, model, type, objectName)
{
	Image* imageItem	= ImageManager::GetInstance()->GetImage(imagePath);
	specificPixmap		= NULL;

	switch(scene()->getRenderer()->getRendererType())
	{
		case PluginType::OPENGLES20_STATIC_PLUGIN:
			specificPixmap	= new GLES20Pixmap(imageItem, textureType);
			break;

		case PluginType::OPENGLES31_STATIC_PLUGIN:
			printf("\n Pipeline not implemented PluginType::OPENGLES31_STATIC_PLUGIN: %s, %s.", __FUNCTION__, __LINE__);
			assert(0);
			break;

		case PluginType::VULKAN_STATIC_PLUGIN:
			printf("\n Pipeline not implemented PluginType::VULKAN_STATIC_PLUGIN: %s, %s.", __FUNCTION__, __LINE__);
			assert(0);
			break;

		case PluginType::JCP2016_STATIC_PLUGIN:
			printf("\n Pipeline not implemented PluginType::VULKAN_STATIC_PLUGIN: %s, %s.", __FUNCTION__, __LINE__);
			assert(0);
			break;
		
		default:
			printf("\n Undefined pipeline %s, %s.", __FUNCTION__, __LINE__);
			assert(0);
			break;
	}
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
	if (scene()->getRenderer()->getRendererType() == PluginType::OPENGLES20_STATIC_PLUGIN){
		transformationMethod = &Pixmap::setTransformationForOpenGLES20Pipeline;
	}
	else{
		// Handle other pipelines 
	}

    //specificPixmap->Initialize();
	scene()->getRenderer()->initFlatList.push_back(specificPixmap);
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
		(this->*transformationMethod)();
		scene()->getRenderer()->renderFlatList.push_back(specificPixmap);
		//specificPixmap->Render();
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

void Pixmap::SetIndices(std::vector<unsigned short>* indicesList){
	specificPixmap->SetIndices(indicesList);
}

void Pixmap::SetColor(glm::vec4* color){
   memcpy(&rectColor, color, sizeof(glm::vec4));
   specificPixmap->SetColor(&rectColor);
}

void Pixmap::SetProgram(unsigned int ID)
{
   specificPixmap->SetProgram(ID);
   Model::SetProgram(ID);
}

void Pixmap::setTransformationForOpenGLES20Pipeline(){
	GLES20Pixmap* gles20Pixmap = ((GLES20Pixmap*)specificPixmap);
	*gles20Pixmap->getTempMatrix() = *TransformObj->TransformGetProjectionMatrix() * *TransformObj->TransformGetViewMatrix() * *TransformObj->TransformGetModelMatrix();
	//gles20Pixmap->SetModelMat(TransformObj->TransformGetModelMatrix());
	//gles20Pixmap->SetViewMat(TransformObj->TransformGetViewMatrix());
	//gles20Pixmap->SetProjectionMat(TransformObj->TransformGetProjectionMatrix());
}
