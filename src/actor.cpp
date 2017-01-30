#include "actor.hpp"
#include "particle.hpp"
#include "jumpy.hpp"
#include "level.hpp"
#include "tile.hpp"

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
    m_shooting = false;
    m_shooting_timeout = 1500;
    m_shoot_time = 200;

    // bounding box
    // be sure to add a bounding box

    m_left_foot_emit = new ParticleEmitter(PEMIT_DUST1);
    m_right_foot_emit = new ParticleEmitter(PEMIT_DUST1);
}

Actor::~Actor()
{
    // delete emitters
    delete m_left_foot_emit;
    delete m_right_foot_emit;
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
    case MOVE_UP:
        if(onLadder())
        {
            m_acceleration.y -= 5.f;
            m_commanding_move = true;
            break;
        }

        m_commanding_move = false;
        m_acceleration.x = 0;
        break;
    case MOVE_DOWN:
        if(onLadder())
        {
            m_acceleration.y += 5.f;
            m_commanding_move = true;
            break;
        }

        m_commanding_move = false;
        m_acceleration.x = 0;
        break;
    default:
        m_commanding_move = false;
        m_acceleration.x = 0;
        break;
    }

}

bool Actor::onLadder()
{
    // get bounding box
    sf::FloatRect bb = getBoundingBox();
    // get bottom midpoint of bounding box (floor center), then convert to grid coordinate
    sf::Vector2i c_floor( int((bb.left + bb.width/2)/32), int(bb.top+bb.height)/32 );

    // get tile at grid position
    int btile =  m_jumpy->getCurrentLevel()->getTileBG( c_floor.x, c_floor.y );

    // debug
    //std::cout << "on ladder : tile at pos(" << c_floor.x << "," << c_floor.y << " = " << btile << std::endl;

    // check if tile at grid coordinate is a ladder
    std::vector<Tile*> *tiles = m_jumpy->getTilesBG();
    if(btile < 0 || btile >= int(tiles->size()) ) return false;

    if( (*tiles)[btile]->getType() == OBJ_TILE_LADDER) return true;
    else return false;
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

void Actor::getShot(bool rightside)
{
    int dirmod = 1;
    if( rightside) dirmod = -1;

    sf::FloatRect srect = getBoundingBox();
    sf::Vector2f bpos = sf::Vector2f( (srect.left + srect.width/2), (srect.top + srect.height/2) - (rand()%10+1)  );
    sf::Vector2f baccel(0,0.03);

    int pminlife = 100;
    int pmaxlife = 300;

    for(int i = 0; i < 20; i++)
    {
        ParticleEmitter p1(PEMIT_CUSTOM, bpos);
        if(rand()%100 < 80) p1.m_custom_texture_index = 0;
        else p1.m_custom_texture_index = 1;
        p1.m_custom_min_life = pminlife;
        p1.m_custom_max_life = pmaxlife;
        p1.m_custom_color = sf::Color(240,20,20,200);
        p1.m_custom_accel = baccel;
        int p1h = 1;
        if(rand()%2) p1h = -1;
        p1.createParticle(sf::Vector2f( rand()%3-1,rand()%3-1), sf::Vector2f( (1 + (rand()%4)*0.5)*dirmod,p1h*(rand()%10)*0.1 ) );
    }

}
