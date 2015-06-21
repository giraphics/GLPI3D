#include "ObjectRelative.h"

ObjectRelative::ObjectRelative(std::string objectName, ObjectRelative* parentObj):Object(objectName)
{
    parent  = NULL; // Important: do not remove this check, the SetParent method checks for parent's Null pointer condition.
    SetParent(parentObj);
    return;
}

ObjectRelative::~ObjectRelative()
{
}

void ObjectRelative::SetParent(ObjectRelative* parentModel)
{
    // If there existing a parent
    if (this->parent != NULL){
        RemoveParent();
    }
    
    parent = parentModel;
    if(parent){
        parent->SetChild(this);
    }
}

void ObjectRelative::RemoveParent()
{
    RemoveFromParentChildList();
    parent = NULL;
}

void ObjectRelative::SetChild(ObjectRelative* child)
{
    for(int i =0; i<childList.size(); i++){
        if(child == childList.at(i)){
            return;
        }
    }
    child->parent = this;
    childList.push_back(child);
}

void ObjectRelative::RemoveFromParentChildList()
{
    for(int i =0; parent && i<parent->childList.size(); i++){
        if(this == parent->childList.at(i)){
            parent->childList.erase (parent->childList.begin()+i);
            return;
        }
    }
}

ObjectRelative* ObjectRelative::GetParent(){
    return parent;
}

std::vector<ObjectRelative*>* ObjectRelative::GetChildren()
{
    return &childList;
}
