#include "tile.hpp"

#include "spritesheet.hpp"

Tile::Tile(sf::Sprite *tsprite)
{
    // create sprite for tile from sprite sheet/index
    m_sprites.push_back(tsprite);

    m_bounding_boxes.push_back( m_sprites[0]->getLocalBounds());
}

// copy constructor
Tile::Tile(const Tile &ttile)
{

    // clear sprite pointers
    m_sprites.clear();

    for(int i = 0; i < int(ttile.m_sprites.size()); i++)
    {
        sf::Sprite *newsprite = new sf::Sprite;
        *newsprite = *ttile.m_sprites[i];
        m_sprites.push_back(newsprite);
    }
}

Tile::~Tile()
{

}

void Tile::draw(sf::RenderTarget *tscreen)
{
    tscreen->draw( *m_sprites[m_current_sprite]);
}

void Tile::update()
{
    m_sprites[m_current_sprite]->setPosition(m_position);
}

/*
void Tile::draw(sf::RenderTarget *tscreen)
{

}

*/
