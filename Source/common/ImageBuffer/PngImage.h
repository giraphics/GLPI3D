#pragma once
#include "Image.h"
#include "../../gles2_0_renderer/glutils.h"
#include "../libpng/png.h"

namespace namespaceimage {
class PngImage : public Image
{
	unsigned int byte;
	unsigned int format;
    png_byte colorType;    
public:
	PngImage(void);
	~PngImage(void);

	//! Load the bitmap file helper.
	virtual bool loadImage(const char* fileName, TextureTypeEnum textureType = TWO_DIMENSIONAL_TEXTURE);

   virtual bool loadImageCubeMaps(const char* fileName[6]){ return true; }
	unsigned char* read_png_file(const char* file_name);//, png_bytep * row_pointers, int* width, int* height);
};

}
