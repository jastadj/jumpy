#include "leveleditor.hpp"

#include "jumpy.hpp"
#include "spritesheet.hpp"

LevelEditor::LevelEditor()
{
    m_jumpy = Jumpy::getInstance();

    m_mode = ED_NONE;

    m_screenwidth = m_jumpy->getScreenWidth();
    m_screenheight = m_jumpy->getScreenHeight();

    SpriteSheet *newss;

    // edit buttons
    newss = new SpriteSheet(".\\Data\\Art\\editbut.png", 1,1);
    newss->setScale(2);
    m_spritesheets.push_back(newss);
    m_edit_buttons.push_back(newss->createSprite(0));

    m_coverscreen = new sf::RectangleShape(sf::Vector2f(m_screenwidth, m_screenheight));
    m_coverscreen->setFillColor(sf::Color(0,0,200,230));


}

LevelEditor::~LevelEditor()
{
    // delete sprites
    for(int i = 0; i < int(m_edit_buttons.size()); i++) delete m_edit_buttons[i];
    m_edit_buttons.clear();


    // delete sprite sheets
    for(int i = 0; i < int(m_spritesheets.size()); i++) delete m_spritesheets[i];
    m_spritesheets.clear();
}


void LevelEditor::update()
{
    for(int i = 0; i < int(m_edit_buttons.size()); i++)
    {
        m_edit_buttons[i]->setPosition( m_screenwidth-50, 16 + i*20);
    }
}

void LevelEditor::draw(sf::RenderTarget *tscreen)
{
    update();

    for(int i = 0; i < int(m_edit_buttons.size()); i++)
    {
        tscreen->draw( *m_edit_buttons[i]);
    }

    // if edit mode, draw cover screen
    if(m_mode == ED_TILE) tscreen->draw(*m_coverscreen);

}


void LevelEditor::processEvent(sf::Event *event, sf::RenderWindow *tscreen)
{
    // mouse button pressed
    if(event->type == sf::Event::MouseButtonPressed)
    {
        // left mouse button pressed
        if(event->mouseButton.button == sf::Mouse::Left)
        {
            // capture mouse position
            m_mouseleft = sf::Vector2f(sf::Mouse::getPosition(*tscreen));
            std::cout << "Mouse clicked at :" << m_mouseleft.x << "," << m_mouseleft.y << std::endl;

            // if no edit mode
            if(m_mode == ED_NONE)
            {
                // if edit button clicked
                for(int i = 0; i < int(m_edit_buttons.size()); i++)
                {
                    if(m_edit_buttons[i]->getGlobalBounds().contains(m_mouseleft))
                    {
                        if(i == 0) m_mode = ED_TILE;
                    }
                }
            }

        }
    }
    // key pressed
    else if(event->type == sf::Event::KeyPressed)
    {
        // escape button pressed
        if(event->key.code == sf::Keyboard::Escape)
        {
            // if in an edit mode, exit
            if(m_mode) m_mode = ED_NONE;
        }
    }
}
