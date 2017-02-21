#include "button.hpp"

Button::Button(sf::Texture *ttxt)
{
    // push texture into texture list
    m_textures.push_back(ttxt);

    // create sprite
    m_sprites.push_back( new sf::Sprite(*m_textures[0]));
}

Button::Button(sf::Sprite *tsprite)
{
    m_sprites.push_back(tsprite);
}

Button::~Button()
{

}

bool Button::pressed(sf::Vector2f mousepos)
{
    return m_sprites[m_current_sprite]->getGlobalBounds().contains(mousepos);
}
