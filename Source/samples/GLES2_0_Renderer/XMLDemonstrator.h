#ifndef XML_DEMO_H
#define XML_DEMO_H
#include "Common/Application.h"
#include "Common/Scene.h"
#include "Common/HMIRectangle.h"
#include "Common/Pixmap.h"
#include "Common/ImageManager.h"
#include "gles2_0_renderer/scene_graph/Camera.h"
#include "Plugin.h"
#include "TinyXML2.h"
#include <map>
#include <list>
#include <vector>

#define VERTEX_SHADER_PRG        (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleVertex.glsl"
#define FRAGMENT_SHADER_PRG      (char * )"../Source\\samples\\GLES2_0_Renderer\\shader\\RectangleFragment.glsl"

std::map<tinyxml2::XMLElement*, Object*> mapInfo;
void AddToMapping(tinyxml2::XMLElement* elementItem, Object* objectItem){
	mapInfo[elementItem] = (Object*)objectItem;
}

Object* getObjectByName(char* name){
	typedef std::map<tinyxml2::XMLElement*, Object*>::iterator itor;
	for(itor iterator = mapInfo.begin(); iterator != mapInfo.end(); iterator++){
		if(!strcmp(iterator->first->Attribute("name"), name)){
			return iterator->second;
		}
	}
	return NULL;
}
std::vector<tinyxml2::XMLNode*> getSiblings(tinyxml2::XMLNode* node)
{
	std::vector<tinyxml2::XMLNode*> nodeList;
	if(!node){
		return nodeList;
	}

	nodeList.push_back(node); // First Sibling
	tinyxml2::XMLNode* tempNode		= node;
	tinyxml2::XMLNode* nxtSibling	= NULL;
	while(nxtSibling = tempNode->NextSibling()){
		nodeList.push_back(nxtSibling);
		tempNode = nxtSibling;
	}
	return nodeList;
}

void tokenizeFloat( char* string,char* token, std::vector<float>& tokenList){
	char* pch;
	pch = strtok((char*)string, token);
				
	if(pch){
		tokenList.push_back(std::stoi(std::string(pch)));
	}
				
	while(pch != NULL){
		pch = strtok(NULL, token);
		if(pch){
			tokenList.push_back(std::stof(std::string(pch)));
		}
	}
}

