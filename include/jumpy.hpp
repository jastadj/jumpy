#ifndef CLASS_JUMPY
#define CLASS_JUMPY

#include <SFML\Graphics.hpp>

class Jumpy
{
private:

    // singleton stuff
    static Jumpy *m_instance;
    Jumpy();
    ~Jumpy();

    // render screen
    sf::RenderWindow *m_screen;

    // init
    bool init();
    bool initScreen();

    // main loop logic
    int mainLoop();

public:
    static Jumpy *getInstance()
    {
        if(m_instance == NULL)
        {
            m_instance = new Jumpy;
        }

        return m_instance;
    }


};
#endif // CLASS_JUMPY
