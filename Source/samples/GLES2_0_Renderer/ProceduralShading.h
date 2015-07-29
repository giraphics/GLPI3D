#ifndef PROCEDURAL_SHADING_H
#define PROCEDURAL_SHADING_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "MeshObject.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
#include "Plugin.h"

#define VERTEX_SHADER_PRG_PHONG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\ProceduralVertex.glsl"
#define FRAGMENT_SHADER_PHONG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\ProceduralFragment.glsl"

// The custom scene allow to simply the scene management 
// by moving it to the seperate module.
class SceneProcedureShade : public Scene
{
    
public:
    //! Default Constructor
    SceneProcedureShade(std::string name = "", ObjectRelative* parentObj = NULL);

    //! Destructor
    virtual ~SceneProcedureShade(void);
    
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

SceneProcedureShade::SceneProcedureShade(std::string name, ObjectRelative* parentObj):Scene(name, parentObj)
{
    camera = NULL;
}

void SceneProcedureShade::Initialize()
{
   camera = new Camera("PerespectiveCameraMesh", this);
   camera->SetPosition(glm::vec3(0.0, 0.0, 4.5));
   camera->SetTarget(glm::vec3(0.0, 0.0, 0.0));
   camera->SetClearColor(glm::vec4(0.3, .05, 0.3, 1.0));
    this->addCamera(camera);
    Scene::Initialize();
}

SceneProcedureShade::~SceneProcedureShade(void)
{
}

void SceneProcedureShade::Resize( int w, int h )
{
    camera->Viewport(0, 0, w, h);
    Scene::Resize(w, h);
}

/*!
	Performs rendering for each model
 
	\param[in] void.
 
	\return void.
 */
void SceneProcedureShade::Render()
{
    Scene::Render();
}

class ProceduralShadeModel : public Model{

public:
   ProceduralShadeModel(Scene* parent, Model* model, ModelType type, std::string objectName = "");

   virtual ~ProceduralShadeModel();

   // Initialize our Model class
   void Initialize();

   // Render the Model class
   void Render(bool (*customRender)()=NULL);

private:
	Uniform1f*			TimeUniform;
	MeshObject*			flatSphere;
	MeshObject*			smoothSphere;
	ProgramManager*		ProgramManagerObj;
};

void ProceduralShadeModel::Initialize()
{
	Model::Initialize();
}

void ProceduralShadeModel::Render(bool (*customRender)())
{
	flatSphere->Rotate(0.4, 0.0, 1.0, 0.0);
	smoothSphere->Rotate(0.4, 0.0, 1.0, 0.0);
	static GLfloat time = 0;
	time += 0.01;
	TimeUniform->SetValue(&time);
	Model::Render();
}

ProceduralShadeModel::~ProceduralShadeModel()
{
   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
}

ProceduralShadeModel::ProceduralShadeModel(Scene* scene, Model* model, ModelType type, std::string objectName):Model(scene, model, type, objectName)
{
   ProgramManager* ProgramManagerObj = ProgramManager::GetInstance();
   unsigned int ProgramID = ProgramManagerObj->LoadShader("Meshshader", VERTEX_SHADER_PRG_PHONG, FRAGMENT_SHADER_PHONG)->ProgramID;

	// Flat sphere demo
    flatSphere =  new MeshObject("../Resource/Models/SDMonkey.obj", scene, this, MESH, true, "FlatSphere");
	flatSphere->SetProgram(ProgramID);
	flatSphere->Translate(-1.3, 0.0, 0.0);
    flatSphere->SetMaterial(Material(MaterialGold));
	
	flatSphere->GeometryBuffer()->addUniform(TimeUniform = new Uniform1f("Time"));

	Material redMaterial(PolishedGoldAmbient, PolishedGoldDiffuse, PolishedGoldSpecular, PolishedGoldShiness);
    // Smooth sphere demo
    smoothSphere =  new MeshObject("../Resource/Models/SDMonkey.obj", scene, this, MESH, false, "SmoothSphere" );
	smoothSphere->SetProgram(ProgramID);
    smoothSphere->SetMaterial(Material(MaterialGold));
    smoothSphere->Translate(1.3, 0.0, 0.0);
}

void ProceduralShadingDemo(){
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

   scene = new SceneProcedureShade("ProceduralShadingScene", renderer);

   Light light(Material(MaterialWhite), glm::vec4(10.0, 10.0, 10.0, 0.0));
   scene->addLight(&light);

   ProceduralShadeModel ProceduralShadeModelObject( scene, NULL, MESH, "ProceduralShadeModelObj");
   scene->addModel( &ProceduralShadeModelObject);

   // Intialize the application
   application.Initialize();
   application.Resize(renderer->getWindowWidth(), renderer->getWindowHeight());
   while(renderer->getWindow()->isOpen()){
		application.Render();
   }
}


#endif
