#include "SceneGraphParser.h"

SceneGraphParser::SceneGraphParser(){
	application	 = NULL;
	currentScene = NULL;
}

SceneGraphParser::~SceneGraphParser(){
	application		= NULL;
	currentScene    = NULL;
	// This is causing memory leak not a good idea. fix this.
	mapInfo.clear();
}

Application* SceneGraphParser::LoadXML(const char* filePath){
	doc.LoadFile(filePath);
	int erro = doc.ErrorID();
	if(erro){
		printf("Scene Graphs XML parser Error: %s", filePath);
		// Parminder: TODO: also provide the type of error occurred.
		return NULL;
	}
	tinyxml2::XMLElement* root = doc.FirstChildElement("Application");
	const char* msg3 = root->GetText();
	
	parseNodeInfo((tinyxml2::XMLNode*) root);
	return application;
}

Object* SceneGraphParser::getObjectByName(char* name){
	for(itor iterator = mapInfo.begin(); iterator != mapInfo.end(); iterator++){
		if(!strcmp(iterator->first->Attribute("name"), name)){
			return iterator->second;
		}
	}
	return NULL;
}

void SceneGraphParser::AddToMapping(tinyxml2::XMLElement* elementItem, Object* objectItem){
	mapInfo[elementItem] = (Object*)objectItem;
}

std::vector<tinyxml2::XMLNode*> SceneGraphParser::getSiblings(tinyxml2::XMLNode* node)
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

void SceneGraphParser::tokenizeFloat( char* string,char* token, std::vector<float>& tokenList){
	char* pch    = NULL;
	float number = 0.0f;

	pch = strtok((char*)string, token);
				
	try {
		number = std::stof(std::string(pch));
	}
	catch (const std::invalid_argument& ia) {
		std::cerr << "Invalid argument: " << ia.what() << "in " << __FUNCTION__ <<"at "<< __LINE__ << '\n';
		assert(0);
	}
				
	if(pch){
		tokenList.push_back(number);
	}
				
	while(pch != NULL){
		pch = strtok(NULL, token);
		if(pch){
			try {
				number = std::stof(std::string(pch));
			}
			catch (const std::invalid_argument& ia) {
				std::cerr << "Invalid argument: " << ia.what() << '\n';
				assert(0);
			}
			tokenList.push_back(number);
		}
	}
}

bool SceneGraphParser::parseTagApplication()
{
	if(!application){
		application = new Application;
	}
	return true;
}

bool SceneGraphParser::parseTagPlugin(tinyxml2::XMLElement* element){
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

	return true;
}

bool SceneGraphParser::parseTagRenderer(tinyxml2::XMLElement* element){

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

	return true;
}

bool SceneGraphParser::parseTagScene(tinyxml2::XMLElement* element){
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

	return true;
}

