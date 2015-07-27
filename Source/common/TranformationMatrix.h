#pragma once

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
	
	/*!
	TempMatrix for MV
	*/
	glm::mat4* getTempMatrix(){return &tempMatrix;}

	/*!
	TempMatrix for MVP
	*/
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
