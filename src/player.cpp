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
    m_max_meth = 1000;
    m_current_meth = 0;

    // init animations
    m_animations.clear();
    std::vector<Animation> *ta = m_jumpy->getAnimations();
    for(int i = 0; i < 18; i++)
    {
        m_animations.push_back( (*ta)[i]);
    }

    // bounding box
    m_bounding_boxes.push_back(sf::FloatRect(10 ,2, 12, 30) );

}

Player::~Player()
{

}


void Player::doMove(int movedir)
{
    if(m_jumpy->m_dbg_noclip)
    {
        switch(movedir)
        {
        case MOVE_RIGHT:
            m_position.x += 5;
            break;
        case MOVE_LEFT:
            m_position.x -= 5;
            break;
        case MOVE_UP:
            m_position.y -= 5;
            break;
        case MOVE_DOWN:
            m_position.y += 5;
            break;
        default:
            //m_commanding_move = false;
            //m_acceleration.x = 0;
            break;
        }
    }
    else Actor::doMove(movedir);

}


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

            if(hitobjs[0]->getType() == OBJ_ACTOR)
            {
                Actor *pa;
                pa = dynamic_cast<Actor*>(hitobjs[0]);

                if(pa)
                {
                    pa->getShot( isFacingRight());
                }
            }
        }

    }


}

void Player::update()
{
    if(m_jumpy->m_dbg_noclip)
    {
        // set current sprites to obj position
        m_sprites[ getCurrentSpriteIndex()]->setPosition(m_position.x, m_position.y);
        return;
    }

    sf::FloatRect srect;
    // sprite count divided by two (left and right animations)
    static int spritecount = spriteCount()/2;

    // capture original position
    sf::Vector2f startpos = m_position;
    int prevspriteindex = getCurrentSpriteIndex();

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
    bool active_shooting = false;
    if(m_shooting_clock.getElapsedTime().asMilliseconds() >= m_shooting_timeout)
        m_shooting = false;
    else if(m_shooting_clock.getElapsedTime().asMilliseconds() < m_shoot_time)
        active_shooting = true;

    // update animation
    static int flipped = animationCount()/2;

    if( m_jumping)
    {
        if(m_shooting)
        {
            if(active_shooting) m_current_animation = 8;
            else m_current_animation = 7;
        }
        else m_current_animation = 6;
    }
    else if( m_commanding_move)
    {
        if(m_shooting)
        {
            if(active_shooting) m_current_animation = 5;
            else m_current_animation = 4;
        }
        else m_current_animation = 1;
    }
    else
    {
        if(m_shooting)
        {
            if(active_shooting) m_current_animation = 3;
            else m_current_animation = 2;
        }
        else m_current_animation = 0;
    }

    if(!m_facing_right) m_current_animation = getCurrentAnimationIndex()+flipped;
    for(int i = 0; i < animationCount(); i++) m_animations[i].update();
    //m_animations[m_current_animation].update();




    // set current sprites to obj position
    m_sprites[ getCurrentSpriteIndex()]->setPosition(m_position.x, m_position.y);

    // position foot emitters
    m_left_foot_emit->setPosition( sf::Vector2f(srect.left+8, srect.top+srect.height) );
    m_right_foot_emit->setPosition( sf::Vector2f(srect.left+20, srect.top+srect.height) );

    // if sprite changed
    if(prevspriteindex != getCurrentSpriteIndex())
    {

        int currentframeindex = getCurrentAnimation()->getCurrentFrameIndex();

        if(m_commanding_move)
        {
            if( currentframeindex == 2 || currentframeindex == 6)
            {
                m_jumpy->playSound( rand()%8+1);
                if(randBool())m_right_foot_emit->once();
                else m_left_foot_emit->once();
            }

        }

    }

    // update meth timer
    if( m_meth_timer.getElapsedTime().asMilliseconds() > 500)
    {
        m_meth_timer.restart();

        m_current_meth -= 5;

        if(m_current_meth < 0) m_current_meth = 0;
    }

}


void Player::pee()
{
    int dirmod = 1;
    if( !isFacingRight()) dirmod = -1;

    if( rand()%100 + 1 > 80)
    {
        sf::FloatRect srect = getBoundingBox();
        ParticleEmitter p1(PEMIT_CUSTOM, sf::Vector2f( (srect.left + srect.width/2), (srect.top + srect.height/2)+5 ));
        p1.m_custom_min_life = 500;
        p1.m_custom_max_life = 300;
        p1.m_custom_color = sf::Color(236,191,44,255);
        p1.m_custom_accel = sf::Vector2f(0, 0.05);
        p1.createParticle(sf::Vector2f(0,0), sf::Vector2f(1*dirmod,-0.7));
    }

    if( rand()%100 + 1 > 80)
    {
        sf::FloatRect srect = getBoundingBox();
        ParticleEmitter p1(PEMIT_CUSTOM, sf::Vector2f( (srect.left + srect.width/2), (srect.top + srect.height/2)+5 ));
        //p1.m_custom_texture_index = 1;
        p1.m_custom_min_life = 500;
        p1.m_custom_max_life = 300;
        p1.m_custom_color = sf::Color(247,204,60,200);
        p1.m_custom_accel = sf::Vector2f(0, 0.05);
        p1.createParticle(sf::Vector2f(0,-1), sf::Vector2f(1*dirmod,-0.6));
    }

    if( rand()%100 + 1 > 90)
    {
        sf::FloatRect srect = getBoundingBox();
        ParticleEmitter p1(PEMIT_CUSTOM, sf::Vector2f( (srect.left + srect.width/2), (srect.top + srect.height/2)+5 ));
        //p1.m_custom_texture_index = 1;
        p1.m_custom_min_life = 500;
        p1.m_custom_max_life = 300;
        p1.m_custom_color = sf::Color(228,185,45,200);
        p1.m_custom_accel = sf::Vector2f(0, 0.05);
        p1.createParticle(sf::Vector2f(0,1), sf::Vector2f(1*dirmod,-0.8));
    }
}