bool SceneGraphParser::parseTagCamera(tinyxml2::XMLElement* element){
	tinyxml2::XMLElement* parent = element->Parent()->ToElement();
	Scene* scene				 = (Scene*)mapInfo[parent];
	const char* name			 = element->Attribute("name");
	const char* type			 = element->Attribute("type");
	const char* viewport		 = element->Attribute("viewport");
	const char* clear			 = element->Attribute("clear");
	const char* clearColor		 = element->Attribute("clearColor");
	Camera* cameraItem			 = NULL;
	char* TOKEN					 = ",(){}[]";
	std::vector<float> coordList;

	if(!scene){
		printf("\n Camera's scene unspecified.");
		assert(0);
	}

	if(scene){
		if(type && !strcmp(type,"ortho")){
			cameraItem = new CameraHUD(name, scene);
		}
		else{
			cameraItem		= new Camera(name, scene);
			char* position	= (char*)element->Attribute("currentPosition");
			char* target	= (char*)element->Attribute("targetPosition");

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
		if(clear){
			cameraItem->setClearFlag(strcmp("on",clear)? false : true);
		}

		if(clearColor){
			coordList.clear();
			tokenizeFloat((char*)clearColor, TOKEN, coordList);
			if(coordList.size() == 4){
				cameraItem->SetClearColor(glm::vec4(coordList[0], coordList[1], coordList[2], coordList[3]));
			}
			else{
				printf("\n Viewport coordinates must be 4, using default background color of Camera: %s", cameraItem->GetName().c_str());
			}
		}

		if(!viewport){
			cameraItem->Viewport(0, 0, scene->getRenderer()->getWindowWidth(), scene->getRenderer()->getWindowHeight());
		}
		else{
			char* TOKEN = ",(){}[]";
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

	return true;
}

bool SceneGraphParser::parseTagImage(tinyxml2::XMLElement* element){
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
		char* TOKEN = ",(){}[]";
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

	return true;
}

void SceneGraphParser::handleTags(const char* name, tinyxml2::XMLElement* element){
	if(!strcmp(name, "Application")){
		parseTagApplication();
	}
	else if(!strcmp(name, "Plugin")){
		parseTagPlugin(element);
	}
	else if(!strcmp(name, "Renderer")){
		parseTagRenderer(element);
	}
	else if(!strcmp(name, "Scene")){
		parseTagScene(element);
	}
	else if(!strcmp(name, "Camera")){
		parseTagCamera(element);
	}
	else if(!strcmp(name, "Image")){
		parseTagImage(element);
	}
	else if(!strcmp(name, "Rectangle")){
		parseTagRectangle(element);
	}
	else if(!strcmp(name, "Mesh")){
		parseTagMesh(element);
	}
}

void SceneGraphParser::parseNodeInfo(tinyxml2::XMLNode* node){
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

bool SceneGraphParser::parseTagRectangle(tinyxml2::XMLElement* element){
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
	const char* vertexShader	 = element->Attribute("vertexShader");
	const char* fragmentShader	 = element->Attribute("fragmentShader");
	const char* translate		 = element->Attribute("translate");
	const char* vertex3D		 = element->Attribute("vertex3D");
	const char* texCoord		 = element->Attribute("texCoord");
	const char* color			 = element->Attribute("color");
		
	GRectangle* rectangleItem    = NULL;
	rectangleItem = new GRectangle(currentScene, parentModel, BUTTON, name);
	rectangleItem->Rotate(9.0, 1.0, 1.0, 1.0);
	if(!rectangleItem){
		printf("Unable to create the Rectangle object %s, %s",__FUNCTION__, __LINE__);
	}

	if(vertexShader && fragmentShader){
		ProgramManagerObj = ProgramManager::GetInstance();
		ProgramID		  = ProgramManagerObj->LoadShader("Rectangle", (char*)vertexShader, (char*)fragmentShader )->ProgramID;
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
	   
	// Memory leak need to be prevent in the XML design, unlike the manual coding where its easy to track the memory
	// here we need to perform.
	if(vertex3D){
		std::vector<glm::vec3>* vertices = new std::vector<glm::vec3>;
		char* TOKEN = ",(){}[] ";
		std::vector<float> vertexCoords;
		tokenizeFloat( (char*)vertex3D, TOKEN, vertexCoords);

		if(vertexCoords.size() == 12){
			vertices->push_back(glm::vec3( vertexCoords[0],  vertexCoords[1],   vertexCoords[2] ));
			vertices->push_back(glm::vec3( vertexCoords[3],  vertexCoords[4],   vertexCoords[5] ));
			vertices->push_back(glm::vec3( vertexCoords[6],  vertexCoords[7],   vertexCoords[8] ));
			vertices->push_back(glm::vec3( vertexCoords[9],  vertexCoords[10],   vertexCoords[11] ));
		}else{
			delete vertices;
			printf("\n Total Vertex coordinates for rectangle must be 4(vertices)*3(x|y|z) = 12.");
			for(int i=0; i<vertexCoords.size(); i++){
				printf("\n vertex[%d]: %d", i+1, (int)vertexCoords[i]);
			}
			assert(0);
		}
		rectangleItem->SetVertices(vertices);
	}

	if(texCoord){
		std::vector<glm::vec2>* texCoords = new std::vector<glm::vec2>;
		char* TOKEN = ",(){}[] ";
		std::vector<float> textureCoords;
		tokenizeFloat( (char*)texCoord, TOKEN, textureCoords);

		if(textureCoords.size() == 8){
			texCoords->push_back(glm::vec2( textureCoords[0], textureCoords[1] ));
			texCoords->push_back(glm::vec2( textureCoords[2], textureCoords[3] ));
			texCoords->push_back(glm::vec2( textureCoords[4], textureCoords[5] ));
			texCoords->push_back(glm::vec2( textureCoords[6], textureCoords[7] ));
		}else{
			delete texCoords;
			printf("\n Total Vertex coordinates for rectangle must be 4(vertices)*2(u|v) = 8.");
			for(int i=0; i<textureCoords.size(); i++){
				printf("\n textCoord[%d]: %d", i+1, (int)textureCoords[i]);
			}
			assert(0);
		}
		rectangleItem->SetTexCoords(texCoords);
	}

	if(color){
		char* TOKEN = ",(){}[] ";
		std::vector<float> colorRGBA;
		tokenizeFloat( (char*)color, TOKEN, colorRGBA);

		if(colorRGBA.size() == 4){
			rectangleItem->SetColor(&glm::vec4(colorRGBA[0], colorRGBA[1], colorRGBA[2], colorRGBA[3]));
		}else{
			printf("\n Total Vertex coordinates for rectangle must be 4(r|g|b|a).");
			for(int i=0; i<colorRGBA.size(); i++){
				printf("\n textCoord[%d]: %f", i+1, colorRGBA[i]);
			}
			assert(0);
		}
	}

	// Set the vertex information
	rectangleItem->SetProgram(ProgramID);
	AddToMapping(element, (Object*)rectangleItem);
    

	if(translate){
		char* TOKEN = ",(){}[]";
		std::vector<float> translateCoords;
		tokenizeFloat( (char*)translate, TOKEN, translateCoords);

		if(translateCoords.size() == 3){
			rectangleItem->Translate(translateCoords[0], translateCoords[1], translateCoords[2]);
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
		currentScene->addModel(rectangleItem);
	}

	return true;
}

bool SceneGraphParser::parseTagMesh(tinyxml2::XMLElement* element){
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
	const char* vertexShader	 = element->Attribute("vertexShader");
	const char* fragmentShader	 = element->Attribute("fragmentShader");
	const char* translate		 = element->Attribute("translate");
		
	//GRectangle* rectangleItem    = NULL;
	//rectangleItem = new GRectangle(currentScene, parentModel, BUTTON, name);
    MeshObject* mesh =  new MeshObject("../Resource/Models/mbclass.obj", currentScene, parentModel, MESH, name);
	mesh->Scale(.01, .01, .01);

	if(!mesh){
		printf("Unable to create the Mesh object %s, %s",__FUNCTION__, __LINE__);
	}

	if(vertexShader && fragmentShader){
		ProgramManagerObj = ProgramManager::GetInstance();
		ProgramID		  = ProgramManagerObj->LoadShader("Mesh", (char*)vertexShader, (char*)fragmentShader )->ProgramID;
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
	   
	// Set the vertex information
	mesh->SetProgram(ProgramID);
	AddToMapping(element, (Object*)mesh);

	if(translate){
		char* TOKEN = ",(){}[]";
		std::vector<float> translateCoords;
		tokenizeFloat( (char*)translate, TOKEN, translateCoords);

		if(translateCoords.size() == 3){
			mesh->Translate(translateCoords[0], translateCoords[1], translateCoords[2]);
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
		currentScene->addModel(mesh);
	}

	return true;
}
