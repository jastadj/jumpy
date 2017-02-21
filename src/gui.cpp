#include "gui.hpp"

#include "jumpy.hpp"

GUI::GUI()
{
    m_jumpy = Jumpy::getInstance();

    m_current_sprite = 0;
}

GUI::~GUI()
{
    // delete sprites
    for(int i = 0; i < int(m_sprites.size()); i++)
    {
        delete m_sprites[i];
    }
    m_sprites.clear();

    // delete textures
    for(int i = 0; i < int(m_textures.size()); i++)
    {
        delete m_textures[i];
    }
    m_textures.clear();
}

void GUI::setPosition(sf::Vector2f tpos)
{
    m_position = tpos;
}
void GUI::setPosition(int x, int y)
{
    setPosition(sf::Vector2f( float(x),float(y)) );
}

void GUI::update()
{
    m_sprites[m_current_sprite]->setPosition( m_position);
}

void GUI::draw(sf::RenderTarget *tscreen)
{

    tscreen->draw( *m_sprites[m_current_sprite]);
}
