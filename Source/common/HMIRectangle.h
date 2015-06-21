//
//  HMIRectangle.h
//

#ifndef __HMIRectangle__
#define __HMIRectangle__
#include "Rectangle.h"

#include <iostream>


class HMIRectangle : public GRectangle
{
    
public:
    // Constructor for Rectangle
   HMIRectangle( Scene* parent, Model* model, ModelType type, std::string objectName = "");

    // Destructor for ObjLoader
    virtual ~HMIRectangle();

    // Initialize our Model class
    void Initialize();
    
    // Render the Model class
    void Render(bool (*customRender)()=NULL);

    void SetVertices(glm::vec3* vertices);

protected:
//	ProgramManager* ProgramManagerObj;
};

#endif // HMI_Rectangle_H
 
