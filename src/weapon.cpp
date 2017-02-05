#include "weapon.hpp"

Weapon::Weapon(std::string nname)
{
    m_name = nname;
}

Weapon::~Weapon()
{

}

void Weapon::setCurrentClip(int tclip)
{
    if(tclip < 0) tclip = 0;
    else if(tclip > m_ammo_max_clip)
        tclip = m_ammo_max_clip;

    m_ammo_current_clip = tclip;
}

void Weapon::setMaxClip(int tmaxclip)
{
    if(tmaxclip < 0) tmaxclip = 1;

    m_ammo_max_clip = tmaxclip;

    if(m_ammo_current_clip > m_ammo_max_clip)
        m_ammo_current_clip = m_ammo_max_clip;

}

int Weapon::setRange(int trange)
{
    if(trange < 0) trange = 1;
    m_wpn_range = trange;
}

int Weapon::setFireRate(int tfirerate)
{
    if(tfirerate < 0) tfirerate = 1;

    m_wpn_fire_rate = tfirerate;
}

int Weapon::setReloadRate(int treloadrate)
{
    if(treloadrate < 0) treloadrate = 1;

    m_wpn_reload_rate = treloadrate;
}
