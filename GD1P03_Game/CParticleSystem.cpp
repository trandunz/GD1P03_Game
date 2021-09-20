#include "CParticleSystem.h"

void CParticleSystem::Start()
{
    for (std::size_t i = 0; i < m_Particles.size(); ++i)
    {
        // update the particle lifetime
        Particle& p = m_Particles[i];
        p.m_Lifetime = sf::seconds(0);
    }
}

void CParticleSystem::Update(sf::Time _elapsedTime)
{
    for (std::size_t i = 0; i < m_Particles.size(); ++i)
    {
        // update the particle lifetime
        Particle& p = m_Particles[i];
        p.m_Lifetime -= _elapsedTime;

        // if the particle is dead, respawn it
        if (p.m_Lifetime <= sf::Time::Zero)
        {
            KillParticle(i);
        }
        else
        {
            // update the position of the corresponding vertex
            m_Vertices[i].position += p.m_Velocity * _elapsedTime.asSeconds();

            // update the alpha (transparency) of the particle according to its lifetime
            float ratio = p.m_Lifetime.asSeconds() / m_LifeTime.asSeconds();
            m_Vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
        }

        
    }
    
}

void CParticleSystem::SetEmitter(sf::Vector2f _position)
{
	m_Emitter = _position;

    for (std::size_t i = 0; i < m_Particles.size(); ++i)
    {
        ResetParticle(i);
        m_Vertices[i].position = m_Emitter;
    }
}

void CParticleSystem::SetColor(sf::Color _color)
{
    for (std::size_t i = 0; i < m_Particles.size(); ++i)
    {
        m_Vertices[i].color = _color;
    }
}

void CParticleSystem::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
    // apply the transform
    _states.transform *= getTransform();

    // our particles don't use a texture
    _states.texture = NULL;

    // draw the vertex array
    _target.draw(m_Vertices, _states);
}

void CParticleSystem::ResetParticle(std::size_t _index)
{
    // give a random velocity and lifetime to the particle
    float angle = (std::rand() % 360) * 3.14f / 180.f;
    float speed = (std::rand() % 50) + 50.f;
    m_Particles[_index].m_Velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
    m_Particles[_index].m_Lifetime = m_LifeTime;
    //m_Vertices[_index].position = m_Emitter;

    m_bIsDead = false;
}

void CParticleSystem::KillParticle(std::size_t _index)
{
    m_Vertices[_index].color.a = 0;
}
