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
#include "IRectangle.h"
#include "Pixmap.h"
#include "VBO.h"

using namespace std;
//using namespace namespaceimage;
class GLES20Rectangle : public IRenderable
{
    
public:
    // Constructor for Rectangle
    GLES20Rectangle( /*Scene* parent, Model* model, ModelType type, std::string objectName = ""*/);

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


private:
    GLint mvp;
    GLint col;
    GLint positionAttribHandle;
    GLint positionTextureHandle;

    float* color;
	GeometryMesh gm;
	GeometryBuffer* geoBuffer;
	VBO* vbo;
	VAO* vao;
    //GLuint vao;
    //GLuint vid;
};
#endif // GLES20Rectangle_H
 
