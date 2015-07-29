//
//  MeshObject.h
//

#ifndef __MESHOBJECT_H__
#define __MESHOBJECT_H__

#include <iostream>

#include "ModelEx.h"
#include "glutils.h"
#include "Renderer.h"
#include "Scene.h"
#include "MeshWaveFront\WaveFrontOBJ.h"
#include <string>
using namespace MeshNameSpace;

class MeshObject : public IRenderable, public Model{

private:
	IRenderable* specificMesh;

public:
	// Initialize our Model class
	void Initialize();

	// Render the Model class
	void Render(bool (*customRender)()=NULL);

	void Update( float t ){}

	// Set the Model View Projection matrix
	//void SetMVP(float*){}

	void SetProgram(unsigned int ID);
	// Present the set function of vertice are not implemented for mesh, the geometry is directyly feed into GeometryBuffer
	void SetVertices(std::vector<glm::vec3>* verticesList){}
	void SetTexCoords(std::vector<glm::vec2>* texCoordList){}
	void SetIndices(std::vector<unsigned short>* texCoordList){}
	void SetColor(glm::vec4* color){}

	void (MeshObject::*transformationMethod)();
	void setTransformationForOpenGLES20Pipeline();
	void setTransformationForOpenGLES31Pipeline(){/*Not Supported*/}
	void setTransformationForVulkanPipeline(){/*Not Supported*/}
	GeometryBuffer* GeometryBuffer();
public:
   MeshObject(const char* meshPath, Scene* parent, Model* model, ModelType type, bool flatShading = false,std::string objectName = "");
   virtual ~MeshObject();

protected:
	// Object for wavefront model parser
	OBJMesh waveFrontObjectModel;
};

#endif // __MESHOBJECT_H__
 
