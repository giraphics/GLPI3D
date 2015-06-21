//
//  IMesh.h
//
//

#ifndef __IMesh__
#define __IMesh__
#include "IRenderable.h"
// This class is not inherited from IRenderable because the meshes itself contains all the nessary information 
// for vertices, color, texture and many other attributes

//Place holder of Mesh extension
class IMesh : public IModel, public ILifeCycle
{
public:
    // Constructor for IMesh
   IMesh(){}
   
   // Destructor for IMesh
   virtual ~IMesh(){}
};
#endif // IMESH_H
 
