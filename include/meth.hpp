#ifndef CLASS_METH
#define CLASS_METH

#include "gameobj.hpp"

class Meth:public GameObj
{
private:

    int m_methtype;

public:
    Meth(int nmethtype);
    ~Meth();

    int getType() { return m_methtype;}

    void addCollision(GameObj *tobj) {};

    int getMethValue();

    void update();

};
#endif // CLASS_METH
