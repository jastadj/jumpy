#ifndef CLASS_DECORATION
#define CLASS_DECORATION

#include "tile.hpp"

class Decoration: public GameObj
{
protected:

    bool m_animated;

public:
    Decoration(sf::Sprite *tsprite);
    Decoration(const Decoration &tdec);
    ~Decoration();

    int getType() { return OBJ_DECORATION;}
    void addCollision(GameObj *tobj) {};

    void setAnimated(bool nanim) { m_animated = nanim;}
    bool isAnimated() { return m_animated;}

    void draw(sf::RenderTarget *tscreen);
    void update();
};
#endif // CLASS_DECORATION
