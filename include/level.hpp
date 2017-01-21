#ifndef CLASS_LEVEL
#define CLASS_LEVEL

#include <vector>
#include <SFML\Graphics.hpp>

// forward declarations
class Tile;

class Level
{
private:

    std::vector< std::vector<int> > m_mapdata;

    std::vector< std::vector<Tile*> > m_tiles;

public:
    Level(int width = 10, int height = 10);
    ~Level();

    void fillMap(int tileid);

    int getWidth();
    int getHeight();

    int getTile(int x, int y);
    bool setTile(int x, int y, int tileid);

    bool isColliding(sf::FloatRect trect);

    void generate();

};
#endif // CLASS_LEVEL
