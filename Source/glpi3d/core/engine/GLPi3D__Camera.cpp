#include "GLPi3D__Camera.h"

namespace GLPi3D
{

//! Constructor
Camera::Camera(void)
{
   CamData.left         =  3.0f;
   CamData.right        = -3.0f;
   CamData.bottom       =  3.0f;
   CamData.top          = -3.0f;
   CamData.clip_start   =  0.01f;
   CamData.clip_end     =  1000.0f;
   CamData.fov          =  60.0f;

   CamData.WorldCameraPosition    = glm::vec3(0.0, 1.0, -5.0);
   CamData.WorldTargetPosition    = glm::vec3(0.0, 0.0, 0.0);
   CamData.WorldUp                = glm::vec3(0.0, 1.0, 0.0);
}

//! Destructor
Camera::~Camera(void)
{
}

/*!
	Setup of the view port and projection initialization. In projection initialization the Ortho or Perespective is set
	as per requirement.

	\param[in] width of the screen.
	\param[in] height of the screen.

	\return void.
*/
void Camera::cameraOrtho(float left, float right, float bottom, float top, float clip_start, float clip_end)
{
   CamData.isProjectionSystemOrtho  = true;
   CamData.left                     = left;
   CamData.right                    = right;
   CamData.bottom                   = bottom;
   CamData.top                      = top;
   CamData.clip_start               = clip_start;
   CamData.clip_end                 = clip_end;

   //Initialize Scree params
   CamData.lowerLeftX               = 0;
   CamData.lowerLeftY               = 0;
   CamData.screenWidth              = 800;
   CamData.screenHeight             = 600;
}

/*!
	Setup of the view port and projection initialization. In projection initialization the Ortho or Perespective is set
	as per requirement.

	\param[in] width of the screen.
	\param[in] height of the screen.

	\return void.
*/
void Camera::cameraPrespective(float fovy, float clip_start, float clip_end)
{
   CamData.isProjectionSystemOrtho  = false;
   CamData.fov                      = 60.0f;
   CamData.clip_start               = clip_start;
   CamData.clip_end                 = clip_end;
}

void Camera::setCameraView(glm::vec3 camPos, glm::vec3 target, glm::vec3 upDir)
{
   CamData.WorldCameraPosition    = camPos;
   CamData.WorldTargetPosition    = target;
   CamData.WorldUp                = upDir;
}

void Camera::cameraViewport(float lowerLeftX, float lowerLeftY, float width, float height)
{
   CamData.lowerLeftX      = (int)lowerLeftX;
   CamData.lowerLeftY      = (int)lowerLeftY;
   CamData.screenWidth     = (int)width;
   CamData.screenHeight    = (int)height;
   glViewport( (GLint)lowerLeftX, (GLint)lowerLeftY, (GLsizei)width, (GLsizei)height );
}

}

