#include "ImageManager.h"
#include "ImageBuffer/Image.h"
#include "ImageBuffer/PngImage.h"
#include <map>
using namespace std;
ImageManager* ImageManager::singleTon = NULL;
map<std::string, namespaceimage::Image*>::const_iterator it;

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

Image* ImageManager::GetImage(const char* fileName)
{
   Image* imageItem = NULL;
   ImageManager* imageManagerObject = ImageManager::GetInstance();
   it = imageManagerObject->imageMap.find(fileName);
   if( it != imageManagerObject->imageMap.end()){
      imageItem = (*it).second;
   }
   else{   // Load the image here and pass the Image pointer to the Pixmap
      imageItem = new PngImage(); // This hardcoding need to be fixed, the image should be loaded on the basis of fileextension.
      imageItem->loadImage(fileName);
      imageManagerObject->imageMap[fileName] = imageItem;
   }
   return imageItem;
}

