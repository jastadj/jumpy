#ifndef CLASS_LEVELEDITOR
#define CLASS_LEVELEDITOR

#include <SFML\Graphics.hpp>

// forward declarations
class Jumpy;
class SpriteSheet;

enum EDITMODE{ ED_NONE, ED_TILE};

class LevelEditor
{
private:

    Jumpy *m_jumpy;

    int m_screenwidth;
    int m_screenheight;

    int m_mode;
    sf::Vector2f m_mouseleft;

    std::vector<SpriteSheet*> m_spritesheets;

    std::vector<sf::Sprite*> m_edit_buttons;

    std::vector<sf::Sprite*> m_tiles;

    sf::RectangleShape *m_coverscreen;

public:
    LevelEditor();
    ~LevelEditor();


    void update();

    void draw(sf::RenderTarget *tscreen);

    void processEvent(sf::Event *event, sf::RenderWindow *tscreen);
};
#endif // CLASS_LEVELEDITOR
