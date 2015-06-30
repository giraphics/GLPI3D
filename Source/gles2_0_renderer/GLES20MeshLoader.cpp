#include "GLES20MeshLoader.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "Cache.h"
#include "constant.h"
using namespace glm;

#define GetAttribute ProgramManager::GetInstance()->ProgramGetVertexAttribLocation
#define GetUniform ProgramManager::GetInstance()->ProgramGetUniformLocation
extern map<std::string, namespaceimage::Image*>::const_iterator it;

/*!
	Simple Default Constructor

	\param[in] None.
	\return None

*/
GLES20MeshLoader::GLES20MeshLoader(Mesh* inMesh, Model* parent)
{
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
    
	MaterialAmbient		= GetUniform(ProgramID, (char*)"MaterialAmbient");
	MaterialSpecular	= GetUniform(ProgramID, (char*)"MaterialSpecular");
	MaterialDiffuse		= GetUniform(ProgramID, (char*)"MaterialDiffuse");
	LightAmbient		= GetUniform(ProgramID, (char*)"LightAmbient");
	LightSpecular		= GetUniform(ProgramID, (char*)"LightSpecular");
	LightDiffuse		= GetUniform(ProgramID, (char*)"LightDiffuse");
	ShininessFactor		= GetUniform(ProgramID, (char*)"ShininessFactor");
	LightPosition		= GetUniform(ProgramID, (char*)"LightPosition");
    
    //if (MaterialAmbient >= 0){
    //    glUniform3f(MaterialAmbient, 0.04f, 0.04f, 0.04f);
    //}
    //
    //if (MaterialSpecular >= 0){
    //    glUniform3f(MaterialSpecular, 1.0, 0.5, 0.5);
    //}
    
    //glm::vec3 color = glm::vec3(1.0, 0.0, 0.0);
    //if (MaterialDiffuse >= 0){
    //    glUniform3f(MaterialDiffuse, color.r, color.g, color.b);
    //}

    //if (LightAmbient >= 0){
    //    glUniform3f(LightAmbient, 1.0f, 1.0f, 1.0f);
    //}
    //
    //if (LightSpecular >= 0){
    //    glUniform3f(LightSpecular, 1.0, 1.0, 1.0);
    //}

    //if (LightDiffuse >= 0){
    //    glUniform3f(LightDiffuse, 1.0f, 1.0f, 1.0f);
    //}

    //if (ShininessFactor >= 0){
    //    glUniform1f(ShininessFactor, 40);
    //}
    
    //if (LightPosition >= 0){
    //    glUniform3f(LightPosition, 0.0, 0.0, 10.0 );
    //}

	MVP				= GetUniform(ProgramID, (char*)"ModelViewProjectionMatrix");
	MV				= GetUniform(ProgramID, (char*)"ModelViewMatrix");
	NormalMatrix	= GetUniform(ProgramID, (char*)"NormalMatrix");
	Tex				= GetUniform(ProgramID, (char *) "Tex1");

	glUniform1i(Tex, 0);
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
	glUniformMatrix4fv( MV, 1, GL_FALSE,(float*)&tempMatrix[0] );

	// Calculate the Normal Matrix
	normalMat = glm::mat3(glm::vec3(tempMatrix[0]), glm::vec3(tempMatrix[1]), glm::vec3(tempMatrix[2]));
	glUniformMatrix3fv( NormalMatrix, 1, GL_FALSE, (float*)&normalMat );

	// Calculate the Model-View-Projection Matrix
	tempMatrix = *ProjectionMatrix * tempMatrix;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, (float*)&tempMatrix[0]);
    
    // Bind with Vertex Array Object for OBJ
    glBindVertexArray(OBJ_VAO_Id);
    
    // Draw Geometry
	glDrawArrays(GL_TRIANGLES, 0, indexCount);
    
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
	// Optimize this GetMaterial call
	if ( MaterialAmbient >= 0 ){
        glUniform3f(MaterialAmbient, matObj->ambient.r, matObj->ambient.g, matObj->ambient.b);
    }
    
    if ( MaterialSpecular >= 0){
        glUniform3f( MaterialSpecular, matObj->specular.r, matObj->specular.g, matObj->specular.b );
    }
    
    if ( MaterialDiffuse >= 0 ){
        glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
        glUniform3f( MaterialDiffuse, matObj->diffuse.r, matObj->diffuse.g, matObj->diffuse.b );
    }
    
    if ( ShininessFactor >= 0 ){
        glUniform1f(ShininessFactor, matObj->shiness);
    }
}

void GLES20MeshLoader::ApplyLight()
{
    for(int i =0; i<parentModel->scene()->getLights().size(); i++){
        Light*& light = parentModel->scene()->getLights().at(i);
        if ( LightAmbient >= 0 ){
            glUniform3f( LightAmbient, light->material.ambient.r, light->material.ambient.g, light->material.ambient.b );
        }
        
        if ( LightSpecular >=  0 ){
            glUniform3f( LightSpecular, light->material.specular.r, light->material.specular.g, light->material.specular.b );
        }
        
        if ( LightDiffuse >= 0 ){
            glUniform3f(LightDiffuse, light->material.diffuse.r, light->material.diffuse.g, light->material.diffuse.b);
        }
        
        if ( LightPosition >= 0 ){
            glm::vec3 lightPosition(light->position.x,light->position.y,light->position.z);
            glUniform3fv(LightPosition, 1, (float*)&lightPosition);
        }
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