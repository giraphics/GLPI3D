#include "VulkanPlugin.h"

VulkanPlugin::VulkanPlugin(void)
{
}

VulkanPlugin::~VulkanPlugin(void)
{
}

bool VulkanPlugin::load()
{
   printf("\nInside [%s]: %s", name.c_str(), __FUNCTION__);
   return true;
}

bool VulkanPlugin::unload()
{
   printf("\nInside [%s]: %s", name.c_str(), __FUNCTION__);
   return true;
}

Renderer* VulkanPlugin::createRenderer(int width, int height)
{
   // Place holder for Vulkan Pipeline
   currentRenderer = NULL;
   if(currentRenderer){
		renderEngines.push_back(currentRenderer);
   }
   return currentRenderer;
}
