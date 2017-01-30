#ifndef CLASS_ACTOR
#define CLASS_ACTOR

#include "gameobj.hpp"

// forward declarations
class ParticleEmitter;

class Actor:public GameObj
{
protected:

    bool m_jumping;

    bool m_shooting;
    int m_shooting_timeout;
    int m_shoot_time;
    sf::Clock m_shooting_clock;

    ParticleEmitter *m_left_foot_emit;
    ParticleEmitter *m_right_foot_emit;

public:
    Actor();
    ~Actor();

    virtual int getType()=0;
    virtual XMLNode *saveToNode(XMLDocument *tdoc)=0;

    void doMove(int movedir);
    bool onLadder();
    bool jumping() { return m_jumping;}
    void setJumping(bool njumping) { m_jumping = njumping;}

    void addCollision(GameObj *tobj);

    void getShot(bool rightside = true);
    //virtual void update()=0;
};
#endif // CLASS_ACTOR