Application* application = NULL;
Scene* currentScene      = NULL;
void handleTags(const char* name, tinyxml2::XMLElement* element){
	if(!strcmp(name, "Application")){
		if(!application){
			application = new Application;
		}
	}
	else if(!strcmp(name, "Plugin")){
		if(!application){
			application = new Application;
		}

		const char* name = element->Attribute("name");
		const char* type = element->Attribute("type");
		Plugin* plugin	 = NULL;

		if(!strcmp(type, "OPENGLES20_STATIC_PLUGIN")){
			plugin = application->loadPlugin(OPENGLES20_STATIC_PLUGIN);
		}
		
		if(plugin){
			plugin->SetName(std::string(name));
			AddToMapping(element, (Object*)plugin);
		}
	}
	else if(!strcmp(name, "Renderer")){
		tinyxml2::XMLElement* parent = element->Parent()->ToElement();
		Plugin* plugin				 = (Plugin*)mapInfo[parent];
		Renderer* renderer			 = NULL;
		const char* name			 = element->Attribute("name");
		const char* windowTitle		 = element->Attribute("windowTitle");

		if(plugin){
			 renderer = plugin->createRenderer();
		}

		if(renderer){
			renderer->SetName(std::string(name));
			renderer->setWindowTitle((char*)windowTitle);
			AddToMapping(element, (Object*)renderer);
		}
	}
	else if(!strcmp(name, "Scene")){
		tinyxml2::XMLElement* parent = element->Parent()->ToElement();
		Renderer* renderer			 = (Renderer*)mapInfo[parent];
		const char* name			 = element->Attribute("name");
		Scene* sceneItem			 = new Scene(name);
		currentScene				 = sceneItem;
		if(renderer && sceneItem){
			renderer->addScene( sceneItem );
			sceneItem->SetName( name );
			AddToMapping(element, (Object*)sceneItem);
		}
	}
	else if(!strcmp(name, "Camera")){
		tinyxml2::XMLElement* parent = element->Parent()->ToElement();
		Scene* scene				 = (Scene*)mapInfo[parent];
		const char* name			 = element->Attribute("name");
		const char* type			 = element->Attribute("type");
		const char* viewport		 = element->Attribute("viewport");
		Camera* cameraItem			 = NULL;
		if(!scene){
			printf("\n Camera's scene unspecified.");
			assert(0);
		}

		if(scene){
			if(!strcmp(type,"ortho")){
				cameraItem = new CameraHUD(name, scene);
			}
			else{
				cameraItem		= new Camera(name, scene);
				char* position	= (char*)element->Attribute("currentPosition");
				char* target	= (char*)element->Attribute("targetPosition");
				char* TOKEN		= ",";
				std::vector<float> coordList;

				// Tokenize currentPosition
				tokenizeFloat(position, TOKEN, coordList);
				
				if(coordList.size() == 3){
					cameraItem->SetPosition(glm::vec3 (coordList[0], coordList[1], coordList[2]));
				}
				else{
					printf("\n Camera 'currentPosition' must have 3 coordinates:");
					for(int i=0; i<coordList.size(); i++){
						printf("\n currentPosition[%d]: %f", i+1, coordList[i]);
					}
					assert(0);
				}
				coordList.clear();

				// Tokenize currentPosition
				tokenizeFloat(target, TOKEN, coordList);
				
				if(coordList.size() == 3){
					cameraItem->SetTarget(glm::vec3 (coordList[0], coordList[1], coordList[2]));
				}
				else{
					printf("\n Camera 'targetPosition' must have 3 coordinates:");
					for(int i=0; i<coordList.size(); i++){
						printf("\n targetPosition[%d]: %f", i+1, coordList[i]);
					}
					assert(0);
				}

			}
		}

		if(cameraItem){
			cameraItem->SetName( name );
			AddToMapping(element, (Object*)cameraItem);
			if(!viewport){
				cameraItem->Viewport(0, 0, scene->getRenderer()->getWindowWidth(), scene->getRenderer()->getWindowHeight());
			}
			else{
				char* TOKEN = ",";
				std::vector<float> viewportCoord;
				tokenizeFloat( (char*)viewport, TOKEN, viewportCoord);

				if(viewportCoord.size() == 4){
					cameraItem->Viewport((int)viewportCoord[0], (int)viewportCoord[1], (int)viewportCoord[2], (int)viewportCoord[3]);
				}
				else{
					printf("\n Viewport coordinates must be 4:");
					for(int i=0; i<viewportCoord.size(); i++){
						printf("\n Viewport[%d]: %d", i+1, (int)viewportCoord[i]);
					}
					assert(0);
				}
			}
		}
	}
	else if(!strcmp(name, "Image")){
		tinyxml2::XMLElement* parent		= element->Parent()->ToElement();
		Model* parentModel					= NULL;
		unsigned int ProgramID				= 0;
		ProgramManager* ProgramManagerObj	= NULL;

		// If the parent of current model is equal to scene object that means, it has not parent model.
		// Perhaps the object class must introduce a type of object so that it is easy to recognize the
		// object if it is Application, Renderer, Scene or Camera.
		if(currentScene == mapInfo[parent]){
			parentModel = NULL;
		}
		else{
			parentModel = (Model*)mapInfo[parent];
		}
		const char* name			 = element->Attribute("name");
		const char* imageSource		 = element->Attribute("imageSource");
		const char* vertexShader	 = element->Attribute("vertexShader");
		const char* fragmentShader	 = element->Attribute("fragmentShader");
		const char* translate		 = element->Attribute("translate");
		
		Image* imageItem			 = ImageManager::GetInstance()->GetImage(imageSource);
		Pixmap* pixmap				 = new Pixmap(imageItem, currentScene, parentModel, PIXMAP, TWO_DIMENSIONAL_TEXTURE,name);
		if(!pixmap){
			printf("Unable to create the Pixmap object %s, %s",__FUNCTION__, __LINE__);
		}

		if(vertexShader && fragmentShader){
			ProgramManagerObj = ProgramManager::GetInstance();
			ProgramID		  = ProgramManagerObj->LoadShader("Texture", (char*)vertexShader, (char*)fragmentShader )->ProgramID;
		}
		else{
			// If shader not specified the used the parents shader
			if(parentModel){
				ProgramID = parentModel->GetProgram();
			}
			else{
				printf("Error: Shader is not specified for Image: %s", name);
				assert(0);
			}
		}
	   
		float width  = imageItem->imageWidth();
		float height = imageItem->imageHeight();
		
		// Memory leak need to be prevent in the XML design, unlike the manual coding where its easy to track the memory
		// here we need to perform.
		std::vector<glm::vec3>* vertices = new std::vector<glm::vec3>;
		vertices->push_back(glm::vec3( 0.0f,  0.0f,   0.0f ));
		vertices->push_back(glm::vec3( width, 0.0f,   0.0f ));
		vertices->push_back(glm::vec3( 0.0f,  height, 0.0f ));
		vertices->push_back(glm::vec3( width, height, 0.0f ));

		std::vector<glm::vec2>* texCoords = new std::vector<glm::vec2>;
		texCoords->push_back(glm::vec2(0.0f, 0.0f));
		texCoords->push_back(glm::vec2(1.0f, 0.0f));
		texCoords->push_back(glm::vec2(0.0f, 1.0f));
		texCoords->push_back(glm::vec2(1.0f, 1.0f));

		// Set the vertex information
		pixmap->SetVertices(vertices);
		pixmap->SetTexCoords(texCoords);
		pixmap->SetProgram(ProgramID);
		AddToMapping(element, (Object*)pixmap);

		if(translate){
			char* TOKEN = ",";
			std::vector<float> translateCoords;
			tokenizeFloat( (char*)translate, TOKEN, translateCoords);

			if(translateCoords.size() == 3){
				pixmap->Translate(translateCoords[0], translateCoords[1], translateCoords[2]);
			}else{
				printf("\n Translation coordinates must be 3:");
				for(int i=0; i<translateCoords.size(); i++){
					printf("\n translation[%d]: %d", i+1, (int)translateCoords[i]);
				}
				assert(0);
			}

		}

		// If parent model is NULL that mean there is not parent of this model.
		// Therefore, add it to scene, otherwise if it has the parent the the parent
		// will automatically take care of the life cycle.
		if(!parentModel){
			currentScene->addModel(pixmap);
		}
	}
}

