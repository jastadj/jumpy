#include "particle.hpp"
#include "jumpy.hpp"


/////////////////////////////////////////////////////////////////////
//  PARTICLE MANAGER
ParticleManager::ParticleManager()
{
    sf::Texture *newtexture = new sf::Texture;
    newtexture->loadFromFile(".\\Data\\Art\\particleverytiny.png");
    m_textures.push_back(newtexture);

    newtexture = new sf::Texture;
    newtexture->loadFromFile(".\\Data\\Art\\particletiny.png");
    m_textures.push_back(newtexture);


    // init linked list
    head = NULL;
    tail = NULL;
}

ParticleManager::~ParticleManager()
{

}

int ParticleManager::getCount()
{
    int c = 0;

    Particle *tp = head;

    while(tp)
    {
        c++;
        tp = tp->next;
    }

    return c;
}

void ParticleManager::addParticle(Particle *tparticle)
{
    // if valid particle
    if(tparticle)
    {
        // if there is currently no head (no particles)
       if(!head)
       {
           head = tparticle;
           tail = tparticle;
       }
       // else add particle to end of linked list
       else
       {
           tail->next = tparticle;
           tparticle->prev = tail;

           tail = tparticle;
       }
    }
}

void ParticleManager::deleteParticle(Particle *tparticle)
{
    // if valid particle
    if(tparticle)
    {
        // if target particle is the head
        if(head == tparticle)
        {
            head = tparticle->next;
        }

        // if target particle is the tail
        if(tail == tparticle)
        {
            tail = tparticle->prev;
        }

        // link prev if valid
        if(tparticle->prev)
        {
            tparticle->prev = tparticle->next;
        }

        // link next if valid
        if(tparticle->next)
        {
            tparticle->next = tparticle->prev;
        }

        // delete particle
        delete tparticle->m_sprite;
        delete tparticle;
    }
}

void ParticleManager::clear()
{
    // start with head and delete all particles
    Particle *tp = head;

    // while a valid particle
    while(tp)
    {
        Particle *dp = tp;

        tp = tp->next;

        deleteParticle(dp);
    }
}

sf::Texture *ParticleManager::getTexture(int index)
{
    if(index < 0 || index >= int(m_textures.size())) return NULL;

    return m_textures[index];
}





void ParticleManager::draw(sf::RenderTarget *tscreen)
{
    // draw all particles starting with head
    Particle *tp = head;

    // do while valid particle pointer
    while(tp)
    {
        // draw particle sprite
        tscreen->draw( *tp->m_sprite);

        // advance to next particle in linked list
        tp = tp->next;

    }
}

void ParticleManager::update()
{
    // go through linked list and update all particles

    Particle *tp = head;

    // do while particle pointer is not null
    while( tp)
    {
        // if particle has exceeded it's lifetime
        if(m_clock.getElapsedTime().asMilliseconds() >= tp->m_born.asMilliseconds() + tp->m_life_time)
        {
            // get temporary particle pointer for deletion
            Particle *dp = tp;
            // advance iterator pointer to next
            tp = tp->next;
            // delete particle
            deleteParticle(dp);
        }

        // else, update particle
        else
        {
            // update particle position based on velocity
            tp->m_position += tp->m_vel;

            // update sprite color
            tp->m_sprite->setColor( tp->m_color);

            // update sprite pos
            tp->m_sprite->setPosition(tp->m_position);

            // advance particle pointer
            tp = tp->next;
        }

    }
}

/////////////////////////////////////////////////////////////////////
//  PARTICLE EMITTER

ParticleEmitter::ParticleEmitter(int ptype, sf::Vector2f ppos)
{
    Jumpy *jumpy;
    jumpy = Jumpy::getInstance();

    m_particle_manager = jumpy->getParticleManager();

    m_type = ptype;
    m_position = ppos;
}

ParticleEmitter::~ParticleEmitter()
{

}

void ParticleEmitter::createParticle( sf::Vector2f offsetpos, sf::Vector2f initialvel, int lifetimems)
{
    // create new particle
    Particle *newparticle = new Particle;

    // init particle
    newparticle->m_position = m_position + offsetpos;
    newparticle->m_vel = initialvel;
    newparticle->m_born = m_particle_manager->getClock()->getElapsedTime();
    newparticle->m_life_time = lifetimems;


    // init linked list
    newparticle->next = NULL;
    newparticle->prev = NULL;

    switch(m_type)
    {
    case PEMIT_DEFAULT:
    default:
        newparticle->m_color = sf::Color::Blue;
        newparticle->m_sprite = new sf::Sprite( *m_particle_manager->getTexture(0));
        break;
    }

    m_particle_manager->addParticle(newparticle);
}

void ParticleEmitter::setPosition( sf::Vector2f tpos)
{
    m_position = tpos;
}
