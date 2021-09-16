#pragma once
#ifndef _ZOMBIE_H__
#define _ZOMBIE_H__

#include "Enemy.h"
#include "Player.h"
class Zombie : public CEnemy
{
public:
	Zombie(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY);
	virtual ~Zombie();

	virtual void Start();
	virtual void Update();
	virtual void Render();

	void SetPlayer(CPlayer* _player);
	virtual void LoosePlayer();
	bool bHasPlayer();

	virtual void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);

	virtual void Movement();

private:
	sf::Clock m_AnimationTimer;
	CPlayer* m_Player;
};
#endif

