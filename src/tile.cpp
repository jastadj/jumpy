#include "tile.hpp"

#include "spritesheet.hpp"

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
