#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include "actor.hpp"

class Player: public Actor
{
private:

    int m_current_meth;
    int m_max_meth;

public:
    Player();
    ~Player();
    int getType() { return OBJ_PLAYER;}

    void addMeth(int val);
    int getMeth() const { return m_current_meth;}
    int getMaxMeth() const { return m_max_meth;}

    //void doMove(int movedir);

    //void addCollision(GameObj *tobj);

    void update();
};

#endif // CLASS_PLAYER
