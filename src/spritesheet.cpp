#include "spritesheet.hpp"

SpriteSheet::SpriteSheet(std::string filename, int tiles_x, int tiles_y)
{
    int tile_width;
    int tile_height;

    // load texture
    m_texture.loadFromFile(filename);

    // calculate tile dimensions
    tile_width = m_texture.getSize().x / tiles_x;
    tile_height = m_texture.getSize().y / tiles_y;

    // create clips
    for(int i = 0; i < tiles_y; i++)
    {
        for(int n = 0; n < tiles_x; n++)
        {
            m_clips.push_back( sf::IntRect( n*tile_width, i*tile_height, tile_width, tile_height) );
        }
    }
}

SpriteSheet::~SpriteSheet()
{

}

sf::Sprite *SpriteSheet::createSprite(int index)
{
    if(index < 0 || index >= int(m_clips.size()) )
    {
        std::cout << "Error creating sprite from sprite sheet, index out of range!\n";
        return NULL;
    }

    return new sf::Sprite(m_texture, m_clips[index]);
}
