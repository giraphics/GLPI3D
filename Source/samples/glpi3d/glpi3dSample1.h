#ifndef GLPI3DSAMPLE1_H
#define GLPI3DSAMPLE1_H
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "glpi3dt_demoobjloader.h"
#include "glpi3dt_demotriangle.h"

void GLPISample()
{
    // Request a 32-bits depth buffer when creating the window
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 32;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(400, 300), "Window 1", sf::Style::Default, contextSettings);
    sf::RenderWindow window1(sf::VideoMode(400, 300), "Window 2", sf::Style::Default, contextSettings);

    window.setVerticalSyncEnabled(true);
    window1.setVerticalSyncEnabled(true);

    window.setActive();
    window1.setActive();

    glewInit();

    // Create a clock for measuring the time elapsed
    sf::Clock clock;
   glm::vec3 WorldCameraPosition    = glm::vec3(0.0, 1.0, 2.0); // 0 unit up and 0 unit forward
   glm::vec3 WorldTargetPosition    = glm::vec3(0.0, 0.0, 0.0);  // Target is at center 0.0, 0.0, 0.0
   glm::vec3 WorldUp                = glm::vec3(0.0, 1.0, 0.0);  // Head is the upward direction

   // Create renderer's instance.
   Renderer* Renderer = Renderer::dynamicObj();

   // Create renderer's instance.
   Scene* scene = new Scene();

   // Add to the render
   Renderer->addScene(scene);

   // Initialize the renderer
   Renderer->initializeScene((::int16)0, (::int16)0);

   // Setup Projection System
   scene->Camera()->cameraPrespective(60.0f, 0.01f, 1000);
   //Renderer->Camera()->cameraOrtho( -1, 1, -1, 1, -30, 30);
   scene->Camera()->setCameraView(WorldCameraPosition, WorldTargetPosition, WorldUp);
   scene->Camera()->cameraViewport(0.0, 0.0, window.getSize().x, window.getSize().y);

   Model* car1 = new ObjLoader         (ObjFileType);
   Model* car2 = new ObjLoader         (ObjFileType, car1);
   Model* car3 = new ObjLoader         (ObjFileType, car2);
   car1->Translate(-1.5,0.0,0.0);
   car1->SetMaterial(glm::vec4(0.0, 1.0, 0.0, 1.0));

   car2->Translate(1.5,0.0,0.0);
   car2->SetMaterial(glm::vec4(1.0, 0.0, 0.0, 1.0));

   car3->Translate(0.5,0.0,0.0);
   car3->SetMaterial(glm::vec4(0.0, 0.0, 1.0, 1.0));

   // Add the Custom Models
   scene->addModel( new DemoTriangle      (TriangleType));
   scene->addModel( car1);
   //scene->addModel( car2);
   //scene->addModel( car3);

    ////////////////////////////////
    // Start game loop
    while (window.isOpen())
    {
        // Process events for first window
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();
        }
         Renderer->run();
        // Finally, display the rendered frame on screen
        window.display();

        // Process events for second window
        while (window1.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window1.close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window1.close();
        }

        Renderer->run();
        // Finally, display the rendered frame on screen
        window1.display();

        int band = scene->frameCount()%500;
        if(band<150){
           car1->Rotate(1 , 1.0f, 0.0f, 0.0f);
        }
        else if(band<300){
           car2->Rotate(1 , 1.0f, 0.0f, 0.0f);
        }
        else if(band<450){
           car3->Rotate(1 , 1.0f, 0.0f, 0.0f);
        }
    }
}

#endif
