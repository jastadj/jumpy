#include "level.hpp"

#include <math.h>
#include <sstream>

#include "jumpy.hpp"
#include "tile.hpp"
#include "meth.hpp"
#include "methhead.hpp"
#include "decoration.hpp"
#include "player.hpp"

#include "tools.hpp"

#include "tinyxml2.h"

using namespace tinyxml2;

struct sortable
{
    GameObj *gobj;
    float dist;
};

Level::Level(int width, int height)
{
    m_initialized = init(width, height);

    m_filename = std::string("temp.xml");
}

Level::Level(std::string filename)
{
    m_initialized = false;

    m_filename = filename;

    if(!load(m_filename))
    {
        std::cout << "Error loading level!\n";
    }

}

Level::~Level()
{
    // delete tiles
    for(int i = 0; i < getHeight(); i++)
    {
        for(int n = 0; n < getWidth(); n++)
        {
            delete m_tiles[i][n];
            delete m_tiles_bg[i][n];
            delete m_tiles_fg[i][n];
        }
        m_tiles[i].clear();
        m_tiles_bg[i].clear();
        m_tiles_fg[i].clear();
    }
    m_tiles.clear();
    m_tiles_bg.clear();
    m_tiles_fg.clear();

    // delete objects
    for(int i = 0; i < int(m_objects.size()); i++)
    {
        delete m_objects[i];
    }
    m_objects.clear();

    // delete decorations
    for(int i = 0; i < int(m_decorations.size()); i++)
    {
        delete m_decorations[i];
    }
    m_decorations.clear();

    std::cout << "Deleted level " << m_level_name << std::endl;
}

