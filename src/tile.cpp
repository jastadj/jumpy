#include "tile.hpp"

#include "spritesheet.hpp"

Tile::Tile(SpriteSheet *tsheet, int spriteindex)
{
    // save sprite sheet and index
    m_tile_sprite_index = spriteindex;
    m_tile_sprite_sheet = tsheet;

    // create sprite for tile from sprite sheet/index
    addSprite( m_tile_sprite_sheet->createSprite(m_tile_sprite_index) );

    m_bounding_boxes.push_back( m_sprites[0]->getLocalBounds());
}

// copy constructor
Tile::Tile(const Tile &ttile)
{

    m_tile_sprite_sheet = ttile.m_tile_sprite_sheet;
    m_tile_sprite_index = ttile.m_tile_sprite_index;

    // clear sprite pointers
    m_sprites.clear();

    // create new sprite for tile copy
    addSprite( m_tile_sprite_sheet->createSprite(m_tile_sprite_index) );
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
