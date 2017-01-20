#include "jumpy.hpp"

Jumpy *Jumpy::m_instance = NULL;

Jumpy::Jumpy()
{

}

Jumpy::~Jumpy()
{

}

void Jumpy::start()
{
    // init jumpy
    init();

    // init player
    m_player = new Player(this);

    // start main loop
    mainLoop();
}

bool Jumpy::init()
{
    // init screen
    initScreen();

    // init resources
    initResources();

    return true;
}

bool Jumpy::initScreen()
{
    m_screen = new sf::RenderWindow( sf::VideoMode(640,480,32), "Jumpy");

    return true;
}

bool Jumpy::initResources()
{
    // init player texture
    m_playerTXT = new sf::Texture;
    m_playerTXT->loadFromFile(".\\Data\\Art\\jumpyman.png");
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
        drawScreen();


        // display
        m_screen->display();
    }

    return 0;
}

void Jumpy::drawScreen()
{
    // draw player
    m_player->draw(m_screen);
}
