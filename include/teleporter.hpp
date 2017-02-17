#ifndef CLASS_TELEPORTER
#define CLASS_TELEPORTER

#include <SFML\Graphics.hpp>
#include <string>

struct Teleporter
{
    // origin
    sf::FloatRect m_area_rect;

    // destination
    std::string m_dest_level_name;
    sf::Vector2f m_dest_pos;

};

#endif // CLASS_TELEPORTER
