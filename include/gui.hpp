#ifndef CLASS_GUI
#define CLASS_GUI

#include <SFML\Graphics.hpp>

// forward dec
class Jumpy;

enum GUITYPE{ GUI_NONE, GUI_METHUI, GUI_HEALTHUI, GUI_BUTTON, GUI_TOTAL};

class GUI
{
protected:

    Jumpy *m_jumpy;

    std::vector<sf::Texture*> m_textures;
    std::vector<sf::Sprite*> m_sprites;

    int m_current_sprite;
    sf::Vector2f m_position;

public:
    GUI();
    virtual ~GUI();

    virtual GUITYPE getType()=0;

    virtual void draw(sf::RenderTarget *tscreen);
    virtual void update();

    void setPosition(sf::Vector2f tpos);
    void setPosition(int x, int y);
    sf::Vector2f getPosition() { return m_position;}

};
#endif // CLASS_GUI
