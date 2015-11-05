#include "AllInclude.h"

#include <SFML/Graphics.hpp>
#include <iostream>

int main(int argc, char* argv[])
{

    // ---------------------------- Initialize SDLOpenGL ---------------------------

    // Must set before initializing
    window_width = 1000;
    window_height = 800;

    const float half_window_width = window_width/2.0f;
    const float half_window_height = window_height/2.0f;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height),"Shadows");
    window.setVerticalSyncEnabled(true);
     window.setActive();

    if(!InitSDLOpenGL())
    {
        std::cerr<<"Could not initialize SDLOpenGL"<<std::endl;
        exit(-1);
    }

    // ---------------------------- Set up OpenGL for 2D ---------------------------

    // SDLOpenGL defaults to perspective, change to ortho
    ViewOrtho(-1.0f, 100.0f);

    // ---------------------------- Lighting System ----------------------------

    LightSystem lightSystem;

    Light testLight;
    testLight.radius = 400.0f;
    testLight.center.x = half_window_width;
    testLight.center.y = half_window_height;
    lightSystem.addLight(&testLight);

    ConvexHull testHull;

    if(!testHull.LoadShape("data/testShape.txt"))
    {
        std::cerr<<"Could not load shape"<<std::endl;

        exit(-1);
    }

    testHull.worldCenter.x = half_window_width;
    testHull.worldCenter.y = half_window_height;

    lightSystem.addConvexHull(&testHull);

    // ----------------------------- Load Resources ----------------------------

    TextureDesc floor;
    LoadIMG("data/floor1.png",floor);

    TextureDesc testImage;
    LoadIMG("data/testImage.png",testImage);

    // ------------------------------- Game Loop -------------------------------

    sf::Event event;
    while(window.isOpen())
    {

        while(window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if(event.type == sf::Event::MouseMoved)
            {
                testLight.center.x = static_cast<float>(event.mouseMove.x);
                testLight.center.y = static_cast<float>(window_height - event.mouseMove.y);
            }
        }

        // -------------------------------------- Game Step ---------------------------------------

        // Clear screen and depth buffer
        window.clear();
        window.pushGLStates();

        glLoadIdentity();
        lightSystem.RenderLights();

        // Render background
        glTranslatef(half_window_width, half_window_height, 0.0f);
        glScalef(4.0f, 4.0f, 1.0f);
        Draw2DQuad(floor, 4.0f);

        // Render test image
        glLoadIdentity();
        glTranslatef(half_window_width, half_window_height, 0.0f);
        Draw2DQuad(testImage, 1.0f);

        // Update screen
        window.popGLStates();
        window.display();

    }


    return EXIT_SUCCESS;
}
