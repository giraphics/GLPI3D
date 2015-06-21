#ifndef GRLC_DEMO_RECTANGLE_H
#define GRLC_DEMO_RECTANGLE_H

#include "GLPi3D__Model.h"
#include "GLPi3D__Glutils.h"
#include "GLPi3D__Renderer.h"
using namespace GLPi3D;

class DemoRectangle : public Model
{
private:
   GLint    positionAttribHandle;
   GLint    colorAttribHandle;
   GLuint   radianAngle;
   GLint MVP;

   float    degree;
   float    radian;

public:
   DemoRectangle( ModelType typeOfModel, Model* parent = NULL);
   ~DemoRectangle();

   void     InitModel();
   void     Render();
};

#endif // GRLC_DEMO_RECTANGLE_H

