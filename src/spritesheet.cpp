#include "spritesheet.hpp"

SpriteSheet::SpriteSheet(std::string filename, int tiles_x, int tiles_y)
{
    int tile_width;
    int tile_height;

    m_scale = 1;

    // load image
    m_image.loadFromFile(filename);

    // create texture from image
    m_texture.loadFromImage(m_image);

    // create flipped image
    m_image.flipHorizontally();
    m_texture_flipped.loadFromImage(m_image);

    // flip image back
    m_image.flipHorizontally();

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

        // add flipped clips
        for(int n = tiles_x-1; n >= 0; n--)
        {
            m_clips_flipped.push_back( m_clips[n] );
        }
    }
}

SpriteSheet::~SpriteSheet()
{

}

int SpriteSheet::getCount()
{
    return int(m_clips.size());
}

void SpriteSheet::setScale(int tscale)
{
    if(tscale < 0) return;

    m_scale = tscale;
}

sf::Sprite *SpriteSheet::createSprite(int index, bool flipped)
{
    if(index < 0 || index >= int(m_clips.size()) )
    {
        std::cout << "Error creating sprite from sprite sheet, index out of range!\n";
        return NULL;
    }

    // create sprite
    sf::Sprite *newsprite;
    if(flipped)
        newsprite = new sf::Sprite(m_texture_flipped, m_clips_flipped[index]);
    else newsprite = new sf::Sprite(m_texture, m_clips[index]);

    // scale sprite if scale is not 1
    if(m_scale != 1) newsprite->setScale( m_scale, m_scale);

    return newsprite;
}
