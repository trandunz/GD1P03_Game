#pragma once
#include "Enemy.h"
class Zombie : public CEnemy
{
public:
	Zombie(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY);
	virtual ~Zombie();

	virtual void Start();
	virtual void Update();
	void Render();

	virtual void Movement();

private:
	sf::Clock m_AnimationTimer;
};

