#ifndef HEATHUI_H
#define HEATHUI_H

#include <SFML\Graphics.hpp>

class Jumpy;

class HealthUI
{
private:

    sf::Texture m_healthuitxt;
    sf::Sprite m_healthspr;

    sf::RectangleShape m_healthbar;

    Jumpy *m_jumpy;

public:
    HealthUI();
    ~HealthUI();

    void draw(int x, int y, sf::RenderTarget *tscreen);
};



#endif // HEATHUI_H
