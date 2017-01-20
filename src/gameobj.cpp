#include "gameobj.hpp"

#include "jumpy.hpp"

GameObj::GameObj()
{
    // get main callback reference
    m_jumpy = Jumpy::getInstance();

    m_current_sprite = 0;
}

GameObj::~GameObj()
{
    // delete sprites
    for(int i = 0; i < int(m_sprites.size()); i++)
    {
        delete m_sprites[i];
    }
    m_sprites.clear();
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

void GameObj::draw(sf::RenderWindow *tscreen)
{
    tscreen->draw( *m_sprites[m_current_sprite] );
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
