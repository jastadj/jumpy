#include "jumpy.hpp"
#include "player.hpp"
#include "spritesheet.hpp"

Jumpy *Jumpy::m_instance = NULL;

Jumpy::Jumpy()
{
    // init pointers
    m_player = NULL;
}

Jumpy::~Jumpy()
{

}

void Jumpy::start()
{
    // init jumpy
    init();

    // init player
    initPlayer();

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
    // create player spritesheet
    SpriteSheet *newsheet = new SpriteSheet(".\\Data\\Art\\jumpyman.png", 1, 1);
    m_spritesheets.push_back(newsheet);
}




void Jumpy::initPlayer()
{
    if(m_player != NULL) delete m_player;

    m_player = new Player;


}




SpriteSheet *Jumpy::getSpriteSheet(int index)
{
    if( index < 0 || index >= int(m_spritesheets.size()))
    {
        std::cout << "Error getting spritesheet, index out of range!\n";
        return NULL;
    }

    return m_spritesheets[index];
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
