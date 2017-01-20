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

    std::vector<sf::IntRect> m_clips;

public:
    SpriteSheet(std::string filename, int tiles_x = 1, int tile_y = 1);
    ~SpriteSheet();

    sf::Sprite *createSprite(int index);
};
#endif // CLASS_SPRITESHEET
