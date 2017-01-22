#ifndef CLASS_TILE
#define CLASS_TILE

#include "gameobj.hpp"

class SpriteSheet;

class Tile: public GameObj
{
private:

    int m_tile_sprite_index;
    SpriteSheet *m_tile_sprite_sheet;

public:
    Tile(SpriteSheet *tsheet, int spriteindex);
    Tile( const Tile &ttile);
    ~Tile();
    int getType() { return OBJ_TILE;}

    void addCollision(GameObj *tobj) {};

    void draw(sf::RenderTarget *tscreen);
    void update();

    //void draw(sf::RenderWindow *tscreen, int x, int y);
    //void draw(sf::RenderTarget *tscreen);
};

#endif
