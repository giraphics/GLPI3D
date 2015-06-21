#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "GLPi3D__Model.h"
#include "GLPi3D__Glutils.h"
#include "GLPi3D__Renderer.h"
#include <string>
//#include "WaveFrontOBJ/Wavefrontobj.h"
using namespace GLPi3D;

using namespace std;
class ObjLoader : public Model
{

private:
public:
   // Constructor for ObjLoader
   ObjLoader( ModelType typeOfModel, Model* parent = NULL);

   // Destructor for ObjLoader
   ~ObjLoader();

   // Initialize our Model class
   void InitModel();

   // Render the Model class
   void Render();
   void DrawCubeOfObjects();
   void CircularArrangement();
   void SetGLStates();
   void QuatRot();
   void SetMaterial(glm::vec4 color);
private:
   // Load the mesh model
   void LoadMesh();

   // Load the mesh from header file model
   void LoadMeshFromHeader();

   // Switch the mesh model
   void SwitchMesh();

   // Release the mesh model resources
   void ReleaseMeshResources();

   // Draw the diffuse light cube.
   void DrawDiffuseObject();

   // Object for wavefront model parser
   //OBJMesh waveFrontObjectModel;

   // Mesh structure object for storing parsed information.
   //Mesh* objMeshModel;

   // Parse the Wavefront OBJ mesh
   unsigned char RenderPrimitive;

   // Number of vertex Indices
   int IndexCount;

   // Handle for position
   GLint positionAttribHandle;

   // Handle for normal
   GLint normalAttribHandle;

   // Handles
   GLint MVP;
   GLint MV;
   GLint NormalMatrix;
   GLint LightPos;
   GLint MaterialColor;

   GLuint OBJ_VAO_Id;
   int stride;
   GLvoid* offset;
   GLvoid* offsetTexCoord;
   int ModelNumber;
   GLuint vertexBuffer;
};

#endif // OBJLOADER_H

