//
//  IRenderable.h
//
//

#ifndef __IRenderable__
#define __IRenderable__
#include "glutils.h"
#include "IModel.h"
#include "ILifeCycle.h"
#include "Geometry.h"
#include <vector>

class IGeoBuffer{
public:
	IGeoBuffer(){}
	~IGeoBuffer(){}
	GeometryBuffer* getGeometry(){ return geoBuffer; }
protected:
GeometryBuffer* geoBuffer;

};

class IRenderable : public IModel, public ILifeCycle
{

public:
    // Constructor for Button
   IRenderable(){}

    // Destructor for IRenderable
   virtual ~IRenderable(){}

   virtual void SetVertices(std::vector<glm::vec3>* verticesList) = 0;
   virtual void SetTexCoords(std::vector<glm::vec2>* texCoordList) = 0;
   virtual void SetIndices(std::vector<unsigned short>* texCoordList) = 0;
   virtual void SetColor(glm::vec4* color) = 0;
   virtual void setDrawingPrimitive(GLenum drawPrimitive){} //Need to add into pixmap and mesh
};

class Renderable : public IRenderable
{
public:
    // Constructor for Renderable
   Renderable(){}

    // Destructor for Renderable
   virtual ~Renderable(){}

};
#endif // IRENDERABLE_H