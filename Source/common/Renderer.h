#pragma once
#include "Object.h"
#include "../common/Scene.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "ILifeCycle.h"

class Renderer : public ILifeCycle, public Object
{
protected:
    //! Scene List
    std::vector <Scene*> scenes;

public:
    //! Renderer Constructor
    Renderer(int width = 800, int height = 600);

    //! Renderer Destructor
    ~Renderer();

    //! Initialize Engine
    void Initialize();

    //! resize screen
    void Resize( int w, int h );

    //! Render the Models
    void Render(bool (*customRender)()=NULL);

    //! Cache the scene
    void addScene( Scene* scene);

    //! Remove the scene
    bool removeScene( Scene* scene);

    //! Get Scene by Name
    Scene* getScene(char* name);

    // Get the width of the window
   int getWindowWidth();

   int getWindowHeight();

   sf::RenderWindow* getWindow();
    ////! Initialize rendering engine
    //virtual void initialize() = 0;

    ////! resize screen
    //virtual void resize( int w, int h ) = 0;

    ////! Render the Models
    //virtual void render() = 0;

    // !This method does the software initialization of the pipeline.
    virtual void SWInit() = 0;

    //! This method does the software deinitialization of the pipeline.
    virtual void SWDeInit() = 0;


    //! This method does the hardware initialization of the pipeline.
    virtual void HWInit() = 0;


    //! This method does the hardware deinitialization of the pipeline.
    virtual void HWDeInit() = 0;

    PluginType getRendererType();
    virtual Object* createObject(Scene* handler, Model* parent, ModelType type, std::string objectName = "") = 0;
    //SFML
private:
   // Request a 32-bits depth buffer when creating the window
   sf::ContextSettings contextSettings;
   // Create the main window
   sf::RenderWindow* window;//window(sf::VideoMode(400, 300), "Window 1 New", sf::Style::Default, contextSettings);

   PluginType rendererType;
};
