#ifndef SAMPLE0_H
#define SAMPLE0_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "Common/HMIRectangle.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
#include "Plugin.h"

#define VERTEX_SHADER_PRG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleVertex.glsl"
#define FRAGMENT_SHADER_PRG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleFragment.glsl"

void Sample0(){
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
   Renderer* renderer			= NULL;
   GRectangle* rectangleItem    = NULL;
   GRectangle* rectangleItem2   = NULL;
   CameraHUD* hudCamera			= NULL;
   Scene* hudScene				= NULL;
   Plugin* plugin				= NULL;
   
   plugin = application.loadPlugin(OPENGLES20_STATIC_PLUGIN);
   if(plugin){
	   renderer = plugin->createRenderer();
   }

   if (!renderer){
	   return;
   }

   hudScene   = new Scene("RectangleScene");
   hudCamera  = new CameraHUD("hudCamera", hudScene);
   hudCamera->Viewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());
   
   ProgramManager* ProgramManagerObj = NULL;
   ProgramManagerObj = ProgramManager::GetInstance();
   unsigned int ProgramID = ProgramManagerObj->LoadShader("square", VERTEX_SHADER_PRG, FRAGMENT_SHADER_PRG )->ProgramID;
   rectangleItem = new GRectangle(hudScene, NULL, BUTTON,"");
   rectangleItem->SetName(std::string("My Rectangle 1"));
   rectangleItem->SetProgram(ProgramID);

   rectangleItem2 = new HMIRectangle(hudScene, rectangleItem, BUTTON,"");
   rectangleItem2->SetName(std::string("My Rectangle 1"));
   rectangleItem2->SetParent(rectangleItem);
   rectangleItem2->SetProgram(ProgramID);

   // Set the vertex information
   rectangleItem->SetVertices(tempVtx);
   rectangleItem2->SetVertices(tempVtx);

   rectangleItem->SetTexCoords(texCoords);
   rectangleItem2->SetTexCoords(texCoords);

   // Set Color information
   rectangleItem->SetColor(&glm::vec4(0.0, 0.0, 1.0, 1.0));
   rectangleItem2->SetColor(&glm::vec4(0.0, 1.0, 0.0,1.0));
   rectangleItem->SetCenter(glm::vec3(50, 50, 0.0));
   rectangleItem->Translate(renderer->getWindowWidth()/2.0f, renderer->getWindowHeight()/2.0f, 0.0);
   hudScene->addModel( rectangleItem );
   renderer->addScene( hudScene );
   
   // Intialize the application
   application.Initialize();
   /////////////////////////////////
   while(renderer->getWindow()->isOpen()){
      application.Render();
   }
   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
}


#endif