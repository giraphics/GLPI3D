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
#include <string>
#include "ImageBuffer/Image.h"
#include "ImageBuffer/PngImage.h"

using namespace namespaceimage;


class GRectangle : public IRectangle, public Model
{
private:
IRectangle* specificRectangle;

public:
   // Constructor for Rectangle
   GRectangle( Scene* parent, Model* model, ModelType type, std::string objectName = "");

   // Destructor for ObjLoader
   virtual ~GRectangle();

   // Initialize our Model class
   void Initialize();

   // Render the Model class
   void Render(bool (*customRender)()=NULL);

   void Update( float t ){}

   // Set the vertex information for the rectangle
   void SetVertices(glm::vec3* vertices);

   // Set the texture coordinate information for the rectangle
   void SetTexCoords(glm::vec2* texCoords);

   // Set the Model View Projection matrix
   void SetMVP(float*){}

   // Set the color or rectangle
   void SetColor(glm::vec4* color);//{ rectColor = color; }
   void SetProgram(unsigned int ID);

protected:
    glm::vec2 texCoordinatesRectangle[4];
    glm::vec3 verticesRectangle[4];
    glm::vec4 rectColor;
};

#endif // Rectangle_H
 
