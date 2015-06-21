#include "PngImage.h"

using namespace namespaceimage;

PngImage::PngImage(void)
{
	memData.extension = PNG;
}

PngImage::~PngImage(void)
{
	//LOGI("destructor: PNG");
	if ( memData.bitsraw )
		delete memData.bitsraw;
	memData.bitsraw = NULL;
	if (memData.texID){
		glDeleteTextures( 1,&memData.texID );
		memData.texID	= 0;
	}
}

unsigned char* PngImage::read_png_file(const char* file_name)
{
        char header[8];    // 8 is the maximum size that can be checked

        // png variables
        png_structp pngPointer;
        png_infop infoPointer;
        png_byte bitDepth;
        int x, y;
        int number_of_passes;

        /* open file and test for it being a png */
        FILE *fp = fopen(file_name, "rb");
        if (!fp)
        	return NULL; //[read_png_file] File could not be opened for reading
        fread(header, 1, 8, fp);

        if (png_sig_cmp( (png_const_bytep) header, 0, 8))
        	return NULL; //[read_png_file] File is not recognized as a PNG file

        /* initialize stuff */
        pngPointer = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!pngPointer)
        	return NULL; //[read_png_file] png_create_read_struct failed

        infoPointer = png_create_info_struct(pngPointer);
        if (!infoPointer)
        	return NULL; //[read_png_file] png_create_info_struct failed

        png_init_io(pngPointer, fp);
        png_set_sig_bytes(pngPointer, 8);

        png_read_info(pngPointer, infoPointer);

        memData.width		= png_get_image_width(pngPointer, infoPointer);
        memData.height		= png_get_image_height(pngPointer, infoPointer);
        colorType			= png_get_color_type(pngPointer, infoPointer);
        bitDepth			= png_get_bit_depth(pngPointer, infoPointer);
        number_of_passes	= png_set_interlace_handling(pngPointer);
		if( colorType == PNG_COLOR_TYPE_PALETTE )
		{ png_set_expand( pngPointer ); }

		if( colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8 )
		{ png_set_expand( pngPointer ); }
		
		if( png_get_valid( pngPointer, infoPointer, PNG_INFO_tRNS ) )
		{ png_set_expand( pngPointer ); }
		
		if( bitDepth == 16 )
		{ png_set_strip_16( pngPointer ); }
		
		if( colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA )
		{ png_set_gray_to_rgb( pngPointer ); }

		unsigned int byte = 4;
		switch( colorType )
		{
			case PNG_COLOR_TYPE_GRAY:
			{
				byte = 1;
				break;
			}

			case PNG_COLOR_TYPE_GRAY_ALPHA:
			{
				byte = 2;
				break;
			}

			case PNG_COLOR_TYPE_RGB:
			{
				byte = 3;
				break;
			}

			case PNG_COLOR_TYPE_RGB_ALPHA:
			{
				byte = 4;
				break;
			}
		}
    
        png_read_update_info(pngPointer, infoPointer);
		png_bytep *bytep;
		unsigned char* textureArray = ( unsigned char * ) malloc( memData.width * memData.height * 4 );

		bytep = ( png_bytep * ) malloc( memData.height * sizeof( png_bytep ) );
		int i = 0, n;
		while( i != memData.height )
		{
			n = memData.height - ( i + 1 );
			
			bytep[ n ] = textureArray + ( n * memData.width * byte );
			
			++i;
		}
    
		png_read_image( pngPointer, bytep );
		png_read_end( pngPointer, NULL );
		png_destroy_read_struct( &pngPointer, &infoPointer, NULL );

        fclose(fp);
		free ( bytep);
		return textureArray;
}

//! Load the bitmap file helper.
bool PngImage::loadImage(const char* fileName, TextureTypeEnum textureType )
{
   memData.bitsraw = read_png_file( fileName);

   if (!memData.bitsraw){
      LOGI("Fail to read PNG file: %s", fileName);
      return false;
   }

   switch (colorType) {
      case PNG_COLOR_TYPE_GRAY:{
         memData.imgColorType = COLOR_TYPE_GRAY;
         break;
      }

      case PNG_COLOR_TYPE_GRAY_ALPHA:{
         memData.imgColorType = COLOR_TYPE_GRAY_ALPHA;
         break;
      }

      case PNG_COLOR_TYPE_RGB:{
         memData.imgColorType = COLOR_TYPE_RGB;
         break;
      }

      case PNG_COLOR_TYPE_RGB_ALPHA:{
         memData.imgColorType = COLOR_TYPE_RGB_ALPHA;
         break;
      }
   }

return true;
}
