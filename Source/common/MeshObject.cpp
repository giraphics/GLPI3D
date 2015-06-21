#include "MeshObject.h"
#include "GLES20MeshLoader.h"

MeshObject::MeshObject(const char* meshPath, Scene* parent, Model* model, ModelType type, std::string objectName) : Model(parent, model, type, objectName)
{
	Mesh* inMesh = waveFrontObjectModel.ParseObjModel(meshPath, true);
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
	specificMesh->Initialize();
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
		specificMesh->SetModelMat(TransformObj->TransformGetModelMatrix());
		specificMesh->SetViewMat(TransformObj->TransformGetViewMatrix());
		specificMesh->SetProjectionMat(TransformObj->TransformGetProjectionMatrix());
		specificMesh->Render();
        TransformObj->TransformPopMatrix(); // Local Level
    }

    Model::Render();
    TransformObj->TransformPopMatrix();  // Parent Child Level
}

void MeshObject::SetProgram(unsigned int ID)
{
   specificMesh->SetProgram(ID);
}