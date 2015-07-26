#pragma once
#include "Application.h"
#include "Scene.h"
#include "HMIRectangle.h"
#include "Pixmap.h"
#include "MeshObject.h"
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
	bool parseTagApplication();
	
	//! Parse the "Plugin" tag
	bool parseTagPlugin(tinyxml2::XMLElement* element);

	//! Parse the "Renderer" tag
	bool parseTagRenderer(tinyxml2::XMLElement* element);

	//! Parse the "Scene" tag
	bool parseTagScene(tinyxml2::XMLElement* element);

	//! Parse the "Camera" tag
	bool parseTagCamera(tinyxml2::XMLElement* element);

	//! Parse the "Image" tag
	bool parseTagImage(tinyxml2::XMLElement* element);

	//! Parse the "Rectangle" tag
	bool parseTagRectangle(tinyxml2::XMLElement* element);

	//! Parse the "Mesh" tag
	bool parseTagMesh(tinyxml2::XMLElement* element);

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
