#ifndef CLASS_BUTTON
#define CLASS_BUTTON

#include "gui.hpp"

enum BUTTON_STATE{BUTTON_NONE, BUTTON_PRESSED, BUTTON_RELEASED, BUTTON_MOUSEOVER};

////////////////////////////////////////////////////////////
// BUTTON BASE CLASS

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

    virtual void processMousePressEvent(sf::Event *tevent);
    virtual void update();
};

////////////////////////////////////////////////////////////
// BUTTON GRAPHIC CLASS

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
    //void processMousePressEvent(sf::Event *tevent);
    void update();
    void draw(sf::RenderTarget *tscreen);
};

////////////////////////////////////////////////////////////
// BUTTON TYPE 1 CLASS (rectangle shape and text)
class ButtonType1: public Button
{
private:

    std::string m_button_string;

    std::vector<sf::RectangleShape*> m_button_shapes;
    std::vector<sf::Text*> m_button_texts;

    int m_margins;

    void generateButton();

public:
    ButtonType1(std::string nstring = std::string("no text"));
    ~ButtonType1();
    GUITYPE getType() { return GUI_BUTTON_TYPE1;}

    // draw and update
    bool mouseOver();
    //void processMousePressEvent(sf::Event *tevent);
    void update();
    void draw(sf::RenderTarget *tscreen);
};
#endif // CLASS_BUTTON
