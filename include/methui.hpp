#ifndef CLASS_METHUI
#define CLASS_METHUI

#include <SFML\Graphics.hpp>

class Jumpy;

class MethUI
{
private:

    sf::Texture m_methuitxt;
    sf::Sprite m_methuispr;

    sf::RectangleShape m_methbar;

    Jumpy *m_jumpy;

public:
    MethUI();
    ~MethUI();

    void draw(int x, int y, sf::RenderTarget *tscreen);
};
#endif // CLASS_METHUI
