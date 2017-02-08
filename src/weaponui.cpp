#include "weaponui.hpp"

#include "jumpy.hpp"
#include "player.hpp"

WeaponUI::WeaponUI()
{

    //ar15
    m_weaponui_guntxt.push_back(sf::Texture());
    m_weaponui_guntxt.back().loadFromFile(".\\Data\\Art\\gunui_ar15.png");
    m_weaponui_gunspr.push_back(sf::Sprite(m_weaponui_guntxt.back()));
    m_weaponui_gunspr.back().setScale(2,2);

    m_jumpy=Jumpy::getInstance();
}



void WeaponUI::draw(int x, int y, sf::RenderTarget *tscreen)
{
    int tindex = 0;
    m_weaponui_gunspr[tindex].setPosition(sf::Vector2f(x,y));
    tscreen->draw(m_weaponui_gunspr[tindex]);
}
