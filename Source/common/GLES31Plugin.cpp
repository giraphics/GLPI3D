#include "GLES31Plugin.h"

GLES31Plugin::GLES31Plugin(void)
{
}

GLES31Plugin::~GLES31Plugin(void)
{
}

bool GLES31Plugin::load()
{
   printf("\nInside [%s]: %s", name.c_str(), __FUNCTION__);
   return true;
}

bool GLES31Plugin::unload()
{
   printf("\nInside [%s]: %s", name.c_str(), __FUNCTION__);
   return true;
}

Renderer* GLES31Plugin::createRenderer(int width, int height)
{
   // Place holder for GLES 31 Pipeline
   currentRenderer = NULL;
   if(currentRenderer){
		renderEngines.push_back(currentRenderer);
   }
   return currentRenderer;
}
