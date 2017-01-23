#include "particle.hpp"

ParticleManager::ParticleManager()
{
    head = NULL;
    tail = NULL;

    sf::Texture *newtexture = new sf::Texture;
    newtexture->loadFromFile(".\\Data\\Art\\particletiny.png");
    m_textures.push_back(newtexture);
}

ParticleManager::~ParticleManager()
{
    clear();
}

void ParticleManager::clear()
{
    Particle *tparticle = head;

    if(head == NULL) return;

    while(tparticle != NULL)
    {
        Particle *nextp = tparticle->next;

        delete tparticle;

        tparticle = nextp;
    }

    head = NULL;
    tail = NULL;
}


void ParticleManager::addParticle(Particle *tparticle)
{
    if(head == NULL)
    {
        head = tparticle;
        tail = tparticle;
        tparticle->next = NULL;
        tparticle->prev = NULL;

    }
    else
    {
        tail->next = tparticle;
        tparticle->prev = tail;
        tparticle->next = NULL;
        tail = tail->next;
    }

    tparticle->m_sprite = new sf::Sprite(*m_textures[0]);
    tparticle->m_born = m_clock.getElapsedTime();
}

bool ParticleManager::deleteParticle(Particle *tparticle)
{
    Particle *iparticle = head;

    if(head == NULL) return false;

    if(tparticle == NULL) return false;

    // if onyl one particle
    if(tparticle == head && tparticle == tail)
    {
        delete tparticle;
        head = NULL;
        tail = NULL;
    }

    // while iterator particle is not null
    while(iparticle != NULL)
    {
        // if found target particle
        if(iparticle == tparticle)
        {
            // if target particle is the head
            if(tparticle == head)
            {
                head = tparticle->next;
                head->prev = NULL;
                delete tparticle;

                return true;
            }
            // if target particle is the tail
            else if(tparticle == tail)
            {
                tail = tail->prev;
                tail->next = NULL;
                delete tparticle;

                return true;
            }

            // remove and stitch linked list
            tparticle->prev->next = tparticle->next;
            tparticle->next->prev = tparticle->prev;

            delete tparticle;
            return true;
        }

        // advance pointer
        iparticle = iparticle->next;
    }

    return false;
}

int ParticleManager::particleCount()
{
    int pcount = 0;
    Particle *ip = head;

    while(ip != NULL)
    {
        pcount++;

        ip = ip->next;
    }

    return pcount;
}

void ParticleManager::draw(sf::RenderTarget *tscreen)
{
    Particle *ip = head;

    while(ip != NULL)
    {
        ip->draw(tscreen);
        ip = ip->next;
    }
}

void ParticleManager::update()
{
    Particle *ip = head;

    while(ip != NULL)
    {
        if( m_clock.getElapsedTime().asMilliseconds() >= ip->m_born.asMilliseconds() + ip->m_life_time )
        {
            Particle *dp = ip;

            ip = ip->next;

            deleteParticle(dp);
        }
    }
}

//////////////////////////////////////////////////////////////////

ParticleEmitter::ParticleEmitter()
{

}

ParticleEmitter::~ParticleEmitter()
{

}

//////////////////////////////////////////////////////////////////

Particle::Particle()
{
    prev = NULL;
    next = NULL;

    m_life_time = 1000;
}

Particle::~Particle()
{
    delete m_sprite;
}

void Particle::setColor(sf::Color tcolor)
{
    m_color = tcolor;
    m_sprite->setColor(tcolor);
}

void Particle::draw(sf::RenderTarget *tscreen)
{
    if(tscreen == NULL) return;

    tscreen->draw(*m_sprite);
}

void Particle::update()
{


    // update position
    m_sprite->setPosition(m_position);
}
