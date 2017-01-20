#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include "gameobj.hpp"

class Player : public GameObj
{
private:

    // main callback
    Jumpy *m_jumpy;

public:
    Player(Jumpy *jumpy);
    ~Player();
};

#endif // CLASS_PLAYER
