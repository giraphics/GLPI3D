#ifndef OBJECT_H
#define OBJECT_H

#include "../gles2_0_renderer/constant.h"
#include "Object.h"
#include <vector>
#include <typeinfo>

// Parminder: Interface for the rendering models.
class ObjectRelative : public Object
{
public:
    ObjectRelative(std::string name = "", ObjectRelative* parentObj = NULL);
    
    virtual ~ObjectRelative();
    
    void SetParent(ObjectRelative* parent = 0);
    
    void RemoveParent();
    
    void SetChild(ObjectRelative* child = 0);
    
    void RemoveFromParentChildList();
    
    ObjectRelative*  GetParent();
    
    std::vector<ObjectRelative*>* GetChildren();

protected:

    /*!
     Model's parent
     */
    ObjectRelative* parent;
    
    /*!
     Model's child list
     */
    std::vector<ObjectRelative*> childList;
};

#endif // OBJECT_H
