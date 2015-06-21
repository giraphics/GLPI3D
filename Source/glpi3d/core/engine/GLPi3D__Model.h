#ifndef MODEL_H
#define MODEL_H

#include "GLPi3D__Constant.h"
#include "GLPi3D__Programmanager.h"
#include <vector>
#include "GLPi3D__Transform.h"

namespace GLPi3D
{

//! Forward declaration
class Scene;


// Parminder: Interface for the rendering models.
class Model
{
public:
    Model(ModelType type, Model* model = NULL);
    virtual ~Model();
    /*!
      Load textures, initialize shaders, etc.
      */
    virtual void InitModel();

    /*!
      This is called prior to every frame.  Use this
      to update your animation.
      */
	virtual void Update( float t ) {}

    /*!
      Draw your Model.
      */
    virtual void Render() = 0;

    /*!
      This is called prior to every frame.  Use this
      to update your animation.
      */
	virtual void SetGLStates( ) {}

    /*!
      This is called prior to every frame.  Use this
      to update your animation.
      */
	virtual void ResetGLStates( ) {}

    /*!
      Called when screen is resized
      */
	virtual void Resize(int, int) {}


    /*!
      Return the model type that uniquely recognized among various models.
      */
	virtual ModelType GetModelType() { return modelType; }

    /*!
      Called when screen touch down or mouse button pressed
      */
   virtual void TouchEventDown( unsigned int x, unsigned int y ){}

    /*!
      Called when gesture move or mouse move
      */
   virtual void TouchEventMove( unsigned int x, unsigned int y ){}

    /*!
      Called when screen touch realease or mouse released
      */
   virtual void TouchEventRelease( unsigned int x, unsigned int y ){}

    /*!
      Called when key is pressed.
      */
   virtual void KeyPress( unsigned int key ){}

    /*!
      Called when key is released.
      */
   virtual void KeyRealease( unsigned int key ){}

   void Rotate( float angle, float x, float y, float z );

   void Translate(float x, float y, float z );

   void Scale(float x, float y, float z );

   void RotateLocal( float angle, float x, float y, float z );

   void TranslateLocal(float x, float y, float z );

   void ScaleLocal(float x, float y, float z );

   void SetParent(Model* parent = 0);

   Model* GetParent();

   void RemoveParent();

   void SetChild(Model* child = 0);

   void GetParent(Model* parent = 0);

   std::vector<Model*>* GetChildren();

   void RemoveFromParentsChildList();

   void SetMaterial(glm::vec4 materialColor);

   void SetScene(Scene* scene);

protected:
    /*!
      Store current program ID
      */
	unsigned int	ProgramID;

	/*!
      Store current program metaData
      */
	tstProgram*		program;

	/*!
      Store current program handler
      */
	ProgramManager* ProgramManagerObj;

	/*!
      Handle Projection-View current program metaData
      */
	Transform*	TransformObj;

	/*!
      Drawing engine
      */
	Scene*	sceneHandler;

	/*!
      Type of the model rendered
      */
	ModelType		modelType;
    
    /*!
    Transformation of the model
     */
    glm::mat4 transformation;

    /*!
     Local transformation of the model
     */
    glm::mat4 transformationLocal;
    
    /*!
     Model's parent
     */
    Model* parent;
    
    /*!
     Model's child list
     */
    std::vector<Model*> childList;
   glm::vec4 material;
};
}


#endif // MODEL_H
