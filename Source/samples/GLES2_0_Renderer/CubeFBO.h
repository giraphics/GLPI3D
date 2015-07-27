#ifndef CUBEFBODEMO_H
#define CUBEFBODEMO_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "MeshObject.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
#include "gles2_0_renderer/Texture.h"
#include "gles2_0_renderer/FrameBufferObject.h"
#include "Plugin.h"
#include "Common/Pixmap.h"
#include "Common/ImageManager.h"

#define VERTEX_SHADER_PRG_PHONG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\PhongVertex.glsl"
#define FRAGMENT_SHADER_PHONG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\PhongFragment.glsl"
#define TEX_VERTEX_SHADER_PRG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\SimpleTextureVertex.glsl"
#define TEX_FRAGMENT_SHADER_PRG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\SimpleTextureFragment.glsl"
#define VERTEX_SHADER_PRG_TEX        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\MeshTexVertex.glsl"
#define FRAGMENT_SHADER_TEX      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\MeshTexFragment.glsl"

Scene* sharedScenePtr = NULL;
// The custom scene allow to simply the scene management 
// by moving it to the seperate module.
class CustomSceneFBO2 : public Scene
{
    
public:
    //! Default Constructor
    CustomSceneFBO2(std::string name = "", ObjectRelative* parentObj = NULL);

    //! Destructor
    virtual ~CustomSceneFBO2(void);
    
	//! Initialize the Custom Scene
    void Initialize();

	//! Handle Render
    void Render(bool (*customRender)());
    
	//! Handle Resize
	void Resize( int w, int h );
    
	Texture* getFBOColorTexture(){ return &colorTexture; }
private:
    /**
     Actual camera view.
     */
    Camera* camera;

    /**
     FBO Scene object.
     */
    FrameBufferObjectSurface* fbo;

    /**
     Depth texture buffer.
     */
    Texture depthTexture;

	/**
     Color texture buffer.
     */
    Texture colorTexture;

};

CustomSceneFBO2::CustomSceneFBO2(std::string name, ObjectRelative* parentObj):Scene(name, parentObj)
{
	// Parminder: Strange observation, when dynamic cast is used the renderingEngine returns NULL.
	// The dynamic cast works fine if the inheritance of GestureEvent is removed from GLES20Renderer.
	Renderer* renderEngine = (Renderer*)parentObj;
	
	//Create the Frame buffer object
    fbo = new FrameBufferObjectSurface(renderEngine->getWindowWidth(),renderEngine->getWindowHeight());
    fbo->SetParent(this);

    // Generate the FBO ID
    fbo->GenerateFBO();
    
    depthTexture.generateTexture2D(GL_TEXTURE_2D, fbo->GetWidth(), fbo->GetHeight(), GL_DEPTH_COMPONENT32F, GL_FLOAT, GL_DEPTH_COMPONENT,0,true,0,0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_NEAREST,GL_NEAREST );

	// Attache the Depth Buffer to FBO's depth attachment
    fbo->AttachTexture(depthTexture, GL_DEPTH_ATTACHMENT);

    colorTexture.generateTexture2D(GL_TEXTURE_2D, fbo->GetWidth(), fbo->GetHeight(), GL_RGBA, GL_FLOAT, GL_RGBA,0,true,0,0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_NEAREST,GL_NEAREST );

    fbo->AttachTexture(colorTexture, GL_COLOR_ATTACHMENT0);
    
    // Check the status of the FBO
    fbo->CheckFboStatus();

    camera = NULL;
}

void CustomSceneFBO2::Initialize()
{
   camera = new Camera("PerespectiveCameraMesh", this);
   camera->SetPosition(glm::vec3(0.0, 5.0, 10.0));
   camera->SetTarget(glm::vec3(0.0, 0.0, 0.0));
   camera->SetClearColor(glm::vec4(0.7,0.7,0.7,1.0));
   this->addCamera(camera);
    Scene::Initialize();
}

CustomSceneFBO2::~CustomSceneFBO2(void)
{
}

void CustomSceneFBO2::Resize( int w, int h )
{
    camera->Viewport(0, 0, w, h);
    Scene::Resize(w, h);
}

/*!
	Performs rendering for each model
 
	\param[in] void.
 
	\return void.
 */
void CustomSceneFBO2::Render(bool (*customRender)())
{
    fbo->Push();
	Scene::Render();
    fbo->Pop();
}

// The custom scene allow to simply the scene management 
// by moving it to the seperate module.
class CubeTexScene : public Scene
{
    
public:
    //! Default Constructor
    CubeTexScene(std::string name = "", ObjectRelative* parentObj = NULL);

