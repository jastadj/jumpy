#include "gameobj.hpp"

#include "jumpy.hpp"

GameObj::GameObj()
{
    // get main callback reference
    m_jumpy = Jumpy::getInstance();

    setName("unnamed");

    m_current_sprite = 0;

    m_commanding_move = false;
    m_facing_right = true;


    // default terminal velocity
    m_velocity_max = 5.f;

    // dont forget to add sprites from a sprite sheet
    // dont forget to add a bounding box for update collision
}

GameObj::~GameObj()
{
    // delete sprites
    for(int i = 0; i < int(m_sprites.size()); i++)
    {
        delete m_sprites[i];
    }
    m_sprites.clear();

    // clear collision list
    clearCollisions();
}

void GameObj::addSprite(sf::Sprite *tsprite)
{
    if(tsprite == NULL)
    {
        std::cout << "Error adding sprite to gameobj, sprite is null!\n";
        return;
    }

    m_sprites.push_back(tsprite);
}

void GameObj::draw(sf::RenderTarget *tscreen)
{
    tscreen->draw( *m_sprites[m_current_sprite] );

    // if debug on to draw bounding boxes
    if(m_jumpy->m_dbg_showboundingboxes)
    {
        drawFloatRect(getBoundingBox(), tscreen);
    }
}

void GameObj::doMove(int movedir)
{

}

void GameObj::setCurrentFrame(int tframe)
{
    if(tframe >= int(m_sprites.size()) ) tframe = 0;
    else if(tframe < 0) tframe = int(m_sprites.size())-1;

    m_current_sprite = tframe;
}


sf::FloatRect GameObj::getBoundingBox()
{
    // currently only getting index 0, may add more to support each frame
    return sf::FloatRect(m_position.x + m_bounding_boxes[0].left, m_position.y + m_bounding_boxes[0].top,
                         m_bounding_boxes[0].width, m_bounding_boxes[0].height);
}

void GameObj::clearCollisions()
{
    m_collisions.clear();
}

void GameObj::update()
{
    // apply acceleration to velocity
    m_velocity += m_acceleration;

    // apply velocity to position
    m_position += m_velocity;

    // set current sprites to obj position
    m_sprites[m_current_sprite]->setPosition(m_position);
}
