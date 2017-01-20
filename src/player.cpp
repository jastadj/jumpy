#include "player.hpp"

#include "jumpy.hpp"

Player::Player()
{
    addSprite( m_jumpy->getSpriteSheet(0)->createSprite(0) );
}

Player::~Player()
{

}
