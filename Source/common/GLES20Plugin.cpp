#include "GLES20Plugin.h"
#include "GLES20Renderer.h"

GLES20Plugin::GLES20Plugin(void)
{
}

GLES20Plugin::~GLES20Plugin(void)
{
}

bool GLES20Plugin::unload()
{
   printf("\nInside [%s]: %s", name.c_str(), __FUNCTION__);
   return true;
}

Renderer* GLES20Plugin::createRenderer(int width, int height)
{
   currentRenderer = new GLES20Renderer(width, height);
   if(currentRenderer){
		renderEngines.push_back(currentRenderer);
   }
   return currentRenderer;
}
