#ifndef CLASS_PARTICLE
#define CLASS_PARTCILE

#include <SFML\Graphics.hpp>

#include <vector>

class Particle
{
private:


public:
    Particle();
    ~Particle();

    Particle *next;
    Particle *prev;

    sf::Sprite *m_sprite;

    sf::Color m_color;
    sf::Time m_born;
    int m_life_time;

    sf::Vector2f m_position;
    sf::Vector2f m_vel;
    sf::Vector2f m_accel;

    void setColor(sf::Color tcolor);

    void draw(sf::RenderTarget *tscreen);
    void update();
};

class ParticleEmitter
{
private:

public:
    ParticleEmitter();
    ~ParticleEmitter();

};

class ParticleManager
{
private:

    Particle *head;
    Particle *tail;
    sf::Clock m_clock;

    std::vector<sf::Texture*> m_textures;
public:
    ParticleManager();
    ~ParticleManager();

    void addParticle(Particle *tparticle);
    bool deleteParticle(Particle *tparticle);
    void clear();

    sf::Time getTime() { return m_clock.getElapsedTime();}

    int particleCount();

    void draw(sf::RenderTarget *tscreen);
    void update();




};
#endif // CLASS_PARTICLE
