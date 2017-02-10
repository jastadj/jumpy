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

    // getters
    int getCurrentClip() { return m_ammo_current_clip;}
    int getMaxClip() { return m_ammo_max_clip;}
    int getRange() { return m_wpn_range;}
    int getFireRate() { return m_wpn_fire_rate;}
    int getReloadRate() { return m_wpn_reload_rate;}

    // setters
    void setCurrentClip(int tclip);
    void setMaxClip(int tmaxclip);
    int setRange(int trange);
    int setFireRate(int tfirerate);
    int setReloadRate(int treloadrate);

};




#endif // CLASS_WEAPON
