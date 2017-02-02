#include "heathui.hpp"

#include "jumpy.hpp"
#include "player.hpp"

HealthUI::HealthUI()
{
    m_healthuitxt.loadFromFile(".\\Data\\Art\\healthui.png");
    m_healthuispr = sf::Sprite(m_healthuitxt);

    m_jumpy = Jumpy::getInstance();

    m_healthbar = sf::RectangleShape( sf::Vector2f(80-44, 19-10));
    m_healthbar.setFillColor( sf::Color(255, 0, 0) );
}

HealthUI::~HealthUI()
{

}

void HealthUI::draw(int x, int y, sf::RenderTarget *tscreen)
{
    m_healthuispr.setPosition(sf::Vector2f(x,y));
    m_healthbar.setPosition( m_healthuispr.getPosition() + sf::Vector2f(45,11));

    if(m_jumpy->getPlayer()->getMaxHealth() != 0)
        m_healthbar.setScale(sf::Vector2f( float(m_jumpy->getPlayer()->getHealth()) / float(m_jumpy->getPlayer()->getMaxHealth()),1) );
    else
        m_healthbar.setScale(sf::Vector2f( 0,1) );

    tscreen->draw( m_healthuispr);
    tscreen->draw( m_healthbar);
}

