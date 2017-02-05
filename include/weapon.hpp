#ifndef CLASS_WEAPON
#define CLASS_WEAPON

#include "gameobj.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

class Weapon: public GameObj
{

private:

    // ammo
    int m_ammo_current_clip;
    int m_ammo_max_clip;

    // properties
    int m_wpn_range;
    int m_wpn_damage;
    float m_wpn_damage_multiplier;
    int m_wpn_fire_rate;
    int m_wpn_reload_rate;

public:
    Weapon(std::string nname);
    ~Weapon();
    int getType() { return OBJ_WEAPON;}
    XMLNode *saveToNode(XMLDocument *tdoc) {return NULL;}

    // getters


    // setters


};




#endif // CLASS_WEAPON
