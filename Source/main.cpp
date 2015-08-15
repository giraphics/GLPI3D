////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
/*! \mainpage Manual SDK
*
* \section intro_sec Introduction
* Features				                                                                | Done		| Sample Demo
* ------------------------------------------------------------------------------------- |-----------|------------
* This is introduction																	| Yes		| Yes
* Flat Rendering System                    												| Yes		| Partially implemented Vulkan pipeline need to be put in
* Separate Transformation Graph System													| Yes		| Yes
* Multiple Pipeline Support                 											| Yes		| Demo available in old implementation
* Multiple Thread Support                   											| Yes		| Yes
* Shared and Local Resources Handling       											| Yes		| Yes
* Shader Mgmt:- Dedicated Shader, Shared Shader											| Yes		| Yes
* Uniform and Attribute Automation             											| Yes		| Yes
* Transformation class for seamless 2D and 3D  											| Yes		| Yes
* Frame Buffer Object Support															| Yes		| Yes
* Texture Mangment classes for 2D/3D            										| Yes		| Yes
* Compressed Texture(ETC2)                   											| No		| No
* Run Primitive Support for Point, Line, Triangle Variants								| Yes		| Yes
* Drawing Scheme: Array, Element                   										| Yes		| Yes (See.. DrawScheme)
* Buffer Scheme: VA, VBO, VAO                   										| Yes		| Yes (See.. BufferScheme)
* View and Camera system: Ortho and Perespective										| Yes		| Yes
* Texture Caching Support: retrive object by user defined name  						| Yes		| Yes
* Shader Caching Support: retrive object by user defined name   						| Yes		| Yes
* Light and Material Handling                   										| Yes		| Yes
* Per-Vertex and Per-Fragment shading with Gouraud and Phong Shading					| Yes		| Yes
* Custom Model, Custom Scenes, Extendable classes for HMI								| Yes		| Yes
* Visible flag for hierarchical controling of visibility.                   			| Yes		| No (See.. XMLDemo)
* Parent-Child care                   													| Yes		| Yes (See.. )
* Code Style: XML and C++                   											| Yes		| Yes (See.. XMLDemo)
* Light type: Point, Directional, Spot 													| No		| No
* Multi-Texture support																	| Yes		| No
* Procedural texturing                   												| Infrasture is avialable, the demo required		| No
* Font rendering                   														| No		| Font rendering is available in old design
* Central rendering management with Geometry Buffer- Rect, Img, Mesh					| Yes		| Yes
* Texture-Atlas                   														| Yes		| Available in old design for demo
* Ray traced object selection                   										| No		| No
* Anti-Aliasing Software                   												| No		| No
* Anti-Aliasing Hardware                   												| No		| No
* Real-time Shadow                   													| Infrasture is avialable, the demo required to test		| No
* Post processing on real time scenes                   								| Infrasture is avialable, the demo required to test		| No
* Bump Mapping                   														| Infrasture is avialable, the demo required to test		| No
* Streaming                                                                             | Demo      | Yes
* *
*/

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
   #include "samples\GLES2_0_Renderer\FlatSmooth.h"
   #include "samples\GLES2_0_Renderer\ProceduralShading.h"
   #include "samples\GLES2_0_Renderer\InDashBoard.h"
   #include "samples\GLES2_0_Renderer\InDashBoardPrespective.h"
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
    demoList.push_back(StartUpAnimationPrespective);

    demoList.push_back(StartUpAnimation);

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

	// Procedural shading
	demoList.push_back(ProceduralShadingDemo);

	// Flat and Smooth shading
	demoList.push_back(FlatAndSmoothShadingDemo);

    // Sample texture Menu
	demoList.push_back(SampleMenu);

	// Frame Buffer Object Demo
	demoList.push_back(FBODemo);
  
	// Cube Frame Buffer Object Demo
	demoList.push_back(CubeFBODemo);
	
	demoList.push_back(XMLDemo);

	int totalSize = demoList.size();
	while(totalSize){
		demoList.at(demoNumber<0 ? demoNumber=0 : demoNumber%(totalSize))();
	}
 #endif

   return EXIT_SUCCESS;
}


