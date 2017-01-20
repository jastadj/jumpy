#include "tile.hpp"

#include "spritesheet.hpp"

Tile::Tile(SpriteSheet *tsheet, int spriteindex)
{
    addSprite( tsheet->createSprite(spriteindex) );
}

Tile::~Tile()
{

}

void Tile::draw(sf::RenderWindow *tscreen, int x, int y)
{
    m_sprites[m_current_sprite]->setPosition( sf::Vector2f(x,y) );
    tscreen->draw( *m_sprites[m_current_sprite]);
}
