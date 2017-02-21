#ifndef CLASS_METHUI
#define CLASS_METHUI

#include <SFML\Graphics.hpp>

#include "gui.hpp"

class MethUI: public GUI
{
private:

    sf::RectangleShape m_methbar;

public:
    MethUI();
    ~MethUI();

    GUITYPE getType() { return GUI_METHUI;}

    void draw(sf::RenderTarget *tscreen);
    void update();
};
#endif // CLASS_METHUI
