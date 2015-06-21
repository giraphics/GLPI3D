#ifndef GRLC_DEMO_TEX_QUAD_H
#define GRLC_DEMO_TEX_QUAD_H

#include "GLPi3D__Model.h"
#include "GLPi3D__Glutils.h"
#include "GLPi3D__Renderer.h"
using namespace GLPi3D;

class DemoTexQuad : public Model
{
private:
   // Handles
   GLint    MVP;
   GLuint   positionAttribHandle;
   GLuint   texCoordAttribHandle;
   GLuint   TEX;
   GLuint   texture_id;

public:
   DemoTexQuad(  ModelType typeOfModel, Model* parent = NULL);
   ~DemoTexQuad();

   void InitModel();
   void Render();
   void SetGLStates();

   // Internal Test case
   void TextureFilteringTest();
   void GRLCDrawBMPCompare();
};

#endif // GRLC_DEMO_TEX_QUAD_H

