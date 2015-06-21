#include "ImageManager.h"

ImageManager* ImageManager::singleTon = NULL;

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
   //std::map<std::string, namespaceimage::Image*>::const_iterator it;

   //for (std::map<std::string, namespaceimage::Image*t>::iterator it=imageMap.begin(); it!=imageMap.end(); ++it){
   //   it->second;
   //}
   // Delete all the images from the Image manager and release the open gl memory.

}


ImageManager* ImageManager::GetInstance(){ 
   if(!singleTon){ 
      singleTon = new ImageManager();
   }
   return singleTon; 
}

void ImageManager::DeleteInstance(){ 


   delete singleTon;
   singleTon = NULL;
}

