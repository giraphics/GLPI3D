#include "Plugin.h"
#include "GLES20Plugin.h"
#include "GLES31Plugin.h"
#include "VulkanPlugin.h"

Plugin::Plugin(void)
{
}

Plugin::~Plugin(void)
{
   for (std::vector<Renderer*>::iterator it = renderEngines.begin() ; it != renderEngines.end(); ++it){
      delete (*it);
      (*it) = NULL;
   }
}

Plugin* Plugin::load(PluginType plugin)
{
   Plugin* currentPlugin = NULL;
   switch( plugin )
   {
      case OPENGLES20_STATIC_PLUGIN:
         currentPlugin = new GLES20Plugin();
      break;

      case OPENGLES31_STATIC_PLUGIN:
		  currentPlugin = new GLES31Plugin();
      break;

      case VULKAN_STATIC_PLUGIN:
		  currentPlugin = new VulkanPlugin();
      break;

      case JCP2016_STATIC_PLUGIN:
		  // plugin not available
      break;

      default:
      break;
   }

   return currentPlugin;
}

void Plugin::Render(bool (*customRender)())
{
   for (std::vector<Renderer*>::iterator it = renderEngines.begin() ; it != renderEngines.end(); ++it){
      currentRenderer = (*it);
	  currentRenderer->Render();
   }
}

void Plugin::Initialize()
{
   for (std::vector<Renderer*>::iterator it = renderEngines.begin() ; it != renderEngines.end(); ++it){
      currentRenderer = (*it);
	  currentRenderer->Initialize();
   }
}

void Plugin::Resize( int w, int h )
{
   for (std::vector<Renderer*>::iterator it = renderEngines.begin() ; it != renderEngines.end(); ++it){
      currentRenderer = (*it);
	  currentRenderer->Resize(w, h);
   }
}