//
//  GLES20Rectangle.h
//
//  Created by macbook on 1/16/14.
//  Copyright (c) 2014 macbook. All rights reserved.
//

#ifndef __GLES20RECTANGLE_H__
#define __GLES20RECTANGLE_H__

#include <iostream>

#include "ModelEx.h"
#include "glutils.h"
#include "Renderer.h"
#include "Scene.h"
#include "Pixmap.h"
#include "Geometry.h"
#include "Interfaces/IModel.h"

using namespace std;

class GLES20Rectangle : public IRenderable, public IGeoBuffer, public TransformationMatrix
{
    
public:
    // Constructor for Rectangle
    GLES20Rectangle(BufferScheme bufScheme = BUFFER_VAO, DrawingScheme drawScheme = DRAW_ARRAY);

	// Destructor for ObjLoader
    virtual ~GLES20Rectangle();

    // Initialize our Model class
    void Initialize();
    
    // Render the Model class
    void Render(bool (*customRender)()=NULL);

	void Update( float t ){}

	void SetVertices(std::vector<glm::vec3>* verticesList);

	void SetTexCoords(std::vector<glm::vec2>* texCoordList);

	void SetColor(glm::vec4* colors);

	void SetIndices(std::vector<unsigned short>* indicesList);
	void setDrawingPrimitive(GLenum drawPrimitive);
private:
	UniformMatrix4fv* mvpUniform;
	Uniform4fv* colUniform;
    float* color;
};
#endif // GLES20Rectangle_H
 
