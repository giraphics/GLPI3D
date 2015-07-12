////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <conio.h>
//#define OLDENGINE 1
//#define OLDENGINE
#ifdef OLDENGINE
   #include "samples\glpi3d\glpi3dSample1.h"
#else
   #include "samples\GLES2_0_Renderer\luaSample1.h"
   #include "samples\GLES2_0_Renderer\DrawScheme.h"
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
	DrawScheme();

	SimpleAndHMIRectangle();  // HUD View, Simple, HMI rectangle overide render

   // HUD View, Window filled with interpolated color rectangles
   FilledWindow();			 

   // Perespective view
   SamplePerespective();	 
   
   // Parent child Transformation and Camera view
   SamplePerspctivePlusParentChild();
   
   // Perspective Image rotating about its own axises
   SampleImage();
   
   SampleMultipleImage();
	
   // Simple Diffuse mesh demonstrator
   DiffuseMeshSample();
   //return 0;

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

void test()
{
	char selection = getch();
	do{
	switch(selection){
		case 'a':
			// HUD View, Simple, HMI rectangle overide render
			SimpleAndHMIRectangle();  
		break;

		case 'b':
			// HUD View, Window filled with interpolated color rectangles
			FilledWindow();			 
		break;
		
		case 'c':
			// Perespective view
			SamplePerespective();
		break;

		case 'd':
			// Parent child Transformation and Camera view
			SamplePerspctivePlusParentChild();
		break;

		case 'e':
			// Perspective Image rotating about its own axises
			SampleImage();
	    break;
		
		case 'f':
			SampleMultipleImage();
		break;
		
		case 'g':
			// Simple Diffuse mesh demonstrator
			DiffuseMeshSample();
		break;
		
		case 'h':
			// Mesh with texture + Diffuse shader
			MeshSample();
		break;
   
		case 'i':
			// Multiple scenes in one renderer + Ortho + prespective
			MeshRectSample();
		break;

		case 'j':
			// Use simple object and produce transformation graph + Gouraud shading
			WindMillDemo();
		break;
   
		case 'k':
			// Custom Scene and Custom modeling. + Phong Shading
			CustomSceneDemo();
		break;
		case 'l':
			// Light and Material Demo
			LighMaterailDemo();
		break;

		case 'm':
			// Sample texture Menu
			SampleMenu();
		break;

		case 'n':
			// Frame Buffer Object Demo
			FBODemo();
		break;

		case 'o':
			// Cube Frame Buffer Object Demo
			CubeFBODemo();
		break;
	}
	}
	while(selection = getch());

}
