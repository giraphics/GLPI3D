//
//  Pixmap.h
//

#ifndef __PIXMAP_H__
#define __PIXMAP_H__

#include <iostream>

#include "ModelEx.h"
#include "glutils.h"
#include "Renderer.h"
#include "Scene.h"
#include "IPixmap.h"
#include <string>
#include "ImageBuffer/Image.h"
#include "ImageBuffer/PngImage.h"

class Pixmap : public IPixmap, public Model{

private:
IPixmap* specificPixmap;

public:
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
   //void SetMVP(float*){}

   // Set the color or rectangle
   void SetColor(glm::vec4* color);

   void SetProgram(unsigned int ID);
public:
   Pixmap(const char* imagePath, Scene* parent, Model* model, ModelType type, TextureTypeEnum textureType = TWO_DIMENSIONAL_TEXTURE, std::string objectName = "");
   void SetImage(const char*){}
   virtual ~Pixmap();

protected:
    glm::vec2 texCoordinatesRectangle[4];
    glm::vec3 verticesRectangle[4];
    glm::vec4 rectColor;

};

#endif // Rectangle_H
 
