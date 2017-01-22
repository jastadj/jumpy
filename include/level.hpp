#ifndef CLASS_LEVEL
#define CLASS_LEVEL

#include <vector>
#include <SFML\Graphics.hpp>

// forward declarations
class Jumpy;
class Tile;
class GameObj;

class Level
{
private:

    Jumpy *m_jumpy;

    std::vector< std::vector<int> > m_mapdata;
    std::vector< std::vector<int> > m_mapdata_bg;

    sf::RenderTexture m_skybox_rtxt;
    sf::Sprite m_skybox;

    std::vector< std::vector<Tile*> > m_tiles;
    std::vector< std::vector<Tile*> > m_tiles_bg;

    std::vector< GameObj*> m_objects;
public:
    Level(int width = 10, int height = 10);
    ~Level();

    void fillMap(int tileid);
    void fillMapBG(int tileid);

    int getWidth();
    int getHeight();

    int getTile(int x, int y);
    int getTileBG(int x, int y);
    bool setTile(int x, int y, int tileid);
    bool setTileBG(int x, int y, int tileid);

    bool isCollidingWithMap(sf::FloatRect trect);
    void getObjectCollisions(sf::FloatRect trect, GameObj *source);

    void drawTile(int x, int y, sf::RenderTarget *tscreen);
    void drawTileBG(int x, int y, sf::RenderTarget *tscreen);
    //void generate();

    bool addObject( GameObj *tobj);
    bool deleteObject( GameObj *tobj);
    void addMeth(int x, int y, int val);

    sf::Sprite *getSkyBox() { return &m_skybox;}

    void drawObjects(sf::RenderTarget *tscreen);
    void update();
};
#endif // CLASS_LEVEL
