#ifndef SAMPLE1_H
#define SAMPLE1_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "Common/HMIRectangle.h"
#include "gles2_0_renderer/scene_graph/Camera.h"

#define VERTEX_SHADER_PRG1        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleVertex.glsl"
#define FRAGMENT_SHADER_PRG1      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleFragment.glsl"

// Demo Description: Use's HUD projection system
// Fill the screen with interpolated colored triangles.
// Each rect is set with new center to rotate about (50, 50) translation axises.
void FilledWindow(){
   float width  = 35.0f;
   float height = 35.0f;

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
   Renderer*     renderer    = NULL;
   GRectangle* rectangleItem = NULL;
   CameraHUD*    hudCamera   = NULL;
   Scene*        hudScene    = NULL;               // HUD Scene
   Plugin* plugin			 = NULL;
   
   plugin = application.loadPlugin(OPENGLES20_STATIC_PLUGIN);
   if(plugin){
	   renderer = plugin->createRenderer();
	   renderer->setWindowTitle("Window filled with rectangles, interpolated colors, shared shader");
   }

   if (!renderer){
	   return;
   }

   hudScene   = new Scene("RectangleScene");
   hudCamera  = new CameraHUD("hudCamera", hudScene);
   hudCamera->Viewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());

   ProgramManager* ProgramManagerObj	= ProgramManager::GetInstance();
   unsigned int ProgramID = ProgramManagerObj->LoadShader("square", VERTEX_SHADER_PRG1, FRAGMENT_SHADER_PRG1 )->ProgramID;
   GRectangle* parent = NULL;
   float ROWS = renderer->getWindowWidth()/width;
   float COLS = renderer->getWindowHeight()/height;
   
   std::vector<GRectangle*> recthandles;
   for(float i = 0; i<COLS; i++)
   {
      for(float j = 0; j<ROWS; j++)
      {
         rectangleItem = new HMIRectangle(hudScene, NULL, BUTTON,"", BUFFER_VAO);
		 rectangleItem->SetProgram(ProgramID);
         rectangleItem->SetName(std::string("My Rectangle 1"));
         recthandles.push_back(rectangleItem);

		   // Set the vertex information
		   rectangleItem->SetVertices(&vertices);

		   // Set the texture coordinate information
		   rectangleItem->SetTexCoords(&textureCoords);

         rectangleItem->Translate(width*j, height*i, 0);
         rectangleItem->SetCenter(glm::vec3(width/2, height/2, 0.0));

		 // Set Color information
         rectangleItem->SetColor(&glm::vec4(0.0, j/(float)ROWS, i/(float)COLS, 1.0));
         hudScene->addModel( rectangleItem );
      }
   }

   rectangleItem = parent;
   renderer->addScene( hudScene );

   // Intialize the application
   application.Initialize();

   printf("\nTotal Number of Rectangle: %f", ROWS*COLS);
   printf("\nTotal Number of Vertices: %f", ROWS*COLS*4);
   printf("\nTotal Memory in MB: %f", ROWS*COLS*4*3*(sizeof(float))/(1024*1024));

   long long k=0;
   int ran = rand()%255;
   glm::vec4 color = glm::vec4(rand()%255/255.0f,rand()%255/255.0f,rand()%255/255.0f, 1.0);
   clock_t last;

   while(renderer->getWindow()->isOpen()){
      if(k>recthandles.size()-1){
         k = 0;
      }
	  last = clock();
      application.Render();
	  printf("\n%d",clock() - last);
      k++;
   }

   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
}


#endif
