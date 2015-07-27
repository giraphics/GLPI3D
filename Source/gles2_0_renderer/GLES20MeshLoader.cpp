#include "GLES20MeshLoader.h"

extern map<std::string, namespaceimage::Image*>::const_iterator it;

/*!
	Simple Default Constructor

	\param[in] None.
	\return None
*/
GLES20MeshLoader::GLES20MeshLoader(Mesh* inMesh, Model* parent)
{
	geoBuffer = new GeometryBuffer(this, BUFFER_VAO, DRAW_ARRAY, true);
	geoBuffer->setDrawingPrimitive(GL_TRIANGLES);

	//indexCount	= inMesh->indexCount;
	meshModel	= inMesh;
	parentModel = parent;
	matObj		= parentModel->GetMaterial();
	
	processTexture();
	// At present the OBJ Parser does not support the reading of material information.
	// Therefore hardcoding the texture value.
	char imagePath[512] = "../Resource/Models/mbclass/mbcclass.png";
	
	ImageManager* imageManagerObject = ImageManager::GetInstance();
	it = imageManagerObject->imageMap.find(imagePath);
	if (it != imageManagerObject->imageMap.end()){
		imageItem = (*it).second;
	}
	else{   // Load the image here and pass the Image pointer to the Pixmap
		imageItem = new PngImage(); // This hardcoding need to be fixed, the image should be loaded on the basis of fileextension.
		imageItem->loadImage(imagePath);
		imageManagerObject->imageMap[imagePath] = imageItem;
	}

	glActiveTexture(GL_TEXTURE0);
	if (imageItem->getTextureID() == 0){
		GLint texInternalFormat = textureObj.getInternalFormat(imageItem);
		textureObj.generateTexture2D(textureObj.getTarget(TWO_DIMENSIONAL_TEXTURE), imageItem->imageWidth(), imageItem->imageHeight(), texInternalFormat, GL_UNSIGNED_BYTE, texInternalFormat, imageItem->bits(), false);
		imageItem->getTextureID() = textureObj.getTextureID();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLES20MeshLoader::ReleaseMeshResources()
{
	delete geoBuffer;
	geoBuffer = NULL;
}

/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
GLES20MeshLoader::~GLES20MeshLoader()
{
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void GLES20MeshLoader::Initialize()
{
    // Manage Uniform variables
	geoBuffer->addUniform(MaterialAmbientUniform	= new Uniform3f("MaterialAmbient"));
	geoBuffer->addUniform(MaterialSpecularUniform	= new Uniform3f("MaterialSpecular"));
	geoBuffer->addUniform(MaterialDiffuseUniform	= new Uniform3f("MaterialDiffuse"));
	geoBuffer->addUniform(LightAmbientUniform		= new Uniform3f("LightAmbient"));
	geoBuffer->addUniform(LightSpecularUniform		= new Uniform3f("LightSpecular"));
	geoBuffer->addUniform(LightDiffuseUniform		= new Uniform3f("LightDiffuse"));
	geoBuffer->addUniform(ShininessFactorUniform	= new Uniform1f("ShininessFactor"));
	geoBuffer->addUniform(LightPositionUniform		= new Uniform3fv("LightPosition"));
	geoBuffer->addUniform(MVPUniform				= new UniformMatrix4fv("ModelViewProjectionMatrix"));
	geoBuffer->addUniform(MVUniform					= new UniformMatrix4fv("ModelViewMatrix"));
	geoBuffer->addUniform(NormalMatrixUniform		= new UniformMatrix3fv("NormalMatrix"));
	geoBuffer->addUniform(TexUniform				= new Uniform1i("Tex1"));

	
	if (!meshModel){
		return;
	}

	// Manage interleaved data 
    stride			= sizeof(Vertex);
	offsetNormal	= (GLvoid*)offsetof(Vertex,normal);
	offsetTexCoord	= (GLvoid*)offsetof(Vertex,uv);
	offsetTangent	= (GLvoid*)offsetof(Vertex, tangent);

	void* base						= (void*)(&meshModel->vertices.at(0));
	GeometryMesh* mesh				= geoBuffer->geometry();

	mesh->positions.size			= meshModel->vertices.size();
	mesh->positions.positionData	= base; 

	mesh->normals.size				= meshModel->vertices.size();
	mesh->normals.normalData		= (void*)((char*)base + (char)offsetNormal);

	mesh->texCoords.size			= meshModel->vertices.size();
	mesh->texCoords.textureData		= (void*)((char*)base + (char)offsetTexCoord);

	mesh->tangents.size				= meshModel->vertices.size();
	mesh->tangents.tagnetData		= (void*)((char*)base + (char)offsetTangent);

    // Manage Attribute variables
	geoBuffer->addAttribute(new Attribute("VertexPosition", 3, mesh->positions.size , GL_FLOAT, mesh->positions.positionData, stride));
	geoBuffer->addAttribute(new Attribute("Normal", 3, mesh->normals.size , GL_FLOAT, mesh->normals.normalData, stride));
	geoBuffer->addAttribute(new Attribute("VertexTexCoord", 2, mesh->texCoords.size , GL_FLOAT, mesh->texCoords.textureData, stride));
	geoBuffer->addAttribute(new Attribute("VertexTangent", 3, mesh->tangents.size , GL_FLOAT, mesh->tangents.tagnetData, stride));
	geoBuffer->setInterleavedBuffer((void*)&meshModel->vertices[0],meshModel->vertices.size() * sizeof(meshModel->vertices[0]));


	geoBuffer->init();

	// Manage texture unit
	textureUnit = 0; //Need to set from a setter function.
	TexUniform->SetValue(&textureUnit);
	glEnable(GL_DEPTH_TEST);

	// Realease vertices
	meshModel->vertices.clear();

    return;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void GLES20MeshLoader::Render(bool(*customRender)())
{
	ApplyMaterial();
	ApplyLight();
	
	textureObj.BindTexture();

	// Calculate the Model-View Matrix
	//tempMatrix = *ViewMatrix * *ModelMatrix;
	MVUniform->SetValue((GLfloat*)&tempMatrix[0]);

	// Calculate the Normal Matrix
	normalMat = glm::mat3(glm::vec3(tempMatrix[0]), glm::vec3(tempMatrix[1]), glm::vec3(tempMatrix[2]));
	NormalMatrixUniform->SetValue((GLfloat*)&normalMat[0]);

	// Calculate the Model-View-Projection Matrix
	//tempMVPMatrix = *ProjectionMatrix * tempMatrix;
	MVPUniform->SetValue((GLfloat*)&tempMVPMatrix[0]);
    
	geoBuffer->update();
	geoBuffer->bind();
	geoBuffer->draw();
	geoBuffer->unbind();
	return;
}

// Apply material on the object
void GLES20MeshLoader::ApplyMaterial()
{
	MaterialAmbientUniform->SetValue((GLfloat*)&matObj->ambient);
	MaterialSpecularUniform->SetValue((GLfloat*)&matObj->specular);    
	MaterialDiffuseUniform->SetValue((GLfloat*)&matObj->diffuse);
	ShininessFactorUniform->SetValue((GLfloat*)&matObj->shiness);
}

void GLES20MeshLoader::ApplyLight()
{
	// Presently, there is only one light, for multiple ligths this loop does not satisfy the uniform arrays.
    for(int i =0; i<parentModel->scene()->getLights().size(); i++){
        Light*& light = parentModel->scene()->getLights().at(i);
		LightAmbientUniform->SetValue((GLfloat*)&light->material.ambient);
		LightSpecularUniform->SetValue((GLfloat*)&light->material.specular);
		LightDiffuseUniform->SetValue((GLfloat*)&light->material.diffuse);
		LightPositionUniform->SetValue((GLfloat*)&light->position);
    }
}

void GLES20MeshLoader::processTexture()
{
	// At present the OBJ Parser does not support the reading of material information.
	// Therefore hardcoding the texture value.
	char imagePath[512] = "../Resource/Models/mbclass/mbcclass.png";
	
	ImageManager* imageManagerObject = ImageManager::GetInstance();
	it = imageManagerObject->imageMap.find(imagePath);
	if (it != imageManagerObject->imageMap.end()){
		imageItem = (*it).second;
	}
	else{   // Load the image here and pass the Image pointer to the Pixmap
		imageItem = new PngImage(); // This hardcoding need to be fixed, the image should be loaded on the basis of fileextension.
		imageItem->loadImage(imagePath);
		imageManagerObject->imageMap[imagePath] = imageItem;
	}

	glActiveTexture(GL_TEXTURE0);
	if (imageItem->getTextureID() == 0){
		GLint texInternalFormat = textureObj.getInternalFormat(imageItem);
		textureObj.generateTexture2D(textureObj.getTarget(TWO_DIMENSIONAL_TEXTURE), imageItem->imageWidth(), imageItem->imageHeight(), texInternalFormat, GL_UNSIGNED_BYTE, texInternalFormat, imageItem->bits(), false);
		imageItem->getTextureID() = textureObj.getTextureID();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}