#include "meth.hpp"

#include "jumpy.hpp"

Meth::Meth(int methval)
{
    SpriteSheet *ssheet = m_jumpy->getSpriteSheet(3);
    addSprite( ssheet->createSprite(0) );

    m_bounding_boxes.push_back( sf::FloatRect(14,14,4,4));

    m_value = methval;

    setName("meth");
}

Meth::~Meth()
{

}

XMLNode *Meth::saveToNode(XMLDocument *tdoc)
{

    if(!tdoc) return NULL;

    XMLNode *anode = tdoc->NewElement("Meth");

    XMLElement *element = tdoc->NewElement("Value");
    element->SetText(m_value);
    anode->InsertEndChild(element);

    element = tdoc->NewElement("X");
    element->SetText(m_position.x);
    anode->InsertEndChild(element);

    element = tdoc->NewElement("Y");
    element->SetText( m_position.y);
    anode->InsertEndChild(element);

    return anode;
}

void Meth::update()
{
    m_sprites[ getCurrentSpriteIndex()]->setPosition(m_position);
}
