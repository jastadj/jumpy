#ifndef CLASS_JUMPY
#define CLASS_JUMPY

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <time.h>
#include <math.h>

//#include "player.hpp"
//#include "level.hpp"
#include "gameobj.hpp"

#include "methui.hpp"
#include "heathui.hpp"
#include "weaponui.hpp"

// forward declarations
class Player;
class SpriteSheet;
class Level;
class Tile;
class ParticleManager;
class Decoration;
class Animation;
class LevelEditor;

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
    std::vector<Animation> m_animations;
    std::vector<sf::SoundBuffer*> m_sounds;
    std::vector<Tile*> m_tiles;
    std::vector<Tile*> m_tiles_bg;
    std::vector<Tile*> m_tiles_fg;
    std::vector<Decoration*> m_decorations;

    // sounds
    bool loadSound(std::string filename);
    std::vector<sf::Sound*> m_soundqueue;
    void processSoundQueue();

    // init
    bool init();
    bool initScreen();
    bool initResources();
    bool initSounds();
    bool initAnimations();
    //bool initTiles();
    //bool initDecorations();

    // init newgame
    void initPlayer();
    void initLevel();


    // player
    Player *m_player;

    //
    time_t m_seed;


    // particles
    ParticleManager *m_particle_manager;

    // UI elements
    MethUI *m_meth_ui;

    HealthUI *m_health_ui;

    WeaponUI *m_weapon_ui;

    // levels
    Level *m_current_level;

    // main loop logic
    int mainLoop();
    void drawScreen();
    void update();
    void drawSkyBox();
    void drawLevel(Level *tlevel);
    void drawLevelFG(Level *tlevel);

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
    int getScreenZoom() { return m_zoom;}
    sf::View *getView() { return &m_camera;}

    void setCurrentLevel(Level *tlevel);

    SpriteSheet *getSpriteSheet(int index);
    void addSpriteSheet(SpriteSheet *tsheet);
    std::vector<Animation> *getAnimations() { return &m_animations;}
    std::vector<Tile*> *getTiles() { return &m_tiles;}
    std::vector<Tile*> *getTilesBG() { return &m_tiles_bg;}
    std::vector<Tile*> *getTilesFG() { return &m_tiles_fg;}
    std::vector<Decoration*> *getDecorations() { return &m_decorations;}

    Level *getCurrentLevel() { return m_current_level;}
    Player *getPlayer() { return m_player;}

    GameObj *createObject(GAMEOBJTYPES tobj);

    ParticleManager *getParticleManager() { return m_particle_manager;}

    bool playSound(int soundindex);

    // debug
    bool m_dbg_showboundingboxes;
    bool m_dbg_noclip;
    LevelEditor *m_dbg_editor;


};
#endif // CLASS_JUMPY
