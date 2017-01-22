#include "meth.hpp"

#include "jumpy.hpp"

Meth::Meth(int methval)
{
    SpriteSheet *ssheet = m_jumpy->getSpriteSheet(3);
    addSprite( ssheet->createSprite(0) );

    m_bounding_boxes.push_back(m_sprites[0]->getLocalBounds());

    m_value = methval;

    setName("meth");
}

Meth::~Meth()
{

}


void Meth::update()
{
    m_sprites[m_current_sprite]->setPosition(m_position);
}
