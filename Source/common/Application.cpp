#include "Application.h"
#include "GLES20Renderer.h"
#include "ImageManager.h"
#include "Plugin.h"
#include "GLES20Plugin.h"
#include "GLES31Plugin.h"
#include "VulkanPlugin.h"

Application::Application(int argc, char** argv)
{
   parseCommandLineParam(argc, argv);
}

Application::~Application()
{
   for (std::vector<Plugin*>::iterator it = pluginList.begin() ; it != pluginList.end(); ++it){
      delete (*it);
      (*it) = NULL;
   }

   ImageManager::DeleteInstance();
}

void Application::parseCommandLineParam(int argc, char** argv)
{
}

Plugin* Application::loadPlugin(PluginType plugin)
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

   if(currentPlugin){
		pluginList.push_back(currentPlugin);
   }

   return currentPlugin;
}

void Application::Render(bool (*customRender)())
{
   for (std::vector<Plugin*>::iterator it = pluginList.begin() ; it != pluginList.end(); ++it){
      (*it)->Render();
   }
}

//! resize screen
void Application::Resize( int w, int h )
{
   for (std::vector<Plugin*>::iterator it = pluginList.begin() ; it != pluginList.end(); ++it){
      (*it)->Resize(w, h);
   }
}

void Application::Initialize()
{
   for (std::vector<Plugin*>::iterator it = pluginList.begin() ; it != pluginList.end(); ++it){
      (*it)->Initialize();
   }
}
