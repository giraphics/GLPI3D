#include "GLPi3D__Model.h"
#include "GLPi3D__Renderer.h"

namespace GLPi3D
{

Model::Model(ModelType type, Model* model)
{
   if(model){
      parent = model;
      parent->SetChild(this);
   }

   modelType            = type;
   transformation       = glm::mat4();
   transformationLocal  = glm::mat4();
   sceneHandler         = NULL;
   ProgramManagerObj    = NULL;
   TransformObj         = NULL;
   material             = glm::vec4(1.0, 0.64, 0.0, 1.0);
}

Model::~Model()
{
    RemoveParent();
    RemoveFromParentsChildList();
}
/*Model::Model( ModelType typeOfModel, Model* parentModel ){
   modelType         = typeOfModel;
   parent            = parentModel;
   bool duplicate    = false;
   for(int i=0; parent && i<parent->childList.size(); i++){
      if(parent->childList.at(i) == this){
         duplicate = true;
         break;
      }
   }

   if(!duplicate && parent){
      parent->childList.push_back(this);
   }

   sceneHandler      = GLPi3D::Scene::Instance();
   ProgramManagerObj = Scene::Instance()->ProgramManager();
   TransformObj      = Scene::Instance()->Transformation();
   material          = glm::vec4(1.0, 0.64, 0.0, 1.0);
}*/

void Model::SetScene(Scene* scene)
{
   if(!scene){
      return;
   }

   sceneHandler      = scene;
   ProgramManagerObj = scene->ProgramManager();
   TransformObj      = scene->Transformation();

   //Propagate information to child level
   for(unsigned int i=0; i<GetChildren()->size(); i++){
      GetChildren()->at(i)->SetScene(scene);
   }
}

void Model::Rotate( float angle, float x, float y, float z )
{
    transformation = glm::rotate( transformation, angle, glm::vec3( x, y, z ) );
}

void Model::Translate(float x, float y, float z )
{
    transformation = glm::translate( transformation, glm::vec3( x, y, z ));
}

void Model::Scale(float x, float y, float z )
{
    transformation = glm::scale(transformation, glm::vec3( x, y, z ));
}

void Model::RotateLocal( float angle, float x, float y, float z )
{
    transformationLocal = glm::rotate( transformationLocal, angle, glm::vec3( x, y, z ) );
}

void Model::TranslateLocal(float x, float y, float z )
{
    transformationLocal = glm::translate( transformationLocal, glm::vec3( x, y, z ));
}

void Model::ScaleLocal(float x, float y, float z )
{
    transformationLocal = glm::scale(transformationLocal, glm::vec3( x, y, z ));
}


void Model::SetParent(Model* parentModel)
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

void Model::RemoveParent()
{
    RemoveFromParentsChildList();
    parent = NULL;
}

void Model::SetChild(Model* child)
{
   if(!child){
      return;
   }

   for(unsigned int i =0; i<childList.size(); i++){
     if(child == childList.at(i)){
         printf("Child of type: %d already added into parent of type: %d", child->GetModelType(), GetModelType());
         return;
     }
   }
   child->parent = this;
   childList.push_back(child);
}

void Model::RemoveFromParentsChildList()
{
    for(unsigned int i =0; parent && i<parent->childList.size(); i++){
        if(this == parent->childList.at(i)){
            parent->childList.erase (parent->childList.begin()+i);
            printf("Child of type: %d removed from parent of type: %d", this->GetModelType(), GetModelType());
            return;
        }
    }
}

Model* Model::GetParent(){
    return parent;
}

std::vector<Model*>* Model::GetChildren()
{
    return &childList;
}

void Model::SetMaterial(glm::vec4 materialColor)
{
   material = materialColor;
}

void Model::InitModel(){
   for(unsigned int i=0; i<GetChildren()->size(); i++){
      GetChildren()->at(i)->InitModel();
   }
}
}

