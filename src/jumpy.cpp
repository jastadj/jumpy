#include "jumpy.hpp"
#include "player.hpp"
#include "spritesheet.hpp"
#include "level.hpp"
#include "tile.hpp"

#include <sstream>

Jumpy *Jumpy::m_instance = NULL;

Jumpy::Jumpy()
{
    // init pointers
    m_player = NULL;
    m_current_level = NULL;
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

    // init level
    initLevel();

    // start main loop
    mainLoop();
}

bool Jumpy::init()
{
    // init screen
    if(!initScreen())
    {
        std::cout << "Error initializing screen!\n";
    }

    // init resources
    if(!initResources())
    {
        std::cout << "Error initializing resources!\n";
    }

    // init tiles
    if(!initTiles())
    {
        std::cout << "Error initializing tiles!\n";
    }

    return true;
}

bool Jumpy::initScreen()
{
    m_screen = new sf::RenderWindow( sf::VideoMode(640,480,32), "Jumpy");
    if(!m_screen) return false;

    return true;
}

bool Jumpy::initResources()
{
    // create player spritesheet
    SpriteSheet *newsheet = new SpriteSheet(".\\Data\\Art\\jumpyman.png", 1, 1);
    m_spritesheets.push_back(newsheet);

    // create a tile spritesheet
    newsheet = new SpriteSheet(".\\Data\\Art\\tiles001.png", 2, 2);
    m_spritesheets.push_back(newsheet);

    // check sprite sheets are valid
    for(int i = 0; i < int(m_spritesheets.size()); i++)
    {
        if(!m_spritesheets[i])
        {
            std::cout << "Error creating sprite sheets, sheet " << i << " did not load!\n";
            return false;
        }
    }

    return true;
}

bool Jumpy::initTiles()
{
    // create tiles
    m_tiles.push_back(new Tile(m_spritesheets[1], 0));
    m_tiles.push_back(new Tile(m_spritesheets[1], 1));
    m_tiles.push_back(new Tile(m_spritesheets[1], 2));
    m_tiles.push_back(new Tile(m_spritesheets[1], 3));

    for(int i = 0; i < int(m_tiles.size()); i++)
    {
        if( m_tiles[i] == NULL) return false;
    }

    return true;
}





void Jumpy::initPlayer()
{
    if(m_player != NULL) delete m_player;

    m_player = new Player;

}

void Jumpy::initLevel()
{
    if(m_current_level != NULL) delete m_current_level;

    m_current_level = new Level(10,10);

    // testing
    for(int i = 0; i < 10; i++) m_current_level->setTile(i, 9, 1);
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

    sf::Clock frameclock;
    sf::Clock framedelayclock;
    int framecount = 0;
    int fps = 0;

    // testing
    sf::Font font;
    font.loadFromFile(".\\Data\\font.ttf");
    sf::Text debugtext("test", font, 12);
    debugtext.setColor( sf::Color::Red);

    while (!quit)
    {
        // lock loop to 60 fps
        sf::sleep( sf::milliseconds(1000/60) );

        m_screen->clear();

        // event que
        sf::Event event;

        // process event que

        /*
        if( sf::Keyboard::isKeyPressed( sf::Keyboard::S))
        {
            //sf::Vector2f paccel = m_player->getAcceleration();
            //paccel.y = 1;
            //m_player->setAcceleration(paccel);

            sf::Vector2f pvel = m_player->getVelocity();
            m_player->setVelocity(sf::Vector2f(pvel.x, 5));
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            sf::Vector2f pvel = m_player->getVelocity();
            m_player->setVelocity(sf::Vector2f(pvel.x,-5));
        }
        else
        {
            sf::Vector2f pvel = m_player->getVelocity();
            m_player->setVelocity(sf::Vector2f(pvel.x,0));
        }
        */

        if( sf::Keyboard::isKeyPressed( sf::Keyboard::A))
        {
            sf::Vector2f pvel = m_player->getVelocity();
            m_player->setVelocity( sf::Vector2f(-5.f, pvel.y) );
        }
        else if( sf::Keyboard::isKeyPressed( sf::Keyboard::D))
        {
            sf::Vector2f pvel = m_player->getVelocity();
            m_player->setVelocity( sf::Vector2f(5, pvel.y) );
        }
        else
        {
            sf::Vector2f pvel = m_player->getVelocity();
            m_player->setVelocity( sf::Vector2f(0, pvel.y) );
        }

        if( sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if(!m_player->m_jumping)
            {
                m_player->m_jumping = true;
                sf::Vector2f paccel = m_player->getAcceleration();
                paccel.y = -1;
                m_player->setAcceleration( paccel);
            }
        }
        else if(m_player->m_jumping)
        {
            m_player->m_jumping = false;
        }

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
        m_player->update();

        // draw
        drawScreen();

        // draw debug test
        std::stringstream debugss;
        debugss << "FPS: " << fps;
        debugtext.setString( debugss.str() );
        m_screen->draw(debugtext);

        // display
        m_screen->display();

        framecount++;

        if(frameclock.getElapsedTime().asMilliseconds() >= 1000)
        {
            fps = framecount;
            framecount = 0;

            frameclock.restart();
        }
    }

    return 0;
}

void Jumpy::drawScreen()
{
    // draw level
    drawLevel(m_current_level);

    // draw player
    m_player->draw(m_screen);
}

void Jumpy::drawLevel(Level *tlevel)
{
    for(int i = 0; i < m_current_level->getHeight(); i++)
    {
        for(int n = 0; n < m_current_level->getWidth(); n++)
        {
            m_tiles[ m_current_level->getTile(n, i) ]->draw(m_screen, n*32, i*32);
        }
    }
}
