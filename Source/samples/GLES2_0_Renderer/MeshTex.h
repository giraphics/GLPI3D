#ifndef MESHPIXRECT_H
#define MESHPIXRECT_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "MeshObject.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
#include "Plugin.h"
#include "Common/HMIRectangle.h"
#include "Common/Pixmap.h"

#define VERTEX_SHADER_PRG_TEX        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\MeshTexVertex.glsl"
#define FRAGMENT_SHADER_TEX      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\MeshTexFragment.glsl"
#define VERTEX_SHADER_PRG_RECT        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleVertex.glsl"
#define FRAGMENT_SHADER_PRG_RECT      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleFragment.glsl"

#define PIX_VERTEX_SHADER_PRG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\SimpleTextureVertex.glsl"
#define PIX_FRAGMENT_SHADER_PRG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\SimpleTextureFragment.glsl"

void MeshRectSample(){
   float width  = 100.0f;
   float height = 100.0f;

   std::vector<glm::vec3> vertices;
   vertices.push_back(glm::vec3( 0.0f,  0.0f,   0.0f ));
   vertices.push_back(glm::vec3( width, 0.0f,   0.0f ));
   vertices.push_back(glm::vec3( 0.0f,  height,   0.0f ));
   vertices.push_back(glm::vec3( width,  height,   0.0f ));

   float pixWidth  = 1.0f;
   float pixHeight = 1.0f;

   std::vector<glm::vec3> tempPixVtx;
   tempPixVtx.push_back(glm::vec3( 0.0f,  0.0f,   0.0f ));
   tempPixVtx.push_back(glm::vec3( pixWidth, 0.0f,   0.0f ));
   tempPixVtx.push_back(glm::vec3( 0.0f,  pixHeight,   0.0f ));
   tempPixVtx.push_back(glm::vec3( pixWidth,  pixHeight,   0.0f ));

   std::vector<glm::vec2> texCoords;
   texCoords.push_back(glm::vec2(0.0f, 1.0f));
   texCoords.push_back(glm::vec2(1.0f, 1.0f));
   texCoords.push_back(glm::vec2(0.0f, 0.0f));
   texCoords.push_back(glm::vec2(1.0f, 0.0f));

   Application application;

   // Create a clock for measuring the time elapsed
   Renderer* renderer	= NULL;
   MeshObject* meshObj	= NULL;
   Camera* camera		= NULL;
   CameraHUD* hudCamera = NULL;
   Camera* hudCamera2	= NULL;
   Scene* scene			= NULL;
   Scene* hudScene		= NULL;
   Scene* hudScene2		= NULL;
   Plugin* plugin		= NULL;
   GRectangle* rectangleItem    = NULL;
   Pixmap* pixmap       = NULL;

   plugin = application.loadPlugin(OPENGLES20_STATIC_PLUGIN);
   if(plugin){
	   renderer = plugin->createRenderer(1280, 720);
	   //renderer = plugin->createRenderer(400, 300);
   }

   if (!renderer){
	   return;
   }

   // SCENE 1
   scene = new Scene("MeshScene");
   camera = new Camera("PerespectiveCameraMesh", scene);
   camera->SetPosition(glm::vec3(0.0, 5.0, 8.0));
   camera->SetTarget(glm::vec3(0.0, 0.0, 0.0));
   camera->Viewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());
   //camera->setClearFlag(false);

   ProgramManager* ProgramManagerObj = ProgramManager::GetInstance();
   unsigned int ProgramID = ProgramManagerObj->LoadShader("Meshshader", VERTEX_SHADER_PRG_TEX, FRAGMENT_SHADER_TEX)->ProgramID;
   meshObj = new MeshObject("../Resource/Models/mbclass.obj", scene, NULL, MESH, "");
   
   meshObj->SetName(std::string("My Mesh 41"));
   meshObj->SetProgram(ProgramID);
   meshObj->Scale(.01, .01, .01);
   scene->addModel(meshObj);

   // SCENE 2
   hudScene   = new Scene("RectangleScene");
   hudCamera  = new CameraHUD("hudCamera", hudScene);
   hudCamera->Viewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());
   hudCamera->setClearFlag(false);

   ProgramID = ProgramManagerObj->LoadShader("RectPrg", VERTEX_SHADER_PRG_RECT, FRAGMENT_SHADER_PRG_RECT )->ProgramID;
   rectangleItem = new GRectangle(hudScene, NULL, BUTTON,"");
   rectangleItem->SetName(std::string("My Rectangle 1"));
   rectangleItem->SetProgram(ProgramID);

   // Set the vertex information
   rectangleItem->SetVertices(&vertices);
   rectangleItem->SetTexCoords(&texCoords);

   // Set Color information
   rectangleItem->SetColor(&glm::vec4(0.0, 0.50, .30, 1.0));
   rectangleItem->Translate(10.0f, 10.0f, 0.0);
   rectangleItem->Scale(3.0, 1.0, 1.0);
   hudScene->addModel( rectangleItem );

   rectangleItem = new GRectangle(hudScene, rectangleItem, BUTTON,"");
   rectangleItem->SetName(std::string("My Rectangle 2"));
   rectangleItem->SetProgram(ProgramID);

   // Set the vertex information
   rectangleItem->SetVertices(&vertices);
   rectangleItem->SetTexCoords(&texCoords);
   
   // Set Color information
   rectangleItem->SetColor(&glm::vec4(8.0, 3.0, 4.0, 1.0));
   rectangleItem->Translate(0.0, height, 0.0);

   // SCENE 3
   /*hudScene2   = new Scene("PixScene");
   hudCamera2   = new Camera("PerespectiveCamera", hudScene2);
   hudCamera2->SetPosition(glm::vec3 (0.0, 0.0,230));
   hudCamera2->SetTarget(glm::vec3 (0.0, 0.0,0.0));
   hudCamera2->Viewport(0, 0, renderer->getWindowWidth(), renderer->getWindowHeight());
   hudCamera2->setClearFlag(false);

   ProgramID = ProgramManagerObj->LoadShader("Texture1", PIX_VERTEX_SHADER_PRG, PIX_FRAGMENT_SHADER_PRG )->ProgramID;
   pixmap = new Pixmap("../Resource/Icons/Volkswagen1500.png",hudScene2, NULL, PIXMAP, TWO_DIMENSIONAL_TEXTURE,"");
   pixmap->SetProgram(ProgramID);
   pixmap->SetName(std::string("My image1"));

   // Set the vertex information
   pixmap->SetVertices(tempPixVtx);
   pixmap->SetTexCoords(texCoords);

   //// Set Color information
   pixmap->SetColor(&glm::vec4(1.0, 0.0, 0.0, 1.0));
   hudScene2->addModel( pixmap );*/

   renderer->addScene( scene );
   renderer->addScene( hudScene );
   //renderer->addScene( hudScene2 );
   
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
