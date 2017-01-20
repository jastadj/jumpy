#include "player.hpp"

#include "jumpy.hpp"
#include "level.hpp"

Player::Player()
{
    addSprite( m_jumpy->getSpriteSheet(0)->createSprite(0) );

    m_jumping = false;
}

Player::~Player()
{

}

void Player::update()
{
    // capture original position
    sf::Vector2f startpos = m_position;

    // apply falling acceleration
    m_acceleration += sf::Vector2f(0, 0.03);

    // limit vertical acceleration
    if( m_acceleration.y > 3 ) m_acceleration.y = 3;

    // apply acceleration to velocity
    m_velocity += m_acceleration;

    // limit vertical velocity
    if( m_velocity.y > 3) m_velocity.y = 3;

    // apply velocity to position
    m_position += m_velocity;

    //std::cout << "vert vel:" << m_velocity.y << std::endl;

    // check if player is colliding with map
    sf::FloatRect srect = m_sprites[m_current_sprite]->getGlobalBounds();
    Level *currentlevel = m_jumpy->getCurrentLevel();
    for( int i = floor(srect.top)/32; i <= ceil(srect.top + srect.height)/32; i++)
    {
        for(int n = floor(srect.left)/32; n <= ceil(srect.left + srect.width)/32; n++)
        {
            if( currentlevel->getTile(n,i) == 1)
            {
                m_position.y = startpos.y;
            }
        }
    }


    // set current sprites to obj position
    m_sprites[m_current_sprite]->setPosition(m_position);
}
