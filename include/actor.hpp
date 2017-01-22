#ifndef CLASS_ACTOR
#define CLASS_ACTOR

#include "gameobj.hpp"

class Actor:public GameObj
{
protected:

    bool m_jumping;

public:
    Actor();
    ~Actor();

    virtual int getType()=0;

    void doMove(int movedir);
    bool jumping() { return m_jumping;}
    void setJumping(bool njumping) { m_jumping = njumping;}

    void addCollision(GameObj *tobj);

    //virtual void update()=0;
};
#endif // CLASS_ACTOR
