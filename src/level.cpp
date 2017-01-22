#include "level.hpp"
#include <math.h>

#include "jumpy.hpp"
#include "tile.hpp"

Level::Level(int height, int width)
{
    m_jumpy = Jumpy::getInstance();

    if(height < 0) height = 1;
    if(width < 0) width = 1;

    // size array map
    m_mapdata.resize(height);
    m_mapdata_bg.resize(height);
    m_tiles.resize(height);
    m_tiles_bg.resize(height);
    for(int i = 0; i < height; i++)
    {
        m_mapdata[i].resize(width);
        m_mapdata_bg[i].resize(width);
        m_tiles[i].resize(width);
        m_tiles_bg[i].resize(width);
    }

    // null out tiles
    for(int i = 0; i < height; i++)
    {
        for(int n = 0; n < width; n++)
        {
            //std::cout << "x:" << n << " , y:" << i << std::endl;
            m_tiles[i][n] = NULL;
            m_tiles_bg[i][n] = NULL;
        }
    }

    fillMap(0);

    // create skybox texture
    std::vector<Tile*> *bgtiles = m_jumpy->getTilesBG();
    Tile *bgtile1 = new Tile( *(*bgtiles)[1]);
    Tile *bgtile2 = new Tile( *(*bgtiles)[2]);
    Tile *bgtile3 = new Tile( *(*bgtiles)[3]);
    int swidth = m_jumpy->getScreenWidth();
    int sheight = m_jumpy->getScreenHeight();
    m_skybox_rtxt.create(swidth, sheight);
    m_skybox_rtxt.display();
    /*
    for(int i = 0; i < (sheight/32)-5; i++)
    {
        for(int n = 0; n < (swidth/32)-5; n++)
        {
            bgtile1->setPosition( sf::Vector2f(n*32, i*32));
            bgtile1->update();
            bgtile1->draw(&m_skybox_rtxt);
        }
    }
    */
    for(int i = 0; i < 5; i++)
    {
        for(int n = 0; n < 13; n++)
        {
            bgtile1->setPosition( sf::Vector2f(n*32, i*32));
            bgtile1->update();
            bgtile1->draw(&m_skybox_rtxt);
        }
    }
    for(int n = 0; n < 13; n++)
    {
        bgtile2->setPosition( sf::Vector2f(n*32, 5*32));
        bgtile2->update();
        bgtile2->draw(&m_skybox_rtxt);
    }
    for(int n = 0; n < 13; n++)
    {
        bgtile3->setPosition( sf::Vector2f(n*32, 6*32));
        bgtile3->update();
        bgtile3->draw(&m_skybox_rtxt);
    }
    m_skybox_rtxt.display();
    m_skybox = sf::Sprite( m_skybox_rtxt.getTexture());
    delete bgtile1;
    delete bgtile2;
    delete bgtile3;
}

Level::~Level()
{
    for(int i = 0; i < getHeight(); i++)
    {
        for(int n = 0; n < getWidth(); n++)
        {
            delete m_tiles[i][n];
            delete m_tiles_bg[i][n];
        }
        m_tiles[i].clear();
        m_tiles_bg[i].clear();
    }
    m_tiles.clear();
    m_tiles_bg.clear();

}

int Level::getWidth()
{
    return int(m_mapdata[0].size());
}

int Level::getHeight()
{
    return int(m_mapdata.size());
}

void Level::fillMap(int tileid)
{
    int width = getWidth();
    int height = getHeight();

    for(int i = 0; i < height; i++)
    {
        for(int n = 0; n < width; n++)
        {
            setTile(n,i, tileid);
        }
    }
}

void Level::fillMapBG(int tileid)
{
    int width = getWidth();
    int height = getHeight();

    for(int i = 0; i < height; i++)
    {
        for(int n = 0; n < width; n++)
        {
            setTileBG(n,i, tileid);
        }
    }
}




int Level::getTile(int x, int y)
{
    if( x < 0 || y < 0 || x >= getWidth() || y >= getHeight()) return 0;

    return m_mapdata[y][x];
}

int Level::getTileBG(int x, int y)
{
    if( x < 0 || y < 0 || x >= getWidth() || y >= getHeight()) return 0;

    return m_mapdata_bg[y][x];
}

bool Level::setTile(int x, int y, int tileid)
{

    if( x < 0 || y < 0 || x >= getWidth() || y >= getHeight()) return false;

    if( m_mapdata[y][x] == tileid) return true;

    if( m_tiles[y][x])
    {
        delete m_tiles[y][x];
    }

    m_mapdata[y][x] = tileid;

    // if tile is blank
    if( tileid == 0)
    {
        // if there was an existing tile there before, delete it
        if(m_tiles[y][x]) delete m_tiles[y][x];
        m_tiles[y][x] = NULL;
    }
    else
    {
        m_tiles[y][x] = new Tile( *(*m_jumpy->getTiles())[tileid] );
        m_tiles[y][x]->setPosition( sf::Vector2f(x*32, y*32) );
        m_tiles[y][x]->update();
    }


    return true;
}

bool Level::setTileBG(int x, int y, int tileid)
{
    if( x < 0 || y < 0 || x >= getWidth() || y >= getHeight()) return false;

    if( m_mapdata_bg[y][x] == tileid) return true;

    if( m_tiles_bg[y][x])
    {
        delete m_tiles_bg[y][x];
    }

    m_mapdata_bg[y][x] = tileid;

    // if tile is blank
    if( tileid == 0)
    {
        // if there was an existing tile there before, delete it
        if(m_tiles_bg[y][x]) delete m_tiles_bg[y][x];
        m_tiles_bg[y][x] = NULL;
    }
    else
    {
        m_tiles_bg[y][x] = new Tile( *(*m_jumpy->getTilesBG())[tileid] );
        m_tiles_bg[y][x]->setPosition( sf::Vector2f(x*32, y*32) );
        m_tiles_bg[y][x]->update();
    }


    return true;
}

/*
void Level::generate()
{
    Jumpy *jumpy;
    jumpy = Jumpy::getInstance();

    std::vector< Tile*> *tiles = jumpy->getTiles();

    for(int i = 0; i < getHeight(); i++)
    {
        for( int n = 0; n < getWidth(); n++)
        {

        }
    }
}
*/

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

    // check if outside of map bounds
    if(trect.left < 0) return true;
    else if(trect.left + trect.width >= getWidth()*32) return true;

    // no collisions found
    return false;
}

void Level::drawTile(int x, int y, sf::RenderTarget *tscreen)
{
    if( x < 0 || y < 0 || x >= getWidth() || y >= getHeight())
    {
        std::cout << "Error in Tile::drawTile, x,y out of bounds\n";
        return;
    }

    if(tscreen == NULL)
    {
        std::cout << "Error in Tile::drawTile, tscreen = NULL!\n";
        return;
    }

    // ignore blanks
    if(m_tiles[y][x] == NULL) return;

    m_tiles[y][x]->draw(tscreen);

}

void Level::drawTileBG(int x, int y, sf::RenderTarget *tscreen)
{
    if( x < 0 || y < 0 || x >= getWidth() || y >= getHeight())
    {
        std::cout << "Error in Tile::drawTile, x,y out of bounds\n";
        return;
    }

    if(tscreen == NULL)
    {
        std::cout << "Error in Tile::drawTile, tscreen = NULL!\n";
        return;
    }

    // ignore blanks
    if(m_tiles_bg[y][x] == NULL) return;

    m_tiles_bg[y][x]->draw(tscreen);

}
