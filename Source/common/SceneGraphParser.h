#pragma once
#include "Application.h"
#include "Scene.h"
#include "HMIRectangle.h"
#include "Pixmap.h"
#include "ImageManager.h"
#include "../gles2_0_renderer/scene_graph/Camera.h"
#include "Plugin.h"
#include "TinyXML2.h"
#include <map>
#include <list>
#include <vector>

class SceneGraphParser{
public:
	//! SceneGraphParser Constructor
	SceneGraphParser();

	//! SceneGraphParser Destructor
	~SceneGraphParser();

	//! Load Scene Graph XML files
	Application* LoadXML(const char* filePath);

	//! Get the objects of Scene Graph tree
	Object* getObjectByName(char* name);

private:
	/************** PRIVATE MEMBER FUNCTIONS **************/

	//! Add maping between XML element and Scene graph objects
	void AddToMapping(tinyxml2::XMLElement* elementItem, Object* objectItem);

	//! Get all the same level nodes
	std::vector<tinyxml2::XMLNode*> getSiblings(tinyxml2::XMLNode* node);

	//! Tokenize function to produce a list of float elements
	void tokenizeFloat( char* string,char* token, std::vector<float>& tokenList);

	//! Handle the XML tags like "Application", "Renderer", "Scene" etc.
	void handleTags(const char* name, tinyxml2::XMLElement* element);

	//! Recursive method to parse the Scene-Graph XML
	void parseNodeInfo(tinyxml2::XMLNode* node);

	//! Parse the "Application" tag
	void parseTagApplication();
	
	//! Parse the "Plugin" tag
	void parseTagPlugin(tinyxml2::XMLElement* element);

	//! Parse the "Renderer" tag
	void parseTagRenderer(tinyxml2::XMLElement* element);

	//! Parse the "Scene" tag
	void parseTagScene(tinyxml2::XMLElement* element);

	//! Parse the "Camera" tag
	void parseTagCamera(tinyxml2::XMLElement* element);

	//! Parse the "Image" tag
	void parseTagImage(tinyxml2::XMLElement* element);

	/************** PRIVATE MEMBER VARIABLES **************/

	//! Application class object
	Application* application;
	
	//! Store the current scene while parsing the XML
	Scene* currentScene;
	
	//! XML document
	tinyxml2::XMLDocument doc;

	//! Map between XML element and Scene graph object
	std::map<tinyxml2::XMLElement*, Object*> mapInfo;

	//! Map iterator between XML element and Scene graph object
	typedef std::map<tinyxml2::XMLElement*, Object*>::iterator itor;
};
