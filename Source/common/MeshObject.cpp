#include "MeshObject.h"
#include "GLES20MeshLoader.h"
//#define USE_FLAT_LIST
MeshObject::MeshObject(const char* meshPath, Scene* parent, Model* model, ModelType type, std::string objectName) : Model(parent, model, type, objectName)
{
	Mesh* inMesh		= waveFrontObjectModel.ParseObjModel(meshPath, !true);
	inMesh->indexCount	= waveFrontObjectModel.IndexTotal();
	specificMesh		= NULL;

	switch(scene()->getRenderer()->getRendererType())
	{
		case PluginType::OPENGLES20_STATIC_PLUGIN:
			specificMesh = new GLES20MeshLoader( inMesh, this);
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

MeshObject::~MeshObject()
{
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void MeshObject::Initialize()
{
	if (scene()->getRenderer()->getRendererType() == PluginType::OPENGLES20_STATIC_PLUGIN){
		transformationMethod = &MeshObject::setTransformationForOpenGLES20Pipeline;
	}
	else{
		// Handle other pipelines 
	}

	#ifdef USE_FLAT_LIST
		scene()->getRenderer()->initFlatList.push_back(specificMesh);
	#else
		specificMesh->Initialize();
	#endif


    Model::Initialize();
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void MeshObject::Render(bool (*customRender)())
{
    TransformObj->TransformPushMatrix(); // Parent Child Level
    ApplyModelsParentsTransformation();
    
    if(isVisible)
    {
        TransformObj->TransformPushMatrix(); // Local Level
        ApplyModelsLocalTransformation();
		//((GLES20MeshLoader*)specificMesh)->SetModelMat(TransformObj->TransformGetModelMatrix());
		//((GLES20MeshLoader*)specificMesh)->SetViewMat(TransformObj->TransformGetViewMatrix());
		//((GLES20MeshLoader*)specificMesh)->SetProjectionMat(TransformObj->TransformGetProjectionMatrix());
		(this->*transformationMethod)();
	#ifdef USE_FLAT_LIST == 1
		scene()->getRenderer()->renderFlatList.push_back(specificMesh);
	#else
		specificMesh->Render();
	#endif
        TransformObj->TransformPopMatrix(); // Local Level
    }

    Model::Render();
    TransformObj->TransformPopMatrix();  // Parent Child Level
}

void MeshObject::SetProgram(unsigned int ID)
{
   specificMesh->SetProgram(ID);
   Model::SetProgram(ID);
}

void MeshObject::setTransformationForOpenGLES20Pipeline(){
	GLES20MeshLoader* gles20Mesh = ((GLES20MeshLoader*)specificMesh);
	*gles20Mesh->getTempMatrix() = *TransformObj->TransformGetViewMatrix() * *TransformObj->TransformGetModelMatrix();
	*gles20Mesh->getTempMVPMatrix() = *TransformObj->TransformGetProjectionMatrix() * *TransformObj->TransformGetViewMatrix() * *TransformObj->TransformGetModelMatrix();
	
	//gles20Mesh->SetModelMat(TransformObj->TransformGetModelMatrix());
	//gles20Mesh->SetViewMat(TransformObj->TransformGetViewMatrix());
	//gles20Mesh->SetProjectionMat(TransformObj->TransformGetProjectionMatrix());
}
