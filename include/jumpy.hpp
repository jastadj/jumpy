#ifndef CLASS_JUMPY
#define CLASS_JUMPY

#include <SFML\Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <time.h>
#include <math.h>

//#include "player.hpp"
//#include "level.hpp"

#include "methui.hpp"

// forward declarations
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
    sf::View m_camera;
    sf::View m_skycamera;

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

    //
    time_t m_seed;

    // UI elements
    MethUI *m_meth_ui;

    // levels
    std::vector<Tile*> m_tiles;
    std::vector<Tile*> m_tiles_bg;
    Level *m_current_level;

    // main loop logic
    int mainLoop();
    void drawScreen();
    void drawSkyBox();
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

    int getScreenWidth() { return m_screen_width;}
    int getScreenHeight() { return m_screen_height;}

    SpriteSheet *getSpriteSheet(int index);
    std::vector<Tile*> *getTiles() { return &m_tiles;}
    std::vector<Tile*> *getTilesBG() { return &m_tiles_bg;}

    Level *getCurrentLevel() { return m_current_level;}
    const Player *getPlayer() const { return m_player;}
    // debug
    bool m_dbg_showboundingboxes;


};
#endif // CLASS_JUMPY
