//
//  Rectangle.h
//

#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include <iostream>

#include "IRectangle.h"
#include "ModelEx.h"
#include "glutils.h"
#include "Renderer.h"
#include "Scene.h"
#include "Geometry.h"
#include <string>
#include "ImageBuffer/Image.h"
#include "ImageBuffer/PngImage.h"

using namespace namespaceimage;


class GRectangle : public IRenderable, public Model
{
private:
IRenderable* specificRectangle;

public:
   // Constructor for Rectangle
   GRectangle( Scene* parent, Model* model, ModelType type, std::string objectName = "",BufferScheme bufScheme = BUFFER_VAO, DrawingScheme drawScheme = DRAW_ARRAY);

   // Destructor for ObjLoader
   virtual ~GRectangle();

   // Initialize our Model class
   void Initialize();

   // Render the Model class
   void Render(bool (*customRender)()=NULL);

   void Update( float t ){}

   // Set the vertex information for the rectangle
   void SetVertices(std::vector<glm::vec3>* verticesList);

   // Set the texture coordinate information for the rectangle
   void SetTexCoords(std::vector<glm::vec2>* texCoordList);

   // Set the Model View Projection matrix
   void SetMVP(float*){}

   // Set the color or rectangle
   void SetColor(glm::vec4* color);

   void SetIndices(std::vector<unsigned short>* indicesList);

   void SetProgram(unsigned int ID);
   void setDrawingPrimitive(GLenum drawPrimitive);
   void (GRectangle::*transformationMethod)();
   void setTransformationForOpenGLES20Pipeline();
   void setTransformationForOpenGLES31Pipeline(){/*Not Supported*/}
   void setTransformationForVulkanPipeline(){/*Not Supported*/}
   void drawElement();

protected:
    glm::vec2 texCoordinatesRectangle[4];
    glm::vec3 verticesRectangle[4];
    glm::vec4 rectColor;
};

#endif // Rectangle_H
 
