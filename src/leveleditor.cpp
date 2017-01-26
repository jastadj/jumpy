#include "leveleditor.hpp"

#include "jumpy.hpp"
#include "spritesheet.hpp"
#include "level.hpp"

LevelEditor::LevelEditor()
{
    m_jumpy = Jumpy::getInstance();

    m_currentlevel = m_jumpy->getCurrentLevel();

    m_mode = ED_NONE;
    m_drawmode = ED_NONE;
    m_brushid = 0;
    m_brushsprite = NULL;

    m_screenwidth = m_jumpy->getScreenWidth();
    m_screenheight = m_jumpy->getScreenHeight();

    SpriteSheet *newss;

    // edit buttons
    newss = new SpriteSheet(".\\Data\\Art\\editbut.png", 2,1);
    newss->setScale(2);
    m_spritesheets.push_back(newss);
    for(int i = 0; i < newss->getCount(); i++)
    {
        m_edit_buttons.push_back(newss->createSprite(i));
    }




    // create tile edit buttons
    SpriteSheet *tss = m_jumpy->getSpriteSheet(1);
    for(int i = 0; i < tss->getCount(); i++)
    {
        m_tiles.push_back( tss->createSprite(i));

    }
    // create tile bg edit buttons
    tss = m_jumpy->getSpriteSheet(2);
    for(int i = 0; i < tss->getCount(); i++)
    {
        m_tilesbg.push_back( tss->createSprite(i));

    }

    m_coverscreen = new sf::RectangleShape(sf::Vector2f(m_screenwidth, m_screenheight));
    m_coverscreen->setFillColor(sf::Color(0,0,200,230));


}

LevelEditor::~LevelEditor()
{
    if(m_brushsprite) delete m_brushsprite;

    // delete sprites
    for(int i = 0; i < int(m_edit_buttons.size()); i++) delete m_edit_buttons[i];
    m_edit_buttons.clear();

    for(int i = 0; i < int(m_tiles.size()); i++)
    {
        delete m_tiles[i];
    }
    m_tiles.clear();

    // delete sprite sheets
    for(int i = 0; i < int(m_spritesheets.size()); i++) delete m_spritesheets[i];
    m_spritesheets.clear();
}


void LevelEditor::update()
{
    for(int i = 0; i < int(m_edit_buttons.size()); i++)
    {
        m_edit_buttons[i]->setPosition( m_screenwidth-50, i*34);
    }
}

void LevelEditor::draw(sf::RenderWindow *tscreen)
{
    update();

    sf::View *tview = m_jumpy->getView();
    //tscreen->setView(*tview);

    sf::Vector2f m_mouseleft;
    // capture mouse position
    m_mouseleft = sf::Vector2f(sf::Mouse::getPosition(*tscreen));
    std::cout << "Mouse clicked at :" << m_mouseleft.x << "," << m_mouseleft.y << std::endl;

    sf::Vector2f m_mouseleftw;
    //m_mouseleftw = tview->getTransform().transformPoint(m_mouseleft);
    m_mouseleftw = tscreen->mapPixelToCoords(sf::Vector2i(m_mouseleft));
    std::cout << "Mouse clicked at :" << m_mouseleftw.x << "," << m_mouseleftw.y << std::endl;

    sf::Vector2i m_mouseleftg = sf::Vector2i(m_mouseleftw);
    m_mouseleftg.x = int(m_mouseleftg.x / 32);
    m_mouseleftg.y = int(m_mouseleftg.y / 32);
    std::cout << "Mouse clicked at :" << m_mouseleftg.x << "," << m_mouseleftg.y << std::endl;

    //tscreen->setView( tscreen->getDefaultView());

    for(int i = 0; i < int(m_edit_buttons.size()); i++)
    {
        tscreen->draw( *m_edit_buttons[i]);
    }

    // if edit mode, draw cover screen
    if(m_mode == ED_TILE)
    {
        tscreen->draw(*m_coverscreen);

        // adjust sprite positions
        for(int i = 0; i < int(m_tiles.size()); i++)
        {
            m_tiles[i]->setPosition(34*i, 32);
        }

        // draw tiles
        for(int i = 0; i < int(m_tiles.size()); i++)
        {
            tscreen->draw( *m_tiles[i]);
        }
    }
    else if(m_mode == ED_TILEBG)
    {
        tscreen->draw(*m_coverscreen);

        // adjust sprite positions
        for(int i = 0; i < int(m_tilesbg.size()); i++)
        {
            m_tilesbg[i]->setPosition(34*i, 32);
        }

        // draw tiles
        for(int i = 0; i < int(m_tilesbg.size()); i++)
        {
            tscreen->draw( *m_tilesbg[i]);
        }
    }
    else if(m_mode == ED_TILEDRAW)
    {
        // draw brush at mouse pos
        if(m_brushsprite)
        {
            m_brushsprite->setPosition(m_mouseleftg.x*32, m_mouseleftg.y*32);
            tscreen->draw(*m_brushsprite);
        }
    }
    else if(m_mode == ED_TILEDRAWBG)
    {
        // draw brush at mouse pos
        if(m_brushsprite)
        {
            m_brushsprite->setPosition(m_mouseleftg.x*32, m_mouseleftg.y*32);
            tscreen->draw(*m_brushsprite);
        }
    }

}


