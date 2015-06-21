#ifndef GRLC_CAMERA_H
#define GRLC_CAMERA_H
#include "GLPi3D__Constant.h"
namespace GLPi3D
{

//! Render Member Data structure
typedef struct
{
   // !Frustum Left, used by ortho
   float left;

   // !Frustum Right,, used by ortho
   float right;

   // !Frustum Bottom, used by ortho
   float bottom;

   // !Frustum Top, used by ortho
   float top;

   // !Frustum Near, used by prespective and ortho
   float clip_start;

   // !Frustum Far, used by prespective and ortho
   float clip_end;

   // !Frustum Field of view, used by prespective
   float fov;

   // !World coordinates position for the camera place in 3D space
   glm::vec3 WorldCameraPosition;

   // !World coordinates position for the target where camera want to look at in 3D space
   glm::vec3 WorldTargetPosition;

   // !Direction of the camera head in world coordiantes.
   glm::vec3 WorldUp;

   //! projection system Identifier
   uint16 isProjectionSystemOrtho;

   //! Viewport lowerLeftX
   int lowerLeftX;

   //! Viewport lowerLeftY
   int lowerLeftY;

   //! Screen Height
   int screenHeight;

   //! Screen Width
   int screenWidth;

} tstCameraData;

class Camera
{

public:
   //! Private Default Constructor
   Camera(void);

   //! Destructor
   ~Camera(void);

   //! Set Orthographics Project System for current camera
   void cameraOrtho(float left, float right, float bottom, float top, float clip_start, float clip_end);

   //! Set Prespective Projection System for current camera
   void cameraPrespective(float fovy, float clip_start, float clip_end);

   //! Set viewport
   void cameraViewport(float lowerLeftX = 0.0, float lowerLeftY = 0.0, float width = 800.0, float height = 600);

   // Set camera position and the target object toward it looks.
   void setCameraView(glm::vec3 WorldCameraPosition, glm::vec3 WorldTargetPosition, glm::vec3 WorldUp);
public: 
   //Scope made public for member variable as these are in frequent use

   //! Camera member variable container
   tstCameraData CamData;
};
}


#endif // GRLC_CAMERA_H
