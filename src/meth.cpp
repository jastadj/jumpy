#include "meth.hpp"

#include "jumpy.hpp"

Meth::Meth(int nmethtype)
{
    SpriteSheet *ssheet = m_jumpy->getSpriteSheet(1);
    addSprite( ssheet->createSprite(0) );

    m_bounding_boxes.push_back( sf::FloatRect(14,14,4,4));

    switch(nmethtype)
    {
    case OBJ_METH_SMALL:
        m_methtype = OBJ_METH_SMALL;
        break;
    case OBJ_METH_MEDIUM:
        m_methtype = OBJ_METH_MEDIUM;
        break;
    case OBJ_METH_LARGE:
        m_methtype = OBJ_METH_LARGE;
        break;
    default:
        m_methtype = OBJ_METH_SMALL;
        std::cout << "Error creating meth, methtype " << nmethtype << " is unknown.  Defaulting to small.\n";
        break;
    }

    setName("meth");
}

Meth::~Meth()
{

}

int Meth::getMethValue()
{
    switch(m_methtype)
    {

    case OBJ_METH_MEDIUM:
        return 500;
        break;
    case OBJ_METH_LARGE:
        return 250;
        break;
    case OBJ_METH_SMALL:
    default:
        break;
    }

    // default or small meth type
    return 100;
}

void Meth::update()
{
    m_sprites[ getCurrentSpriteIndex()]->setPosition(m_position);
}
