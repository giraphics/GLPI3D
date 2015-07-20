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
protected:
GeometryBuffer* geoBuffer;

};

class IRenderable : public IModel, public ILifeCycle
{
//protected:
//GeometryBuffer* geoBuffer;

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

   //void SetVertices(std::vector<glm::vec3>* verticesList) = 0;
   //void SetTexCoords(std::vector<glm::vec2>* texCoordList) = 0;
   //void SetIndices(std::vector<unsigned short>* texCoordList) = 0;
   //void SetColor(glm::vec4* color) = 0;
};

//void Renderable::SetVertices(std::vector<glm::vec3>* verticesList)
//{
//	if(geoBuffer->geometry()->positions.size = verticesList->size()){
//		geoBuffer->geometry()->positions.positionData = (void*)&verticesList->at(0);
//	}
//}
//
//void Renderable::SetTexCoords(std::vector<glm::vec2>* texCoordList)
//{
//	if(geoBuffer->geometry()->texCoords.size = texCoordList->size()){
//		geoBuffer->geometry()->texCoords.textureData = (void*)&texCoordList->at(0);
//	}
//}

#endif // IRENDERABLE_H
 
