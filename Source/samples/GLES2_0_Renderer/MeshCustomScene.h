#ifndef MESHCUSTOMSCENE_H
#define MESHCUSTOMSCENE_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "MeshObject.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
#include "Plugin.h"

#define VERTEX_SHADER_PRG_PHONG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\PhongVertex.glsl"
#define FRAGMENT_SHADER_PHONG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\PhongFragment.glsl"

// The custom scene allow to simply the scene management 
// by moving it to the seperate module.
class CustomScene : public Scene
{
    
public:
    //! Default Constructor
    CustomScene(std::string name = "", ObjectRelative* parentObj = NULL);

    //! Destructor
    virtual ~CustomScene(void);
    
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

CustomScene::CustomScene(std::string name, ObjectRelative* parentObj):Scene(name, parentObj)
{
    camera = NULL;
}

void CustomScene::Initialize()
{
   camera = new Camera("PerespectiveCameraMesh", this);
   camera->SetPosition(glm::vec3(0.0, 5.0, 25.0));
   camera->SetTarget(glm::vec3(0.0, 0.0, 0.0));

    this->addCamera(camera);
    Scene::Initialize();
}

CustomScene::~CustomScene(void)
{
}

void CustomScene::Resize( int w, int h )
{
    camera->Viewport(0, 0, w, h);
    Scene::Resize(w, h);
}

/*!
	Performs rendering for each model
 
	\param[in] void.
 
	\return void.
 */
void CustomScene::Render()
{
    Scene::Render();
}

class WindMill : public Model{

public:
   WindMill(Scene* parent, Model* model, ModelType type, std::string objectName = "");

   virtual ~WindMill();

   // Initialize our Model class
   void Initialize();

   // Render the Model class
   void Render(bool (*customRender)()=NULL);

private:
	MeshObject* Base;
	MeshObject* Stand;
	MeshObject* MotorShaft;
	MeshObject* CubePlane;
	MeshObject* Sphere;
	ProgramManager* ProgramManagerObj;
};

void WindMill::Initialize()
{
	Model::Initialize();
}

void WindMill::Render(bool (*customRender)())
{
	Sphere->Rotate(1.0, 0.0, 0.0, 1.0);
	Base->Rotate(0.4, 0.0, 1.0, 0.0);
	Model::Render();
}

WindMill::~WindMill()
{
   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
}

WindMill::WindMill(Scene* scene, Model* model, ModelType type, std::string objectName):Model(scene, model, type, objectName)
{
   ProgramManager* ProgramManagerObj = ProgramManager::GetInstance();
   unsigned int ProgramID = ProgramManagerObj->LoadShader("Meshshader", VERTEX_SHADER_PRG_PHONG, FRAGMENT_SHADER_PHONG)->ProgramID;

	// Base
    Base =  new MeshObject("../Resource/Models/CubeWithNormal.obj", scene, this, MESH, "Base");
	Base->SetProgram(ProgramID);
	Base->Translate(0.0, -4.0, 0.0);
    Base->SetMaterial(Material(MaterialSilver));
    Base->ScaleLocal(1.5, 0.25, 1.5);

    // Stand
    Stand =  new MeshObject("../Resource/Models/SemiHollowCylinder.obj", scene, Base, MESH, "Stand" );
	Stand->SetProgram(ProgramID);
    Stand->SetMaterial(Material(MaterialSilver));
    Stand->Translate(0.0, 4.0, 0.0);
    Stand->ScaleLocal(0.5, 4.0, 0.5);

    // Motor Shaft
    MotorShaft =  new MeshObject( "../Resource/Models/CubeWithNormal.obj", scene, Stand, MESH, "MotorShaft" );
	MotorShaft->SetProgram(ProgramID);
	MotorShaft->SetMaterial(Material(MaterialSilver));
    MotorShaft->Translate(0.0, 4.0, 1.0);
    MotorShaft->ScaleLocal(0.5, 0.5, 2.0);

    // Motor Engine
    Sphere =  new MeshObject	( "../Resource/Models/Sphere.obj", scene, MotorShaft, MESH, "Sphere" );
	Sphere->SetProgram(ProgramID);
    Sphere->SetMaterial(Material(MaterialGold));
    Sphere->Translate(0.0, 0.0, 2.0);
	
	// Fan Blades
    for(int i=0; i<360; i+=360/18){
        CubePlane =  new MeshObject( "../Resource/Models/CubeWithNormal.obj", scene, Sphere, MESH, "FanBlade" );
		CubePlane->SetProgram(ProgramID);
        CubePlane->SetMaterial(Material(MaterialCopper));
        CubePlane->Translate(0.0, 2.0, 0.0);
        CubePlane->SetCenter(glm::vec3(0.0, -2.0, 0.0));
        CubePlane->ScaleLocal(0.20, 2.0, 0.20);
        CubePlane->Rotate(i, 0.0, 0.0, 1.0);
    }
}

void CustomSceneDemo(){
   Application application;

   // Create a clock for measuring the time elapsed
   Renderer* renderer	= NULL;
   Camera* camera		= NULL;
   Scene* scene			= NULL;
   Plugin* plugin		= NULL;
   
   plugin = application.loadPlugin(OPENGLES20_STATIC_PLUGIN);
   if(plugin){
	   renderer = plugin->createRenderer(1280, 720);
   }

   if (!renderer){
	   return;
   }

   scene = new CustomScene("MeshScene");

   Light light(Material(MaterialWhite), glm::vec4(0.0, 0.0, 10.0, 0.0));
   scene->addLight(&light);

   WindMill windMillObject( scene, NULL, MESH, "WindMillObj");
   scene->addModel( &windMillObject);

   renderer->addScene( scene );

   // Intialize the application
   application.Initialize();
   application.Resize(renderer->getWindowWidth(), renderer->getWindowHeight());
   while(renderer->getWindow()->isOpen()){
		application.Render();
   }
}


#endif
