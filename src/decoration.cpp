#include "decoration.hpp"

#include "jumpy.hpp"
#include "tinyxml2.h"

// init static bool
static bool m_decorations_initialized = false;

bool initDecorations()
{

    // get jumpy reference
    Jumpy *m_jumpy;
    m_jumpy = Jumpy::getInstance();

    // xml elements
    tinyxml2::XMLDocument tdoc;
    tinyxml2::XMLNode *root;
    tinyxml2::XMLNode *tnode;
    tinyxml2::XMLNode *anode;
    tinyxml2::XMLElement *element;

    // target file
    std::string tfile;

    // sprite sheet and object lists
    std::vector<SpriteSheet*> tsheets;
    std::vector<Decoration*> *tlist = m_jumpy->getDecorations();

    // if already initialized, return
    if(m_decorations_initialized)
    {
        std::cout << "Decorations already initialized!\n";
        return m_decorations_initialized;
    }


    tfile = std::string(std::string(DECORATIONS_PATH) + "decorations.xml");

    if(tdoc.LoadFile( tfile.c_str()))
    {
        std::cout << "Error loading decoration xml file:" << tfile << std::endl;
        return false;
    }

    root = tdoc.FirstChildElement("Methd_Up");

    if(!root)
    {
        std::cout << "Not a valid Methd Up file!\n";
        return false;
    }

    tnode = root->FirstChildElement("Decorations");




    // add all sprite sheets for tiles
    anode = tnode->FirstChildElement("AddSpriteSheet");
    while(anode)
    {
        // create sprite sheet from xml node
        SpriteSheet *newsheet = new SpriteSheet(anode);

        // if sprite sheet failed to init
        if(!newsheet->initialized())
        {
            std::cout << "Error initializing a decoration sprite sheet!\n";
            return false;
        }

        // add sprite sheet to jumpy
        m_jumpy->addSpriteSheet(newsheet);

        // add sprite sheet to temp list for creating sprites
        tsheets.push_back(newsheet);

        // add next sprite sheet
        anode = anode->NextSiblingElement("AddSpriteSheet");

    }

    // create and add decorations from file
    anode = tnode->FirstChildElement("Decoration");

    while(anode)
    {
        SpriteSheet *tss = NULL;
        int tindex = 0;
        std::string ssfilename;

        // get target sprite sheet
        element = anode->FirstChildElement("SpriteSheetSource");
        if(!element)
        {
            std::cout << "No sprite sheet source provided for decoration in file:" << tfile << std::endl;
            return false;
        }
        ssfilename = std::string( element->GetText());

        // get sprite sheet index
        element = anode->FirstChildElement("SpriteSheetIndex");
        if(element)
        {
            element->QueryIntText(&tindex);
        }

        // find target sprite sheet
        for(int i = 0; i < int(tsheets.size()); i++)
        {
            if( tsheets[i]->getFilename() == ssfilename)
            {
                Decoration *newdec = new Decoration(tsheets[i]->createSprite(tindex));
                tlist->push_back(newdec);
                break;
            }
        }

        anode = anode->NextSiblingElement("Decoration");
    }


    m_decorations_initialized = true;
    return m_decorations_initialized;
}

Decoration::Decoration(sf::Sprite *tsprite)
{
    // create sprite for tile from sprite sheet/index
    m_sprites.push_back(tsprite);

    m_bounding_boxes.push_back( m_sprites[0]->getLocalBounds());

    m_animated = false;
}

// copy constructor
Decoration::Decoration(const Decoration &tdec)
{
    // clear sprite pointers
    m_sprites.clear();

    // create sprite copies
    for(int i = 0; i < int(tdec.m_sprites.size()); i++)
    {
        sf::Sprite *newsprite = new sf::Sprite;
        *newsprite = *tdec.m_sprites[i];
        m_sprites.push_back(newsprite);
    }

    // copy animations
    m_animated = tdec.m_animated;
    m_animations = tdec.m_animations;

    m_bounding_boxes.push_back( m_sprites[0]->getLocalBounds());

}

Decoration::~Decoration()
{

}

XMLNode *Decoration::saveToNode(XMLDocument *tdoc)
{
    if(!tdoc) return NULL;

    XMLNode *anode = tdoc->NewElement("Decoration");

    XMLElement *element = tdoc->NewElement("ID");
    element->SetText(m_id);
    anode->InsertEndChild(element);

    element = tdoc->NewElement("X");
    element->SetText(m_position.x);
    anode->InsertEndChild(element);

    element = tdoc->NewElement("Y");
    element->SetText( m_position.y);
    anode->InsertEndChild(element);

    return anode;
}

void Decoration::draw(sf::RenderTarget *tscreen)
{
    tscreen->draw( *m_sprites[getCurrentSpriteIndex()]);
}

void Decoration::update()
{
    if(m_animated)getCurrentAnimation()->update();

    m_sprites[getCurrentSpriteIndex()]->setPosition(m_position);
}
