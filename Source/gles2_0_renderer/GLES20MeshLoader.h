#ifndef GLES20MESHLOADER_H
#define GLES20MESHLOADER_H

//#include "IModel.h"
#include "glutils.h"
#include "Renderer.h"
#include <string>
#include "MeshWaveFront\WaveFrontOBJ.h"
#include "IMesh.h"
#include "ImageBuffer/Image.h"
#include "ImageBuffer/PngImage.h"
#include "ImageManager.h"
#include "Texture.h"

using namespace std;
using namespace MeshNameSpace;
using namespace namespaceimage;

class GLES20MeshLoader : public IMesh
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

    // Touch Events
	void TouchEventDown( float x, float y );
	void TouchEventMove( float x, float y );
	void TouchEventRelease( float x, float y );
    
private:
    // Prepare the geometry of the mesh model
	void PrepareGeometry(Mesh* objMeshModel);
    
    // Switch the mesh model
    void SwitchMesh();
    
    // Release the mesh model resources
    void ReleaseMeshResources();
    
    // Object for wavefront model parser
    OBJMesh waveFrontObjectModel;
	
	// Mesh data structure holding temparary array of geometry
	Mesh* meshModel;

	// Index count of the geometry
	unsigned int indexCount;

	glm::mat3 normalMat;
	Image* imageItem;
	Texture textureObj;
	GLuint OBJ_VAO_Id;

	int stride;
	GLvoid* offsetNormal, *offsetTexCoord, *offsetTangent;
	char MVP, MV;
	GLint NormalMatrix;
	GLint Tex;
	GLuint vertexBuffer;

protected:
	float* MVPMatrix;
};

#endif // GLES20MESHLOADER_H
