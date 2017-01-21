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
    sf::Image m_image;

    int m_scale;

    std::vector<sf::IntRect> m_clips;

public:
    SpriteSheet(std::string filename, int tiles_x = 1, int tile_y = 1);
    ~SpriteSheet();

    sf::Sprite *createSprite(int index);

    void setScale(int tscale);
};
#endif // CLASS_SPRITESHEET
