#ifndef FRAMEBUFFERDEMO_H
#define FRAMEBUFFERDEMO_H
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

static int num_segments = 500;
static int radius = 10;

// The custom scene allow to simply the scene management 
// by moving it to the seperate module.
class CustomSceneFBO : public Scene
{
    
public:
    //! Default Constructor
    CustomSceneFBO(std::string name = "", ObjectRelative* parentObj = NULL);

    //! Destructor
    virtual ~CustomSceneFBO(void);
    
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

CustomSceneFBO::CustomSceneFBO(std::string name, ObjectRelative* parentObj):Scene(name, parentObj)
{
	// Parminder: Strange observation, when dynamic cast is used the renderingEngine returns NULL.
	// The dynamic cast works fine if the inheritance of GestureEvent is removed from GLES20Renderer.
	Renderer* renderEngine = (Renderer*)parentObj;
	
	//Create the Frame buffer object
    fbo = new FrameBufferObjectSurface(renderEngine->getWindowWidth(),renderEngine->getWindowHeight());
    
    // Generate the FBO ID
    fbo->GenerateFBO();
    
    depthTexture.generateTexture2D(GL_TEXTURE_2D, fbo->GetWidth(), fbo->GetHeight(), GL_DEPTH_COMPONENT32F, GL_FLOAT, GL_DEPTH_COMPONENT,0,true,0,0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_NEAREST,GL_NEAREST );
    fbo->AttachTexture(depthTexture, GL_DEPTH_ATTACHMENT); 	// Attache the Depth Buffer to FBO's depth attachment

    colorTexture.generateTexture2D(GL_TEXTURE_2D, fbo->GetWidth(), fbo->GetHeight(), GL_RGBA, GL_FLOAT, GL_RGBA,0,true,0,0,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_NEAREST,GL_NEAREST );

    fbo->AttachTexture(colorTexture, GL_COLOR_ATTACHMENT0);
    
    // Check the status of the FBO
    fbo->CheckFboStatus();

    camera = NULL;
}

void CustomSceneFBO::Initialize()
{
   camera = new Camera("PerespectiveCameraMesh", this);
   camera->SetPosition(glm::vec3(0.0, 5.0, 10.0));
   camera->SetTarget(glm::vec3(0.0, 0.0, 0.0));
   camera->SetClearColor(glm::vec4(0.7,0.7,0.7,1.0));
   this->addCamera(camera);
    Scene::Initialize();
}

CustomSceneFBO::~CustomSceneFBO(void)
{
}

void CustomSceneFBO::Resize( int w, int h )
{
    camera->Viewport(0, 0, w, h);
    Scene::Resize(w, h);
}

/*!
	Performs rendering for each model
 
	\param[in] void.
 
	\return void.
 */
void CustomSceneFBO::Render(bool (*customRender)())
{
    fbo->Push();
	Scene::Render();
    fbo->Pop();
}

// The custom scene allow to simply the scene management 
// by moving it to the seperate module.
class TexScene : public Scene
{
    
public:
    //! Default Constructor
    TexScene(std::string name = "", ObjectRelative* parentObj = NULL);

    //! Destructor
    virtual ~TexScene(void);
    
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
};

TexScene::TexScene(std::string name, ObjectRelative* parentObj):Scene(name, parentObj)
{
}

void TexScene::Initialize()
{
   camera2 = new Camera("PerespectiveCameraMesh2", this);
   camera2->SetPosition(glm::vec3(0.0, 0.0, 2.0));
   camera2->SetTarget(glm::vec3(0.0, 0.0, 0.0));
   //camera2	= new CameraHUD("hudCamera", this);

    this->addCamera(camera2);
    Scene::Initialize();
}

TexScene::~TexScene(void)
{
}

void TexScene::Resize( int w, int h )
{
    camera2->Viewport(0, 0, w, h);
    Scene::Resize(w, h);
}

/*!
	Performs rendering for each model
 
	\param[in] void.
 
	\return void.
 */
void TexScene::Render(bool (*customRender)())
{
	Scene::Render();
}


class FBOModel : public Model{

public:
   FBOModel(Scene* parent, Model* model, ModelType type, std::string objectName = "");

   virtual ~FBOModel();

   // Initialize our Model class
   void Initialize();

   // Render the Model class
   void Render(bool (*customRender)()=NULL);

   void SetMaterial(Material mat);

private:
	MeshObject* Car;
	ProgramManager* ProgramManagerObj;
};

void FBOModel::SetMaterial(Material mat)
{
    for(int i =0; i<childList.size(); i++){
        dynamic_cast<Model*>(childList.at(i))->SetMaterial(mat);
    }
}

void FBOModel::Initialize()
{
	Model::Initialize();
}

void FBOModel::Render(bool (*customRender)())
{
	Car->Rotate(0.1, 0.0, 0.0, 1.0);
	Model::Render();
}

FBOModel::~FBOModel()
{
   ProgramManager::DeleteInstance();
   ProgramManagerObj = NULL;
}

FBOModel::FBOModel(Scene* scene, Model* model, ModelType type, std::string objectName):Model(scene, model, type, objectName)
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

void FBODemo(){
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
   
   sceneFBO = new CustomSceneFBO("MeshScene", renderer);
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
   sceneTex = new TexScene("TexScene");
   //sceneTex->addLight(&light);

   ProgramManager* ProgramManagerObj	= ProgramManager::GetInstance();
   unsigned int ProgramID				= ProgramManagerObj->LoadShader("FBOTexture", TEX_VERTEX_SHADER_PRG, TEX_FRAGMENT_SHADER_PRG )->ProgramID;

   Pixmap* pixmap = new Pixmap(((CustomSceneFBO*)sceneFBO)->getFBOColorTexture()->getTextureID(), sceneTex, NULL, PIXMAP, TWO_DIMENSIONAL_TEXTURE,"");
   pixmap->SetProgram(ProgramID);
	float width  = 1;//imageItem->imageWidth();
	float height = 1;//imageItem->imageHeight();
	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3( 0.0f,  0.0f,   0.0f ));
	vertices.push_back(glm::vec3( width, 0.0f,   0.0f ));
	vertices.push_back(glm::vec3( 0.0f,  height,   0.0f ));
	vertices.push_back(glm::vec3( width,  height,   0.0f ));


	std::vector<glm::vec2> texCoords;
	texCoords.push_back(glm::vec2(0.0f, 0.0f));
	texCoords.push_back(glm::vec2(1.0f, 0.0f));
	texCoords.push_back(glm::vec2(0.0f, 1.0f));
	texCoords.push_back(glm::vec2(1.0f, 1.0f));

	// Set the vertex information
	pixmap->SetVertices(&vertices);
	pixmap->SetTexCoords(&texCoords);
	pixmap->SetCenter(glm::vec3(width/2.0,height/2.0, 0.0));
   sceneTex->addModel( pixmap );

   // Add the second scene
   renderer->addScene( sceneTex );

   // Intialize the application
   application.Initialize();

   application.Resize(renderer->getWindowWidth(), renderer->getWindowHeight());
   while(renderer->getWindow()->isOpen()){
      for(int i = 0; i < num_segments; i++){
	    pixmap->Rotate(0.50, 1.0, 1.0, 1.0);
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
