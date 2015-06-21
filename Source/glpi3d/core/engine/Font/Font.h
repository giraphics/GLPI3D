#ifndef _FONT_H
#define _FONT_H

#include "GLPi3D__Model.h"
#include "Fontgenerator.h"
namespace GLPi3D
{

class Font : public Model, public FontGenerator {

public:
    ~Font();
   void Render();
   void InitModel();
   Font(char* ttfFile, int Size, ModelType Language, Model* parent = 0);
   void plainText (char* str, GLfloat Red = 1.0f, GLfloat Green = 1.0f, GLfloat Blue = 1.0f, GLfloat Alpha = 1.0f);
   void animateText(char* str, GLfloat Red = 1.0f, GLfloat Green = 1.0f, GLfloat Blue = 1.0f, GLfloat Alpha = 1.0f,float radius = 1.0,float rot = 0.0);
private:
	void drawGlyph (const Glyph& gi);
    GLint MVP, TEX, FRAG_COLOR;
    GLint positionAttribHandle, texAttribHandle;
};

}

#endif
