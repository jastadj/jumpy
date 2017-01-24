#include "player.hpp"

#include "jumpy.hpp"
#include "level.hpp"
#include "meth.hpp"
#include "particle.hpp"

Player::Player()
{
    SpriteSheet *ssheet = m_jumpy->getSpriteSheet(0);

    setName("Player");

    if(ssheet)
    {
        // add all sprites from sprite sheet
        for(int i = 0; i < ssheet->getCount(); i++)
            addSprite( ssheet->createSprite(i) );

        // add flipped versions
        for(int i = 0; i < ssheet->getCount(); i++)
            addSprite( ssheet->createSprite(i, true) );
    }

    // initial variables
    m_jumping = false;
    m_shooting = false;
    m_shooting_timeout = 1500;
    m_shoot_time = 200;
    m_max_meth = 1000;
    m_current_meth = 0;

    // bounding box
    m_bounding_boxes.push_back(sf::FloatRect(10 ,2, 12, 30) );

    // particle emitter
    m_particle_emitter = new ParticleEmitter(PEMIT_DEFAULT);
}

Player::~Player()
{

}

/*
void Player::doMove(int movedir)
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
*/

void Player::addMeth(int val)
{
    m_current_meth += val;

    std::cout << "Added " << val << " meth to player.\n";
}

/*
void Player::addCollision(GameObj *tobj)
{
    if(tobj == NULL)
    {
        std::cout << "Error adding collision to player, obj is null!\n";
        return;
    }

    //std::cout << "Colliding with object:" << tobj->getName() << std::endl;

    m_collisions.push_back(tobj);
}
*/

void Player::shoot()
{
    Level *currentlevel = m_jumpy->getCurrentLevel();

    if(m_shooting_clock.getElapsedTime().asMilliseconds() > m_shoot_time)
    {
        std::vector<GameObj*> hitobjs;

        m_shooting = true;
        m_shooting_clock.restart();

        m_jumpy->playSound(0);


        sf::Vector2f p1;
        sf::Vector2f p2;

        sf::FloatRect pbb = getBoundingBox();

        if(m_facing_right)
        {
            p1.x = pbb.left + pbb.width;
            p1.y = pbb.top + pbb.height/2;
            p2.x = p1.x + 400;
            p2.y = p1.y;
        }
        else
        {
            p1.x = pbb.left;
            p1.y = pbb.top + pbb.height/2;
            p2.x = p1.x - 400;
            p2.y = p1.y;
        }

        hitobjs = currentlevel->getObjectCollisionsWithLine(p1, p2);

        if(!hitobjs.empty())
        {
            std::cout << "Player shot " << hitobjs[0]->getName() << std::endl;
        }

    }


}

