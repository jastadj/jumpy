#include "player.hpp"

#include "jumpy.hpp"
#include "level.hpp"

Player::Player()
{
    SpriteSheet *ssheet = m_jumpy->getSpriteSheet(0);

    if(ssheet)
    {
        // add all sprites from sprite sheet
        for(int i = 0; i < ssheet->getCount(); i++)
            addSprite( ssheet->createSprite(i) );

        // add flipped versions
        for(int i = 0; i < ssheet->getCount(); i++)
            addSprite( ssheet->createSprite(i, true) );
    }

    m_jumping = false;
}

Player::~Player()
{

}

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

void Player::update()
{
    // capture original position
    sf::Vector2f startpos = m_position;

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

    // get bounding box of sprite
    sf::FloatRect srect = m_sprites[m_current_sprite]->getGlobalBounds();
    // update bounding box position current working position
    srect.left = m_position.x;
    srect.top = m_position.y;

    // check if player is colliding with map
    Level *currentlevel = m_jumpy->getCurrentLevel();
    if( currentlevel->isColliding(srect) )
    {
        sf::FloatRect vertrect = srect;
        sf::FloatRect horizrect = srect;

        vertrect.left = startpos.x;
        horizrect.top = startpos.y;

        // clean up coordinates?
        floor(srect.top);
        floor(srect.left);

        // if intersecting vertically
        if( currentlevel->isColliding(vertrect))
        {
            while( currentlevel->isColliding(vertrect))
            {
                // if intersecting below
                if( startpos.y < vertrect.top)
                {
                    vertrect.top--;

                    if(m_jumping) m_jumping = false;
                }
                // if intersecting above
                else if(startpos.y > vertrect.top)vertrect.top++;
            }

            srect.top = horizrect.top;
            m_velocity.y = 0;
            m_acceleration.y = 0;
        }

        // if intersecting horizontally
        if( currentlevel->isColliding(horizrect))
        {
            while( currentlevel->isColliding(horizrect))
            {
                // if intersecting left
                if( startpos.x > horizrect.left) horizrect.left++;
                // if intersecting right
                else if( startpos.x < horizrect.left) horizrect.left--;
            }

            srect.left = vertrect.left;
            m_velocity.x = 0;
            m_acceleration.x = 0;
        }

    }

    // adjust position to valid bounding box position
    m_position = sf::Vector2f(srect.left, srect.top);

    if( currentlevel->isColliding(srect) )
    {
        std::cout <<"STILL COLLIDING\n";
    }



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

        if( !m_facing_right) setCurrentFrame(aframe + 8);
        else setCurrentFrame(aframe);
    }
    else if(m_jumping)
    {
        if(m_facing_right) setCurrentFrame(2);
        else setCurrentFrame(2+9);
    }
    else
    {
        if(m_facing_right) m_current_sprite = 0;
        else m_current_sprite = 9;
    }



    //std::cout << "current frame = " << getCurrentFrame() << std::endl;

    // set current sprites to obj position
    m_sprites[m_current_sprite]->setPosition(m_position);

}
