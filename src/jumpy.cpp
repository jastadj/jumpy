#include "jumpy.hpp"
#include "player.hpp"
#include "spritesheet.hpp"
#include "level.hpp"
#include "tile.hpp"
#include "particle.hpp"

#include <sstream>

#include "tools.hpp"

Jumpy *Jumpy::m_instance = NULL;

Jumpy::Jumpy()
{
    // init pointers
    m_player = NULL;
    m_current_level = NULL;
    m_meth_ui = NULL;
    m_particle_manager = NULL;

    // screen settings
    m_screen_width = 800;
    m_screen_height = 600;
    m_zoom = 2;

    // debug
    m_dbg_showboundingboxes = false;
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

    // init random seed
    srand( time(NULL));

    // init ui elements
    m_meth_ui = new MethUI();

    // init particle manager
    m_particle_manager = new ParticleManager;

    return true;
}

bool Jumpy::initScreen()
{
    m_screen = new sf::RenderWindow( sf::VideoMode(m_screen_width,m_screen_height,32), "Jumpy");
    if(!m_screen) return false;

    // enable v-sync?
    m_screen->setVerticalSyncEnabled(true);

    // enable fps limit in sfml?
    m_screen->setFramerateLimit(60);

    return true;
}

bool Jumpy::initResources()
{
    // create player spritesheet - 0
    SpriteSheet *newsheet = new SpriteSheet(".\\Data\\Art\\jumpyman.png", 27, 1);
    m_spritesheets.push_back(newsheet);

    // create a tile spritesheet - 1
    newsheet = new SpriteSheet(".\\Data\\Art\\tiles001.png", 4, 4);
    m_spritesheets.push_back(newsheet);

    // create tile background spritesheet - 2
    newsheet = new SpriteSheet(".\\Data\\Art\\tiles001bg.png", 4, 4);
    m_spritesheets.push_back(newsheet);

    // create meth spritesheet - 3
    newsheet = new SpriteSheet(".\\Data\\Art\\meth.png", 1, 1);
    m_spritesheets.push_back(newsheet);

    // create methhead spritesheet - 4
    newsheet = new SpriteSheet(".\\Data\\Art\\methhead.png", 9, 1);
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

    // load sounds
    loadSound(".\\Data\\Sound\\pistol01.wav");
    loadSound(".\\Data\\Sound\\gravel_left_01.wav");
    loadSound(".\\Data\\Sound\\gravel_right_01.wav");
    loadSound(".\\Data\\Sound\\gravel_left_02.wav");
    loadSound(".\\Data\\Sound\\gravel_right_02.wav");
    loadSound(".\\Data\\Sound\\gravel_left_03.wav");
    loadSound(".\\Data\\Sound\\gravel_right_03.wav");
    loadSound(".\\Data\\Sound\\gravel_left_04.wav");
    loadSound(".\\Data\\Sound\\gravel_right_04.wav");

    return true;
}

bool Jumpy::initTiles()
{
    // create tiles
    for(int i = 0; i < m_spritesheets[1]->getCount(); i++)
    {
        m_tiles.push_back(new Tile(m_spritesheets[1], i));
    }

    // create background tiles
    for(int i = 0; i < m_spritesheets[2]->getCount(); i++)
    {
        m_tiles_bg.push_back(new Tile(m_spritesheets[2], i));
    }


    // check for validity
    for(int i = 0; i < int(m_tiles.size()); i++)
    {
        if( m_tiles[i] == NULL) return false;
    }

    for(int i = 0; i < int(m_tiles_bg.size()); i++)
    {
        if( m_tiles_bg[i] == NULL) return false;
    }

    return true;
}



void Jumpy::initPlayer()
{
    if(m_player != NULL) delete m_player;

    m_player = new Player;
    m_player->setPosition( sf::Vector2f(100,100));

}

