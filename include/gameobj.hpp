#ifndef CLASS_GAMEOBJ
#define CLASS_GAMEOBJ


#include <SFML\Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "spritesheet.hpp"

// forward declarations
class Jumpy;

enum GAMEOBJTYPES{OBJ_PLAYER};

class GameObj
{

protected:
    Jumpy *m_jumpy;

    std::vector<sf::Sprite*> m_sprites;

    int m_current_sprite = 0;

public:
    GameObj();
    virtual ~GameObj();
    virtual int getType()=0;

    void addSprite(sf::Sprite *tsprite);
    virtual void draw(sf::RenderWindow *tscreen);

};
#endif // CLASS_GAMEOBJ
