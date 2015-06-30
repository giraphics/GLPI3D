#ifndef WINDMILL_H
#define WINDMILL_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "MeshObject.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
#include "Plugin.h"

#define VERTEX_SHADER_PRG_GOURAUD	(char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\GouraudVertex.glsl"
#define FRAGMENT_SHADER_GOURAUD		(char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\GouraudFragment.glsl"

void WindMillDemo(){
   Application application;

   // Create a clock for measuring the time elapsed
   Renderer* renderer	= NULL;
   MeshObject* meshObj	= NULL;
   Camera* camera		= NULL;
   Scene* scene			= NULL;
   Plugin* plugin		= NULL;
   
   plugin = application.loadPlugin(OPENGLES20_STATIC_PLUGIN);
   if(plugin){
	   renderer = plugin->createRenderer(2000, 1200);
	   //renderer = plugin->createRenderer(400, 300);
   }

   if (!renderer){
	   return;
   }

   scene = new Scene("MeshScene");
   Light light(Material(MaterialWhite), glm::vec4(0.0, 0.0, 10.0, 0.0));
   scene->addLight(&light);

   camera = new Camera("PerespectiveCameraMesh", scene);
   camera->SetPosition(glm::vec3(0.0, 5.0, 25.0));
   camera->SetTarget(glm::vec3(0.0, 0.0, 0.0));
   camera->Viewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());

   ProgramManager* ProgramManagerObj = ProgramManager::GetInstance();
   unsigned int ProgramID = ProgramManagerObj->LoadShader("Meshshader", VERTEX_SHADER_PRG_GOURAUD, FRAGMENT_SHADER_GOURAUD)->ProgramID;
   
	MeshObject* Base         = NULL;
	MeshObject* Stand        = NULL;
	MeshObject* MotorShaft   = NULL;
	MeshObject* CubePlane    = NULL;
	MeshObject* Sphere       = NULL;

	// Base
    Base =  new MeshObject("../Resource/Models/CubeWithNormal.obj", scene, NULL, MESH, "Base");
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
    scene->addModel( Base);

   renderer->addScene( scene );
    //Sphere->SetVisible(!true, true);
    //Sphere->SetVisible(true, !true);

   // Intialize the application
   application.Initialize();
static int i = 0;
   while(renderer->getWindow()->isOpen()){
   application.Resize(renderer->getWindowWidth()-i++, renderer->getWindowHeight()-i);
		Sphere->Rotate(1.0, 0.0, 0.0, 1.0);
		Base->Rotate(0.4, 0.0, 1.0, 0.0);

		application.Render();
   }

   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
}


#endif
