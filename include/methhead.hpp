#ifndef CLASS_METHHEAD
#define CLASS_METHHEAD

#include "actor.hpp"

class MethHead:public Actor
{
private:

public:
    MethHead();
    ~MethHead();

    int getType() { return OBJ_ACTOR;}
    XMLNode *saveToNode(XMLDocument *tdoc);

    void update();
};
#endif // CLASS_METHHEAD
