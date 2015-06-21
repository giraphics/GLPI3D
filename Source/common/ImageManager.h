//
//  ImageManager.h
//

#ifndef __IMAGE_MANAGER_H__
#define __IMAGE_MANAGER_H__

#include <string>
#include <map>
#include "ImageBuffer/Image.h"

class ImageManager
{
private:
   ImageManager();
   ~ImageManager();

public:
   std::map<std::string, namespaceimage::Image*> imageMap;
   static ImageManager* GetInstance();
   static void DeleteInstance();

private:
   // Static singleton object
   static ImageManager* singleTon;

};

#endif // __IMAGE_MANAGER_H__
 
