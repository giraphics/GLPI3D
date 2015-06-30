#ifndef SAMPLE_IMAGE_H
#define SAMPLE_IMAGE_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "Common/Pixmap.h"
#include "gles2_0_renderer/scene_graph/Camera.h"

#define TEX_VERTEX_SHADER_PRG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\SimpleTextureVertex.glsl"
#define TEX_FRAGMENT_SHADER_PRG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\SimpleTextureFragment.glsl"

void SampleImage(){
   float width  = 1.0f;
   float height = 1.0f;
  // glm::vec3 tempVtx[4] = {
  //    glm::vec3( 0.0f,  0.0f,   0.0f ),
  //    glm::vec3( width, 0.0f,   0.0f ),
  //    glm::vec3( 0.0f,  height, 0.0f ),
  //    glm::vec3( width, height, 0.0f )
  // };
  // glm::vec2 texCoords[4] = {
  //    glm::vec2(0.0f, 1.0f),
  //    glm::vec2(1.0f, 1.0f),
  //    glm::vec2(0.0f, 0.0f),
  //    glm::vec2(1.0f, 0.0f),
  //};

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

   Renderer* renderer   = NULL;
   Pixmap* pixmap       = NULL;
   Camera* camera       = NULL;
   Scene* scene         = NULL;
   Plugin* plugin		= NULL;
   
   plugin = application.loadPlugin(OPENGLES20_STATIC_PLUGIN);
   if(plugin){
	   renderer = plugin->createRenderer();
   }

   if (!renderer){
	   return;
   }

   scene    = new Scene("RectangleScene");

   camera   = new Camera("PerespectiveCamera", scene);
   camera->SetPosition(glm::vec3 (0.0, 0.0,13));
   camera->SetTarget(glm::vec3 (0.0, 0.0,0.0));
   camera->Viewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());

   Pixmap* parent = NULL;
   // Because this is perspective

   ProgramManager* ProgramManagerObj = ProgramManager::GetInstance();
   unsigned int ProgramID = ProgramManagerObj->LoadShader("Texture", TEX_VERTEX_SHADER_PRG, TEX_FRAGMENT_SHADER_PRG )->ProgramID;
   pixmap = new Pixmap("../Resource/Icons/continental.png",scene, parent, PIXMAP, TWO_DIMENSIONAL_TEXTURE,"");
   pixmap->SetProgram(ProgramID);
   pixmap->SetName(std::string("My image"));

   // Set the vertex information
   pixmap->SetVertices(&vertices);
   pixmap->SetTexCoords(&textureCoords);

   pixmap->Scale(15.0, 15.0, 15.0);
   pixmap->Translate(0.0, -height / 2, 0.0);
   pixmap->SetCenter(glm::vec3(width/2, height/2, 0.0));

   // Set Color information
   pixmap->SetColor(&glm::vec4(1.0, 0.0, 0.0, 1.0));
   scene->addModel( pixmap );

   renderer->addScene( scene );

   // Intialize the application
   application.Initialize();

   static float xd = 0.01f;
   int num_segments = 500;
   int radius =20;
   float delta = 0.01f;

   while(renderer->getWindow()->isOpen()){

      for(int i = 0; i < num_segments; i++)
      {
         float theta = 2.0f * PI * float(i) / float(num_segments);//get the current angle

         float x = radius * cosf(theta);//calculate the x component
         float y = radius * sinf(theta);//calculate the y component
         camera->SetPosition(glm::vec3 (x, 0.0, y));

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
   }

   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
}


#endif
