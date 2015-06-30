//
//  ImageManager.h
//

#ifndef __IMAGE_MANAGER_H__
#define __IMAGE_MANAGER_H__

#include <string>
#include <map>
#include "ImageBuffer/Image.h"
using namespace namespaceimage;

class ImageManager
{
private:
   ImageManager();
   ~ImageManager();

public:
   std::map<std::string, Image*> imageMap;
   static ImageManager* GetInstance();
   static void DeleteInstance();
   static Image* GetImage(const char* fileName);

private:
   // Static singleton object
   static ImageManager* singleTon;

};

#endif // __IMAGE_MANAGER_H__
 
