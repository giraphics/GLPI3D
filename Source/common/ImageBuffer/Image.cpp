#include "Image.h"
#include <cstdlib>
using namespace namespaceimage;

Image::Image(void)
{
	memData.bitsraw     = NULL;
	memData.extension	= NO_IMAGE;
	memData.height		= -1;
	memData.width		= -1;
	memData.timeStamp	= 0;
	memData.texID		= 0;
	memData.dontCareTimeStamp   = false;
	memset( memData.fileName, 0, MAX_IMAGE_FILE_LENGTH);
}

Image::~Image(void)
{
	//LOGI("destructor: Image: %s", fileName());
   if ( memData.bitsraw ){
      delete memData.bitsraw;
      memData.bitsraw = NULL;
   }

	if (memData.texID){
		glDeleteTextures( 1,&memData.texID );
		memData.texID	= 0;
	}
}

bool Image::releaseImage()
{
   if ( memData.bitsraw ){
      delete memData.bitsraw;
      memData.bitsraw = NULL;
   }

	memData.bitsraw   = NULL;
	memData.texID     = NULL;
	return true;
}

unsigned char* Image::bits()
{ 
	return memData.bitsraw; 
}

int Image::imageHeight()
{
	return memData.height;
}

int Image::imageWidth()
{
	return memData.width;
}

ImageTypeEnum Image::imageExtension()
{
	return memData.extension;
}

bool Image::setExtension(ImageTypeEnum ext)
{
	memData.extension = ext;
	return true;
}

//! Set the File Name
bool Image::setFileName(char* fileName)
{
	memset( memData.fileName, 0, 500 );
	strncpy( memData.fileName, fileName, MAX_IMAGE_FILE_LENGTH );
	memData.fileName[MAX_IMAGE_FILE_LENGTH-1] = '\0';
   return true;
}

const char* Image::fileName()
{
	return memData.fileName;
}


bool Image::setTextureID(unsigned int id)
{
	memData.texID = id;
   return true;
}

unsigned int& Image::getTextureID()
{
	return memData.texID;
}

bool Image::setTimeStamp(clock_t time)
{
	memData.timeStamp = time;
   return true;
}

clock_t Image::getTimeStamp()
{
	return memData.timeStamp;
}

//! Get the color type of the image
ImageColorType Image::colorType()
{
   return memData.imgColorType;
}

//! Set the color type of the image
void Image::setImageColorType(ImageColorType colorType) 
{
   memData.imgColorType = colorType;
}

bool Image::deleteBits(){
	if(!memData.bitsraw){
		return true;
	}

	free(memData.bitsraw); 
	memData.bitsraw = NULL; 
	return true; 
}
