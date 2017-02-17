#include "zone.hpp"

#include <iostream>


#include "level.hpp"
#include "tinyxml2.h"


Zone::Zone(std::string zonefile)
{
    tinyxml2::XMLDocument tdoc;
    tinyxml2::XMLNode *root;
    tinyxml2::XMLNode *anode;
    tinyxml2::XMLElement *element;

    if( tdoc.LoadFile(std::string(ZONE_FILE + zonefile).c_str()) )
    {
        std::cout << "Error loading zone from file : " << ZONE_FILE + zonefile << std::endl;
        return;
    }

    // get root node
    root = tdoc.FirstChildElement("Methd_Up");
    if(!root)
    {
        std::cout << ZONE_FILE+zonefile << " is not a valid Methd Up file!\n";
        return;
    }

    anode = root->FirstChildElement("Zone");

    if(!anode)
    {
        std::cout << ZONE_FILE+zonefile << " is not a valid Methd Up Zone file!\n";
        return;
    }

    element = anode->FirstChildElement("Level");

    if(!element)
    {
        std::cout << "No levels found in zone file : " << ZONE_FILE+zonefile << std::endl;
        return;
    }

    while(element)
    {
        Level *newlevel = new Level(element->GetText());

        m_levels.push_back(newlevel);

        element = element->NextSiblingElement("Level");
    }

    std::cout << "Zone loaded with " << getCount() << " levels.\n";

    // set file name
    m_filename = zonefile;

}

Zone::~Zone()
{
    // delete all the levels
    for(int i = 0; i < getCount(); i++)
    {
        delete m_levels[i];
    }
    m_levels.clear();
}
