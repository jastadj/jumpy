#ifndef CLASS_SPRITESHEET
#define CLASS_SPRITESHEET

#include <SFML\Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

class SpriteSheet
{
private:

    sf::Texture m_texture;
    sf::Texture m_texture_flipped;
    sf::Image m_image;

    int m_scale;

    std::vector<sf::IntRect> m_clips;
    std::vector<sf::IntRect> m_clips_flipped;

public:
    SpriteSheet(std::string filename, int tiles_x = 1, int tile_y = 1);
    ~SpriteSheet();

    sf::Sprite *createSprite(int index, bool flipped = false);

    void setScale(int tscale);

    int getCount();
};
#endif // CLASS_SPRITESHEET
