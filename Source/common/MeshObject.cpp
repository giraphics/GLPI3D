#include "MeshObject.h"
#include "GLES20MeshLoader.h"

MeshObject::MeshObject(const char* meshPath, Scene* parent, Model* model, ModelType type, std::string objectName) : Model(parent, model, type, objectName)
{
	Mesh* inMesh = waveFrontObjectModel.ParseObjModel(meshPath, !true);
	inMesh->indexCount = waveFrontObjectModel.IndexTotal();
	specificMesh = new GLES20MeshLoader( inMesh, this);
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

	//specificMesh->Initialize();
	scene()->getRenderer()->initFlatList.push_back(specificMesh);

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
		scene()->getRenderer()->renderFlatList.push_back(specificMesh);
		//specificMesh->Render();
        TransformObj->TransformPopMatrix(); // Local Level
    }

    Model::Render();
    TransformObj->TransformPopMatrix();  // Parent Child Level
}

void MeshObject::SetProgram(unsigned int ID)
{
   specificMesh->SetProgram(ID);
}

void MeshObject::setTransformationForOpenGLES20Pipeline(){
	GLES20MeshLoader* gles20Mesh = ((GLES20MeshLoader*)specificMesh);
	*gles20Mesh->getTempMatrix() = *TransformObj->TransformGetViewMatrix() * *TransformObj->TransformGetModelMatrix();
	*gles20Mesh->getTempMVPMatrix() = *TransformObj->TransformGetProjectionMatrix() * *TransformObj->TransformGetViewMatrix() * *TransformObj->TransformGetModelMatrix();
	
	//gles20Mesh->SetModelMat(TransformObj->TransformGetModelMatrix());
	//gles20Mesh->SetViewMat(TransformObj->TransformGetViewMatrix());
	//gles20Mesh->SetProjectionMat(TransformObj->TransformGetProjectionMatrix());
}