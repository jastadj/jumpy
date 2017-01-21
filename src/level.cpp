#include "level.hpp"
#include <math.h>

Level::Level(int height, int width)
{
    if(height < 0) height = 1;
    if(width < 0) width = 1;

    // size array map
    m_map.resize(height);
    for(int i = 0; i < height; i++) m_map[i].resize(width);

    fillMap(0);
}

Level::~Level()
{

}

int Level::getWidth()
{
    return int(m_map[0].size());
}

int Level::getHeight()
{
    return int(m_map.size());
}

void Level::fillMap(int tileid)
{
    int width = getWidth();
    int height = getHeight();

    for(int i = 0; i < height; i++)
    {
        for(int n = 0; n < width; n++)
        {
            m_map[i][n] = tileid;
        }
    }
}

int Level::getTile(int x, int y)
{
    if( x < 0 || y < 0 || x >= getWidth() || y >= getHeight()) return 0;

    return m_map[y][x];
}

bool Level::setTile(int x, int y, int tileid)
{
    if( x < 0 || y < 0 || x >= getWidth() || y >= getHeight()) return false;

    m_map[y][x] = tileid;

    return true;
}

bool Level::isColliding(sf::FloatRect trect)
{
    // check if rectangle is colliding with a map tile
    for( int i = floor(trect.top)/32; i <= ceil(trect.top + trect.height)/32; i++)
    {
        for(int n = floor(trect.left)/32; n <= ceil(trect.left + trect.width)/32; n++)
        {
            // rect is colliding with tile
            if( getTile(n,i) != 0) return true;
        }
    }

    // no collisions found
    return false;
}
