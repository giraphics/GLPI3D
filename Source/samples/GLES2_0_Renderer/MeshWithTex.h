#ifndef SIMPLEMESH1_H
#define SIMPLEMESH1_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "MeshObject.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
#include "Plugin.h"

//#define VERTEX_SHADER_PRG_DIFFUSE        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\DiffuseVertex.glsl"
//#define FRAGMENT_SHADER_DIFFUSE      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\DiffuseFragment.glsl"
#define VERTEX_SHADER_PRG_TEX        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\MeshTexVertex.glsl"
#define FRAGMENT_SHADER_TEX      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\MeshTexFragment.glsl"

void MeshSample(){
   float width  = 100.0f;
   float height = 100.0f;
   glm::vec3 tempVtx[4] = {
      glm::vec3( 0.0f,  0.0f,   0.0f ),
      glm::vec3( width, 0.0f,   0.0f ),
      glm::vec3( 0.0f,  height, 0.0f ),
      glm::vec3( width, height, 0.0f )
   };

   glm::vec2 texCoords[4] = {
      glm::vec2(0.0f, 1.0f),
      glm::vec2(1.0f, 1.0f),
      glm::vec2(0.0f, 0.0f),
      glm::vec2(1.0f, 0.0f),
   };

   Application application;

   // Create a clock for measuring the time elapsed
   Renderer* renderer	= NULL;
   MeshObject* meshObj	= NULL;
   Camera* camera		= NULL;
   Scene* scene			= NULL;
   Plugin* plugin		= NULL;
   
   plugin = application.loadPlugin(OPENGLES20_STATIC_PLUGIN);
   if(plugin){
	   renderer = plugin->createRenderer();
   }

   if (!renderer){
	   return;
   }

   scene = new Scene("MeshScene");

   camera = new Camera("PerespectiveCameraMesh", scene);
   camera->SetPosition(glm::vec3(0.0, 5.0, 10.1));
   camera->SetTarget(glm::vec3(0.0, 0.0, 0.0));
   camera->Viewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());

   ProgramManager* ProgramManagerObj = ProgramManager::GetInstance();
   unsigned int ProgramID = ProgramManagerObj->LoadShader("Meshshader", VERTEX_SHADER_PRG_TEX, FRAGMENT_SHADER_TEX)->ProgramID;
   meshObj = new MeshObject("../Resource/Models/mbclass.obj", scene, NULL, MESH, "");
   meshObj->SetName(std::string("My Mesh 41"));
   meshObj->SetProgram(ProgramID);

   meshObj->Scale(.01, .01, .01);
   scene->addModel(meshObj);
   renderer->addScene( scene );
   
   // Intialize the application
   application.Initialize();
   meshObj->Rotate(-90.0, 1.0, 0.0, 0.0);

   while(renderer->getWindow()->isOpen()){
	  meshObj->Rotate(1.0, 0.0, 0.0, 1.0);
      application.Render();
   }

   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
}

#endif
