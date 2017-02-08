#ifndef WEAPONUI_H
#define WEAPONUI_H

#include <vector>
#include <SFML\Graphics.hpp>

class Jumpy;

class WeaponUI
{
private:

    sf::Texture m_weaponui_reloadboxtxt;
    sf::Sprite m_weaponui_reloadboxspr;
    sf::RectangleShape m_reloadbar;


    std::vector<sf::Texture> m_weaponui_guntxt;
    std::vector<sf::Sprite> m_weaponui_gunspr;
    std::vector<sf::Sprite> m_weaponui_bulletspr;
    std::vector<sf::Texture> m_weaponui_bullettxt;




    Jumpy *m_jumpy;



public:
    WeaponUI();
    ~WeaponUI();

    void draw(int x, int y, sf::RenderTarget *tscreen);
};



#endif // WeaponUI_H
