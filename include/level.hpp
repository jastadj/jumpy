#ifndef CLASS_LEVEL
#define CLASS_LEVEL

#include <vector>
#include <SFML\Graphics.hpp>

#define LEVEL_FILE ".\\Data\\Levels\\"

// forward declarations
class Jumpy;
class Tile;
class GameObj;
class Decoration;
class Level
{
private:

    Jumpy *m_jumpy;

    std::string m_filename;
    std::string m_level_name;

    bool m_initialized;

    std::vector< std::vector<int> > m_mapdata;
    std::vector< std::vector<int> > m_mapdata_bg;
    std::vector< std::vector<int> > m_mapdata_fg;

    std::vector<Decoration*> m_decorations;

    sf::RenderTexture m_skybox_rtxt;
    sf::Sprite m_skybox;

    std::vector< std::vector<Tile*> > m_tiles;
    std::vector< std::vector<Tile*> > m_tiles_bg;
    std::vector< std::vector<Tile*> > m_tiles_fg;

    std::vector< GameObj*> m_objects;

    sf::Vector2f m_player_start_pos;

    bool m_fade_fg;
    sf::Clock m_fg_fade_timer;
public:
    Level(int width = 10, int height = 10);
    Level(std::string filename);
    bool init(int width, int height);
    bool save(std::string filename);
    bool load(std::string filename);
    ~Level();

    std::string getFilename() { return m_filename;}
    std::string getName() { return m_level_name;}

    void fillMap(int tileid);
    void fillMapBG(int tileid);
    void fillMapFG(int tileid);

    void resizeX(int tdir);
    void resizeY(int tdir);

    int getWidth();
    int getHeight();

    sf::Vector2f getPlayerStartPos() { return m_player_start_pos;}
    void setPlayerStartPos(sf::Vector2f tpos);

    int getTile(int x, int y);
    int getTileBG(int x, int y);
    int getTileFG(int x, int y);
    bool setTile(int x, int y, int tileid);
    bool setTileBG(int x, int y, int tileid);
    bool setTileFG(int x, int y, int tileid);

    bool isCollidingWithMap(sf::FloatRect trect);
    void getObjectCollisions(sf::FloatRect trect, GameObj *source);
    std::vector<GameObj*> getObjectCollisionsWithLine(sf::Vector2f p1, sf::Vector2f p2);

    void drawTile(int x, int y, sf::RenderTarget *tscreen);
    void drawTileBG(int x, int y, sf::RenderTarget *tscreen);
    void drawTileFG(int x, int y, sf::RenderTarget *tscreen, bool fade = false);
    //void generate();

    bool addObject( GameObj *tobj);
    bool deleteObject( GameObj *tobj);

    void addDecoration(int dindex, sf::Vector2f dpos);
    bool deleteDecoration(Decoration *tdec);
    std::vector<Decoration*> *getDecorations() { return &m_decorations;}

    sf::Sprite *getSkyBox() { return &m_skybox;}

    void drawObjects(sf::RenderTarget *tscreen);
    void drawDecorations(sf::RenderTarget *tscreen);
    void update();


};
#endif // CLASS_LEVEL
