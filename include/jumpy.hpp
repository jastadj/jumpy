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

class Jumpy
{
private:

    // singleton stuff
    static Jumpy *m_instance;
    Jumpy();
    ~Jumpy();

    // render screen
    sf::RenderWindow *m_screen;

    // resources
    std::vector<SpriteSheet*> m_spritesheets;


    // init
    bool init();
    bool initScreen();
    bool initResources();
    void initPlayer();

    // player
    Player *m_player;

    // main loop logic
    int mainLoop();
    void drawScreen();

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

};
#endif // CLASS_JUMPY
