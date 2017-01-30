#ifndef CLASS_TILE
#define CLASS_TILE

#include "gameobj.hpp"

class Tile: public GameObj
{
protected:

    bool m_animated;

public:
    Tile(sf::Sprite *tsprite);
    Tile( const Tile &ttile);
    ~Tile();
    virtual int getType() { return OBJ_TILE;}
    XMLNode *saveToNode(XMLDocument *tdoc) { return NULL;}

    void addCollision(GameObj *tobj) {};

    void setAnimated(bool nanim) { m_animated = nanim;}
    bool isAnimated() { return m_animated;}

    void draw(sf::RenderTarget *tscreen);
    void update();
};

class Ladder: public Tile
{
protected:

public:
    Ladder(sf::Sprite *tsprite);
    Ladder(const Ladder &ttile);
    ~Ladder();

    int getType() { return OBJ_TILE_LADDER;}
    //XMLNode *saveToNode(XMLDocument *tdoc) { return NULL;}

    //void addCollision(GameObj *tobj) {};

    //void setAnimated(bool nanim) { m_animated = nanim;}
    //bool isAnimated() { return m_animated;}

    //void draw(sf::RenderTarget *tscreen);
    //void update();
};

#endif
