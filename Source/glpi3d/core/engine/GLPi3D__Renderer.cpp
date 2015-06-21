#include "GLPi3D__Renderer.h"
namespace GLPi3D
{

//! Renderer Constructor
Renderer::Renderer()
{
    //createScenes();
}

//! Renderer Destructor
Renderer::~Renderer()
{
    
}

//! Initialize Scene
void Renderer::initializeScene(::uint16 context, ::uint16 layer)
{
    for( unsigned int i=0; i<RendererMemberData.scenes.size();  i++ ){
        RendererMemberData.scenes.at(i)->initializeScene( context, layer);
    }
}

//! resize screen
void Renderer::resize( int w, int h )
{
    for( unsigned int i=0; i<RendererMemberData.scenes.size();  i++ ){
        RendererMemberData.scenes.at(i)->resize(w, h);
    }
}

//! Set Project System
void Renderer::setUpProjection()
{
    for( unsigned int i=0; i<RendererMemberData.scenes.size();  i++ ){
        RendererMemberData.scenes.at(i)->setUpProjection();
    }
}

//! Cache the scene
void Renderer::addScene( Scene* scene)
{
    RendererMemberData.scenes.push_back( scene );
}

//! Cache the scene
void Renderer::removeScene( Scene* scene)
{
    RendererMemberData.scenes.push_back( scene );
}

//! Render the Models
void Renderer::render()
{
    glClearColor( 0.7f, 0.7f, 0.70f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    for( unsigned int i=0; i<RendererMemberData.scenes.size();  i++ ){
        RendererMemberData.scenes.at(i)->render();
    }
}

void Renderer::run()
{
   setUpProjection();
   render();
}

//! Gesture Press Event
void Renderer::TouchEventDown( float x, float y )
{

}

//! Gesture Move Event
void Renderer::TouchEventMove( float x, float y )
{
}

//! Gesture Release Event
void Renderer::TouchEventRelease( float x, float y )
{
}

}
