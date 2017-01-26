#ifndef CLASS_TILE
#define CLASS_TILE

#include "gameobj.hpp"

class SpriteSheet;

class Tile: public GameObj
{
private:

    // animation
    sf::Clock *m_anim_clock;
    int m_anim_time;
    std::vector<int> m_anim_sequence;

public:
    Tile(sf::Sprite *tsprite);
    Tile( const Tile &ttile);
    ~Tile();
    int getType() { return OBJ_TILE;}

    void addCollision(GameObj *tobj) {};

    void draw(sf::RenderTarget *tscreen);
    void update();

    void clearAnimationSequence();
    void addAnimationIndex(int nspriteindex);
    bool isAnimated() { if(m_anim_clock) return true; else return false;}

    //void draw(sf::RenderWindow *tscreen, int x, int y);
    //void draw(sf::RenderTarget *tscreen);
};

#endif
