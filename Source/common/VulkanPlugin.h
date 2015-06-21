#pragma once
#include "Plugin.h"

class VulkanPlugin : public Plugin
{
public:
   //! VulkanPlugin CTOR
   VulkanPlugin(void);

   //! VulkanPlugin DTOR
   virtual ~VulkanPlugin(void);

   //! VulkanPlugin loading
   bool load();

   //! VulkanPlugin unloading
   bool unload();

   //! Create the renderer
   Renderer* createRenderer(int width = 800, int height = 600);
};
