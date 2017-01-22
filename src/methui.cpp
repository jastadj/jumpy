#include "methui.hpp"
#include "jumpy.hpp"
#include "player.hpp"

MethUI::MethUI()
{
    m_methuitxt.loadFromFile(".\\Data\\Art\\methui.png");
    m_methuispr = sf::Sprite(m_methuitxt);

    m_jumpy = Jumpy::getInstance();

    m_methbar = sf::RectangleShape( sf::Vector2f(115-44, 19-10));
    m_methbar.setFillColor( sf::Color(49, 162, 242) );
}

MethUI::~MethUI()
{

}

void MethUI::draw(int x, int y, sf::RenderTarget *tscreen)
{
    m_methuispr.setPosition(sf::Vector2f(x,y));
    m_methbar.setPosition( m_methuispr.getPosition() + sf::Vector2f(45,11));

    if(m_jumpy->getPlayer()->getMaxMeth() != 0)
        m_methbar.setScale(sf::Vector2f( float(m_jumpy->getPlayer()->getMeth()) / float(m_jumpy->getPlayer()->getMaxMeth()),1) );
    else
        m_methbar.setScale(sf::Vector2f( 0,1) );

    tscreen->draw( m_methuispr);
    tscreen->draw( m_methbar);
}
