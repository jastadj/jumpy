#include "gameobj.hpp"

#include "jumpy.hpp"

GameObj::GameObj()
{
    // get main callback reference
    m_jumpy = Jumpy::getInstance();

    // set default name
    setName("unnamed");

    // set default id (used for save / load), -1 by default
    // level class will set ID when using an add method
    m_id = -1;

    // init animation
    m_animations.push_back(Animation());
    m_current_animation = 0;


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

////////////////////////////////////////////////
//  ANIMATIONS

int GameObj::animationCount()
{
    return int(m_animations.size());
}

bool GameObj::setCurrentAnimationIndex(int tindex)
{
    if(tindex < 0 || tindex >= animationCount()) return false;

    m_current_animation = tindex;
}

void GameObj::draw(sf::RenderTarget *tscreen)
{
    tscreen->draw( *m_sprites[ getCurrentSpriteIndex()] );

    // if debug on to draw bounding boxes
    if(m_jumpy->m_dbg_showboundingboxes)
    {
        drawFloatRect(getBoundingBox(), tscreen);
    }
}

void GameObj::doMove(int movedir)
{

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

void GameObj::setPosition(sf::Vector2f tpos)
{
    m_position = tpos;
    m_sprites[getCurrentSpriteIndex()]->setPosition(tpos);
}

void GameObj::update()
{
    // apply acceleration to velocity
    m_velocity += m_acceleration;

    // apply velocity to position
    m_position += m_velocity;

    // set current sprites to obj position
    m_sprites[ getCurrentSpriteIndex() ]->setPosition(m_position);
}

XMLNode *GameObj::saveToNode(XMLDocument *tdoc)
{

    if(!tdoc) return NULL;

    XMLNode *anode = tdoc->NewElement("Obj");

    XMLElement *element = tdoc->NewElement("Type");
    element->SetText(getType());
    anode->InsertEndChild(element);

    element = tdoc->NewElement("X");
    element->SetText(int(m_position.x));
    anode->InsertEndChild(element);

    element = tdoc->NewElement("Y");
    element->SetText( int(m_position.y));
    anode->InsertEndChild(element);

    return anode;
}
