#include "tile.hpp"

#include "spritesheet.hpp"
#include "jumpy.hpp"
#include "tinyxml2.h"

// init static bool
static bool m_tiles_initialized = false;

bool initTiles()
{

    // get jumpy reference
    Jumpy *m_jumpy;
    m_jumpy = Jumpy::getInstance();

    if(m_tiles_initialized)
    {
        std::cout << "Tiles already initialized!\n";
        return m_tiles_initialized;
    }


    tinyxml2::XMLDocument tdoc;
    tinyxml2::XMLNode *root;
    std::vector<tinyxml2::XMLNode*> tnode;
    tinyxml2::XMLNode *anode;
    tinyxml2::XMLElement *element;

    std::string tilefile;

    tilefile = std::string(std::string(TILES_PATH) + "tiles.xml");

    if(tdoc.LoadFile( tilefile.c_str()))
    {
        std::cout << "Error loading tile xml file:" << tilefile << std::endl;
        return false;
    }


    root = tdoc.FirstChildElement("Methd_Up");
    if(!root)
    {
        std::cout << "Not a valid Methd Up file!\n";
        return false;
    }

    tnode.push_back(root->FirstChildElement("Tiles"));
    tnode.push_back(root->FirstChildElement("BackgroundTiles"));
    tnode.push_back(root->FirstChildElement("ForegroundTiles"));

    for(int i = 0; i < int(tnode.size()); i++)
    {
        std::vector<SpriteSheet*> tsheets;

        std::vector<Tile*> *ttiles = NULL;

        // point to tile list we are working with
        switch(i)
        {
        case 0:
            ttiles = m_jumpy->getTiles();
            break;
        case 1:
            ttiles = m_jumpy->getTilesBG();
            break;
        case 2:
            ttiles = m_jumpy->getTilesFG();
            break;
        default:
            // wut?
            break;
        }

        // not sure what tile list we are working with!!
        if(!ttiles)
        {
            std::cout << "Undefined tile list type during init tiles!\n";
            return false;
        }

        if(!tnode[i])
        {
            std::cout << "Missing expected tile type in tile xml:" << tilefile << std::endl;
            return false;
        }

        // add all sprite sheets for tiles
        anode = tnode[i]->FirstChildElement("AddSpriteSheet");

        while(anode)
        {
            // create sprite sheet from xml node
            SpriteSheet *newsheet = new SpriteSheet(anode);

            // if sprite sheet failed to init
            if(!newsheet->initialized())
            {
                std::cout << "Error initializing a tile sprite sheet!\n";
                return false;
            }

            // add sprite sheet to jumpy
            m_jumpy->addSpriteSheet(newsheet);

            // add sprite sheet to temp list for creating sprites
            tsheets.push_back(newsheet);

            // add next sprite sheet
            anode = anode->NextSiblingElement("AddSpriteSheet");

        }

        // generate sprites from tile sprite sheets
        // go through each sprite sheet
        for(int n = 0; n < int(tsheets.size()); n++)
        {
            // add each tile in sprite sheet to target tile list
            for(int k = 0; k < tsheets[n]->getCount(); k++)
            {
                ttiles->push_back( new Tile( tsheets[n]->createSprite(k) ) );
            }

            std::cout << "Created " << tsheets[n]->getCount() << " tiles from sprite sheet " << tsheets[n]->getFilename() << std::endl;

        }

    }

    m_tiles_initialized = true;
    return m_tiles_initialized;
}

Tile::Tile(sf::Sprite *tsprite)
{
    // create sprite for tile from sprite sheet/index
    m_sprites.push_back(tsprite);

    m_bounding_boxes.push_back( m_sprites[0]->getLocalBounds());

    m_animated = false;
}

// copy constructor
Tile::Tile(const Tile &ttile)
{

    // clear sprite pointers
    m_sprites.clear();

    // create sprite copies
    for(int i = 0; i < int(ttile.m_sprites.size()); i++)
    {
        sf::Sprite *newsprite = new sf::Sprite;
        *newsprite = *ttile.m_sprites[i];
        m_sprites.push_back(newsprite);
    }

    // copy animations
    m_animated = ttile.m_animated;
    m_animations = ttile.m_animations;

    m_bounding_boxes.push_back( m_sprites[0]->getLocalBounds());

}

Tile::~Tile()
{

}


void Tile::draw(sf::RenderTarget *tscreen)
{
    tscreen->draw( *m_sprites[getCurrentSpriteIndex()]);
}

void Tile::update()
{
    if(m_animated)getCurrentAnimation()->update();

    m_sprites[getCurrentSpriteIndex()]->setPosition(m_position);
}

/*
void Tile::draw(sf::RenderTarget *tscreen)
{

}

*/

//////////////////////////////////////////////////////////////
//

Ladder::Ladder(sf::Sprite *tsprite) : Tile(tsprite)
{

}

Ladder::Ladder(const Ladder& ttile) : Tile(Tile(ttile))
{
    /*
    // clear sprite pointers
    m_sprites.clear();

    // create sprite copies
    for(int i = 0; i < int(ttile.m_sprites.size()); i++)
    {
        sf::Sprite *newsprite = new sf::Sprite;
        *newsprite = *ttile.m_sprites[i];
        m_sprites.push_back(newsprite);
    }

    // copy animations
    m_animated = ttile.m_animated;
    m_animations = ttile.m_animations;

    m_bounding_boxes.push_back( m_sprites[0]->getLocalBounds());
    */
}

Ladder::~Ladder()
{

}
