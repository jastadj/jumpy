#include "jumpy.hpp"

Jumpy *Jumpy::m_instance = NULL;

Jumpy::Jumpy()
{
    // init jumpy
    init();

    // start main loop
    mainLoop();
}

Jumpy::~Jumpy()
{

}

bool Jumpy::init()
{
    // init screen
    initScreen();
}

bool Jumpy::initScreen()
{
    m_screen = new sf::RenderWindow( sf::VideoMode(640,480,32), "Jumpy");

    return true;
}

int Jumpy::mainLoop()
{
    bool quit = false;

    while (!quit)
    {
        m_screen->clear();

        // event que
        sf::Event event;

        // process event que
        while(m_screen->pollEvent(event))
        {
            // if 'x' on window was closed
            if( event.type == sf::Event::Closed) quit = true;

            // if key pressed
            if( event.type == sf::Event::KeyPressed)
            {
                // quit if escape pressed
                if(event.key.code == sf::Keyboard::Escape) quit = true;
            }
        }

        // update


        // draw


        // display
        m_screen->display();
    }
}
