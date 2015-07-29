#pragma once
#include "../common/Event.h"
#include "../common/ObjectRelative.h"
#include "../common/Renderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#define DEBUG 0
class Scene;

// The GLES20Render class implements the Gesturehere which my not be supported for others pipelines
class GLES20Renderer : public Renderer , public GestureEvent
{
    
public:
    //! Renderer Constructor
	GLES20Renderer(int width = 800, int height = 600);

    //! Renderer Destructor
    ~GLES20Renderer();

    //! Gesture Press Event
    void TouchEventDown( float a, float b );
    
    //! Gesture Move Event
    void TouchEventMove( float a, float b );
    
    //! Gesture Release Event
    void TouchEventRelease( float a, float b );

    virtual void SWInit(){}

    //! This method does the software deinitialization of the pipeline.
    virtual void SWDeInit(){}


    //! This method does the hardware initialization of the pipeline.
    virtual void HWInit(){}


    //! This method does the hardware deinitialization of the pipeline.
    virtual void HWDeInit(){}

};
