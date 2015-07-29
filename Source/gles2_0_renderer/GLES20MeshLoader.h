#ifndef GLES20MESHLOADER_H
#define GLES20MESHLOADER_H

#include "glutils.h"
#include "Renderer.h"
#include <string>
#include "MeshWaveFront\WaveFrontOBJ.h"
#include "ImageBuffer/Image.h"
#include "ImageBuffer/PngImage.h"
#include "ImageManager.h"
#include "Texture.h"
#include "Geometry.h"

using namespace std;
using namespace MeshNameSpace;
using namespace namespaceimage;

class GLES20MeshLoader : public IRenderable, public IGeoBuffer, public TransformationMatrix
{

public:
    // Constructor for ObjLoader
	GLES20MeshLoader(Mesh* inMesh, Model* parent);

    // Destructor for ObjLoader
	~GLES20MeshLoader();

    // Initialize our Model class
    void Initialize();

	// Render the Model class
	void Render(bool(*customRender)() = NULL);

	// Render the Model class
	void Update(float t){}
	void SetVertices(std::vector<glm::vec3>* verticesList){}
	void SetTexCoords(std::vector<glm::vec2>* texCoordList){}
	void SetIndices(std::vector<unsigned short>* texCoordList){}
	void SetColor(glm::vec4* color){}

private:    
    // Release the mesh model resources
    void ReleaseMeshResources();
    
    // Object for wavefront model parser
    OBJMesh waveFrontObjectModel;
	
	void processTexture();

	void ApplyMaterial();
	
	void ApplyLight();
	
	// Mesh data structure holding temparary array of geometry
	Mesh*				meshModel;
	glm::mat3			normalMat;
	Image*				imageItem;
	Texture				textureObj;
	int					stride;
	GLvoid*				offsetNormal, *offsetTexCoord, *offsetTangent;
	Material*			matObj;

protected:
	float*				MVPMatrix;
	Model*				parentModel;

private:
	Uniform3f*			MaterialAmbientUniform;	
	Uniform3f*			MaterialSpecularUniform;	
	Uniform3f*			MaterialDiffuseUniform;	
	Uniform3f*			LightAmbientUniform;	
	Uniform3f*			LightSpecularUniform;		
	Uniform3f*			LightDiffuseUniform;	
	Uniform1f*			ShininessFactorUniform;	
	Uniform3fv*			LightPositionUniform;		

	UniformMatrix4fv*	MVPUniform;
	UniformMatrix4fv*	MVUniform;
	UniformMatrix3fv*	NormalMatrixUniform;
	Uniform1i*			TexUniform;
	GLint				textureUnit;
};

#endif // GLES20MESHLOADER_H
