//
//  ILifeCycle.h
//
//

#ifndef __ILIFECYCLE__
#define __ILIFECYCLE__

// The ILifeCycle interface provide bare minimum interfaces for management of the life cycle of modules
// These are: initialization, processing or updating, change in dimensions, set and reseting states.
class ILifeCycle
{
public:
   ILifeCycle(){}

    // Destructor for ObjLoader
   virtual ~ILifeCycle(){}

    // Initialize our Model class
   virtual void Initialize() = 0;

   // Render the Model class, the function pointer here as an argument is not in use
   // The purpose of this argument is to override the default rendering behaviour thus giving much more flexibility to upper layer.
   virtual void Render(bool (*customRender)()=NULL) = 0;

   // For deterministic rendering, currently empty, this is used to perform processing
   virtual void Update( float t ){}

    /*!
      Called when window/screen/viewport size changes as result of change in orientation or window resizing.
      */
	virtual void Resize(int width, int height) {}

	/*!
      The method will be called before rendering, this help to set the different states of the model.
      */
	virtual void setStates(){}

	/*!
      The method will be called after rendering, this help to set the different states of the model.
      */
	virtual void releaseStates(){}
};


#endif // __ILIFECYCLE__
 
