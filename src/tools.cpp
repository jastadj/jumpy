#include "tools.hpp"

float getMagnitude(sf::Vector2f tvec)
{
    return sqrt( tvec.x*tvec.x + tvec.y*tvec.y);
}

sf::Vector2f normalizeVector(sf::Vector2f tvec)
{
    float mag = getMagnitude(tvec);

    return sf::Vector2f( tvec.x / mag, tvec.y / mag);
}

