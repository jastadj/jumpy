#include "animation.hpp"

#include <iostream>
#include "tinyxml2.h"

using namespace tinyxml2;

Animation::Animation(std::string nname)
{
    m_name = nname;

    m_current_frame = 0;

    AnimationFrame newframe;
    newframe.m_sprite_index = 0;
    newframe.m_time = 0;
    m_animation_frames.push_back(newframe);

}

Animation::~Animation()
{

}

bool Animation::loadFromFile(std::string filename)
{
    XMLDocument tdoc;
    XMLElement *element = NULL;
    XMLNode *anode = NULL;

    int globaltime = 100;

    // load xml
    if(tdoc.LoadFile(filename.c_str()))
    {
        std::cout << "Error loading animation file:" << filename <<  std::endl;
        return false;
    }

    // clear data
    m_animation_frames.clear();
    m_current_frame = 0;

    //get nodes
    XMLNode *root = tdoc.FirstChildElement("Methd_Up");
    if(!root) {std::cout << "Not a Methd Up xml!\n"; return false;}

    XMLNode *animnode = root->FirstChildElement("Animation");
    if(!animnode) {std::cout << "Could not find animation in file!\n"; return false;}

    // get animation name
    element = animnode->FirstChildElement("Name");
    if(element)
    {
        m_name = std::string(element->GetText());
    }

    // get global animation time
    // this allows frame times to be set if no specific time is provided
    element = animnode->FirstChildElement("GlobalAnimTime");
    if(element)
    {
        element->QueryIntText(&globaltime);
    }

    // get frames
    anode = animnode->FirstChildElement("Frame");
    if(anode)
    {
        while(anode)
        {
            AnimationFrame newframe;

            // get sprite index
            element = anode->FirstChildElement("SpriteIndex");
            if(!element)
            {
                std::cout << "Error getting sprite index frame from animation file!\n";
                return false;
            }
            element->QueryIntText(&newframe.m_sprite_index);

            // get optional animation time
            element = anode->FirstChildElement("AnimTime");

            if(element)
            {
                element->QueryIntText(&newframe.m_time);
            }
            else newframe.m_time = globaltime;

            // add frame
            m_animation_frames.push_back(newframe);

            // advance to next frame node
            anode = anode->NextSiblingElement("Frame");
        }
    }

    return true;
}

void Animation::offsetFrames(int offset)
{
    for(int i = 0; i < int(m_animation_frames.size()); i++)
    {
        m_animation_frames[i].m_sprite_index += offset;
    }
}

bool Animation::setCurrentFrameIndex(int tframe)
{
    if(tframe < 0 || tframe >= frameCount())
    {
        std::cout << "Error setting current frame #" << tframe << ", out of bounds!\n";
        return false;
    }

    m_current_frame = tframe;

    return true;
}

void Animation::addAnimationFrame(int spriteindex, int ttime)
{
    AnimationFrame newframe;

    newframe.m_sprite_index = spriteindex;
    newframe.m_time = ttime;

    m_animation_frames.push_back(newframe);
}

AnimationFrame *Animation::getCurrentAnimationFrame()
{
    return &m_animation_frames[m_current_frame];
}

int Animation::getCurrentAnimationSpriteIndex()
{
    return m_animation_frames[m_current_frame].m_sprite_index;
}

void Animation::update()
{
    if( m_animation_frames[m_current_frame].m_time == 0) return;

    if( m_animation_clock.getElapsedTime().asMilliseconds() > m_animation_frames[m_current_frame].m_time)
    {
        m_current_frame++;

        if(m_current_frame >= frameCount()) m_current_frame = 0;

        m_animation_clock.restart();
    }
}

void Animation::show()
{
    std::cout << "Animation:\n";
    std::cout << "name:" << m_name << std::endl;
    for(int i = 0; i < int(m_animation_frames.size()); i++)
    {
        std::cout << i << ":\n";
        std::cout << "  index - " << m_animation_frames[i].m_sprite_index << std::endl;
        std::cout << "  time  - " << m_animation_frames[i].m_time << std::endl;
    }

    std::cout << "total animation frames:" << frameCount() << std::endl;
}
