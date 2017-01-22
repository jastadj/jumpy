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

int rollDice(int num, int sides, int modifier)
{
    int total = 0;

    if( num <= 0 || sides <= 0) return 0;

    for(int i = 0; i < sides; i++)
    {
        total += rand()%sides + 1;
    }

    return total + modifier;

}
