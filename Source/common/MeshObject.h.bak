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
#include "IMesh.h"
#include "MeshWaveFront\WaveFrontOBJ.h"
#include <string>
using namespace MeshNameSpace;

class MeshObject : public IMesh, public Model{

private:
	IMesh* specificMesh;

public:
   // Initialize our Model class
   void Initialize();

   // Render the Model class
   void Render(bool (*customRender)()=NULL);

   void Update( float t ){}

   // Set the Model View Projection matrix
   //void SetMVP(float*){}

   void SetProgram(unsigned int ID);

public:
   MeshObject(const char* meshPath, Scene* parent, Model* model, ModelType type, std::string objectName = "");
   virtual ~MeshObject();

protected:
	// Object for wavefront model parser
	OBJMesh waveFrontObjectModel;
};

#endif // __MESHOBJECT_H__
 
