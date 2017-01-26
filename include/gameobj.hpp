#ifndef CLASS_GAMEOBJ
#define CLASS_GAMEOBJ


#include <SFML\Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>

#include "spritesheet.hpp"
#include "tools.hpp"

#include "animation.hpp"

// forward declarations
class Jumpy;

enum GAMEOBJTYPES{OBJ_PLAYER, OBJ_TILE, OBJ_METH, OBJ_ACTOR, OBJ_DECORATION};

enum MOVETYPE{MOVE_NONE, MOVE_RIGHT, MOVE_LEFT, MOVE_UP, MOVE_DOWN};



class GameObj
{

protected:
    Jumpy *m_jumpy;

    std::string m_name;

    // list of sprite frames
    std::vector<sf::Sprite*> m_sprites;

    // frame and animation
    sf::Clock m_anim_clock;
    std::vector<Animation> m_animations;
    int m_current_animation;
    int m_facing_right;

    // bounding box
    std::vector<sf::FloatRect> m_bounding_boxes;
    std::vector<GameObj*> m_collisions;

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

    std::string getName() { return m_name;}
    void setName(std::string nname) { m_name = nname;}

    // sprites
    void addSprite(sf::Sprite *tsprite);
    int spriteCount() { return int(m_sprites.size());}
    virtual void draw(sf::RenderTarget *tscreen);


    // animations
    int animationCount();
    int getCurrentSpriteIndex() { return m_animations[m_current_animation].getCurrentAnimationSpriteIndex();}
    Animation *getCurrentAnimation() { return &m_animations[m_current_animation];}
    bool setCurrentAnimationIndex(int tindex);
    int getCurrentAnimationIndex() { return m_current_animation;}
    void addAnimation(Animation tanim) { m_animations.push_back(tanim);}
    bool isFacingRight() { return m_facing_right;}

    sf::FloatRect getBoundingBox();

    // movement
    bool isCommandingMove() { return m_commanding_move;}
    void commandingMove(bool iscommanding) { m_commanding_move = iscommanding;}
    virtual void doMove(int movedir);

    // physics
    sf::Vector2f getPosition() { return m_position;}
    void setPosition(sf::Vector2f tpos) { m_position = tpos;}
    sf::Vector2f getVelocity() { return m_velocity;}
    void setVelocity( sf::Vector2f tvel) { m_velocity = tvel;}
    sf::Vector2f getAcceleration() { return m_acceleration;}
    void setAcceleration( sf::Vector2f taccel) { m_acceleration = taccel;}

    // collisions
    virtual void addCollision(GameObj *tobj)=0;
    void clearCollisions();
    int getCollisionCount() { return int(m_collisions.size());}

    virtual void update();
};
#endif // CLASS_GAMEOBJ