void Jumpy::initLevel()
{
    // if a level currently exists, delete it
    if(m_current_level != NULL) delete m_current_level;

    // create a new level
    m_current_level = new Level(25,15);

    // debug output
    std::cout << "map height = " << m_current_level->getHeight() << std::endl;
    std::cout << "map width = " << m_current_level->getWidth() << std::endl;

    // testing - some level data
    for(int n = 0; n < m_current_level->getWidth(); n++)
    {
        m_current_level->setTile(n,m_current_level->getHeight()-5 , 1);
    }
    for(int i = m_current_level->getHeight()-4; i < m_current_level->getHeight(); i++)
    {
        for(int n = 0; n < m_current_level->getWidth(); n++)
        {
            m_current_level->setTile(n, i, 2);
        }
    }

    m_current_level->addMeth(400,288, 500);
    m_current_level->addMethHead(350,200);


    // test bg tile
    for(int i = 0; i < m_current_level->getWidth(); i++)
    {
        if( i%2) m_current_level->setTileBG(i, 9, 6);
        else m_current_level->setTileBG(i, 9, 7);
    }
    m_current_level->setTileBG(5, 9, 5);

    // generate map
    //m_current_level->generate();
}

bool Jumpy::loadSound(std::string filename)
{
    sf::SoundBuffer *newsound = new sf::SoundBuffer();
    if(!newsound->loadFromFile(filename))
    {
        std::cout << "Error loading sound:" << filename << std::endl;
        return false;
    }
    m_sounds.push_back(newsound);

    return true;
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

    // fps stuff
    sf::Clock frameclock;
    sf::Clock framedelayclock;
    int framecount = 0;
    int fps = 0;

    // testing
    sf::Font font;
    font.loadFromFile(".\\Data\\font.ttf");
    sf::Text debugtext("test", font, 12);
    debugtext.setFillColor( sf::Color::Red);

    // camera
    m_camera = sf::View( sf::FloatRect(0,0,m_screen_width,m_screen_height));
    m_camera.zoom(1/m_zoom);

    // skybox camera
    m_skycamera = m_screen->getDefaultView();
    m_skycamera.setCenter(sf::Vector2f( (m_screen_width/m_zoom)/2, (m_screen_height/m_zoom)/2 ));
    m_skycamera.zoom(1/m_zoom);

    while (!quit)
    {
        // lock loop to 60 fps
        //sf::sleep( sf::milliseconds(1000/60) );

        m_screen->clear();

        // draw skybox
        m_screen->setView(m_skycamera);
        drawSkyBox();

        //std::cout << "w:" << (m_screen_width/m_zoom)/32 << " h:" << (m_screen_height/m_zoom)/32 << std::endl;
        //camera.setCenter(sf::Vector2f( (m_screen_width/m_zoom)/32, (m_screen_height/m_zoom)/32 ) );
        //m_screen->setView(m_camera);

        // event que
        sf::Event event;

        sf::Vector2i mousePosi = sf::Mouse::getPosition(*m_screen);
        sf::Vector2f mousePos(mousePosi);


        // update
        m_player->update();
        m_current_level->update();
        m_particle_manager->update();

        m_camera.setCenter(m_player->getPosition());
        m_screen->setView(m_camera);

        // process event que

        // handle key states
        if( sf::Keyboard::isKeyPressed( sf::Keyboard::A)) m_player->doMove(MOVE_LEFT);
        else if( sf::Keyboard::isKeyPressed( sf::Keyboard::D)) m_player->doMove(MOVE_RIGHT);
        else m_player->doMove(MOVE_NONE);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            m_player->pee();
        }

        // process events (mouse clicks, key presses, etc)
        while(m_screen->pollEvent(event))
        {
            // if 'x' on window was closed
            if( event.type == sf::Event::Closed) quit = true;

            // if key pressed
            if( event.type == sf::Event::KeyPressed)
            {
                // quit if escape pressed
                if(event.key.code == sf::Keyboard::Escape) quit = true;

                else if(event.key.code == sf::Keyboard::E)
                {
                    m_player->setCurrentFrame( m_player->getCurrentFrame()+1);
                }
                else if(event.key.code == sf::Keyboard::Space)
                {
                    if(!m_player->jumping())
                    {
                        m_player->setJumping(true);
                        sf::Vector2f paccel = m_player->getAcceleration();
                        paccel.y = -5;
                        m_player->setAcceleration( paccel);
                    }
                }
                else if(event.key.code == sf::Keyboard::F1)
                {
                    m_dbg_showboundingboxes = !m_dbg_showboundingboxes;
                }


            } // end key pressed
            else if( event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    m_player->shoot();
                }
            }
        }





        // draw
        drawScreen();

        // debug
        //drawFloatRect(m_player->getBoundingBox(), m_screen);

        // change view back to default for ui
        m_screen->setView( m_screen->getDefaultView());
        // draw debug test

        // draw ui elements
        m_meth_ui->draw(m_screen_width - 200,20, m_screen);

        std::stringstream debugss;
        sf::Vector2f pvel = m_player->getVelocity();
        sf::Vector2f paccel = m_player->getAcceleration();
        sf::Vector2f ppos = m_player->getPosition();
        debugss << "FPS: " << fps << " playerv:" << pvel.x << "," << pvel.y << "  playera:" << paccel.x << "," << paccel.y << " pos:" << ppos.x << "," << ppos.y << " colcnt:" << m_player->getCollisionCount() << " FRAME:" << m_player->getCurrentFrame();
        debugtext.setString( debugss.str() );
        m_screen->draw(debugtext);


        //std::cout << "particles:" << m_particle_manager->getCount() << std::endl;

        // display
        m_screen->display();

        // fps calc
        framecount++;
        if(frameclock.getElapsedTime().asMilliseconds() >= 1000)
        {
            fps = framecount;
            framecount = 0;

            frameclock.restart();
        }

        // process sound queue and cleanup
        processSoundQueue();
    }

    return 0;
}

