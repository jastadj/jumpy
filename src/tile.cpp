#include "tile.hpp"

#include "spritesheet.hpp"

Tile::Tile(sf::Sprite *tsprite)
{
    // create sprite for tile from sprite sheet/index
    m_sprites.push_back(tsprite);

    m_bounding_boxes.push_back( m_sprites[0]->getLocalBounds());

    // animation
    m_anim_clock = NULL;
    m_anim_time = 300;
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

    m_bounding_boxes.push_back( m_sprites[0]->getLocalBounds());

    // animation
    m_anim_clock = NULL;
    m_anim_time = ttile.m_anim_time;
    m_anim_sequence = ttile.m_anim_sequence;
    if(spriteCount() > 1) m_anim_clock = new sf::Clock;
}

Tile::~Tile()
{

}

void Tile::clearAnimationSequence()
{
    m_anim_sequence.clear();
    if(m_anim_clock) delete m_anim_clock;
    m_current_sprite = 0;
}

void Tile::addAnimationIndex(int nspriteindex)
{
    // check for sprite index validity
    if(nspriteindex < 0 || nspriteindex >= spriteCount() )
    {
        std::cout << "Error adding animation index to tile, sprite index out of bounds!\n";
        return;
    }

    // if tile was never animated, create animated clock
    if(!m_anim_clock) m_anim_clock = new sf::Clock;

    // add sprite index to animation sequence
    m_anim_sequence.push_back(nspriteindex);
}

void Tile::draw(sf::RenderTarget *tscreen)
{
    if(isAnimated())
    {
        tscreen->draw( *m_sprites[ m_anim_sequence[m_current_sprite] ] );
    }
    else tscreen->draw( *m_sprites[m_current_sprite]);
}

void Tile::update()
{
    if(isAnimated())
    {
        if(m_anim_clock->getElapsedTime().asMilliseconds() > m_anim_time)
        {
            m_current_sprite++;

            if(m_current_sprite >= int(m_anim_sequence.size()) ) m_current_sprite = 0;

            m_anim_clock->restart();
        }

        m_sprites[ m_anim_sequence[m_current_sprite] ]->setPosition(m_position);
    }
    else m_sprites[m_current_sprite]->setPosition(m_position);
}

/*
void Tile::draw(sf::RenderTarget *tscreen)
{

}

*/
