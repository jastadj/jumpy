#include "heathui.hpp"

#include "jumpy.hpp"
#include "player.hpp"

HealthUI::HealthUI()
{

    // create sprites from textures
    sf::Sprite *newspr = m_jumpy->getSpriteSheet(3)->createSprite();
    m_sprites.push_back(newspr);

    m_healthbar = sf::RectangleShape( sf::Vector2f(106-44, 20-10));
    m_healthbar.setFillColor( sf::Color(255, 0, 0) );
}

HealthUI::~HealthUI()
{

}

void HealthUI::update()
{
    m_sprites[m_current_sprite]->setPosition(m_position);
    m_healthbar.setPosition( m_position + sf::Vector2f(54,10));

    if(m_jumpy->getPlayer()->getMaxHealth() != 0)
        m_healthbar.setScale(sf::Vector2f( float(m_jumpy->getPlayer()->getCurrentHealth()) / float(m_jumpy->getPlayer()->getMaxHealth()),1) );
    else
        m_healthbar.setScale(sf::Vector2f( 0,1) );
}

void HealthUI::draw(sf::RenderTarget *tscreen)
{
    tscreen->draw( *m_sprites[m_current_sprite]);
    tscreen->draw( m_healthbar);
}