void Jumpy::drawScreen()
{
    // draw level
    drawLevel(m_current_level);

    // draw player
    m_player->draw(m_screen);

    // draw particles
    m_particle_manager->draw(m_screen);
}

void Jumpy::drawLevel(Level *tlevel)
{


    // draw each tile in map
    for(int i = 0; i < m_current_level->getHeight(); i++)
    {
        for(int n = 0; n < m_current_level->getWidth(); n++)
        {
            /*
            // get tile at x/y position
            int ttile = m_current_level->getTile(n, i);

            // if tile is 0, do nothing, tile 0 is hardcoded as blank
            if( ttile == 0) continue;

            // else, draw tile index
            m_tiles[ m_current_level->getTile(n, i) ]->draw(m_screen, n*32, i*32);
            */
            m_current_level->drawTileBG(n, i, m_screen);
            m_current_level->drawTile(n,i, m_screen);
        }
    }

    m_current_level->drawObjects(m_screen);
}

void Jumpy::drawSkyBox()
{
    m_screen->draw( *m_current_level->getSkyBox());
}

bool Jumpy::playSound(int soundindex)
{
    if(soundindex < 0 || soundindex >= int(m_sounds.size()) )
    {
        std::cout << "Error playing sound index # " << soundindex << ", out of bounds\n";
        return false;
    }

    sf::Sound *newsound = new sf::Sound(*m_sounds[soundindex]);
    newsound->play();
    m_soundqueue.push_back(newsound);

    return true;
}

void Jumpy::processSoundQueue()
{
    for(int i = int(m_soundqueue.size())-1; i >= 0; i--)
    {
        if(m_soundqueue[i]->getStatus() == sf::Sound::Stopped)
        {
            delete m_soundqueue[i];
            m_soundqueue.erase(m_soundqueue.begin() + i);
        }
    }
}
