//
//  GLES20Pixmap.h
//

#ifndef __GLES20PIXMAP_H__
#define __GLES20PIXMAP_H__

#include <iostream>

#include "ModelEx.h"
#include "glutils.h"
#include "Renderer.h"
#include "Scene.h"
#include "Rectangle.h"
#include "IPixmap.h"
#include "Texture.h"
#include "VBO.h"

using namespace std;

class GLES20Pixmap : public IRenderable{

public:
   GLES20Pixmap( Image* imageItem, TextureTypeEnum textureType = TWO_DIMENSIONAL_TEXTURE);
   GLES20Pixmap( unsigned int ID, TextureTypeEnum textureType );
   
   virtual ~GLES20Pixmap();

    void Initialize();
    
    // Render the Model class
    void Render(bool (*customRender)()=NULL);

	void Update( float t ){}

	void SetVertices(std::vector<glm::vec3>* verticesList);

	void SetTexCoords(std::vector<glm::vec2>* texCoordList);

	void SetColor(glm::vec4* colors);

	GLint getInternalFormat(Image* imageItem);

	GLenum getTarget(TextureTypeEnum textureType);

private:
	// Keep the member variables multiple of 32 as much 
	// as possible to avoid explicit packing and alignment 
    GLint mvp, col, positionAttribHandle, positionTextureHandle;    
	float* color;
	Texture textureObj;
    GLint tex;
	GeometryMesh gm;
};

#endif // GLES20PIXMAP_H
 
