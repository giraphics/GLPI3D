#ifndef SAMPLE1_H
#define SAMPLE1_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "Common/HMIRectangle.h"
#include "gles2_0_renderer/scene_graph/Camera.h"

#define VERTEX_SHADER_PRG1        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleVertex.glsl"
#define FRAGMENT_SHADER_PRG1      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleFragment.glsl"

void Sample1(){
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
   Renderer*     renderer    = NULL;
   GRectangle* rectangleItem = NULL;
   CameraHUD*    hudCamera   = NULL;
   Scene*        hudScene    = NULL;               // HUD Scene
   Plugin* plugin			 = NULL;
   
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
   unsigned int ProgramID = ProgramManagerObj->LoadShader("square", VERTEX_SHADER_PRG1, FRAGMENT_SHADER_PRG1 )->ProgramID;
   GRectangle* parent = NULL;
   float ROWS = renderer->getWindowWidth()/width;
   float COLS = renderer->getWindowHeight()/height;
   std::vector<GRectangle*> recthandles;
   for(float i = 0; i<COLS; i++)
   {
      for(float j = 0; j<ROWS; j++)
      {
         rectangleItem = new HMIRectangle(hudScene, NULL, BUTTON,"");
		 rectangleItem->SetProgram(ProgramID);
         rectangleItem->SetName(std::string("My Rectangle 1"));
         recthandles.push_back(rectangleItem);

		 // Set the vertex information
         rectangleItem->SetVertices(tempVtx);
		 rectangleItem->SetTexCoords(texCoords);
         rectangleItem->Translate(width*j, height*i, 0);
         rectangleItem->SetCenter(glm::vec3(width/2, height/2, 0.0));

		 // Set Color information
         rectangleItem->SetColor(&glm::vec4(0.0, j/(float)ROWS, i/(float)COLS, 1.0));
         hudScene->addModel( rectangleItem );
         if (i == 0){
            //parent = rectangleItem;
            //rectangleItem->Translate(150+60*i, 250, 0);
         }
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

   while(renderer->getWindow()->isOpen()){
      if(k>recthandles.size()-1){
         k = 0;
      }
      application.Render();
      k++;
   }

   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
}


#endif
