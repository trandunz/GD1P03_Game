#pragma once
#ifndef _PARTICLESYSTEM_H__
#define _PARTICLESYSTEM_H__

// Non-Local Includes
#include <SFML/Graphics.hpp>

class CParticleSystem : public sf::Drawable, public sf::Transformable
{
public:
	inline CParticleSystem(int _count, sf::Time _lifeTime, sf::Color _color) : 
		m_Particles(_count), 
		m_Vertices(sf::Points, _count), 
		m_LifeTime(_lifeTime),
		m_Emitter(0.0f, 0.0f)
		{
		for (std::size_t i = 0; i < m_Particles.size(); ++i)
		{
			m_Vertices[i].color = _color;
		}
		}

	void Start();
	void Update(sf::Time _elapsedTime);

	void SetEmitter(sf::Vector2f _position);

	void SetColor(sf::Color _color);

private:
	/// <summary>
	/// Individual Particles
	/// </summary>
	struct Particle
	{
		sf::Vector2f m_Velocity;
		sf::Time m_Lifetime;
	};

	virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;

	void ResetParticle(std::size_t _index);

	void KillParticle(std::size_t _index);

	std::vector<Particle> m_Particles;
	sf::VertexArray m_Vertices;
	sf::Time m_LifeTime;
	sf::Vector2f m_Emitter;

	bool m_bIsDead = false;
};
#endif
