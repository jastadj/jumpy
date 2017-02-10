#ifndef CLASS_SPRITESHEET
#define CLASS_SPRITESHEET

#include <SFML\Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <tinyxml2.h>

#define SPRITE_SHEET_PATH ".\\Data\\Art\\"

class SpriteSheet
{
private:

    std::string m_filename;
    bool m_initialized;

    sf::Texture m_texture;
    sf::Texture m_texture_flipped;
    sf::Image m_image;

    int m_scale;

    std::vector<sf::IntRect> m_clips;
    std::vector<sf::IntRect> m_clips_flipped;

public:
    SpriteSheet(std::string filename, int tiles_x = 1, int tiles_y = 1);
    SpriteSheet(tinyxml2::XMLNode *tnode);
    ~SpriteSheet();

    bool initSpriteSheet(std::string filename, int tiles_x, int tiles_y);

    sf::Sprite *createSprite(int index, bool flipped = false);

    void setScale(int tscale);

    int getCount();
    std::string getFilename() { return m_filename;}
    bool initialized() { return m_initialized;}
};
#endif // CLASS_SPRITESHEET
