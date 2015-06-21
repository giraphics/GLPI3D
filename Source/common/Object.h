#pragma once
#include <string>



enum PluginType{
	//! The Triangle Model identifier.
	NONE = 0,
	OPENGLES20_STATIC_PLUGIN = 0,
	OPENGLES31_STATIC_PLUGIN = 1,
	VULKAN_STATIC_PLUGIN = 2,
	JCP2016_STATIC_PLUGIN = 3,
};
class IObject
{
public:
   IObject(){}

   virtual ~IObject(){}

   virtual void SetName(std::string mdlName) = 0;

   virtual std::string GetName() = 0;
};

class Object : public IObject
{
public:
    Object(std::string name = "");
    
    virtual ~Object();

    void SetName(std::string mdlName){ name = mdlName;}
    
    std::string GetName(){return name;}

protected:
    /*!
     Model's name
     */
    std::string name;
};

