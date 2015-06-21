#pragma once
#include "Plugin.h"

class GLES31Plugin : public Plugin
{
public:
   //! GLES31Plugin CTOR
   GLES31Plugin(void);

   //! GLES31Plugin DTOR
   virtual ~GLES31Plugin(void);

   //! GLES31Plugin loading
   bool load();

   //! GLES31Plugin unloading
   bool unload();

   //! Create the renderer
   Renderer* createRenderer(int width = 800, int height = 600);
};
