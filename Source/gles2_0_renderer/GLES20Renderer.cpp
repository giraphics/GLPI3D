#include "GLES20Renderer.h"
#include "GLES20Rectangle.h"

//! Renderer Constructor
GLES20Renderer::GLES20Renderer(int width, int height) : Renderer(width, height)
{
	rendererType = PluginType::OPENGLES20_STATIC_PLUGIN;
}

//! Renderer Destructor
GLES20Renderer::~GLES20Renderer()
{
    
}

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