void parseNodeInfo(tinyxml2::XMLNode* node){
	std::vector<tinyxml2::XMLNode*> nodeSiblingList = getSiblings(node);
	for(int i=0; i<nodeSiblingList.size(); i++){
		tinyxml2::XMLNode* tempSiblingNode = nodeSiblingList[i];
		if (!tempSiblingNode){
			continue;
		}
		tinyxml2::XMLElement* ele = tempSiblingNode->ToElement(); 
		if(ele)
		{
			const char* msg = ele->Name();
			if(msg){
				printf("\nMessage is: %s",msg);
				handleTags(msg, ele);
			}
		}
		// Process the Node first for Application, Plugin, Scene, Camera, Image, REctangle, Mesh etc.
		tinyxml2::XMLNode* firstChild = tempSiblingNode->FirstChild();
		if(firstChild){
			parseNodeInfo(firstChild);
		}
	}
}

// Demo Description: In this demo, we will draw two rectangle with Array and Element (Indices) drawing scheme.
void XMLDemo(){
	tinyxml2::XMLDocument doc;
	doc.LoadFile("E:/renderconcept/RenderConcept2010-ILifeCycle/RenderConcepts/Resource/SceneGraphXML.xml");
	int erro = doc.ErrorID();
	tinyxml2::XMLElement* root = doc.FirstChildElement("Application");
	const char* msg3 = root->GetText();
	
	parseNodeInfo((tinyxml2::XMLNode*) root);
	if(!application){
		printf("Unable to create the application.");
		assert(0);
	}
	
	application->Initialize();
	Renderer* renderer = (Renderer*)getObjectByName("OGL_Renderer_1");
	while(renderer->getWindow()->isOpen()){
		application->Render();
	}
	delete application;
	application = NULL;
    ProgramManager::DeleteInstance();
	currentScene      = NULL;
	// This is causing memory leak not a good idea. fix this.
	mapInfo.clear();
	
}


#endif
