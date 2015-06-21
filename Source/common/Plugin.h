#pragma once
#include <string>
#include "ILifeCycle.h"
#include "Object.h"
#include "Renderer.h"

class Plugin :public ILifeCycle, public Object
{
public:
   //! Plugin CTOR
   Plugin(void);

   //! Plugin DTOR
   virtual ~Plugin(void);

   //! Plugin loading
   virtual Plugin* load(PluginType);

   //! Plugin unloading
   virtual bool unload() = 0;

   //! Create Renderer
   virtual Renderer* createRenderer(int width = 800, int height = 600) = 0;

   //! Plugin initialization
   virtual void Initialize();

   //! Plugin resize handling, when ever the diemension of the windows changes.
   virtual void Resize( int w, int h );

   virtual void Render(bool (*customRender)()=NULL);

protected:
   Renderer* currentRenderer;

   std::vector<Renderer*> renderEngines;
};