bool Level::init(int width, int height)
{
    if(m_initialized)
    {
        std::cout << "Level already initialized!\n";
        return false;
    }

    m_jumpy = Jumpy::getInstance();

    m_fade_fg = false;

    if(height < 0) height = 1;
    if(width < 0) width = 1;

    // size array map
    m_mapdata.resize(height);
    m_mapdata_bg.resize(height);
    m_mapdata_fg.resize(height);
    m_tiles.resize(height);
    m_tiles_bg.resize(height);
    m_tiles_fg.resize(height);
    for(int i = 0; i < height; i++)
    {
        m_mapdata[i].resize(width);
        m_mapdata_bg[i].resize(width);
        m_mapdata_fg[i].resize(width);
        m_tiles[i].resize(width);
        m_tiles_bg[i].resize(width);
        m_tiles_fg[i].resize(width);
    }

    // null out tiles
    for(int i = 0; i < height; i++)
    {
        for(int n = 0; n < width; n++)
        {
            //std::cout << "x:" << n << " , y:" << i << std::endl;
            m_tiles[i][n] = NULL;
            m_tiles_bg[i][n] = NULL;
            m_tiles_fg[i][n] = NULL;
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

    return true;
}

void Level::setPlayerStartPos(sf::Vector2f tpos)
{
    m_player_start_pos = tpos + sf::Vector2f(0,-1.f);
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

void Level::fillMapFG(int tileid)
{
    int width = getWidth();
    int height = getHeight();

    for(int i = 0; i < height; i++)
    {
        for(int n = 0; n < width; n++)
        {
            setTileFG(n,i, tileid);
        }
    }
}

void Level::resizeX(int tdir)
{
    int width = getWidth();
    int height = getHeight();

    if(tdir < 0)
    {
        for(int i = 0; i < height; i++)
        {
            setTile(width-1, i, 0);
            setTileBG(width-1, i, 0);
            setTileFG(width-1, i, 0);

            m_mapdata[i].resize(width-1);
            m_mapdata_bg[i].resize(width-1);
            m_mapdata_fg[i].resize(width-1);

            m_tiles[i].resize(width-1);
            m_tiles_bg[i].resize(width-1);
            m_tiles_fg[i].resize(width-1);
        }
    }
    else if(tdir > 0)
    {
        for(int i = 0; i < height; i++)
        {
            m_mapdata[i].resize(width+1);
            m_mapdata[i].back() = 0;
            m_mapdata_bg[i].resize(width+1);
            m_mapdata_bg[i].back() = 0;
            m_mapdata_fg[i].resize(width+1);
            m_mapdata_fg[i].back() = 0;

            m_tiles[i].resize(width+1);
            m_tiles[i].back() = NULL;
            m_tiles_bg[i].resize(width+1);
            m_tiles_bg[i].back() = NULL;
            m_tiles_fg[i].resize(width+1);
            m_tiles_fg[i].back() = NULL;

            setTile(width, i, getTile(width-1, i));
            setTileBG(width, i, getTileBG(width-1, i));
            setTileFG(width, i, getTileFG(width-1, i));

        }
    }
}

void Level::resizeY(int tdir)
{
    int width = getWidth();
    int height = getHeight();

    sf::Vector2f pos_offset;

    // if decreasing height
    if(tdir < 0)
    {
        // clear top row tiles
        for(int i = 0; i < width; i++)
        {
            setTile(i, 0, 0);
            setTileBG(i, 0, 0);
            setTileFG(i, 0, 0);
        }

        // remove top row
        m_mapdata.erase(m_mapdata.begin());
        m_mapdata_bg.erase(m_mapdata_bg.begin());
        m_mapdata_fg.erase(m_mapdata_fg.begin());

        m_tiles.erase(m_tiles.begin());
        m_tiles_bg.erase(m_tiles_bg.begin());
        m_tiles_fg.erase(m_tiles_fg.begin());

        pos_offset = sf::Vector2f(0,-32);
    }
    else if(tdir > 0)
    {
        // copy top row of tile data
        m_mapdata.insert(m_mapdata.begin(), m_mapdata[0]);
        m_mapdata_bg.insert(m_mapdata_bg.begin(), m_mapdata_bg[0]);
        m_mapdata_fg.insert(m_mapdata_fg.begin(), m_mapdata_fg[0]);

        // insert null top row of tiles
        std::vector<Tile*> nulltiles;
        nulltiles.resize(width);
        for(int i = 0; i < width; i++) nulltiles[i] = NULL;
        m_tiles.insert(m_tiles.begin(), nulltiles);
        m_tiles_bg.insert(m_tiles_bg.begin(), nulltiles);
        m_tiles_fg.insert(m_tiles_fg.begin(), nulltiles);

        // set tiles to map data
        for(int i = 0; i < width; i++) setTile(i, 0, m_mapdata[0][i]);
        for(int i = 0; i < width; i++) setTileBG(i, 0, m_mapdata_bg[0][i]);
        for(int i = 0; i < width; i++) setTileFG(i, 0, m_mapdata_fg[0][i]);

        pos_offset = sf::Vector2f(0,32);

    }
    else return;

    // reposition all map tile sprites
    for(int i = 0; i < int(m_tiles.size()); i++)
    {
        for(int n = 0; n < int(m_tiles[i].size()); n++)
        {
            if(m_tiles[i][n])
            {
                m_tiles[i][n]->setPosition( sf::Vector2f(n*32, i*32));
                m_tiles[i][n]->update();
            }

            if(m_tiles_bg[i][n])
            {
                m_tiles_bg[i][n]->setPosition( sf::Vector2f(n*32, i*32));
                m_tiles_bg[i][n]->update();
            }

            if(m_tiles_fg[i][n])
            {
                m_tiles_fg[i][n]->setPosition( sf::Vector2f(n*32, i*32));
                m_tiles_fg[i][n]->update();
            }

        }
    }

    // reposition everything on the map

        // reposition player
        m_jumpy->getPlayer()->setPosition( m_jumpy->getPlayer()->getPosition() + pos_offset);
        // reposition decorations
        for(int i = 0; i < int(m_decorations.size()); i++) m_decorations[i]->setPosition( m_decorations[i]->getPosition() + pos_offset);
        // reposition game objects
        for(int i = 0; i < int(m_objects.size()); i++) m_objects[i]->setPosition( m_objects[i]->getPosition() + pos_offset);

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

int Level::getTileFG(int x, int y)
{
    if( x < 0 || y < 0 || x >= getWidth() || y >= getHeight()) return 0;

    return m_mapdata_fg[y][x];
}

bool Level::setTile(int x, int y, int tileid)
{

    if( x < 0 || y < 0 || x >= getWidth() || y >= getHeight()) return false;

    //if( m_mapdata[y][x] == tileid) return true;

    if( m_tiles[y][x])
    {
        delete m_tiles[y][x];
        m_tiles[y][x] = 0;
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

    //if( m_mapdata_bg[y][x] == tileid) return true;

    if( m_tiles_bg[y][x])
    {
        delete m_tiles_bg[y][x];
        m_tiles_bg[y][x] = 0;
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

bool Level::setTileFG(int x, int y, int tileid)
{
    if( x < 0 || y < 0 || x >= getWidth() || y >= getHeight()) return false;

    //if( m_mapdata_fg[y][x] == tileid) return true;

    if( m_tiles_fg[y][x])
    {
        delete m_tiles_fg[y][x];
        m_tiles_fg[y][x] = 0;
    }

    m_mapdata_fg[y][x] = tileid;

    // if tile is blank
    if( tileid == 0)
    {
        // if there was an existing tile there before, delete it
        if(m_tiles_fg[y][x]) delete m_tiles_fg[y][x];
        m_tiles_fg[y][x] = NULL;
    }
    else
    {
        m_tiles_fg[y][x] = new Tile( *(*m_jumpy->getTilesFG())[tileid] );
        m_tiles_fg[y][x]->setPosition( sf::Vector2f(x*32, y*32) );
        m_tiles_fg[y][x]->update();
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

bool Level::isCollidingWithMap(sf::FloatRect trect)
{
    /*
    sf::IntRect nearby_tiles( (int(trect.left)-32)/32, (int(trect.top)-32)/32, (ceil(trect.left+trect.width)+32)/32, (ceil(trect.top+trect.height)+32)/32 );

    int topx = nearby_tiles.left;
    int topy = nearby_tiles.top;
    int botx = nearby_tiles.left + nearby_tiles.width;
    int boty = nearby_tiles.top + nearby_tiles.height;

    // check if rectangle is colliding with a map tile
    for( int i = nearby_tiles.top; i <= (nearby_tiles.top+nearby_tiles.height); i++)
    {
        for(int n = nearby_tiles.left; n <= (nearby_tiles.left + nearby_tiles.width); n++)
        {
            if( n < 0 || i < 0 || n >= getWidth() || i >= getHeight()) continue;
            // rect is colliding with tile
            if( getTile(n,i) != 0)
            {
                std::cout << "colliding with tile " << n << "," << i << std::endl;
                return true;
            }
        }
    }

    // no collisions found
    return false;
    */

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

void Level::getObjectCollisions(sf::FloatRect trect, GameObj *source)
{
    for(int i= 0; i < int(m_objects.size()); i++)
    {
        if(m_objects[i]->getBoundingBox().intersects( source->getBoundingBox()) )
        {
            source->addCollision(m_objects[i]);
        }
    }
}

std::vector<GameObj*> Level::getObjectCollisionsWithLine(sf::Vector2f p1, sf::Vector2f p2)
{

    std::vector<sortable> objs_intersected;
    std::vector<sortable> objs_sorted;
    std::vector<GameObj*> objs;

    // get objects that are intersected by line
    for(int i = 0; i < int(m_objects.size()); i++)
    {
        if( lineIntersectsRect(p1, p2, m_objects[i]->getBoundingBox()) )
        {
            //std::cout << "found los intersect with " << m_objects[i]->getName() << std::endl;

            sortable newsortable;
            newsortable.gobj = m_objects[i];
            newsortable.dist = getDistance(p1, m_objects[i]->getPosition());

            objs_intersected.push_back( newsortable );

        }
    }

    // sort intersected objects by distance to player
    for(int i = 0; i < int(objs_intersected.size()); i++)
    {
        if( objs_sorted.empty()) objs_sorted.push_back(objs_intersected[i]);
        else
        {
            bool didsort = false;

            for(int n = 0; n < int(objs_sorted.size()); n++)
            {
                if(objs_intersected[i].dist <= objs_sorted[n].dist)
                {
                    objs_sorted.insert(objs_sorted.begin() + n, objs_intersected[i]);
                    didsort = true;
                    break;
                }
            }

            if(!didsort) objs_sorted.push_back( objs_intersected[i]);
        }

    }

    // create new list of game objects from sorted list
    for(int i = 0; i < int(objs_sorted.size()); i++)
    {
        //std::cout << " LOS HIT : " << objs_sorted[i].gobj->getName() << " at dist:" << objs_sorted[i].dist << std::endl;
        objs.push_back( objs_sorted[i].gobj);
    }

    return objs;
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

    if(m_tiles[y][x]->isAnimated()) m_tiles[y][x]->update();
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

void Level::drawTileFG(int x, int y, sf::RenderTarget *tscreen, bool fade)
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
    if(m_tiles_fg[y][x] == NULL) return;


    // if fade mode changed
    if(m_fade_fg != fade)
    {
        m_fade_fg = fade;
        m_fg_fade_timer.restart();
    }

    float fadeamount = 0;
    static float fadecap = 300;
    static float fademod = fadecap / 255.f;

    if(!m_fade_fg)
    {
        if(m_fg_fade_timer.getElapsedTime().asMilliseconds() > fadecap) fadeamount = 255.f;
        else fadeamount =  m_fg_fade_timer.getElapsedTime().asMilliseconds() * fademod;
    }
    else
    {
        if(m_fg_fade_timer.getElapsedTime().asMilliseconds() > fadecap) fadeamount = 0;
        else fadeamount =  m_fg_fade_timer.getElapsedTime().asMilliseconds() * fademod * (-1) + 255.f;
    }
    // y = mx + b

    if(fadeamount < 0) fadeamount = 0;
    else if(fadeamount > 255) fadeamount = 255;

    sf::Color tcolor = m_tiles_fg[y][x]->getSprite()->getColor();
    tcolor.a = int(fadeamount);
    m_tiles_fg[y][x]->getSprite()->setColor(tcolor);
    m_tiles_fg[y][x]->draw(tscreen);

}

bool Level::addObject(GameObj *tobj)
{
    if(tobj == NULL) return false;

    m_objects.push_back(tobj);
}

bool Level::deleteObject( GameObj *tobj)
{
    if(tobj == NULL)
    {
        std::cout << "Error deleting obj from level, obj is null!\n";
        return false;
    }

    for(int i = 0; i < int(m_objects.size()); i++)
    {
        if( m_objects[i] == tobj)
        {
            delete m_objects[i];
            m_objects.erase(m_objects.begin()+i);
            return true;
        }
    }

    std::cout << "Error deleting obj from level, obj not found!\n";
    return false;
}

void Level::addDecoration(int dindex, sf::Vector2f dpos)
{
    Decoration *newdec = new Decoration( *(*m_jumpy->getDecorations())[dindex] );
    newdec->setPosition(dpos);
    newdec->update();
    newdec->setID(dindex);
    m_decorations.push_back(newdec);
}

bool Level::deleteDecoration(Decoration *tdec)
{
    if(tdec == NULL) return false;

    for(int i = int(m_decorations.size())-1; i >= 0; i--)
    {
        if( m_decorations[i] == tdec)
        {
            delete m_decorations[i];
            m_decorations.erase( m_decorations.begin() + i);
            return true;
        }
    }

    return false;
}

void Level::drawObjects(sf::RenderTarget *tscreen)
{
    for(int i = 0; i < int(m_objects.size()); i++)
    {
        m_objects[i]->draw(tscreen);
    }
}

void Level::drawDecorations(sf::RenderTarget *tscreen)
{
    for(int i = 0; i < int(m_decorations.size()); i++)
    {
        m_decorations[i]->draw(tscreen);
    }
}

void Level::update()
{
    // update all level objects
    for(int i = 0; i < int(m_objects.size()); i++)
    {
        m_objects[i]->update();
    }

    // update all decorations
    for(int i = 0; i < int(m_decorations.size()); i++)
    {
        if(m_decorations[i]->isAnimated()) m_decorations[i]->update();
    }
}


bool Level::save(std::string filename)
{
    std::cout << "Saving level " << LEVEL_FILE << filename << std::endl;

    m_filename = filename;

    XMLDocument ldoc;

    // add root tags
    XMLNode *root = ldoc.NewElement("Methd_Up");
    ldoc.InsertFirstChild(root);

    // create level node
    XMLNode *anode = ldoc.NewElement("Level");
    root->InsertEndChild(anode);

    // level name
    XMLElement *element = ldoc.NewElement("Name");
    element->SetText(m_level_name.c_str());
    anode->InsertEndChild(element);

    // tile data
    element = ldoc.NewElement("Width");
    element->SetText(getWidth());
    anode->InsertEndChild(element);

    element = ldoc.NewElement("Height");
    element->SetText(getHeight());
    anode->InsertEndChild(element);

    // player starting position
    element = ldoc.NewElement("PlayerStartX");
    element->SetText(m_player_start_pos.x);
    anode->InsertEndChild(element);

    element = ldoc.NewElement("PlayerStartY");
    element->SetText(m_player_start_pos.y);
    anode->InsertEndChild(element);

    // tile array node
    anode = ldoc.NewElement("Tiles");
    root->FirstChildElement("Level")->InsertEndChild(anode);

    // add rows
    for(int i = 0; i < getHeight(); i++)
    {
        std::stringstream rowdata;

        for(int n = 0; n < getWidth(); n++)
        {
            if( n != getWidth()-1) rowdata << m_mapdata[i][n] << ",";
            else rowdata << m_mapdata[i][n];
        }

        element = ldoc.NewElement("Row");
        element->SetText(rowdata.str().c_str());
        anode->InsertEndChild(element);

    }

    // tile bg array node
    anode = ldoc.NewElement("TilesBG");
    root->FirstChildElement("Level")->InsertEndChild(anode);

    // add rows
    for(int i = 0; i < getHeight(); i++)
    {
        std::stringstream rowdata;

        for(int n = 0; n < getWidth(); n++)
        {
            if( n != getWidth()-1) rowdata << m_mapdata_bg[i][n] << ",";
            else rowdata << m_mapdata_bg[i][n];
        }

        element = ldoc.NewElement("Row");
        element->SetText(rowdata.str().c_str());
        anode->InsertEndChild(element);

    }

    // tile fg array node
    anode = ldoc.NewElement("TilesFG");
    root->FirstChildElement("Level")->InsertEndChild(anode);

    // add rows
    for(int i = 0; i < getHeight(); i++)
    {
        std::stringstream rowdata;

        for(int n = 0; n < getWidth(); n++)
        {
            if( n != getWidth()-1) rowdata << m_mapdata_fg[i][n] << ",";
            else rowdata << m_mapdata_fg[i][n];
        }

        element = ldoc.NewElement("Row");
        element->SetText(rowdata.str().c_str());
        anode->InsertEndChild(element);

    }

    // objects
    anode = ldoc.NewElement("Objects");
    root->FirstChildElement("Level")->InsertEndChild(anode);

    // save decorations
    for(int i = 0; i < int(m_decorations.size()); i++)
    {
        anode->InsertEndChild( m_decorations[i]->saveToNode(&ldoc));
    }

    // save objects
    for(int i = 0; i < int(m_objects.size()); i++)
    {
        anode->InsertEndChild( m_objects[i]->saveToNode(&ldoc));
    }


    if(ldoc.SaveFile( std::string( std::string(LEVEL_FILE) + filename).c_str() ) ) return false;
    else return true;
}

bool Level::load(std::string filename)
{

    int height = 0;
    int width = 0;

    if(m_initialized)
    {
        std::cout << "Error loading level, already initialized!\n";
        return false;
    }

    XMLDocument tdoc;
    XMLNode *root;
    XMLNode *anode;
    XMLNode *lnode;
    XMLElement *element;

    int errorcode = tdoc.LoadFile( std::string( std::string(LEVEL_FILE) + filename).c_str() );
    if(errorcode)
    {
        std::cout << "Error loading level from " << LEVEL_FILE << filename << ", error code:" << errorcode << std::endl;
        return false;
    }

    root = tdoc.FirstChildElement("Methd_Up");
    if(!root)
    {
        std::cout << "Not a valid Methd Up file!\n";
        return false;
    }

    lnode = root->FirstChildElement("Level");
    if(!lnode)
    {
        std::cout << "Not a valid level file!\n";
        return false;
    }

    // level name
    element = lnode->FirstChildElement("Name");
    if(element)
        m_level_name = element->GetText();

    // level dimensions
    element = lnode->FirstChildElement("Width");
    if(!element) { std::cout << "Error loading level, no width!\n"; return false;}
    element->QueryIntText(&width);

    element = lnode->FirstChildElement("Height");
    if(!element) { std::cout << "Error loading level, no height!\n"; return false;}
    element->QueryIntText(&height);

    // player starting position
    element = lnode->FirstChildElement("PlayerStartX");
    if(element)
    {
        element->QueryFloatText(&m_player_start_pos.x);
    }

    element = lnode->FirstChildElement("PlayerStartY");
    if(element)
    {
        element->QueryFloatText(&m_player_start_pos.y);
    }

    std::cout << "Loading level with dimensions " << width << "x" << height << std::endl;

    if(!init(width, height) )
    {
        std::cout << "Error initializing level!\n";
        return false;
    }

    // load tile data
    anode = lnode->FirstChildElement("Tiles");
    if(!anode){ std::cout << "Error loading level, no tile data found!\n"; return false;}
    XMLElement *trow;
    int rowcount = 0;
    trow = anode->FirstChildElement("Row");
    if(!trow) { std::cout << "Error loading level, no tile data row found!\n"; return false;}

    while(trow)
    {
        std::string rowstr;
        std::vector<std::string> rowdata;
        rowstr = trow->GetText();

        rowdata = csvParse(rowstr);

        for(int i = 0; i < int(rowdata.size()); i++)
        {
            setTile(i, rowcount, atoi( rowdata[i].c_str()));
        }

        rowcount++;

        trow = trow->NextSiblingElement("Row");
    }

    rowcount = 0;

    anode = lnode->FirstChildElement("TilesBG");
    if(!anode){ std::cout << "Error loading level, no bg tile data found!\n"; return false;}
    trow = anode->FirstChildElement("Row");
    if(!trow) { std::cout << "Error loading level, not bg tile data row found!\n"; return false;}

    while(trow)
    {
        std::string rowstr;
        std::vector<std::string> rowdata;
        rowstr = trow->GetText();

        rowdata = csvParse(rowstr);

        for(int i = 0; i < int(rowdata.size()); i++)
        {
            setTileBG(i, rowcount, atoi( rowdata[i].c_str()));
        }

        rowcount++;

        trow = trow->NextSiblingElement("Row");
    }

    rowcount = 0;

    anode = lnode->FirstChildElement("TilesFG");
    if(!anode){ std::cout << "Error loading level, no fg tile data found!\n"; return false;}
    trow = anode->FirstChildElement("Row");
    if(!trow) { std::cout << "Error loading level, not fg tile data row found!\n"; return false;}

    while(trow)
    {
        std::string rowstr;
        std::vector<std::string> rowdata;
        rowstr = trow->GetText();

        rowdata = csvParse(rowstr);

        for(int i = 0; i < int(rowdata.size()); i++)
        {
            setTileFG(i, rowcount, atoi( rowdata[i].c_str()));
        }

        rowcount++;

        trow = trow->NextSiblingElement("Row");
    }

    // load objects
    anode = lnode->FirstChildElement("Objects");
    XMLNode *onode;
    if(anode)
    {
        onode = anode->FirstChildElement();

        // read in each object
        while(onode)
        {
            std::string objtype = onode->Value();

            if(objtype == "Decoration")
            {
                int decid = 0;
                float dx = 0;
                float dy = 0;

                XMLElement *dd = onode->FirstChildElement("ID");
                if(!dd) {std::cout << "Error loading level, decoration id not present!\n"; return false;}

                dd->QueryIntText(&decid);

                dd = onode->FirstChildElement("X");
                dd->QueryFloatText(&dx);
                dd = onode->FirstChildElement("Y");
                dd->QueryFloatText(&dy);

                addDecoration(decid, sf::Vector2f(dx, dy));

            }
            else if(objtype == "Obj")
            {
                int di = 0;
                float dx = 0;
                float dy = 0;

                XMLElement *dd = onode->FirstChildElement("Type");
                dd->QueryIntText(&di);

                dd = onode->FirstChildElement("X");
                dd->QueryFloatText(&dx);
                dd = onode->FirstChildElement("Y");
                dd->QueryFloatText(&dy);

                GameObj *newobj = m_jumpy->createObject( GAMEOBJTYPES(di) );

                if(!newobj)
                {
                    std::cout << "Error creating game object type " << di << std::endl;
                    return false;
                }

                newobj->setPosition(sf::Vector2f(dx, dy));
                addObject(newobj);
            }

            onode = onode->NextSiblingElement();
        }


    }

    std::cout << "Loaded level " << m_level_name << " from file " << filename << std::endl;

    return true;
}

void Level::show()
{
    std::cout << "LEVEL:\n";
    std::cout << "------\n";
    std::cout << "Name:" << getName() << std::endl;
    std::cout << "Filename:" << getFilename() << std::endl;
    std::cout << "Width:" << getWidth() << std::endl;
    std::cout << "Height:" << getHeight() << std::endl;
    std::cout << "PlayerStartPos:" << m_player_start_pos.x << "," << m_player_start_pos.y << std::endl;

}
