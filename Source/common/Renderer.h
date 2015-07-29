#pragma once
#include "Object.h"
#include "../common/Scene.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "Interfaces/ILifeCycle.h"
#include "Interfaces/IRenderable.h"

class Renderer : public ILifeCycle, public ObjectRelative
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
    virtual void addScene( Scene* scene);

    //! Remove the scene
    virtual bool removeScene( Scene* scene);

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

	void setWindowTitle(char* title);
	
	std::vector<ILifeCycle*> initFlatList;
	
	std::vector<ILifeCycle*> renderFlatList;

protected:
   PluginType rendererType;

private:
   // Request a 32-bits depth buffer when creating the window
   sf::ContextSettings contextSettings;

   // Create the main window
   sf::RenderWindow* window;

   std::string titleName;
};

