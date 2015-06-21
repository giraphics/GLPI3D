#pragma once
#include "GLPi3D__Scene.h"
namespace GLPi3D
{
#include <vector>

//! Render Member Data structure
typedef struct
{
    //! Model List
    std::vector <Scene*> scenes;
} RendererData;

class Renderer
{
    RendererData RendererMemberData;
    
public:
    //! Renderer Constructor
    Renderer();

    //! Renderer Destructor
    ~Renderer();

    //! Initialize Engine
    void initializeScene(::uint16 context, ::uint16 layer);

   static Renderer* dynamicObj(){ return new Renderer(); }

    //! resize screen
    void resize( int w, int h );

    //! Set Project System
    void setUpProjection();

    //! Render the Models
    void render();

    void run();

    //! Gesture Press Event
    void TouchEventDown( float a, float b );
    
    //! Gesture Move Event
    void TouchEventMove( float a, float b );
    
    //! Gesture Release Event
    void TouchEventRelease( float a, float b );

    //! Cache the scene
    void addScene( Scene* scene);
    
    //! Remove the scene
    void removeScene( Scene* scene);
};

class Application{
private:
   std::vector<Renderer*> renderingEngines;
public:
   void addEngine(Renderer* engine){ renderingEngines.push_back(engine); }
   void initialize(){}
   void exec()
   {
      for(int i=0; i<renderingEngines.size(); i++)
      {
         renderingEngines.at(i)->run();
      }
   }
};


}