    //! Destructor
    virtual ~CubeTexScene(void);
    
	//! Initialize the Custom Scene
    void Initialize();

	//! Handle Render
    void Render(bool (*customRender)() = NULL);
    
	//! Handle Resize
	void Resize( int w, int h );
    
private:
    /**
     Actual camera view.
     */
    Camera* camera2;
	Pixmap* pixmap[6];
};

CubeTexScene::CubeTexScene(std::string name, ObjectRelative* parentObj):Scene(name, parentObj)
{
	camera2	= NULL;
	for(int i=0; i<6; i++){
		pixmap[i]	= NULL;
	}
}

void CubeTexScene::Initialize()
{
   camera2 = new Camera("PerespectiveCameraMesh2", this);
   camera2->SetPosition(glm::vec3(0.0, 0.0, 2.0));
   camera2->SetTarget(glm::vec3(0.0, 0.0, 0.0));
   //camera2	= new CameraHUD("hudCamera", this);

    this->addCamera(camera2);

   ProgramManager* ProgramManagerObj	= ProgramManager::GetInstance();
   unsigned int ProgramID				= ProgramManagerObj->LoadShader("FBOTexture", TEX_VERTEX_SHADER_PRG, TEX_FRAGMENT_SHADER_PRG )->ProgramID;

	for(int i=0; i<6; i++){
		pixmap[i] = new Pixmap(((CustomSceneFBO*)sharedScenePtr)->getFBOColorTexture()->getTextureID(), this, pixmap[0], PIXMAP, TWO_DIMENSIONAL_TEXTURE,"");
		pixmap[i]->SetProgram(ProgramID);

		float width  = 1;
		float height = 1;
		// Memeory leak for vertices and texture coordinates need to be fixed.
		std::vector<glm::vec3>* vertices = new std::vector<glm::vec3>;
		vertices->push_back(glm::vec3( -0.25f,  0.25f,   0.0f ));
		vertices->push_back(glm::vec3( 0.25f, 0.25f,   0.0f ));
		vertices->push_back(glm::vec3( -0.25f,  -0.25,   0.0f ));
		vertices->push_back(glm::vec3( 0.25f,  -0.25,   0.0f ));


		std::vector<glm::vec2>* texCoords = new std::vector<glm::vec2>;
		texCoords->push_back(glm::vec2(0.0f, 0.0f));
		texCoords->push_back(glm::vec2(1.0f, 0.0f));
		texCoords->push_back(glm::vec2(0.0f, 1.0f));
		texCoords->push_back(glm::vec2(1.0f, 1.0f));

		// Set the vertex information
		pixmap[i]->SetVertices(vertices);
		pixmap[i]->SetTexCoords(texCoords);
		//pixmap->SetCenter(glm::vec3(width/2.0,height/2.0, 0.0));
		addModel( pixmap[i] );
	}

	pixmap[0]->Translate(0.0, 0.0, 0.5);
	pixmap[1]->Translate(0.0, 0.0, -0.5);
	pixmap[2]->Rotate(90.0, 0.0, 1.0, 0.0);
	pixmap[3]->Rotate(90.0, 0.0, 1.0, 0.0);
	pixmap[2]->Translate(0.0, 0.0, 0.25);
	pixmap[3]->Translate(0.0, 0.0, -0.25);
	pixmap[2]->Translate(0.25, 0.0, 0.0);
	pixmap[3]->Translate(0.25, 0.0, 0.0);
	pixmap[4]->Rotate(90.0, 1.0, 0.0, 0.0);
	pixmap[5]->Rotate(90.0, 1.0, 0.0, 0.0);
	pixmap[4]->Translate(0.0, -0.25, -0.25);
	pixmap[5]->Translate(0.0, -0.25, 0.25);
	pixmap[0]->SetCenter(glm::vec3(-0.0, -0.0, -0.5));
	pixmap[0]->Scale(2.0, 2.0, 2.0);

	Scene::Initialize();
}

CubeTexScene::~CubeTexScene(void)
{
}

void CubeTexScene::Resize( int w, int h )
{
    camera2->Viewport(0, 0, w, h);
    Scene::Resize(w, h);
}

/*!
	Performs rendering for each model
 
	\param[in] void.
 
	\return void.
 */
void CubeTexScene::Render(bool (*customRender)())
{
	pixmap[0]->Rotate(0.3, 1.0, 1.0, 1.0);
	Scene::Render();
}


class CubeFBOModel : public Model{

public:
   CubeFBOModel(Scene* parent, Model* model, ModelType type, std::string objectName = "");

