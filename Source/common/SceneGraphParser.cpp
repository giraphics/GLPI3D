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
