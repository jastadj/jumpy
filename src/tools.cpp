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

void drawFloatRect(sf::FloatRect trect, sf::RenderTarget *tscreen)
{
    sf::RectangleShape rbox(sf::Vector2f( trect.width-2, trect.height-2));
    rbox.setOutlineThickness(1);
    rbox.setFillColor(sf::Color::Transparent);
    rbox.setOutlineColor(sf::Color::Green);
    rbox.setPosition(trect.left+1, trect.top+1);

    tscreen->draw(rbox);

}

bool lineIntersectsRect(sf::Vector2f p1, sf::Vector2f p2, sf::FloatRect trect, std::vector<sf::Vector2f> *ipoints)
{
    if( hasIntersection(p1.x, p1.y, p2.x, p2.y, trect.left, trect.top, trect.left + trect.width, trect.top))
        return true;
    else if( hasIntersection(p1.x, p1.y, p2.x, p2.y, trect.left + trect.width, trect.top, trect.left + trect.width, trect.top + trect.height))
        return true;
    else if( hasIntersection(p1.x, p1.y, p2.x, p2.y, trect.left, trect.top + trect.height, trect.left + trect.width, trect.top+trect.height))
        return true;
    else if( hasIntersection(p1.x, p1.y, p2.x, p2.y, trect.left, trect.top, trect.left, trect.top+trect.height))
        return true;

    // no intersection found
    return false;
}

float getDistance(sf::Vector2f p1, sf::Vector2f p2)
{
    return sqrt( (p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y) );
}

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the floats i_x and i_y.
bool hasIntersection(float p0_x, float p0_y, float p1_x, float p1_y,
    float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (i_x != NULL)
            *i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            *i_y = p0_y + (t * s1_y);
        return true;
    }

    return false; // No collision
}
