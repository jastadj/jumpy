#ifndef CLASS_METH
#define CLASS_METH

#include "gameobj.hpp"

class Meth:public GameObj
{
private:

    int m_value;

public:
    Meth(int methval);
    ~Meth();

    int getType() { return OBJ_METH;}

    void addCollision(GameObj *tobj) {};

    int getMethValue() { return m_value;}

    void update();

};
#endif // CLASS_METH
