#ifndef CLASS_BUTTON
#define CLASS_BUTTON

#include "gui.hpp"

class Button:public GUI
{
private:

public:
    Button(sf::Texture *ttxt);
    Button(sf::Sprite *tsprite);
    ~Button();
    GUITYPE getType() { return GUI_BUTTON;}

    bool pressed(sf::Vector2f mousepos);

};
#endif // CLASS_BUTTON
