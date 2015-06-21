#ifndef GRLC_DEMO_TRIANGLE_H
#define GRLC_DEMO_TRIANGLE_H

#include "GLPi3D__Model.h"
#include "GLPi3D__Renderer.h"
using namespace GLPi3D;

class DemoTriangle : public Model
{
private:
   GLint    positionAttribHandle;
   GLint    colorAttribHandle;
   GLuint   radianAngle;

   float    degree;
   float    radian;

public:
   DemoTriangle( ModelType typeOfModel, Model* parent = NULL);
   ~DemoTriangle();

   void     InitModel();
   void     Render();
};

#endif // GRLC_DEMO_TRIANGLE_H

