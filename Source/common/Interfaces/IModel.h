//
//  IModel.h
//
//

#ifndef __IMODEL__
#define __IMODEL__
#include "glutils.h"
#include "TranformationMatrix.h"

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

protected:
	// Store the ID of the current shader program handle locally in class object
	unsigned int ProgramID;
};

#endif // __IMODEL__
 
