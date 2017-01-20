#ifndef CLASS_TILE
#define CLASS_TILE

#include "gameobj.hpp"

class SpriteSheet;

class Tile: public GameObj
{
private:

public:
    Tile(SpriteSheet *tsheet, int spriteindex);
    ~Tile();
    int getType() { return OBJ_TILE;}

    void draw(sf::RenderWindow *tscreen, int x, int y);
};

#endif
