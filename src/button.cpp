#include "button.hpp"

#include "jumpy.hpp"

////////////////////////////////////////////////////////////
// BUTTON BASE CLASS
Button::Button()
{
    m_state = BUTTON_NONE;
}

Button::~Button()
{

}

void Button::processMousePressEvent(sf::Event *tevent)
{
    //if(!mouseOver()) return;
    if( m_state != BUTTON_MOUSEOVER) return;

    m_state = BUTTON_PRESSED;
}

void Button::update()
{
    // if mouse is over button
    if(mouseOver())
    {
        // if button is none, set to mouse over
        if(m_state == BUTTON_NONE) m_state = BUTTON_MOUSEOVER;
    }
    // else if mouse is not over button
    else
    {
        // and button is pressed
        if(m_state == BUTTON_PRESSED)
        {
            // but mouse button is no longer pressed, set to released
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                m_state = BUTTON_RELEASED;
            }
        }
        // if state was mouse over, but no longer actually is
        else if(m_state == BUTTON_MOUSEOVER) m_state = BUTTON_NONE;

    }

    // if button has been released, set to none
    if(m_state == BUTTON_RELEASED) m_state = BUTTON_NONE;
    else if(m_state == BUTTON_PRESSED && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) m_state = BUTTON_RELEASED;
}
////////////////////////////////////////////////////////////
// BUTTON GRAPHIC CLASS
ButtonGraphic::ButtonGraphic()
{
    // resize sprite array for button, button pressed, and button hover sprites
    m_sprites.resize(3);
    for(int i = 0; i < 3; i++) m_sprites.push_back(NULL);
}

ButtonGraphic::ButtonGraphic(sf::Sprite *tsprite)
{
    // resize sprite array for button, button pressed, and button hover sprites
    m_sprites.resize(3);
    for(int i = 0; i < 3; i++) m_sprites.push_back(NULL);

    // set parameter sprite as button sprite
    m_sprites[0] = tsprite;
}

ButtonGraphic::~ButtonGraphic()
{
    // sprites are destroyed in gui destructor
}

void ButtonGraphic::setButtonSprite(sf::Sprite *tsprite)
{
    // if sprite already exists at index 0
    if(m_sprites[0]) delete m_sprites[0];

    m_sprites[0] = tsprite;
}

void ButtonGraphic::setButtonPressedSprite(sf::Sprite *tsprite)
{
    // if sprite already exists at index 0
    if(m_sprites[1]) delete m_sprites[1];

    m_sprites[1] = tsprite;
}

void ButtonGraphic::setButtonMouseOverSprite(sf::Sprite *tsprite)
{
    if(m_sprites[2]) delete m_sprites[2];

    m_sprites[2] = tsprite;
}

bool ButtonGraphic::mouseOver()
{
    if(m_sprites[m_current_sprite])
    {
        return m_sprites[m_current_sprite]->getGlobalBounds().contains( m_jumpy->getMousePos());
    }

    return false;
}

/*
void ButtonGraphic::processMousePressEvent(sf::Event *tevent)
{
    //if(!mouseOver()) return;
    if( m_state != BUTTON_MOUSEOVER) return;

    m_state = BUTTON_PRESSED;
}
*/

void ButtonGraphic::update()
{
    /*
    // if mouse is over button
    if(mouseOver())
    {
        // if button is none, set to mouse over
        if(m_state == BUTTON_NONE) m_state = BUTTON_MOUSEOVER;
    }
    // else if mouse is not over button
    else
    {
        // and button is pressed
        if(m_state == BUTTON_PRESSED)
        {
            // but mouse button is no longer pressed, set to released
            if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                m_state = BUTTON_RELEASED;
            }
        }
        // if state was mouse over, but no longer actually is
        else if(m_state == BUTTON_MOUSEOVER) m_state = BUTTON_NONE;

    }

    // if button has been released, set to none
    if(m_state == BUTTON_RELEASED) m_state = BUTTON_NONE;
    else if(m_state == BUTTON_PRESSED && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) m_state = BUTTON_RELEASED;
    */

    Button::update();

    // update state
    switch(m_state)
    {
    case BUTTON_PRESSED:
        if(m_sprites[1]) m_current_sprite = 1;
        else if(m_sprites[0]) m_current_sprite = 0;
        break;
    case BUTTON_MOUSEOVER:
        if(m_sprites[2]) m_current_sprite = 2;
        else if(m_sprites[0]) m_current_sprite = 0;
        break;
    case BUTTON_RELEASED:
        if(m_sprites[0]) m_current_sprite = 0;
        break;
    case BUTTON_NONE:
        if(m_sprites[0]) m_current_sprite = 0;
        break;
    default:
        break;
    }

    // update current sprites position
    if( m_sprites[m_current_sprite])
    {
        m_sprites[m_current_sprite]->setPosition(m_position);
    }
}

void ButtonGraphic::draw(sf::RenderTarget *tscreen)
{
    if(m_sprites[m_current_sprite])
    {
        tscreen->draw(*m_sprites[m_current_sprite]);
    }
}

////////////////////////////////////////////////////////////
// BUTTON TYPE 1 CLASS (rectangle shape and text)

ButtonType1::ButtonType1(std::string nstring)
{
    // set default margins around text
    m_margins = 8;

    // create slots for shapes and text
    m_button_shapes.resize(3);
    m_button_texts.resize(3);

    // 0 = normal button
    // 1 = pressed button
    // 2 = hover button

    // create initial shape and text
    m_button_shapes[0] =  new sf::RectangleShape(sf::Vector2f(32,32) );
    m_button_shapes[0]->setFillColor( sf::Color(91,110,225));
    m_button_shapes[0]->setOutlineThickness(2);
    m_button_shapes[0]->setOutlineColor( sf::Color(128,128,128));
    m_button_shapes[1] = NULL;
    m_button_shapes[2] = NULL;

    m_button_texts[0] = new sf::Text(nstring.c_str(), *m_jumpy->getFont(), 12);
    m_button_texts[1] = NULL;
    m_button_texts[2] = NULL;

    generateButton();

}

ButtonType1::~ButtonType1()
{

}

void ButtonType1::generateButton()
{

    // note : margins use first text object as source for dimensions

    // get rect of text 0
    sf::FloatRect textrect = m_button_texts[0]->getLocalBounds();
    sf::Vector2f textdim( textrect.width, textrect.height);

    // resize button shape to text rect with margins
    for(int i = 0; i < int(m_button_shapes.size()); i++)
    {
        // if valid button shape
        if(m_button_shapes[i])
        {
            // resize button shape to text dimensions plus margins
            m_button_shapes[i]->setSize( textdim + sf::Vector2f(m_margins*2, m_margins*2));
        }
    }

}

bool ButtonType1::mouseOver()
{
    if(m_button_shapes[m_current_sprite]->getGlobalBounds().contains( m_jumpy->getMousePos())) return true;

    return false;
}

/*
void ButtonType1::processMousePressEvent(sf::Event *tevent)
{

}
*/

void ButtonType1::update()
{
    Button::update();

    m_button_shapes[m_current_sprite]->setPosition(m_position);
    m_button_texts[m_current_sprite]->setPosition( m_position + sf::Vector2f(m_margins, m_margins));
}

void ButtonType1::draw(sf::RenderTarget *tscreen)
{
    tscreen->draw(*m_button_shapes[m_current_sprite]);
    tscreen->draw(*m_button_texts[m_current_sprite]);
}
