//
//  IModel.h
//
//

#ifndef __IMODEL__
#define __IMODEL__

class TransformationMatrix{
public:
	TransformationMatrix(){}
	~TransformationMatrix(){}
	/*!
	Set the Model view projection matrix here.
	*/
	virtual void SetModelMat(glm::mat4* ModelMat){ ModelMatrix = ModelMat; }

	/*!
	Set the Model view projection matrix here.
	*/
	virtual void SetViewMat(glm::mat4* ViewMat){ ViewMatrix = ViewMat; }

	/*!
	Set the Model view projection matrix here.
	*/
	virtual void SetProjectionMat(glm::mat4* ProjectionMat){ ProjectionMatrix = ProjectionMat; }
	glm::mat4* getTempMatrix(){return &tempMatrix;}
	glm::mat4* getTempMVPMatrix(){return &tempMVPMatrix;}
protected:
	// Model Matrix
	glm::mat4* ModelMatrix;
	
	// View Matrix
	glm::mat4* ViewMatrix;

	// Projection Matrix
	glm::mat4* ProjectionMatrix;

	// This is a temporary matrix which is use to store temporary calculation like MVP, MV, normalMatrix 
	// and send to the shader program. This way we avoid any temporary local matrices on the stack.
	glm::mat4 tempMatrix;
	glm::mat4 tempMVPMatrix;
};

class IModel
{
public:
   IModel(){}

    // Destructor for ObjLoader
   virtual ~IModel(){}

	/*!
	Set the program on run time.
	*/
	virtual void SetProgram(unsigned int ID){ProgramID = ID;}

	/*!
	Set the program on run time.
	*/
	virtual unsigned int GetProgram(){ return ProgramID; };

	/*!
	Set the Model view projection matrix here.
	*/
	//virtual void SetModelMat(glm::mat4* ModelMat){ ModelMatrix = ModelMat; }

	/*!
	Set the Model view projection matrix here.
	*/
	//virtual void SetViewMat(glm::mat4* ViewMat){ ViewMatrix = ViewMat; }

	/*!
	Set the Model view projection matrix here.
	*/
	//virtual void SetProjectionMat(glm::mat4* ProjectionMat){ ProjectionMatrix = ProjectionMat; }

protected:
/*
	// Model Matrix
	glm::mat4* ModelMatrix;
	
	// View Matrix
	glm::mat4* ViewMatrix;

	// Projection Matrix
	glm::mat4* ProjectionMatrix;

	// This is a temporary matrix which is use to store temporary calculation like MVP, MV, normalMatrix 
	// and send to the shader program. This way we avoid any temporary local matrices on the stack.
	glm::mat4 tempMatrix;
	glm::mat4 tempMVPMatrix;*/

	// Store the ID of the current shader program handle locally in class object
	unsigned int ProgramID;
};

#endif // __IMODEL__
 
