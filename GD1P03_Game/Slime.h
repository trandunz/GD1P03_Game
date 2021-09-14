#pragma once
#include "Enemy.h"
class Slime : CEnemy
{
public:
	Slime(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY);
	virtual ~Slime();

	virtual void Start();
	virtual void Update();
	virtual void Render();

	virtual void Movement();
};

