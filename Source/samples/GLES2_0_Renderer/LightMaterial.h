#ifndef LIGHTMATERIALDEMO_H
#define LIGHTMATERIALDEMO_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "MeshObject.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
#include "Plugin.h"

#define VERTEX_SHADER_PRG_PHONG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\PhongVertex.glsl"
#define FRAGMENT_SHADER_PHONG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\PhongFragment.glsl"

// The custom scene allow to simply the scene management 
// by moving it to the seperate module.
class CustomSceneLM : public Scene
{
    
public:
    //! Default Constructor
    CustomSceneLM(std::string name = "", ObjectRelative* parentObj = NULL);

    //! Destructor
    virtual ~CustomSceneLM(void);
    
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

CustomSceneLM::CustomSceneLM(std::string name, ObjectRelative* parentObj):Scene(name, parentObj)
{
    camera = NULL;
}

void CustomSceneLM::Initialize()
{
   camera = new Camera("PerespectiveCameraMesh", this);
   camera->SetPosition(glm::vec3(0.0, 5.0, 10.0));
   camera->SetTarget(glm::vec3(0.0, 0.0, 0.0));

    this->addCamera(camera);
    Scene::Initialize();
}

CustomSceneLM::~CustomSceneLM(void)
{
}

void CustomSceneLM::Resize( int w, int h )
{
    camera->Viewport(0, 0, w, h);
    Scene::Resize(w, h);
}

/*!
	Performs rendering for each model
 
	\param[in] void.
 
	\return void.
 */
void CustomSceneLM::Render()
{
    Scene::Render();
}

class LMModel : public Model{

public:
   LMModel(Scene* parent, Model* model, ModelType type, std::string objectName = "");

   virtual ~LMModel();

   // Initialize our Model class
   void Initialize();

   // Render the Model class
   void Render(bool (*customRender)()=NULL);

   void SetMaterial(Material mat);
private:
	MeshObject* Car;
	ProgramManager* ProgramManagerObj;
};

void LMModel::SetMaterial(Material mat)
{
    for(int i =0; i<childList.size(); i++){
        dynamic_cast<Model*>(childList.at(i))->SetMaterial(mat);
    }
}

void LMModel::Initialize()
{
	Model::Initialize();
}

void LMModel::Render(bool (*customRender)())
{
	Car->Rotate(0.1, 0.0, 0.0, 1.0);
	Model::Render();
}

LMModel::~LMModel()
{
   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
}

LMModel::LMModel(Scene* scene, Model* model, ModelType type, std::string objectName):Model(scene, model, type, objectName)
{
   ProgramManager* ProgramManagerObj = ProgramManager::GetInstance();
   unsigned int ProgramID = ProgramManagerObj->LoadShader("Meshshader", VERTEX_SHADER_PRG_PHONG, FRAGMENT_SHADER_PHONG)->ProgramID;
   this->SetProgram(ProgramID);
	// Car
    Car =  new MeshObject("../Resource/Models/mbclass.obj", scene, this, MESH, "Mercedes Benz");
	Car->SetProgram(ProgramID);
    Car->Scale(.01, .01, .01);
    Car->Rotate(-90.0, 1.0, 0.0, 0.0);
    Car->SetMaterial(Material(MaterialPolishedGold));
}

void LighMaterailDemo(){
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

   scene = new CustomSceneLM("MeshScene", renderer);
   glm::vec4 lightPosition(0.0, -5.0, -10.0, 0.0);
   Light light(Material(MaterialWhite), lightPosition);
   scene->addLight(&light);

   // Add LM Model
   LMModel LMModelObject( scene, NULL, MESH, "LMModelObj");
   
   MeshObject lightBulb("../Resource/Models/Sphere.obj", scene, NULL, MESH, "lightBulb");
   lightBulb.SetProgram(LMModelObject.GetProgram());
   //lightBulb.TranslateLocal(0.0, 5.0, 0.0);
   lightBulb.SetMaterial(Material(MaterialPewter));
   lightBulb.ScaleLocal(0.5, 0.5, .5);

   scene->addModel( &LMModelObject);
   scene->addModel( &lightBulb);

   // Intialize the application
   application.Initialize();
   application.Resize(renderer->getWindowWidth(), renderer->getWindowHeight());
	int num_segments = 500;
	int radius =10;
   while(renderer->getWindow()->isOpen()){
      for(int i = 0; i < num_segments; i++){
		static int mat = 0;
		static clock_t start = clock();
		if(clock()-start > 3*CLOCKS_PER_SEC){
			start = clock();
			(mat %=6)++; //Plus one to avoid none type
			LMModelObject.SetMaterial(Material(MaterialType(mat)));
		}

         float theta = 2.0f * PI * float(i) / float(num_segments);//get the current angle

         lightPosition.x = radius * cosf(theta);//calculate the x component
         lightPosition.y = radius * sinf(theta);//calculate the y component
         scene->getLights().at(0)->position = lightPosition;
		 lightBulb.Translate(lightPosition.x, lightPosition.y, lightPosition.z);
		 application.Render();
		 lightBulb.Translate(-lightPosition.x, -lightPosition.y, -lightPosition.z);
	  }
   }
}


#endif
