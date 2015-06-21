#pragma once
#include "Plugin.h"

class GLES20Plugin : public Plugin
{
public:
   //! GLES20Plugin CTOR
   GLES20Plugin(void);

   //! GLES20Plugin DTOR
   virtual ~GLES20Plugin(void);

   //! GLES20Plugin unloading
   bool unload();

   //! Create the renderer
   Renderer* createRenderer(int width = 800, int height = 600);
};
