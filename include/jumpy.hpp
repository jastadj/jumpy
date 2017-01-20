#ifndef CLASS_JUMPY
#define CLASS_JUMPY

#include <SFML\Graphics.hpp>

#include "player.hpp"
#include "level.hpp"

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
    sf::Texture *m_playerTXT;


    // init
    bool init();
    bool initScreen();
    bool initResources();

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

};
#endif // CLASS_JUMPY
