#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include "actor.hpp"

// forward declarations
class ParticleEmitter;

class Player: public Actor
{
private:

    int m_current_meth;
    int m_max_meth;
    sf::Clock m_meth_timer;

    bool m_shooting;
    int m_shooting_timeout;
    int m_shoot_time;
    sf::Clock m_shooting_clock;


public:
    Player();
    ~Player();
    int getType() { return OBJ_PLAYER;}

    void addMeth(int val);
    int getMeth() const { return m_current_meth;}
    int getMaxMeth() const { return m_max_meth;}

    void doMove(int movedir);

    //void addCollision(GameObj *tobj);

    void shoot();

    void update();

    // test particles
    void pee();
};

#endif // CLASS_PLAYER
