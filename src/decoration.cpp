#include "decoration.hpp"

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
