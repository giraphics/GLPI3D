//
//  IRenderable.h
//
//

#ifndef __IRenderable__
#define __IRenderable__
#include "glutils.h"
#include "IModel.h"
#include "ILifeCycle.h"

class IRenderable : public IModel, public ILifeCycle
{
public:
    // Constructor for Button
   IRenderable(){}

    // Destructor for IRenderable
   virtual ~IRenderable(){}

   virtual void SetVertices(glm::vec3* vertices) = 0;
   virtual void SetTexCoords(glm::vec2* texCoords) = 0;
   virtual void SetColor(glm::vec4* color) = 0;
};
#endif // IRENDERABLE_H
 
