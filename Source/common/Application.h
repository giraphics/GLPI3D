#pragma once
#include "Object.h"
#include "Renderer.h"
#include "Interfaces/ILifeCycle.h"
#include <vector>
class Plugin;

class Application : public ILifeCycle, public Object
{
public:
   Application(int argc = 0, char** argv = NULL);

   virtual ~Application();

   void parseCommandLineParam(int argc, char** argv);

   Plugin* loadPlugin(PluginType plugin);
   
   //! resize screen
   void Resize( int w, int h );

   void Render(bool (*customRender)()=NULL);

   void Initialize();

private:
   std::vector<Plugin*> pluginList;
};

