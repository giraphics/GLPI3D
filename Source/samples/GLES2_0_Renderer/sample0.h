#ifndef SAMPLE0_H
#define SAMPLE0_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "Common/HMIRectangle.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
#include "Plugin.h"
#include <list>
#define VERTEX_SHADER_PRG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleVertex.glsl"
#define FRAGMENT_SHADER_PRG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleFragment.glsl"

// Demo Description: In this demo, there are two rectangle are drawn.
// rectangleItem (Blue color)   : using GRectangle class from Graphics.
// rectangleItem2 (Green color) : using HMIRectangle class, this class overrides the render method and perform rotation.
// HUD scene in screen coordiante system.
void SimpleAndHMIRectangle(){
   float width  = 100.0f;
   float height = 100.0f;

   std::vector<glm::vec3> vertices;
   vertices.push_back(glm::vec3( 0.0f,  0.0f,   0.0f ));
   vertices.push_back(glm::vec3( width, 0.0f,   0.0f ));
   vertices.push_back(glm::vec3( 0.0f,  height,   0.0f ));
   vertices.push_back(glm::vec3( width,  height,   0.0f ));
   
   std::vector<glm::vec2> textureCoords;
   textureCoords.push_back(glm::vec2(0.0f, 1.0f));
   textureCoords.push_back(glm::vec2(1.0f, 1.0f));
   textureCoords.push_back(glm::vec2(0.0f, 0.0f));
   textureCoords.push_back(glm::vec2(1.0f, 0.0f));

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
	   renderer->setWindowTitle("Custom extentable Rectangle class and Generic implementation");
   }

   if (!renderer){
	   return;
   }

   hudScene   = new Scene("RectangleScene", renderer);  // Add to Renderer
   hudCamera  = new CameraHUD("hudCamera", hudScene);
   hudCamera->Viewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());
   
   unsigned int ProgramID = ProgramManager::GetInstance()->LoadShader("square", VERTEX_SHADER_PRG, FRAGMENT_SHADER_PRG )->ProgramID;
   rectangleItem = new GRectangle(hudScene, NULL, BUTTON, "GenericRectangle");
   rectangleItem->SetProgram(ProgramID);

   rectangleItem2 = new HMIRectangle(hudScene, rectangleItem, BUTTON, "HMIRectangle");
   rectangleItem2->SetParent(rectangleItem);
   rectangleItem2->SetProgram(ProgramID);

   // Set the vertex information
   rectangleItem->SetVertices(&vertices);
   rectangleItem2->SetVertices(&vertices);

   // Set the texture coordinate information
   rectangleItem->SetTexCoords(&textureCoords);
   rectangleItem2->SetTexCoords(&textureCoords);

   // Set Color information
   rectangleItem->SetColor(&glm::vec4(0.0, 0.0, 1.0, 1.0));
   rectangleItem2->SetColor(&glm::vec4(0.0, 1.0, 0.0,1.0));
   rectangleItem->SetCenter(glm::vec3(50, 50, 0.0));
   rectangleItem->Translate(renderer->getWindowWidth()/2.0f, renderer->getWindowHeight()/2.0f, 0.0);
   hudScene->addModel( rectangleItem );
   
   // Intialize the application
   application.Initialize();
   
   while(renderer->getWindow()->isOpen()){
      application.Render();
   }

   ProgramManager::DeleteInstance();
}


#endif
