#include "GLES20Renderer.h"
#include "GLES20Rectangle.h"

//! Renderer Constructor
GLES20Renderer::GLES20Renderer(int width, int height) : Renderer(width, height)
{
}

//! Renderer Destructor
GLES20Renderer::~GLES20Renderer()
{
    
}

////! Initialize Scene
//void GLES20Renderer::initializeScenes()
//{
//    for( int i=0; i<scenes.size();  i++ ){
//        scenes.at(i)->initializeScene();
//    }
//}
//
////! resize screen
//void GLES20Renderer::resize( int w, int h )
//{
//    for( int i=0; i<scenes.size();  i++ ){
//        scenes.at(i)->resize(w, h);
//    }
//}
//
////! Cache the scene
//void GLES20Renderer::addScene( Scene* scene)
//{
//    if(!scene)
//        return;
//    
//    for (std::vector<Scene*>::iterator it = scenes.begin() ; it != scenes.end(); ++it){
//        if( (*it) == scene ){
//            //If already added return;
//            return;
//        }
//    }
//
//    scenes.push_back( scene );
//    scene->setRenderer(this);
//}
//
//bool GLES20Renderer::removeScene(Scene* scene)
//{
//    int i = 0;
//    for (std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); it++,i++)
//    {
//        if( (*it) == scene){
//            scenes.erase(scenes.begin()+i);
//            return true;
//        }
//    }
//    return false;
//}
//
//Scene* GLES20Renderer::getScene(char* name)
//{
//    for (std::vector<Scene*>::iterator it = scenes.begin() ; it != scenes.end(); ++it){
//        if( (*it)->GetName().compare(name) == 0 ){
//            return (*it);
//        }
//    }
//    return NULL;
//}
//
////! Render the Models
//void GLES20Renderer::render()
//{
////    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
////    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//    for( int i=0; i<scenes.size();  i++ ){
//        scenes.at(i)->render();
//    }
//}

//! Gesture Press Event
void GLES20Renderer::TouchEventDown( float x, float y )
{
    for( int i=0; i<scenes.size();  i++ ){
        scenes.at(i)->TouchEventDown(x, y);
    }
}

//! Gesture Move Event
void GLES20Renderer::TouchEventMove( float x, float y )
{
    for( int i=0; i<scenes.size();  i++ ){
        scenes.at(i)->TouchEventMove(x, y);
    }
}

//! Gesture Release Event
void GLES20Renderer::TouchEventRelease( float x, float y )
{
    for( int i=0; i<scenes.size();  i++ ){
        scenes.at(i)->TouchEventRelease(x, y);
    }
}

Object* GLES20Renderer::createObject(Scene* handler, Model* parent, ModelType type, std::string objectName)
{
   switch(type)
   {
      case BUTTON:
         return NULL;
      //return new GLES20Button(handler, parent, type, objectName);
      break;
   }
}
