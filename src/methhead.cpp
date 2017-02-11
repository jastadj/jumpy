#include "methhead.hpp"
#include "jumpy.hpp"
#include "level.hpp"

MethHead::MethHead()
{
    SpriteSheet *ssheet = m_jumpy->getSpriteSheet(2);

    setName("Meth Head");

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

MethHead::~MethHead()
{

}

void MethHead::update()
{


    sf::FloatRect srect;
    // sprite count divided by two (left and right animations)
    static int spritecount = spriteCount()/2;

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

    /*
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
    */
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

}
