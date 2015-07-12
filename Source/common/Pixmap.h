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
using namespace namespaceimage;

class Pixmap : public IRenderable, public Model{

private:
IRenderable* specificPixmap;

public:
   // Initialize our Model class
   void Initialize();

   // Render the Model class
   void Render(bool (*customRender)()=NULL);

   void Update( float t ){}

   // Set the vertex information for the rectangle
   void SetVertices(std::vector<glm::vec3>* verticesList);

   // Set the texture coordinate information for the rectangle
   void SetTexCoords(std::vector<glm::vec2>* texCoordList);

   // Set the index of the geomtry
   void SetIndices(std::vector<unsigned short>* indicesList);

   // Set the color or rectangle
   void SetColor(glm::vec4* color);

   void SetProgram(unsigned int ID);
public:
   Pixmap(const char* imagePath, Scene* parent, Model* model, ModelType type, TextureTypeEnum textureType = TWO_DIMENSIONAL_TEXTURE, std::string objectName = "");
   Pixmap(Image* imgItem, Scene* parent, Model* model, ModelType type, TextureTypeEnum textureType = TWO_DIMENSIONAL_TEXTURE, std::string objectName = "");
   Pixmap(unsigned int ID, Scene* parent, Model* model, ModelType type, TextureTypeEnum textureType = TWO_DIMENSIONAL_TEXTURE, std::string objectName = "");
   void SetImage(const char*){}
   virtual ~Pixmap();

protected:
    glm::vec2 texCoordinatesRectangle[4];
    glm::vec3 verticesRectangle[4];
    glm::vec4 rectColor;

};

#endif // Rectangle_H
 
