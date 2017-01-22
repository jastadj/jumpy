#include "actor.hpp"

Actor::Actor()
{
    //SpriteSheet *ssheet = m_jumpy->getSpriteSheet(0);

    setName("unnamed actor");

    /* be sure to add a sprite sheet
    if(ssheet)
    {
        // add all sprites from sprite sheet
        for(int i = 0; i < ssheet->getCount(); i++)
            addSprite( ssheet->createSprite(i) );

        // add flipped versions
        for(int i = 0; i < ssheet->getCount(); i++)
            addSprite( ssheet->createSprite(i, true) );
    }
    */

    // initial variables
    m_jumping = false;

    // bounding box
    // be sure to add a bounding box
}

Actor::~Actor()
{

}


void Actor::doMove(int movedir)
{

    switch(movedir)
    {
    case MOVE_RIGHT:
        m_acceleration.x += 5.f;
        m_commanding_move = true;
        m_facing_right = true;
        break;
    case MOVE_LEFT:
        m_acceleration.x -= 5.f;
        m_commanding_move = true;
        m_facing_right = false;
        break;
    default:
        m_commanding_move = false;
        m_acceleration.x = 0;
        break;
    }

}

void Actor::addCollision(GameObj *tobj)
{
    if(tobj == NULL)
    {
        std::cout << "Error adding collision to player, obj is null!\n";
        return;
    }

    //std::cout << "Colliding with object:" << tobj->getName() << std::endl;

    m_collisions.push_back(tobj);
}
