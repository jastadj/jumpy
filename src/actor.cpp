#include "actor.hpp"
#include "particle.hpp"

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

void Actor::getShot(bool rightside)
{
    int dirmod = 1;
    if( rightside) dirmod = -1;

    sf::FloatRect srect = getBoundingBox();
    sf::Vector2f bpos = sf::Vector2f( (srect.left + srect.width/2), (srect.top + srect.height/2) - (rand()%10+1)  );
    sf::Vector2f baccel(0,0.05);

    for(int i = 0; i < 5; i++)
    {


        ParticleEmitter p1(PEMIT_CUSTOM, bpos);
        p1.m_custom_min_life = 500;
        p1.m_custom_max_life = 300;
        p1.m_custom_color = sf::Color(240,20,20,200);
        p1.m_custom_accel = baccel;
        int p1h = 1;
        if(rand()%2) p1h = -1;
        p1.createParticle(sf::Vector2f(0,0), sf::Vector2f( (1 + (rand()%4)*0.5)*dirmod,p1h*(rand()%25)*0.1 ) );


        ParticleEmitter p2(PEMIT_CUSTOM, bpos);
        //p1.m_custom_texture_index = 1;
        p2.m_custom_min_life = 500;
        p2.m_custom_max_life = 300;
        p2.m_custom_color = sf::Color(178,19,19,200);
        p2.m_custom_accel = baccel;
        int p2h = 1;
        if(rand()%2) p2h = -1;
        p2.createParticle(sf::Vector2f(0,-1), sf::Vector2f((1 + (rand()%4)*0.5)*dirmod,p2h*(rand()%70)*0.1));


        ParticleEmitter p3(PEMIT_CUSTOM, bpos);
        p3.m_custom_texture_index = 1;
        p3.m_custom_min_life = 500;
        p3.m_custom_max_life = 300;
        p3.m_custom_color = sf::Color(205,7,7,200);
        p3.m_custom_accel = baccel;
        int p3h = 1;
        if(rand()%2) p3h = -1;
        p3.createParticle(sf::Vector2f(0,1), sf::Vector2f((1 + (rand()%4)*0.5)*dirmod,p3h*(rand()%25)*0.1));
    }


}