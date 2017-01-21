#ifndef CLASS_JUMPY
#define CLASS_JUMPY

#include <SFML\Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <string>


//#include "player.hpp"
//#include "level.hpp"


class Player;
class SpriteSheet;
class Level;
class Tile;

class Jumpy
{
private:

    // singleton stuff
    static Jumpy *m_instance;
    Jumpy();
    ~Jumpy();

    // render screen
    sf::RenderWindow *m_screen;
    float m_zoom;
    int m_screen_width;
    int m_screen_height;

    // resources
    std::vector<SpriteSheet*> m_spritesheets;


    // init
    bool init();
    bool initScreen();
    bool initResources();
    bool initTiles();
    void initPlayer();
    void initLevel();

    // player
    Player *m_player;

    // levels
    std::vector<Tile*> m_tiles;
    Level *m_current_level;

    // main loop logic
    int mainLoop();
    void drawScreen();
    void drawLevel(Level *tlevel);

public:
    static Jumpy *getInstance()
    {
        if(m_instance == NULL)
        {
            m_instance = new Jumpy;
        }

        return m_instance;
    }

    void start();

    SpriteSheet *getSpriteSheet(int index);
    std::vector<Tile*> *getTiles() { return &m_tiles;}

    Level *getCurrentLevel() { return m_current_level;}

};
#endif // CLASS_JUMPY
