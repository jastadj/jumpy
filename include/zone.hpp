#ifndef CLASS_ZONE
#define CLASS_ZONE

#include <SFML\Graphics.hpp>
#include <string>

#define ZONE_FILE ".\\Data\\Zones\\"

// forward declarations
class Level;

class Zone
{
private:

    std::string m_filename;

    std::vector<Level*> m_levels;

public:
    Zone(std::string zonefile);
    ~Zone();

    int getCount() { return int(m_levels.size());}

    std::vector<Level*> *getLevels() { return &m_levels;}
    Level *getLevelWithName(std::string tname);
    Level *getLevelWithFilename(std::string tname);

    void show();

};
#endif // CLASS_ZONE
