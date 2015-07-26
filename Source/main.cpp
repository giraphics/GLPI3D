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
   #include "samples\GLES2_0_Renderer\BufferScheme.h"
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
   #include "samples\GLES2_0_Renderer\City.h"
   #include "samples\GLES2_0_Renderer\XMLDemonstrator.h"
#endif
std::vector<void (*)()> demoList;
int demoNumber = 0;
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
	demoList.push_back(XMLDemo);

	demoList.push_back(DrawScheme);
	
	demoList.push_back(BufferScheme);

	// HUD View, Simple, HMI rectangle overide render
	demoList.push_back(SimpleAndHMIRectangle);

   // HUD View, Window filled with interpolated color rectangles
	demoList.push_back(FilledWindow);

   // Perespective view
	demoList.push_back(SamplePerespective);
   
   // Parent child Transformation and Camera view
	demoList.push_back(SamplePerspctivePlusParentChild);
   
   // Perspective Image rotating about its own axises
	demoList.push_back(SampleImage);
	demoList.push_back(SampleMultipleImage);
   
	// Simple sci-fi City mesh
	demoList.push_back(SciFiSample);
	
   // Simple Diffuse mesh demonstrator
	demoList.push_back(DiffuseMeshSample);

   // Mesh with texture + Diffuse shader
	demoList.push_back(MeshSample);
   
   // Multiple scenes in one renderer + Ortho + prespective
	demoList.push_back(MeshRectSample);

   // Use simple object and produce transformation graph + Gouraud shading
 	demoList.push_back(WindMillDemo);
   
   // Custom Scene and Custom modeling. + Phong Shading
	demoList.push_back(CustomSceneDemo);

    // Light and Material interaction demo
	demoList.push_back(LighMaterailDemo);

   // Sample texture Menu
	demoList.push_back(SampleMenu);

   // Frame Buffer Object Demo
	demoList.push_back(FBODemo);

   // Cube Frame Buffer Object Demo
	demoList.push_back(CubeFBODemo);
	
	int totalSize = demoList.size();
	while(totalSize){
		demoList.at(demoNumber<0 ? demoNumber=0 : demoNumber%(totalSize))();
	}
#endif

   return EXIT_SUCCESS;
}


