#ifndef FLAT_SMOOTH_SHADING_H
#define FLAT_SMOOTH_SHADING_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "MeshObject.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
#include "Plugin.h"

#define VERTEX_SHADER_PRG_PHONG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\PhongVertex.glsl"
#define FRAGMENT_SHADER_PHONG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\PhongFragment.glsl"

// The custom scene allow to simply the scene management 
// by moving it to the seperate module.
class SceneFlatSmooth : public Scene
{
    
public:
    //! Default Constructor
    SceneFlatSmooth(std::string name = "", ObjectRelative* parentObj = NULL);

    //! Destructor
    virtual ~SceneFlatSmooth(void);
    
	//! Initialize the Custom Scene
    void Initialize();

	//! Handle Render
    void Render();
    
	//! Handle Resize
	void Resize( int w, int h );
    
private:
    /**
     Actual camera view.
     */
    Camera* camera;
};

SceneFlatSmooth::SceneFlatSmooth(std::string name, ObjectRelative* parentObj):Scene(name, parentObj)
{
    camera = NULL;
}

void SceneFlatSmooth::Initialize()
{
   camera = new Camera("PerespectiveCameraMesh", this);
   camera->SetPosition(glm::vec3(0.0, 0.0, 4.5));
   camera->SetTarget(glm::vec3(0.0, 0.0, 0.0));

    this->addCamera(camera);
    Scene::Initialize();
}

SceneFlatSmooth::~SceneFlatSmooth(void)
{
}

void SceneFlatSmooth::Resize( int w, int h )
{
    camera->Viewport(0, 0, w, h);
    Scene::Resize(w, h);
}

/*!
	Performs rendering for each model
 
	\param[in] void.
 
	\return void.
 */
void SceneFlatSmooth::Render()
{
    Scene::Render();
}

class FlatSmoothShade : public Model{

public:
   FlatSmoothShade(Scene* parent, Model* model, ModelType type, std::string objectName = "");

   virtual ~FlatSmoothShade();

   // Initialize our Model class
   void Initialize();

   // Render the Model class
   void Render(bool (*customRender)()=NULL);

private:
	MeshObject*			flatSphere;
	MeshObject*			smoothSphere;
	ProgramManager*		ProgramManagerObj;
};

void FlatSmoothShade::Initialize()
{
	Model::Initialize();
}

void FlatSmoothShade::Render(bool (*customRender)())
{
	flatSphere->Rotate(0.4, 0.0, 1.0, 0.0);
	smoothSphere->Rotate(0.4, 0.0, 1.0, 0.0);
	Model::Render();
}

FlatSmoothShade::~FlatSmoothShade()
{
   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
}

FlatSmoothShade::FlatSmoothShade(Scene* scene, Model* model, ModelType type, std::string objectName):Model(scene, model, type, objectName)
{
   ProgramManager* ProgramManagerObj = ProgramManager::GetInstance();
   unsigned int ProgramID = ProgramManagerObj->LoadShader("Meshshader", VERTEX_SHADER_PRG_PHONG, FRAGMENT_SHADER_PHONG)->ProgramID;

	// Flat sphere demo
    flatSphere =  new MeshObject("../Resource/Models/SphereNoNormal.obj", scene, this, MESH, true, "FlatSphere");
	flatSphere->SetProgram(ProgramID);
	flatSphere->Translate(-1.3, 0.0, 0.0);
    flatSphere->SetMaterial(Material(MaterialGold));

	Material redMaterial(PolishedGoldAmbient, PolishedGoldDiffuse, PolishedGoldSpecular, PolishedGoldShiness);
    // Smooth sphere demo
    smoothSphere =  new MeshObject("../Resource/Models/SphereNoNormal.obj", scene, this, MESH, false, "SmoothSphere" );
	smoothSphere->SetProgram(ProgramID);
    smoothSphere->SetMaterial(Material(MaterialGold));
    smoothSphere->Translate(1.3, 0.0, 0.0);
}

void FlatAndSmoothShadingDemo(){
   Application application;

   // Create a clock for measuring the time elapsed
   Renderer* renderer	= NULL;
   Camera* camera		= NULL;
   Scene* scene			= NULL;
   Plugin* plugin		= NULL;
   
   plugin = application.loadPlugin(OPENGLES20_STATIC_PLUGIN);
   if(plugin){
	   renderer = plugin->createRenderer(2000, 1200);
   }

   if (!renderer){
	   return;
   }

   scene = new SceneFlatSmooth("MeshScene", renderer);

   Light light(Material(MaterialWhite), glm::vec4(10.0, 10.0, 10.0, 0.0));
   scene->addLight(&light);

   FlatSmoothShade FlatSmoothShadeObject( scene, NULL, MESH, "FlatSmoothShadeObj");
   scene->addModel( &FlatSmoothShadeObject);

   // Intialize the application
   application.Initialize();
   application.Resize(renderer->getWindowWidth(), renderer->getWindowHeight());
   while(renderer->getWindow()->isOpen()){
		application.Render();
   }
}


#endif
