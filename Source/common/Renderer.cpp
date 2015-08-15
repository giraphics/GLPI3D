#include "Renderer.h"
#define demoN demoNumber
Renderer::Renderer(int width, int height)
{
   contextSettings.depthBits = 32;

   window = new sf::RenderWindow(sf::VideoMode(width, height), titleName.c_str(), sf::Style::Default, contextSettings);
   
   window->setVerticalSyncEnabled(true);

   window->setActive();

   GLenum err = glewInit();
   if( err != GLEW_OK )
   {
       printf("GlewInit error");
       exit(1);
   }

   // Resize should be triggered automatically by the system. Temporarily it initiated in the constructor
   //Resize(window->getSize().x, window->getSize().y);
}

Renderer::~Renderer()
{
   delete window;
   window = NULL;
}

//! Initialize Scene
void Renderer::Initialize()
{
	this->initFlatList.clear();
    for( int i=0; i<scenes.size();  i++ ){
        scenes.at(i)->Initialize();
    }

	for(int i=0; i<	this->initFlatList.size(); i++){
		this->initFlatList.at(i)->Initialize();
	}
}

//! resize screen
void Renderer::Resize( int w, int h )
{
    for( int i=0; i<scenes.size();  i++ ){
        scenes.at(i)->Resize(w, h);
    }
}

//! Cache the scene
void Renderer::addScene( Scene* scene)
{
    if(!scene)
        return;
    
    for (std::vector<Scene*>::iterator it = scenes.begin() ; it != scenes.end(); ++it){
        if( (*it) == scene ){
            //If already added return;
            return;
        }
    }

    scenes.push_back( scene );
    scene->setRenderer(this);
}

bool Renderer::removeScene(Scene* scene)
{
    int i = 0;
    for (std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); it++,i++)
    {
        if( (*it) == scene){
            scenes.erase(scenes.begin()+i);
            return true;
        }
    }
    return false;
}

Scene* Renderer::getScene(char* name)
{
    for (std::vector<Scene*>::iterator it = scenes.begin() ; it != scenes.end(); ++it){
        if( (*it)->GetName().compare(name) == 0 ){
            return (*it);
        }
    }
    return NULL;
}

extern int demoNumber;
//! Render the Models
void Renderer::Render(bool (*customRender)())
{
   this->renderFlatList.clear();
   for( int i=0; i<scenes.size();  i++ ){
     scenes.at(i)->Render();
   }
	
   for (int i = 0; i< this->renderFlatList.size(); i++){
	   this->renderFlatList.at(i)->Render();
   }

   // Process events for first window
   sf::Event event;
   while (window->pollEvent(event))
   {
      // Close window: exit
      if (event.type == sf::Event::Closed){
          window->close();
		  exit(0);
	  }

      // Escape key: exit
	  if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Left)){
		  window->close();
		  demoNumber--;
	  }

	  if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Right || 
																						   event.key.code == sf::Keyboard::Escape)){
		  window->close();
		  demoNumber++;
	  }
   }

   // Finally, display the rendered frame on screen
   window->display();
}

int Renderer::getWindowWidth()
{
   return window->getSize().x;
}

int Renderer::getWindowHeight()
{
   return window->getSize().y;
}

sf::RenderWindow* Renderer::getWindow()
{
   return window;
}

PluginType Renderer::getRendererType()
{
   return rendererType;
}

void Renderer::setWindowTitle(char* title){ 
	titleName = std::string(title); 
	window->setTitle(titleName); 
}