#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include "gameobj.hpp"

class Player: public GameObj
{
private:

public:
    Player();
    ~Player();
    int getType() { return OBJ_PLAYER;}
};

#endif // CLASS_PLAYER
