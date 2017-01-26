#include "level.hpp"

#include <math.h>
#include <sstream>

#include "jumpy.hpp"
#include "tile.hpp"
#include "meth.hpp"
#include "methhead.hpp"
#include "decoration.hpp"

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
}

Level::Level(std::string filename)
{
    m_initialized = false;

    if(!load(filename))
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
        }
        m_tiles[i].clear();
        m_tiles_bg[i].clear();
    }
    m_tiles.clear();
    m_tiles_bg.clear();

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
}

bool Level::init(int width, int height)
{
    if(m_initialized)
    {
        std::cout << "Level already initialized!\n";
        return false;
    }

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

    return true;
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

bool Level::addObject(GameObj *tobj)
{
    if(tobj == NULL) return false;

    m_objects.push_back(tobj);
}

void Level::addMeth(int val, sf::Vector2f tpos)
{
    Meth *newmeth = new Meth(val);
    newmeth->setPosition( tpos );
    newmeth->update();

    addObject(newmeth);
}

void Level::addMethHead(sf::Vector2f tpos)
{
    MethHead *newmethhead = new MethHead();
    newmethhead->setPosition( tpos);

    addObject(newmethhead);
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
    std::cout << "Saving level " << filename << std::endl;

    XMLDocument ldoc;

    // add root tags
    XMLNode *root = ldoc.NewElement("Methd_Up");
    ldoc.InsertFirstChild(root);

    // create level node
    XMLNode *anode = ldoc.NewElement("Level");
    root->InsertEndChild(anode);

    // tile data
    XMLElement *element = ldoc.NewElement("Width");
    element->SetText(getWidth());
    anode->InsertEndChild(element);

    element = ldoc.NewElement("Height");
    element->SetText(getHeight());
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


    if(ldoc.SaveFile(filename.c_str())) return false;
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

    int errorcode = tdoc.LoadFile(filename.c_str());
    if(errorcode)
    {
        std::cout << "Error loading level from " << filename << ", error code:" << errorcode << std::endl;
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

    element = lnode->FirstChildElement("Width");
    if(!element) { std::cout << "Error loading level, no width!\n"; return false;}
    element->QueryIntText(&width);

    element = lnode->FirstChildElement("Height");
    if(!element) { std::cout << "Error loading level, no height!\n"; return false;}
    element->QueryIntText(&height);

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
            else if(objtype == "Meth")
            {
                int di = 0;
                float dx = 0;
                float dy = 0;

                XMLElement *dd = onode->FirstChildElement("Value");
                dd->QueryIntText(&di);

                dd = onode->FirstChildElement("X");
                dd->QueryFloatText(&dx);
                dd = onode->FirstChildElement("Y");
                dd->QueryFloatText(&dy);

                addMeth(di, sf::Vector2f(dx,dy));
            }
            else if(objtype == "MethHead")
            {
                float dx = 0;
                float dy = 0;

                XMLElement *dd;

                dd = onode->FirstChildElement("X");
                dd->QueryFloatText(&dx);
                dd = onode->FirstChildElement("Y");
                dd->QueryFloatText(&dy);

                addMethHead(sf::Vector2f(dx,dy));
            }
            onode = onode->NextSiblingElement();
        }


    }

    return true;
}
