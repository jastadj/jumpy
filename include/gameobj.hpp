#ifndef CLASS_GAMEOBJ
#define CLASS_GAMEOBJ

#include "jumpy.hpp"

class GameObj
{
private:

    Jumpy *m_jumpy;

protected:

public:
    GameObj(Jumpy *jumpy);
    ~GameObj();
};
#endif // CLASS_GAMEOBJ
