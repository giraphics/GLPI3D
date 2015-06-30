////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <GL/glew.h>
//#define OLDENGINE 1
//#define OLDENGINE
#ifdef OLDENGINE
   #include "samples\glpi3d\glpi3dSample1.h"
#else
   #include "samples\GLES2_0_Renderer\luaSample1.h"
   #include "samples\GLES2_0_Renderer\Sample0.h"
   #include "samples\GLES2_0_Renderer\Sample1.h"
   #include "samples\GLES2_0_Renderer\Sample2.h"
   #include "samples\GLES2_0_Renderer\Sample3.h"
   #include "samples\GLES2_0_Renderer\Sample4.h"
   #include "samples\GLES2_0_Renderer\SampleImage.h"
   #include "samples\GLES2_0_Renderer\sampleMultipleImage.h"
   #include "samples\GLES2_0_Renderer\MeshDiffuse.h"
   #include "samples\GLES2_0_Renderer\MeshTex.h"
   #include "samples\GLES2_0_Renderer\MeshCustomScene.h"
   #include "samples\GLES2_0_Renderer\WindMill.h"
   #include "samples\GLES2_0_Renderer\MeshWithTex.h"
   #include "samples\GLES2_0_Renderer\SampleMenu.h"
   #include "samples\GLES2_0_Renderer\FrameBufferObj.h"
   #include "samples\GLES2_0_Renderer\CubeFBO.h"
   #include "samples\GLES2_0_Renderer\LightMaterial.h"
#endif

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
#ifdef OLDENGINE
   GLPISample();
#else
   //Lua_Sample1();
   Sample0();  // HUD View, Simple
   Sample1();  // HUD View
   SamplePerespective();  // Perespective view
   SampleParentChild();
   SamplePerspctivePlusParentChild();
   SampleImage();
   SampleMultipleImage();
	
   // Simple Diffuse mesh demonstrator
   DiffuseMeshSample();

   // Mesh with texture + Diffuse shader
   MeshSample();
   
   // Multiple scenes in one renderer + Ortho + prespective
   MeshRectSample();

   // Use simple object and produce transformation graph + Gouraud shading
   WindMillDemo();
   
   // Custom Scene and Custom modeling. + Phong Shading
   CustomSceneDemo();

   // Light and Material Demo
   LighMaterailDemo();

   // Sample texture Menu
   SampleMenu();

   // Frame Buffer Object Demo
   FBODemo();

   // Cube Frame Buffer Object Demo
   CubeFBODemo();
#endif

   return EXIT_SUCCESS;
}


