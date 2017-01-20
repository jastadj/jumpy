#include "gameobj.hpp"

#include "jumpy.hpp"

GameObj::GameObj()
{
    // get main callback reference
    m_jumpy = Jumpy::getInstance();
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
