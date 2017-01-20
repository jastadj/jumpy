#ifndef CLASS_GAMEOBJ
#define CLASS_GAMEOBJ


#include <SFML\Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "spritesheet.hpp"

#include "tools.hpp"

// forward declarations
class Jumpy;

enum GAMEOBJTYPES{OBJ_PLAYER, OBJ_TILE};

class GameObj
{

protected:
    Jumpy *m_jumpy;

    std::vector<sf::Sprite*> m_sprites;

    int m_current_sprite;

    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Vector2f m_acceleration;

public:
    GameObj();
    virtual ~GameObj();
    virtual int getType()=0;

    void addSprite(sf::Sprite *tsprite);
    virtual void draw(sf::RenderWindow *tscreen);
    virtual void update();

    sf::Vector2f getPosition() { return m_position;}
    void setPosition(sf::Vector2f tpos) { m_position = tpos;}

    sf::Vector2f getVelocity() { return m_velocity;}
    void setVelocity( sf::Vector2f tvel) { m_velocity = tvel;}

    sf::Vector2f getAcceleration() { return m_acceleration;}
    void setAcceleration( sf::Vector2f taccel) { m_acceleration = taccel;}
};
#endif // CLASS_GAMEOBJ
