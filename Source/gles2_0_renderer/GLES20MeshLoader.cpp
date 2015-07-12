#include "GLES20MeshLoader.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"
using namespace glm;

#define GetAttribute ProgramManager::GetInstance()->ProgramGetVertexAttribLocation
//#define GetUniform ProgramManager::GetInstance()->ProgramGetUniformLocation
extern map<std::string, namespaceimage::Image*>::const_iterator it;

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
GLES20MeshLoader::GLES20MeshLoader(Mesh* inMesh, Model* parent)
{
	geoBuffer = new GeometryBuffer(this);

	indexCount	= inMesh->indexCount;
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

    glDeleteVertexArrays(1, &OBJ_VAO_Id);
    glDeleteBuffers(1, &vertexBuffer);
}

void GLES20MeshLoader::SwitchMesh()
{

}

void GLES20MeshLoader::PrepareGeometry(Mesh* objMeshModel)
{
	glUseProgram(ProgramID);

	// QueryAttributes
	// Handle for position
	GLint positionAttribHandle	= GetAttribute(ProgramID, (char*)"VertexPosition");

	// Handle for normal
	GLint normalAttribHandle	= GetAttribute(ProgramID, (char*)"Normal");

	// Handle for texture coord attribute
	GLint texCoordAttribHandle	= GetAttribute(ProgramID, (char*)"VertexTexCoord");

	// Handle for tangent attribute
	GLint tangentAttribHandle	= GetAttribute(ProgramID, (char*)"VertexTangent");

	stride = (2 * sizeof(vec3)) + sizeof(vec2) + sizeof(vec4);
	offsetNormal = (GLvoid*)(sizeof(glm::vec3) + sizeof(vec2));
	offsetTexCoord = (GLvoid*)(sizeof(glm::vec3));
	offsetTangent = (GLvoid*)(sizeof(glm::vec3) + sizeof(vec2) + sizeof(vec3));

    // Create the VBO for our obj model vertices.
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, objMeshModel->vertices.size() * sizeof(objMeshModel->vertices[0]), &objMeshModel->vertices[0], GL_STATIC_DRAW);
    
    // Create the VAO, this will store the vertex attributes into vectore state.
    glGenVertexArrays(1, &OBJ_VAO_Id);
    glBindVertexArray(OBJ_VAO_Id);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	if (positionAttribHandle >= 0){
		glEnableVertexAttribArray(positionAttribHandle);
		glVertexAttribPointer(positionAttribHandle, 3, GL_FLOAT, GL_FALSE, stride, 0);
	}

	if (texCoordAttribHandle >= 0){
		glEnableVertexAttribArray(texCoordAttribHandle);
		glVertexAttribPointer(texCoordAttribHandle, 2, GL_FLOAT, GL_FALSE, stride, offsetTexCoord);
	}

	if (normalAttribHandle >= 0){
		glEnableVertexAttribArray(normalAttribHandle);
		glVertexAttribPointer(normalAttribHandle, 3, GL_FLOAT, GL_FALSE, stride, offsetNormal);
	}

	if (tangentAttribHandle >= 0){
		glEnableVertexAttribArray(tangentAttribHandle);
		glVertexAttribPointer(tangentAttribHandle, 4, GL_FLOAT, GL_FALSE, stride, offsetTangent);
	}

    glBindVertexArray(0);
    
    objMeshModel->vertices.clear();
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
	// Use Phong Shade Program
    glUseProgram( ProgramID );
    
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
	geoBuffer->initUniforms();

	textureUnit = 0; //Need to set from a setter function.
	TexUniform->SetValue(&textureUnit);
	//glUniform1i(Tex, 0);
	glEnable(GL_DEPTH_TEST);
	
	if (meshModel){
		PrepareGeometry(meshModel);
	}
    return;
}

/*!
	Initialize the scene, reserve shaders, compile and chache program

	\param[in] None.
	\return None

*/
void GLES20MeshLoader::Render(bool(*customRender)())
{
    glUseProgram(ProgramID);
	ApplyMaterial();
	ApplyLight();
	
	textureObj.BindTexture();
	// Calculate the Model-View Matrix
	tempMatrix = *ViewMatrix * *ModelMatrix;
	MVUniform->SetValue((GLfloat*)&tempMatrix[0]);

	// Calculate the Normal Matrix
	normalMat = glm::mat3(glm::vec3(tempMatrix[0]), glm::vec3(tempMatrix[1]), glm::vec3(tempMatrix[2]));
	NormalMatrixUniform->SetValue((GLfloat*)&normalMat[0]);

	// Calculate the Model-View-Projection Matrix
	tempMVPMatrix = *ProjectionMatrix * tempMatrix;
	MVPUniform->SetValue((GLfloat*)&tempMVPMatrix[0]);
    
	geoBuffer->update();
    
    // Bind with Vertex Array Object for OBJ
    glBindVertexArray(OBJ_VAO_Id);
    
    // Draw Geometry
	glDrawArrays(GL_TRIANGLES, 0, indexCount);
	//glDrawElements(GL_TRIANGLES, 0, GL_UNSIGNED_SHORT, (void*)&meshModel->indices[0]);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
}
//
void GLES20MeshLoader::TouchEventDown(float x, float y)
{
    SwitchMesh();
}

void GLES20MeshLoader::TouchEventMove(float x, float y)
{
}

void GLES20MeshLoader::TouchEventRelease(float x, float y)
{
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