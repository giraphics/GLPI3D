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
   DiffuseMeshSample();
   MeshSample();
   MeshRectSample();
   WindMillDemo();
   CustomSceneDemo();
#endif

   return EXIT_SUCCESS;
}


