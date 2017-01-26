#ifndef CLASS_LEVELEDITOR
#define CLASS_LEVELEDITOR

#include <SFML\Graphics.hpp>

// forward declarations
class Jumpy;
class SpriteSheet;
class Level;
class Player;

enum EDITMODE{ ED_NONE, ED_SAVE, ED_TILE, ED_TILEDRAW, ED_TILEBG, ED_TILEDRAWBG};

class LevelEditor
{
private:

    Jumpy *m_jumpy;
    Player *m_player;
    Level *m_currentlevel;

    sf::Font m_font;
    sf::Text m_saveastext;
    sf::Text m_filenametxt;
    std::string m_savefilename;

    int m_screenwidth;
    int m_screenheight;

    int m_mode;
    int m_drawmode;

    sf::Vector2f m_mouseleft;
    int m_brushid;
    sf::Sprite *m_brushsprite;

    std::vector<SpriteSheet*> m_spritesheets;

    std::vector<sf::Sprite*> m_edit_buttons;

    std::vector<sf::Sprite*> m_tiles;
    std::vector<sf::Sprite*> m_tilesbg;

    sf::RectangleShape *m_coverscreen;

public:
    LevelEditor();
    ~LevelEditor();


    void update();

    void draw(sf::RenderWindow *tscreen);

    void processEvent(sf::Event *event, sf::RenderWindow *tscreen);

    int getMode() { return m_mode;}
};
#endif // CLASS_LEVELEDITOR
