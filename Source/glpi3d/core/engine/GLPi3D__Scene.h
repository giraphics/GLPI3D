#pragma once
#include "GLPi3D__Model.h"
#include "GLPi3D__Programmanager.h"
#include "GLPi3D__Transform.h"
#include "GLPi3D__Camera.h"
//#include "Profile/GLPi3D__Profiler.h"
#include <vector>

namespace GLPi3D
{

//! Render Member Data structure
typedef struct
{
   //! Program handler for render
   ProgramManager	ProgramManagerObj;

   //! Tranform operation handler
   Transform	TransformObj;

   //! Camera handler
   Camera Camera;

   //! Profiler object
//   Profiler ProfileObj;

   //! Model List
   std::vector <Model*> models;

   //! Context Identifier
   uint16 contextId;

   //! Layer Identifier
   uint16 layerId;

   //! projection system Identifier
   uint32 FrameCount;

   //! projection system Identifier
   uint32 FPS;

} tstSceneData;

class Scene
{

public:
   //! Default Constructor
   Scene(void);

   //! Destructor
   ~Scene(void);

   //! Initialize Scene
   void initializeScene(::uint16 context, ::uint16 layer);

   //! Return the ProgramManager handler
   inline ProgramManager* ProgramManager(){ return &RenderMemData.ProgramManagerObj; }

   //! Return the Transform handler
   inline Transform*  Transformation() { return &RenderMemData.TransformObj;  }

   //! Return the Camera handler
   inline Camera*  Camera() { return &RenderMemData.Camera;  }

   //! Get profiler object
//   Profiler* Profiler(){ return &RenderMemData.ProfileObj;}

   //! Screen Width returned
   inline int screenWidthPixel() { return Camera()->CamData.screenWidth; }

   //! Screen Height returned
   inline int screenHeightPixel() { return Camera()->CamData.screenHeight; }

   //! Render the Models
   void render(Model* model);

   //! Render the Models
   void render();

   //! Run the Models
   //void run();

   //! Initializes the Models
   void initializeModels();

   //! Remove all the models
   void clearModels();

   //! Set Project System
   void setUpProjection();

   //! Set Orthographics Project System
   void cameraOrtho(float left, float right, float bottom, float top, float clip_start, float clip_end);

   //! Set Prespective Projection System
   void cameraPrespective(float fovy, float clip_start, float clip_end);

   //! Initializes the Models
   void setUpModels();

   //! Get the model
   Model* getModel( ModelType );

   //! Cache the model
   void addModel( Model* );

   // Delete the model
   void deleteModel( ModelType );

   //! resize screen
   void resize( int w, int h );

   //! Get currect layer
   uint16 currentLayer();

   //! Get currect context
   uint16 currentContext();

   //! Set currect layer
   void setCurrentLayer(uint16 layer);

   //! Set currect context
   void setCurrentContext(uint16 context);

   //! Get Scene singleton instance object
   static Scene* Instance()
   {
      if ( !SceneObj ){
         SceneObj = new Scene();
      }
      return SceneObj;
   }

   //static Scene* dynamicObj()
   //{
   //   SceneObj = new Scene();
   //   return SceneObj;
   //}
   //! Get currect layer
   uint32& fps(){ return RenderMemData.FPS; }

   //! Get currect context
   uint32& frameCount(){ return RenderMemData.FrameCount; }

private:
   //! Map render member variable container
   tstSceneData RenderMemData;
   static Scene* SceneObj;
};

}

