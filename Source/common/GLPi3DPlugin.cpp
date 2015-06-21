#include "GLPi3DPlugin.h"

GLPi3DPlugin::GLPi3DPlugin(void)
{
}

GLPi3DPlugin::~GLPi3DPlugin(void)
{
}

bool GLPi3DPlugin::load()
{
   printf("\nInside [%s]: %s", name.c_str(), __FUNCTION__);
   return true;
}

bool GLPi3DPlugin::unload()
{
   printf("\nInside [%s]: %s", name.c_str(), __FUNCTION__);
   return true;
}

Renderer* GLPi3DPlugin::createRenderer(int width, int height)
{
   // Place holder for GLPi3DPlugin Pipeline
   currentRenderer = NULL;
   if(currentRenderer){
		renderEngines.push_back(currentRenderer);
   }
   return currentRenderer;
}
