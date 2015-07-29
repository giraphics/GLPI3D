//
//  GLES20Viewport.h
//

#ifndef __GLES20VIEWPORT_H__
#define __GLES20VIEWPORT_H__

#include "glutils.h"
#include "Interfaces/ILifeCycle.h"
#include "scene_graph/Camera.h"

class GLES20Viewport : public ILifeCycle{
public:
	GLES20Viewport();

	~GLES20Viewport();

	virtual void Initialize();

   // Render the Model class, the function pointer here as an argument is not in use
   // The purpose of this argument is to override the default rendering behaviour thus giving much more flexibility to upper layer.
   virtual void Render(bool (*customRender)()=NULL);

   void Resize();
public:
    ViewPort            viewPortParam;
    CameraViewParams    cameraViewParameters;
    glm::vec4           clearColor;
    GLbitfield          clearBitFieldMask;


	inline void setScissorTest(bool flag){scissor = flag;}
	inline void setClearFlag(bool flag){clearFlag = flag;}

private:
	bool scissor;
	bool clearFlag;
};

#endif // GLES20VIEWPORT_H
 