   virtual ~CubeFBOModel();

   // Initialize our Model class
   void Initialize();

   // Render the Model class
   void Render(bool (*customRender)()=NULL);

   void SetMaterial(Material mat);

private:
	MeshObject* Car;
	ProgramManager* ProgramManagerObj;
};

void CubeFBOModel::SetMaterial(Material mat)
{
    for(int i =0; i<childList.size(); i++){
        dynamic_cast<Model*>(childList.at(i))->SetMaterial(mat);
    }
}

void CubeFBOModel::Initialize()
{
	Model::Initialize();
}

void CubeFBOModel::Render(bool (*customRender)())
{
	Car->Rotate(0.1, 0.0, 0.0, 1.0);
	Model::Render();
}

CubeFBOModel::~CubeFBOModel()
{
   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
}

CubeFBOModel::CubeFBOModel(Scene* scene, Model* model, ModelType type, std::string objectName):Model(scene, model, type, objectName)
{
   ProgramManager* ProgramManagerObj = ProgramManager::GetInstance();
   unsigned int ProgramID = ProgramManagerObj->LoadShader("Meshshader", VERTEX_SHADER_PRG_PHONG, FRAGMENT_SHADER_PHONG)->ProgramID;
   //unsigned int ProgramID = ProgramManagerObj->LoadShader("Meshshader", VERTEX_SHADER_PRG_TEX, FRAGMENT_SHADER_TEX)->ProgramID;
   this->SetProgram(ProgramID);

   // Car
    Car =  new MeshObject("../Resource/Models/mbclass.obj", scene, this, MESH, "Mercedes Benz");
	Car->SetProgram(ProgramID);
    Car->Scale(.01, .01, .01);
    Car->Rotate(-90.0, 1.0, 0.0, 0.0);
    Car->SetMaterial(Material(MaterialPolishedGold));

}

void CubeFBODemo(){
   Application application;

   // Create a clock for measuring the time elapsed
   Renderer* renderer	= NULL;
   Camera* camera		= NULL;
   Scene* sceneFBO   	= NULL;
   Scene* sceneTex		= NULL;
   Plugin* plugin		= NULL;
   
   plugin = application.loadPlugin(OPENGLES20_STATIC_PLUGIN);
   if(plugin){
	   renderer = plugin->createRenderer(512*2, 512*2);
   }

   if (!renderer){
	   return;
   }
   
   sceneFBO = new CustomSceneFBO2("MeshScene", renderer);
   sharedScenePtr = sceneFBO;
   glm::vec4 lightPosition(0.0, -5.0, -10.0, 0.0);
   Light light(Material(MaterialWhite), lightPosition);
   sceneFBO->addLight(&light);

   // Add FBO Model
   FBOModel fboModelObject( sceneFBO, NULL, MESH, "FBOModelObj");
   
   MeshObject lightBulb("../Resource/Models/Sphere.obj", sceneFBO, NULL, MESH, "lightBulb");
   lightBulb.SetProgram(fboModelObject.GetProgram());
   lightBulb.SetMaterial(Material(MaterialPewter));
   lightBulb.ScaleLocal(0.5, 0.5, .5);

   sceneFBO->addModel( &fboModelObject);
   sceneFBO->addModel( &lightBulb);

   // Add Second scene
   sceneTex = new CubeTexScene("TexScene", renderer); // Add the second scene
   //sceneTex->addLight(&light);

   // Add the second scene
   // renderer->addScene( sceneTex );

   // Intialize the application
   application.Initialize();

   application.Resize(renderer->getWindowWidth(), renderer->getWindowHeight());
   
   static int num_segments = 500;
   static int radius = 10;
   while(renderer->getWindow()->isOpen()){
      for(int i = 0; i < num_segments; i++){
		static int mat = 0;
		static clock_t start = clock();
		if(clock()-start > 3*CLOCKS_PER_SEC){
			start = clock();
			(mat %=6)++; //Plus one to avoid none type
			fboModelObject.SetMaterial(Material(MaterialType(mat)));
		}

         float theta = 2.0f * PI * float(i) / float(num_segments);//get the current angle

         lightPosition.x = radius * cosf(theta);//calculate the x component
         lightPosition.y = radius * sinf(theta);//calculate the y component
         sceneFBO->getLights().at(0)->position = lightPosition;
		 lightBulb.Translate(lightPosition.x, lightPosition.y, lightPosition.z);
		 application.Render();
		 lightBulb.Translate(-lightPosition.x, -lightPosition.y, -lightPosition.z);
	  }
   }
}


#endif
