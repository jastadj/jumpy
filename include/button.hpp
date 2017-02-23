#ifndef CLASS_BUTTON
#define CLASS_BUTTON

#include "gui.hpp"

enum BUTTON_STATE{BUTTON_NONE, BUTTON_PRESSED, BUTTON_RELEASED, BUTTON_MOUSEOVER};

class Button:public GUI
{
protected:

    BUTTON_STATE m_state;

public:
    Button();
    virtual ~Button();
    virtual GUITYPE getType()=0;
    virtual bool mouseOver()=0;


    BUTTON_STATE getState() { return m_state;}


};

class ButtonGraphic: public Button
{
private:

public:
    ButtonGraphic();
    ButtonGraphic(sf::Sprite *tsprite);
    ~ButtonGraphic();
    GUITYPE getType() { return GUI_BUTTON_GRAPHIC;}

    // set button graphics
    void setButtonSprite(sf::Sprite *tsprite);
    void setButtonPressedSprite(sf::Sprite *tsprite);
    void setButtonMouseOverSprite(sf::Sprite *tsprite);

    sf::Sprite *getCurrentSprite();

    // draw and update
    bool mouseOver();
    void processMousePressEvent(sf::Event *tevent);
    void update();
    void draw(sf::RenderTarget *tscreen);
};

#endif // CLASS_BUTTON
