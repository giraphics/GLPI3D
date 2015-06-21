#ifndef SAMPLE3_H
#define SAMPLE3_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "Common/HMIRectangle.h"
#include "gles2_0_renderer/scene_graph/Camera.h"

#define VERTEX_SHADER_PRG3        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleVertex.glsl"
#define FRAGMENT_SHADER_PRG3      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleFragment.glsl"

void SampleParentChild(){
   float width  = 1.0f;
   float height = 1.0f;
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

   Renderer* renderer         = NULL;
   GRectangle* rectangleItem  = NULL;
   Camera* camera             = NULL;
   Scene* scene               = NULL;
   Plugin* plugin			  = NULL;
   
   plugin = application.loadPlugin(OPENGLES20_STATIC_PLUGIN);
   if(plugin){
	   renderer = plugin->createRenderer();
   }

   if (!renderer){
	   return;
   }

   scene    = new Scene("RectangleScene");

   camera   = new Camera("PerespectiveCamera", scene);
   camera->SetPosition(glm::vec3 (0.0, 0.0, 50.0));
   camera->SetTarget(glm::vec3 (0.0, 0.0,0.0));
   camera->Viewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());

   GRectangle* parent = NULL;
   float ROWS = renderer->getWindowWidth()/width;
   float COLS = renderer->getWindowHeight()/height;

   // Because this is perspective
   ROWS = ROWS/100;
   COLS = COLS/100;

   ProgramManager* ProgramManagerObj = ProgramManager::GetInstance();
   unsigned int ProgramID = ProgramManagerObj->LoadShader("square", VERTEX_SHADER_PRG3, FRAGMENT_SHADER_PRG3 )->ProgramID;

   std::vector<GRectangle*> recthandles;
   for(float i = 0; i<COLS; i++)
   {
      for(float j = 0; j<ROWS; j++)
      {
         rectangleItem = new HMIRectangle(scene, parent, BUTTON,"");
		 rectangleItem->SetProgram(ProgramID);
         rectangleItem->SetName(std::string("My Rectangle 1"));
         recthandles.push_back(rectangleItem);

         // Set the vertex information
         rectangleItem->SetVertices(tempVtx);
		 rectangleItem->SetTexCoords(texCoords);
         rectangleItem->Translate(width*j, height*i, 0);
         rectangleItem->SetCenter(glm::vec3(width/2, height/2, 0.0));

         // Set Color information
         rectangleItem->SetColor(&glm::vec4(1.0, j/(float)ROWS, i/(float)COLS, 1.0));
         scene->addModel( rectangleItem );
         if (i == 0 && j==0){
            parent = rectangleItem;
         }
      }
   }

   static float xd = 0.01f;
   rectangleItem = parent;

   renderer->addScene( scene );
   // Intialize the application
   application.Initialize();

   printf("\nTotal Number of Rectangle: %f", ROWS*COLS);
   printf("\nTotal Number of Vertices: %f", ROWS*COLS*4);
   printf("\nTotal Memory in MB: %f", ROWS*COLS*4*3*(sizeof(float))/(1024*1024));

   float delta = 0.01f;
   while(renderer->getWindow()->isOpen()){
      if(parent){
         if(xd > 0.5){
            delta = -0.01f;
         }
         else if(xd < -0.5){
            delta = 0.01f;
         }
         parent->Translate(xd=xd+delta, 0.0, 0.0);
      }
      application.Render();
   }

   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;

}


#endif
