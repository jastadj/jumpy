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

enum MOVETYPE{MOVE_NONE, MOVE_RIGHT, MOVE_LEFT};

class GameObj
{

protected:
    Jumpy *m_jumpy;

    // list of sprite frames
    std::vector<sf::Sprite*> m_sprites;

    // frame and animation
    sf::Clock m_anim_clock;
    int m_current_sprite;
    int m_facing_right;

    // physics
    bool m_commanding_move;
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    float m_velocity_max;
    sf::Vector2f m_acceleration;

public:
    GameObj();
    virtual ~GameObj();
    virtual int getType()=0;

    void addSprite(sf::Sprite *tsprite);
    virtual void draw(sf::RenderWindow *tscreen);
    virtual void update();

    void setCurrentFrame(int tframe);
    int getCurrentFrame() { return m_current_sprite;}

    bool isCommandingMove() { return m_commanding_move;}
    void commandingMove(bool iscommanding) { m_commanding_move = iscommanding;}
    virtual void doMove(int movedir);

    sf::Vector2f getPosition() { return m_position;}
    void setPosition(sf::Vector2f tpos) { m_position = tpos;}

    sf::Vector2f getVelocity() { return m_velocity;}
    void setVelocity( sf::Vector2f tvel) { m_velocity = tvel;}

    sf::Vector2f getAcceleration() { return m_acceleration;}
    void setAcceleration( sf::Vector2f taccel) { m_acceleration = taccel;}
};
#endif // CLASS_GAMEOBJ
