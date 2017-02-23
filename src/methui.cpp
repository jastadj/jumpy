#include "methui.hpp"
#include "jumpy.hpp"
#include "player.hpp"

MethUI::MethUI()
{
    // create sprites from textures
    sf::Sprite *newspr = m_jumpy->getSpriteSheet(4)->createSprite();
    m_sprites.push_back(newspr);

    m_methbar = sf::RectangleShape( sf::Vector2f(115-44, 19-10));
    m_methbar.setFillColor( sf::Color(49, 162, 242) );
}

MethUI::~MethUI()
{

}

void MethUI::update()
{
    m_sprites[m_current_sprite]->setPosition(m_position);
    m_methbar.setPosition( m_position + sf::Vector2f(45,11));

    if(m_jumpy->getPlayer()->getMaxMeth() != 0)
        m_methbar.setScale(sf::Vector2f( float(m_jumpy->getPlayer()->getMeth()) / float(m_jumpy->getPlayer()->getMaxMeth()),1) );
    else
        m_methbar.setScale(sf::Vector2f( 0,1) );
}

void MethUI::draw(sf::RenderTarget *tscreen)
{
    tscreen->draw( *m_sprites[m_current_sprite]);
    tscreen->draw( m_methbar);
}
