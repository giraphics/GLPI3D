#ifndef DRAWSCHEME_H
#define DRAWSCHEME_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "Common/HMIRectangle.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
#include "Plugin.h"
#include <list>
#define VERTEX_SHADER_PRG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleVertex.glsl"
#define FRAGMENT_SHADER_PRG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleFragment.glsl"

// Demo Description: In this demo, we will draw two rectangle with Array and Element (Indices) drawing scheme.
void DrawScheme(){
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

   //unsigned short indices[] = {0, 1, 2, 1, 3, 2}; //
   unsigned short indices[] = {0, 1, 2, 3}; // GL_TRIANGLE_STRIP
   std::vector<unsigned short> indicesList( indices, indices + sizeof(indices)/sizeof(unsigned short) );

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
	   renderer->setWindowTitle("Draw Scheme: Yellow Rect: Draw by element, Sky Blue Rect: Draw by array");
   }

   if (!renderer){
	   return;
   }

   hudScene   = new Scene("RectangleScene", renderer); // Add to Renderer
   hudCamera  = new CameraHUD("hudCamera", hudScene);
   hudCamera->Viewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());
   
   unsigned int ProgramID = ProgramManager::GetInstance()->LoadShader("square", VERTEX_SHADER_PRG, FRAGMENT_SHADER_PRG )->ProgramID;
   rectangleItem = new GRectangle(hudScene, NULL, BUTTON, "RectDrawElement", BUFFER_VA, DRAW_ELEMENT);
   rectangleItem->SetProgram(ProgramID);

   rectangleItem2 = new GRectangle(hudScene, NULL, BUTTON,"RectDrawArray", BUFFER_VA, DRAW_ARRAY);
   rectangleItem2->SetProgram(ProgramID);

   // Set the vertex information
   rectangleItem->SetVertices(&vertices);
   rectangleItem2->SetVertices(&vertices);

   // Set the texture coordinate information
   rectangleItem->SetTexCoords(&textureCoords);
   rectangleItem2->SetTexCoords(&textureCoords);

   // Set Indice information
   rectangleItem->SetIndices(&indicesList);

   // Set Color information
   rectangleItem->SetColor(&glm::vec4(0.0, 1.0, 1.0, 1.0));
   rectangleItem2->SetColor(&glm::vec4(1.0, 1.0, 0.0,1.0));
   rectangleItem->SetCenter(glm::vec3(50, 50, 0.0));
   rectangleItem2->Translate(renderer->getWindowWidth()/2.0f, renderer->getWindowHeight()/2.0f, 0.0);

   hudScene->addModel( rectangleItem );
   hudScene->addModel( rectangleItem2 );
   
   // Intialize the application
   application.Initialize();
   
   while(renderer->getWindow()->isOpen()){
      application.Render();
   }

   ProgramManager::DeleteInstance();
}


#endif