void LevelEditor::processEvent(sf::Event *event, sf::RenderWindow *tscreen)
{
    sf::View *tview = m_jumpy->getView();

    // mouse button pressed
    if(event->type == sf::Event::MouseButtonPressed)
    {



        // capture mouse position
        m_mouseleft = sf::Vector2f(sf::Mouse::getPosition(*tscreen));
        std::cout << "Mouse clicked at :" << m_mouseleft.x << "," << m_mouseleft.y << std::endl;

        sf::Vector2f m_mouseleftw;
        //m_mouseleftw = tview->getTransform().transformPoint(m_mouseleft);
        m_mouseleftw = tscreen->mapPixelToCoords(sf::Vector2i(m_mouseleft));
        std::cout << "Mouse clicked at :" << m_mouseleftw.x << "," << m_mouseleftw.y << std::endl;

        sf::Vector2i m_mouseleftg = sf::Vector2i(m_mouseleftw);
        m_mouseleftg.x = int(m_mouseleftg.x / 32);
        m_mouseleftg.y = int(m_mouseleftg.y / 32);
        std::cout << "Mouse clicked at :" << m_mouseleftg.x << "," << m_mouseleftg.y << std::endl;



        // left mouse button pressed
        if(event->mouseButton.button == sf::Mouse::Left)
        {

            // if no edit mode
            if(m_mode == ED_NONE)
            {
                // if edit button clicked
                for(int i = 0; i < int(m_edit_buttons.size()); i++)
                {
                    if(m_edit_buttons[i]->getGlobalBounds().contains(m_mouseleft))
                    {
                        if(i == 0) m_mode = ED_TILE;
                        else if(i == 1) m_mode = ED_TILEBG;
                    }
                }
            }
            // else if selecting a tile to draw
            else if(m_mode == ED_TILE)
            {
                // check all the tiles for mouse click
                for(int i = 0; i < int(m_tiles.size()); i++)
                {
                    // if a tile was clicked
                    if(m_tiles[i]->getGlobalBounds().contains(m_mouseleft))
                    {
                        // set brush id
                        m_brushid = i;

                        // set draw mode
                        m_drawmode = ED_TILEDRAW;
                        m_mode = ED_TILEDRAW;

                        // create brush sprite
                        if(m_brushsprite) delete m_brushsprite;
                        m_brushsprite = m_jumpy->getSpriteSheet(1)->createSprite(m_brushid);
                        m_brushsprite->setScale(m_jumpy->getScreenZoom(), m_jumpy->getScreenZoom());


                    }
                }
            }
            // if selecting background tile to draw
            else if(m_mode == ED_TILEBG)
            {
                // check all the tiles for mouse click
                for(int i = 0; i < int(m_tilesbg.size()); i++)
                {
                    // if a tile was clicked
                    if(m_tilesbg[i]->getGlobalBounds().contains(m_mouseleft))
                    {
                        // set brush id
                        m_brushid = i;

                        // set draw mode
                        m_drawmode = ED_TILEDRAWBG;
                        m_mode = ED_TILEDRAWBG;

                        // create brush sprite
                        if(m_brushsprite) delete m_brushsprite;
                        m_brushsprite = m_jumpy->getSpriteSheet(2)->createSprite(m_brushid);
                        m_brushsprite->setScale(m_jumpy->getScreenZoom(), m_jumpy->getScreenZoom());


                    }
                }
            }
            // ELSE IF DRAWING A TILE
            else if(m_mode == ED_TILEDRAW)
            {
                // set brush tile to map position
                m_currentlevel->setTile(m_mouseleftg.x, m_mouseleftg.y, m_brushid);
            }
            // ELSE IF DRAWING A TILE
            else if(m_mode == ED_TILEDRAWBG)
            {
                // set brush tile to map position
                m_currentlevel->setTileBG(m_mouseleftg.x, m_mouseleftg.y, m_brushid);
            }


        }

        // if right mouse button clicked
        else if(event->mouseButton.button == sf::Mouse::Right)
        {
            // if in tile draw mode
            if(m_mode == ED_TILEDRAW)
            {
                // erase tile at mouse position
                m_currentlevel->setTile(m_mouseleftg.x, m_mouseleftg.y, 0);
            }
            else if(m_mode == ED_TILEDRAWBG)
            {
                // erase tile at mouse position
                m_currentlevel->setTileBG(m_mouseleftg.x, m_mouseleftg.y, 0);
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
            if(m_mode)
            {
                if(m_mode == ED_TILEDRAW) m_mode = ED_TILE;
                else if(m_mode == ED_TILEDRAWBG) m_mode = ED_TILEBG;
                else m_mode = ED_NONE;
            }
        }
    }
}
