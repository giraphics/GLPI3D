#ifndef XML_DEMO_H
#define XML_DEMO_H
#include "Common/SceneGraphParser.h"

// Demo Description: In this demo, we will draw two rectangle with Array and Element (Indices) drawing scheme.
void XMLDemo(){
	SceneGraphParser parser;
	Application* application = parser.LoadXML("E:/renderconcept/RenderConcept2010-ILifeCycle/RenderConcepts/Source/samples/GLES2_0_Renderer/MenuXML.xml");
	if(!application){
		printf("Unable to create the application.");
		assert(0);
	}

	application->Initialize();

	Renderer* renderer = (Renderer*)parser.getObjectByName("OGL_Renderer_1");
	while(renderer->getWindow()->isOpen()){
		application->Render();
	}

	delete application;
	application = NULL;
	ProgramManager::DeleteInstance();
}


#endif
