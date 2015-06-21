#pragma once
#include "Plugin.h"

class GLPi3DPlugin : public Plugin
{
public:
   //! GLPi3DPlugin CTOR
   GLPi3DPlugin(void);

   //! GLPi3DPlugin DTOR
   virtual ~GLPi3DPlugin(void);

   //! GLPi3DPlugin loading
   bool load();

   //! GLPi3DPlugin unloading
   bool unload();

   //! Create the renderer
   Renderer* createRenderer(int width = 800, int height = 600);
};
