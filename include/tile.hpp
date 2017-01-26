#ifndef CLASS_TILE
#define CLASS_TILE

#include "gameobj.hpp"

class SpriteSheet;

class Tile: public GameObj
{
private:


public:
    Tile(sf::Sprite *tsprite);
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
