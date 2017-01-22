#ifndef CLASS_TOOLS
#define CLASS_TOOLS

#include <SFML\Graphics.hpp>
#include <math.h>
#include <time.h>

// vector stuff
float getMagnitude(sf::Vector2f tvec);
sf::Vector2f normalizeVector(sf::Vector2f tvec);

// misc tools
int rollDice(int num, int sides, int modifier);
void drawFloatRect(sf::FloatRect trect, sf::RenderTarget *tscreen);

#endif // CLASS_TOOLS
