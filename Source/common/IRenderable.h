//
//  IRenderable.h
//
//

#ifndef __IRenderable__
#define __IRenderable__
#include "glutils.h"
#include "IModel.h"
#include "ILifeCycle.h"
#include <vector>

class IRenderable : public IModel, public ILifeCycle
{
public:
    // Constructor for Button
   IRenderable(){}

    // Destructor for IRenderable
   virtual ~IRenderable(){}

   virtual void SetVertices(std::vector<glm::vec3>* verticesList) = 0;
   virtual void SetTexCoords(std::vector<glm::vec2>* texCoordList) = 0;
   virtual void SetColor(glm::vec4* color) = 0;
};
#endif // IRENDERABLE_H
 
