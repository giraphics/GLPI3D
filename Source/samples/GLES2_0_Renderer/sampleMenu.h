#ifndef SAMPLE_MENU_H
#define SAMPLE_MENU_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "Common/Pixmap.h"
#include "Common/ImageManager.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
using namespace namespaceimage;

#define MENU_VERTEX_SHADER_PRG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\SimpleTextureVertex.glsl"
#define MENU_FRAGMENT_SHADER_PRG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\SimpleTextureFragment.glsl"

void SampleMenu(){
   std::vector<glm::vec2>* texCoords = new std::vector<glm::vec2>;
   texCoords->push_back(glm::vec2(0.0f, 0.0f));
   texCoords->push_back(glm::vec2(1.0f, 0.0f));
   texCoords->push_back(glm::vec2(0.0f, 1.0f));
   texCoords->push_back(glm::vec2(1.0f, 1.0f));

   Application application;

   Renderer* renderer   = NULL;
   Pixmap* pixmap       = NULL;
   Pixmap* root	        = NULL;
   Camera* camera       = NULL;
   Scene* scene         = NULL;
   Plugin* plugin		= NULL;
   Image* imageItem		= NULL;
   float verticalDiplacement = 0.0;

   plugin = application.loadPlugin(OPENGLES20_STATIC_PLUGIN);
   if(plugin){
	   renderer = plugin->createRenderer();
   }

   if (!renderer){
	   return;
   }

   scene    = new Scene("MenuScene", renderer); // Add to Renderer
   camera	= new CameraHUD("hudCamera", scene);
   camera->Viewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());

   ProgramManager* ProgramManagerObj	= ProgramManager::GetInstance();
   unsigned int ProgramID				= ProgramManagerObj->LoadShader("Texture", TEX_VERTEX_SHADER_PRG, TEX_FRAGMENT_SHADER_PRG )->ProgramID;

   imageItem	= ImageManager::GetInstance()->GetImage("../Resource/gui/border.png");
   root = pixmap= new Pixmap(imageItem, scene, NULL, PIXMAP, TWO_DIMENSIONAL_TEXTURE,"");
   
   pixmap->SetProgram(ProgramID);
   {  
	   float width  = imageItem->imageWidth();
	   float height = imageItem->imageHeight();
	   verticalDiplacement = height;
	   
	   std::vector<glm::vec3>* vertices = new std::vector<glm::vec3>;
	   vertices->push_back(glm::vec3( 0.0f,  0.0f,   0.0f ));
	   vertices->push_back(glm::vec3( width, 0.0f,   0.0f ));
	   vertices->push_back(glm::vec3( 0.0f,  height,   0.0f ));
	   vertices->push_back(glm::vec3( width,  height,   0.0f ));

	   // Set the vertex information
	   pixmap->SetVertices(vertices);
	   pixmap->SetTexCoords(texCoords);
   }

   for(int i=0; i<5; i++){
	   imageItem	 = ImageManager::GetInstance()->GetImage("../Resource/gui/option.png");
	   pixmap		 = new Pixmap(imageItem, scene, pixmap, PIXMAP, TWO_DIMENSIONAL_TEXTURE,"");
	   pixmap->Translate(0.0, verticalDiplacement, 0.0);
	   pixmap->SetProgram(ProgramID);
	   {  
		   float width  = imageItem->imageWidth();
		   float height = imageItem->imageHeight();
		   verticalDiplacement = height;
		   std::vector<glm::vec3>* vertices = new std::vector<glm::vec3>;
		   vertices->push_back(glm::vec3( 0.0f,  0.0f,   0.0f ));
		   vertices->push_back(glm::vec3( width, 0.0f,   0.0f ));
		   vertices->push_back(glm::vec3( 0.0f,  height,   0.0f ));
		   vertices->push_back(glm::vec3( width,  height,   0.0f ));

		   // Set the vertex information
		   pixmap->SetVertices(vertices);
		   pixmap->SetTexCoords(texCoords);
	   }
   }
   scene->addModel( root );


   root->Translate(250.0f, 100.0f, 0.0f);

   // Intialize the application
   application.Initialize();
   static float scalef = 1.0;
   while(renderer->getWindow()->isOpen()){
		application.Render();
   }

   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
   delete texCoords;
}


#endif
