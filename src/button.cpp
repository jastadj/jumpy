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

void ButtonGraphic::processMousePressEvent(sf::Event *tevent)
{
    //if(!mouseOver()) return;
    if( m_state != BUTTON_MOUSEOVER) return;

    m_state = BUTTON_PRESSED;
}

void ButtonGraphic::update()
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
