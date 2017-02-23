#ifndef HEATHUI_H
#define HEATHUI_H

#include <SFML\Graphics.hpp>

#include "gui.hpp"

class Jumpy;

class HealthUI: public GUI
{
private:

    sf::RectangleShape m_healthbar;

public:
    HealthUI();
    ~HealthUI();
    GUITYPE getType() { return GUI_HEALTHUI;}
    bool mouseOver() { return false;}

    void draw(sf::RenderTarget *tscreen);
    void update();
};



#endif // HEATHUI_H
