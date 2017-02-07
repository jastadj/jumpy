#include "leveleditor.hpp"

#include "jumpy.hpp"
#include "spritesheet.hpp"
#include "level.hpp"
#include "player.hpp"

#include "tile.hpp"

#include "tools.hpp"

LevelEditor::LevelEditor()
{
    m_jumpy = Jumpy::getInstance();
    m_player = m_jumpy->getPlayer();

    m_currentlevel = m_jumpy->getCurrentLevel();

    m_font.loadFromFile(".\\Data\\font.ttf");

    m_mode = ED_NONE;
    m_drawmode = ED_NONE;
    m_brushid = 0;
    m_brushsprite = NULL;

    m_screenwidth = m_jumpy->getScreenWidth();
    m_screenheight = m_jumpy->getScreenHeight();

    SpriteSheet *newss;

    // edit buttons
    newss = new SpriteSheet(".\\Data\\Art\\editbut.png", 4,1);
    newss->setScale(2);
    m_spritesheets.push_back(newss);
    for(int i = 0; i < newss->getCount(); i++)
    {
        m_edit_buttons.push_back(newss->createSprite(i));
    }


    // create tile edit buttons
    std::vector<Tile*> *temptiles = m_jumpy->getTiles();
    for(int i = 0; i < int(temptiles->size()); i++)
    {
        sf::Sprite *tsprite = new sf::Sprite();
        *tsprite = *(*temptiles)[i]->getSprite();
        m_tiles.push_back( tsprite);
    }

    // create tile bg edit buttons
    temptiles = m_jumpy->getTilesBG();
    for(int i = 0; i < int(temptiles->size()); i++)
    {
        sf::Sprite *tsprite = new sf::Sprite();
        *tsprite = *(*temptiles)[i]->getSprite();
        m_tilesbg.push_back(tsprite);
    }

    // create tile fg edit buttons
    temptiles = m_jumpy->getTilesFG();
    for(int i = 0; i < int(temptiles->size()); i++)
    {
        sf::Sprite *tsprite = new sf::Sprite();
        *tsprite = *(*temptiles)[i]->getSprite();
        m_tilesfg.push_back(tsprite);

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

    for(int i = 0; i < int(m_tilesbg.size()); i++)
    {
        delete m_tilesbg[i];
    }
    m_tilesbg.clear();

    for(int i = 0; i < int(m_tilesfg.size()); i++)
    {
        delete m_tilesfg[i];
    }
    m_tilesfg.clear();

    // delete sprite sheets
    for(int i = 0; i < int(m_spritesheets.size()); i++) delete m_spritesheets[i];
    m_spritesheets.clear();
}


void LevelEditor::update()
{

    if(m_mode == ED_NONE || m_mode == ED_TILEDRAW || m_mode == ED_TILEDRAWBG || m_mode == ED_TILEDRAWFG)
    {
        if( sf::Keyboard::isKeyPressed( sf::Keyboard::A)) m_player->doMove(MOVE_LEFT);
        else if( sf::Keyboard::isKeyPressed( sf::Keyboard::D)) m_player->doMove(MOVE_RIGHT);
        else if( sf::Keyboard::isKeyPressed( sf::Keyboard::W)) m_player->doMove(MOVE_UP);
        else if( sf::Keyboard::isKeyPressed( sf::Keyboard::S)) m_player->doMove(MOVE_DOWN);
        else m_player->doMove(MOVE_NONE);
    }

    for(int i = 0; i < int(m_edit_buttons.size()); i++)
    {
        m_edit_buttons[i]->setPosition( m_screenwidth-50, i*34);
    }
}

void LevelEditor::draw(sf::RenderWindow *tscreen)
{
    // capture mouse position
    m_mouseleft = sf::Vector2f(sf::Mouse::getPosition(*tscreen));
    //std::cout << "*********Mouse clicked at :" << m_mouseleft.x << "," << m_mouseleft.y << std::endl;

    sf::Vector2f m_mouseleftw;
    //m_mouseleftw = tview->getTransform().transformPoint(m_mouseleft);
    m_mouseleftw = tscreen->mapPixelToCoords(sf::Vector2i(m_mouseleft));
    //std::cout << "*********Mouse clicked at :" << m_mouseleftw.x << "," << m_mouseleftw.y << std::endl;

    sf::Vector2i m_mouseleftg = sf::Vector2i(m_mouseleftw);
    m_mouseleftg.x = int(m_mouseleftg.x / 32);
    m_mouseleftg.y = int(m_mouseleftg.y / 32);
    //std::cout << "*********Mouse clicked at :" << m_mouseleftg.x << "," << m_mouseleftg.y << std::endl;


    if(m_mode == ED_TILEDRAW || m_mode == ED_TILEDRAWBG || m_mode == ED_TILEDRAWFG)
    {
        // draw brush at mouse pos
        if(m_brushsprite)
        {
            m_brushsprite->setPosition(m_mouseleftg.x*32, m_mouseleftg.y*32);
            tscreen->draw(*m_brushsprite);
        }
    }

}

void LevelEditor::drawUI(sf::RenderWindow *tscreen)
{
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
            m_tiles[i]->setPosition(34*((i/8)+i%8), 34*(i/8) );
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
    else if(m_mode == ED_TILEFG)
    {
        tscreen->draw(*m_coverscreen);

        // adjust sprite positions
        for(int i = 0; i < int(m_tilesfg.size()); i++)
        {
            m_tilesfg[i]->setPosition(34*i, 32);
        }

        // draw tiles
        for(int i = 0; i < int(m_tilesfg.size()); i++)
        {
            tscreen->draw( *m_tilesfg[i]);
        }
    }
    else if(m_mode == ED_SAVE)
    {
        tscreen->draw(*m_coverscreen);

        // filename
        std::string mfilestr("Map File:");
        mfilestr += m_currentlevel->getFilename();
        sf::Text cfilename(mfilestr, m_font);
        cfilename.setPosition(25, 100);
        cfilename.setFillColor(sf::Color::White);


        m_saveastext.setString("SAVE AS :");
        m_saveastext.setFont(m_font);

        m_saveastext.setPosition(200,200);
        m_saveastext.setFillColor(sf::Color::White);

        tscreen->draw(cfilename);
        tscreen->draw(m_saveastext);

        sf::Text myfile;
        myfile.setFont(m_font);
        myfile.setString( m_savefilename.c_str());
        myfile.setPosition(380, 200);
        myfile.setFillColor(sf::Color::White);

        tscreen->draw(myfile);
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
                        else if(i == 2) m_mode = ED_TILEFG;
                        else if(i == 3)
                        {
                            m_savefilename = std::string("");
                            m_mode = ED_SAVE;
                        }

                        std::cout << "SETTING EDIT MODE TO " << m_mode << std::endl;
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
                        m_brushsprite = new sf::Sprite();
                        *m_brushsprite = *m_tiles[i];

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
                        m_brushsprite = new sf::Sprite();
                        *m_brushsprite = *m_tilesbg[i];

                    }
                }
            }
            // if selecting foreground tile to draw
            else if(m_mode == ED_TILEFG)
            {
                // check all the tiles for mouse click
                for(int i = 0; i < int(m_tilesfg.size()); i++)
                {
                    // if a tile was clicked
                    if(m_tilesfg[i]->getGlobalBounds().contains(m_mouseleft))
                    {
                        // set brush id
                        m_brushid = i;

                        // set draw mode
                        m_drawmode = ED_TILEDRAWFG;
                        m_mode = ED_TILEDRAWFG;

                        // create brush sprite
                        if(m_brushsprite) delete m_brushsprite;
                        m_brushsprite = new sf::Sprite();
                        *m_brushsprite = *m_tilesfg[i];

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
            // ELSE IF DRAWING A TILE
            else if(m_mode == ED_TILEDRAWFG)
            {
                // set brush tile to map position
                m_currentlevel->setTileFG(m_mouseleftg.x, m_mouseleftg.y, m_brushid);
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
            else if(m_mode == ED_TILEDRAWFG)
            {
                // erase tile at mouse position
                m_currentlevel->setTileFG(m_mouseleftg.x, m_mouseleftg.y, 0);
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
                else if(m_mode == ED_TILEDRAWFG) m_mode = ED_TILEFG;
                else m_mode = ED_NONE;
            }
        }
        // resize map x bigger
        else if(event->key.code == sf::Keyboard::Numpad6)
        {
            m_currentlevel->resizeX(1);
        }
        // resize map x smaller
        else if(event->key.code == sf::Keyboard::Numpad4)
        {
            m_currentlevel->resizeX(-1);
        }
        // resize map y bigger
        else if(event->key.code == sf::Keyboard::Numpad8)
        {
            m_currentlevel->resizeY(1);
        }
        // resize map y smaller
        else if(event->key.code == sf::Keyboard::Numpad2)
        {
            m_currentlevel->resizeY(-1);
        }
    }
    else if(event->type == sf::Event::TextEntered)
    {
        if(m_mode == ED_SAVE)
        {

            if(event->text.unicode > 80)
            {
                m_savefilename += char(event->text.unicode);

            }
            else if(event->text.unicode == 32)
            {
                m_savefilename += ' ';
            }
            // backspace
            else if(event->text.unicode == 8)
            {
                int slen = m_savefilename.length();

                if(slen > 0)m_savefilename.resize( slen-1);
            }
            // return
            else if(event->text.unicode == 13)
            {
                // if user didn't add extension, go ahead and add it
                if(m_savefilename.find_first_of(".xml") == std::string::npos)
                {
                    m_savefilename += ".xml";
                }

                m_currentlevel->save( std::string(".\\Data\\Levels\\") + m_savefilename);
                m_mode = ED_NONE;
            }

        }

    }
}

std::string LevelEditor::drawSelectLevelFile(sf::RenderWindow *tscreen)
{
    tscreen->draw(*m_coverscreen);

    std::vector <std::string> levelfiles = getFiles(LEVEL_FILE, ".xml");

    std::vector< sf::Text > leveltext;

}
