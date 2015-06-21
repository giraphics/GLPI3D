#include "GLPi3D__Renderer.h"
namespace GLPi3D
{

Scene* Scene::SceneObj = NULL;
   
Scene::Scene(void)
{
   RenderMemData.FrameCount   = 0;
   RenderMemData.FPS          = 0;
}

Scene::~Scene(void)
{
   delete SceneObj;
}

void Scene::initializeScene(::uint16 context, ::uint16 layer)
{
   setCurrentContext(context);
   setCurrentLayer(layer);
//   resize(width, height);
}


/*!
	Remove all the Model classes from the Scene.

	\param[in] None.

	\return None.
*/
void Scene::clearModels()
{
	for( unsigned int i=0; i<RenderMemData.models.size();  i++ ){
		delete RenderMemData.models.at(i);
	}
	RenderMemData.models.clear();
}

/*!
	Setup of the view port and projection initialization. In projection initialization the Ortho or Perespective is set
	as per requirement.

	\param[in] width of the screen.
	\param[in] height of the screen.

	\return void.
*/
#include "GLPi3D__Transform.h"
void Scene::setUpProjection()
{
   Transform* TransformObj = &RenderMemData.TransformObj;
   TransformObj->Init();
   TransformObj->SetMatrixMode( PROJECTION_MATRIX );

   TransformObj->LoadIdentity();
   GLfloat aspectRatio = (GLfloat)RenderMemData.Camera.CamData.screenWidth / (GLfloat)RenderMemData.Camera.CamData.screenHeight;

   if(RenderMemData.Camera.CamData.isProjectionSystemOrtho){
      if ( RenderMemData.Camera.CamData.screenWidth <= RenderMemData.Camera.CamData.screenHeight ){
         TransformObj->Ortho( RenderMemData.Camera.CamData.left, RenderMemData.Camera.CamData.right, RenderMemData.Camera.CamData.bottom / aspectRatio, RenderMemData.Camera.CamData.top / aspectRatio, RenderMemData.Camera.CamData.clip_start, RenderMemData.Camera.CamData.clip_end );
         //TransformObj->Ortho( -span, span, -span / aspectRatio, span / aspectRatio, -span, span);
      }
      else{
         TransformObj->Ortho( RenderMemData.Camera.CamData.left * aspectRatio, RenderMemData.Camera.CamData.right * aspectRatio, RenderMemData.Camera.CamData.bottom, RenderMemData.Camera.CamData.top, RenderMemData.Camera.CamData.clip_start, RenderMemData.Camera.CamData.clip_end );
         //TransformObj->Ortho( -span * aspectRatio, span * aspectRatio, -span, span, -span, span);
      }
   }
   else{
      TransformObj->SetPerspective(RenderMemData.Camera.CamData.fov, aspectRatio, RenderMemData.Camera.CamData.clip_start, RenderMemData.Camera.CamData.clip_end);
   }

   TransformObj->SetMatrixMode( VIEW_MATRIX );
   TransformObj->LoadIdentity();
   TransformObj->LookAt(&RenderMemData.Camera.CamData.WorldCameraPosition, &RenderMemData.Camera.CamData.WorldTargetPosition,&RenderMemData.Camera.CamData.WorldUp);

   TransformObj->SetMatrixMode( MODEL_MATRIX );
   TransformObj->LoadIdentity();
}

/*!
	Setup of the view port and projection initialization. In projection initialization the Ortho or Perespective is set
	as per requirement.

	\param[in] width of the screen.
	\param[in] height of the screen.

	\return void.
*/
void Scene::cameraOrtho(float left, float right, float bottom, float top, float clip_start, float clip_end)
{
   RenderMemData.Camera.cameraOrtho( left, right, bottom, top, clip_start, clip_end);
}

/*!
	Setup of the view port and projection initialization. In projection initialization the Ortho or Perespective is set
	as per requirement.

	\param[in] width of the screen.
	\param[in] height of the screen.

	\return void.
*/
void Scene::cameraPrespective(float fovy, float clip_start, float clip_end)
{
   RenderMemData.Camera.cameraPrespective(fovy, clip_start, clip_end);
}


/*!
	Setup of the view port and projection initialization. In projection initialization the Ortho or Perespective is set
	as per requirement.

	\param[in] width of the screen.
	\param[in] height of the screen.

	\return void.
*/
void Scene::resize(int w, int h)
{
   // 17/Dec-2014: Depreciated please see camera Camera::cameraViewport
//   static float xs = 0.0;
//   static float ys = 0.0;
//	RenderMemData.screenWidth		= xs;
//	RenderMemData.screenHeight		= ys;
//
////	glViewport( 0, 0, xs+=02, ys+=2 );
//	glViewport( 0, 0, xs+=02, ys+=2 );
}

/*!
	This returns the model Scene of the model. A Model Scene is basically a class responsible for managing the same type of models.

	\param[in] type Enumeration of the model type want to query.

	\return Model pointer.
*/
Model* Scene::getModel( ModelType type)
{
	for( unsigned int i=0; i<RenderMemData.models.size();  i++ )
	{
		Model* model = RenderMemData.models.at(i);

		if ( model->GetModelType() == type )
			return model;
	}

	return NULL;
}

/*!
	This function is responsible for adding a model in the Scene model vector.
	The Scene handle all the models from a unique vector based list.

	\param[in] Model pointer of the Model user want to add.

	\return None.
*/
void Scene::addModel(Model* model)
{
   // Add only if no duplicate exists
   if(std::find(RenderMemData.models.begin(),RenderMemData.models.end(), model) == RenderMemData.models.end()){
      RenderMemData.models.push_back( model );
      model->SetScene(this);
      model->InitModel();
   }
}

/*!
	This deletes a model from the system.
	\param[in] type Enumeration of the model type want to delete.
	\return Model pointer.
*/
void Scene::deleteModel( ModelType type)
{
	for( unsigned int i=0; i<RenderMemData.models.size();  i++ )
	{
		Model* model = RenderMemData.models.at(i);

      if ( model->GetModelType() == type ){
			delete model;
         model = NULL;
      }
	}
}


/*!
	Performs initializing of all the models this may include setup for shaders their caching.
	rendering for each model, creation and caching of IBO, VBO, FBO etc.

	\param[in] void.

	\return void.
*/
void Scene::initializeModels()
{
	for( unsigned int i=0; i<RenderMemData.models.size();  i++ )
		RenderMemData.models.at(i)->InitModel();
}

/*!
	Performs rendering for each model

	\param[in] void.

	\return void.
*/
void Scene::render()
{
   glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
   glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

   for( unsigned int i=0; i<RenderMemData.models.size();  i++ ){
      RenderMemData.models.at(i)->SetGLStates();
      RenderMemData.models.at(i)->Render();
   }
}

void render(Model* model)
{
   // Render using parent child relationship
}

//void Scene::run()
//{
//   setUpProjection();
//   render();
//}

/*!
	GRLC current layer

	\param[in] void.

	\return Layer ID.
*/
uint16 Scene::currentLayer()
{
   return RenderMemData.layerId;
}

void Scene::setCurrentLayer(uint16 layer)
{
   RenderMemData.layerId = layer;
}

/*!
	GRLC current context identifier

	\param[in] void.

	\return context ID.
*/
uint16 Scene::currentContext()
{
   return RenderMemData.contextId;
}

void Scene::setCurrentContext(uint16 context)
{
   RenderMemData.contextId = context;
}
}