void Player::update()
{
    sf::FloatRect srect;
    // sprite count divided by two (left and right animations)
    static int spritecount = spriteCount()/2;

    // capture original position
    sf::Vector2f startpos = m_position;
    int prevframe = m_current_sprite;


    // apply gravity
    m_acceleration += sf::Vector2f(0, 0.03);

    // apply horizontal deceleration if move is not commanded
    if(!m_commanding_move)
    {
        if(m_velocity.x > 0) m_acceleration.x -= 1;
        else if(m_velocity.x < 0) m_acceleration.x += 1;
    }

    // apply acceleration to velocity
    m_velocity += m_acceleration;



    // limit vertical velocity to max
    if( m_velocity.y > m_velocity_max)
    {
        m_velocity.y = m_velocity_max;
        m_acceleration.y = 0;
    }
    else if(m_velocity.y < -m_velocity_max)
    {
        m_velocity.y = -m_velocity_max;
        m_acceleration.y = 0;
    }
    // limit horizontal velocity to max
    if( m_velocity.x > m_velocity_max)
    {
        m_velocity.x = m_velocity_max;
        m_acceleration.x = 0;
    }
    else if(m_velocity.x < -m_velocity_max)
    {
        m_velocity.x = -m_velocity_max;
        m_acceleration.x = 0;
    }

    // apply velocity to position
    m_position += m_velocity;

    //std::cout << "vert vel:" << m_velocity.y << std::endl;

    // get bounding box
    srect = m_bounding_boxes[0];
    // update bounding box position current working position
    srect.left += m_position.x;
    srect.top += m_position.y;

    // check if player is colliding with map
    Level *currentlevel = m_jumpy->getCurrentLevel();
    if( currentlevel->isCollidingWithMap(srect) )
    {
        sf::FloatRect vertrect = srect;
        sf::FloatRect horizrect = srect;

        vertrect.left = startpos.x + m_bounding_boxes[0].left;
        horizrect.top = startpos.y + m_bounding_boxes[0].top;

        // clean up coordinates?
        floor(srect.top);
        floor(srect.left);

        // if intersecting vertically
        if( currentlevel->isCollidingWithMap(vertrect))
        {
            while( currentlevel->isCollidingWithMap(vertrect))
            {
                // if intersecting below
                if( startpos.y + m_bounding_boxes[0].top < vertrect.top)
                {
                    vertrect.top--;

                    if(m_jumping) m_jumping = false;
                }
                // if intersecting above
                else if(startpos.y + m_bounding_boxes[0].top > vertrect.top)vertrect.top++;
            }

            srect.top = horizrect.top;
            m_velocity.y = 0;
            m_acceleration.y = 0;
        }

        // if intersecting horizontally
        if( currentlevel->isCollidingWithMap(horizrect))
        {
            while( currentlevel->isCollidingWithMap(horizrect))
            {
                // if intersecting left
                if( startpos.x + m_bounding_boxes[0].left > horizrect.left) horizrect.left++;
                // if intersecting right
                else if( startpos.x + m_bounding_boxes[0].left < horizrect.left) horizrect.left--;
            }

            srect.left = vertrect.left;
            m_velocity.x = 0;
            m_acceleration.x = 0;
        }

    }

    // adjust position to valid bounding box position
    m_position = sf::Vector2f(srect.left - m_bounding_boxes[0].left, srect.top - m_bounding_boxes[0].top);

    if( currentlevel->isCollidingWithMap(srect) )
    {
        std::cout <<"STILL COLLIDING WITH MAP!!\n";
    }


    // get object collisions
    currentlevel->getObjectCollisions(srect, this);

    // if there are collisions to handle
    for(int i = 0; i < int(m_collisions.size()); i++)
    {
        if(m_collisions[i]->getType() == OBJ_METH)
        {
            Meth *methobj;
            methobj = dynamic_cast<Meth*>(m_collisions[i]);

            addMeth( methobj->getMethValue());

            currentlevel->deleteObject( m_collisions[i]);
        }
    }
    clearCollisions();


    // update shooting timeout (holster weapon if haven't fired for some time
    if(m_shooting_clock.getElapsedTime().asMilliseconds() >= m_shooting_timeout)
        m_shooting = false;

    // update animation
    if(m_commanding_move && !m_jumping)
    {
        static int animtiming = 100;
        int animtime = m_anim_clock.getElapsedTime().asMilliseconds();
        int aframe = animtime / animtiming;

        if( aframe > 7)
        {
            aframe = 0;
            m_anim_clock.restart();
        }

        aframe++;

        // if gun is out
        if(m_shooting)
        {
            // if actively firing
            if(m_shooting_clock.getElapsedTime().asMilliseconds() < m_shoot_time)
            {
                if(!m_facing_right) setCurrentFrame(aframe + 18+ spritecount);
                else setCurrentFrame(aframe + 18);
            }
            else
            {
                if(!m_facing_right) setCurrentFrame(aframe + 10 + spritecount);
                else setCurrentFrame(aframe + 10);

            }

        }
        else
        {
            if( !m_facing_right) setCurrentFrame(aframe + spritecount);
            else setCurrentFrame(aframe);
        }

    }
    else if(m_jumping)
    {
        if(m_shooting)
        {
            // if actively firing
            if(m_shooting_clock.getElapsedTime().asMilliseconds() < m_shoot_time)
            {
                if(m_facing_right) setCurrentFrame(20);
                else setCurrentFrame(20+spritecount);
            }
            else
            {
                if(m_facing_right) setCurrentFrame(12);
                else setCurrentFrame(12+spritecount);
            }

        }
        else
        {
            if(m_facing_right) setCurrentFrame(2);
            else setCurrentFrame(2+spritecount);
        }

    }
    else
    {

        if(m_shooting)
        {
            if(m_shooting_clock.getElapsedTime().asMilliseconds() < m_shoot_time)
                m_current_sprite = 10;
            else m_current_sprite = 9;
        }
        else m_current_sprite = 0;

        if(!m_facing_right) m_current_sprite += spritecount;
    }

    //std::cout << "current frame = " << getCurrentFrame() << std::endl;

    // play footsteps
    if(
       (
        m_current_sprite == 3 ||
        m_current_sprite == 7 ||
        m_current_sprite == 13 ||
        m_current_sprite == 17 ||
        m_current_sprite == 21 ||
        m_current_sprite == 25 ||
        m_current_sprite == 3 + spritecount||
        m_current_sprite == 7 + spritecount||
        m_current_sprite == 13 + spritecount||
        m_current_sprite == 17 + spritecount||
        m_current_sprite == 21 + spritecount||
        m_current_sprite == 25 + spritecount
        )
       && m_current_sprite != prevframe)
        m_jumpy->playSound( rand()%8+1);

    // set current sprites to obj position
    m_sprites[m_current_sprite]->setPosition(m_position.x, m_position.y);

    // update meth timer
    if( m_meth_timer.getElapsedTime().asMilliseconds() > 500)
    {
        m_meth_timer.restart();

        m_current_meth -= 5;

        if(m_current_meth < 0) m_current_meth = 0;
    }

    // put particle emitter in center of bounding box
    m_particle_emitter->setPosition( sf::Vector2f( (srect.left + srect.width/2), (srect.top + srect.height/2) ) );

}
