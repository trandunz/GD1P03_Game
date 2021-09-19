#pragma once
#ifndef _SLIME_H__
#define _SLIME_H__

#include "Enemy.h"
#include "Player.h"
class Slime : public CEnemy
{
public:
	enum class SLIMETYPE
	{
		GREEN,
		BLUE,
		RED,
		PURPLE,
		YELLOW,
		BOSS
	};

	Slime(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY);
	virtual ~Slime();

	virtual void Start();
	virtual void Update();
	void Render(sf::Shader* _shader = NULL);

	void SetPlayer(CPlayer* _player);
	void LoosePlayer();
	bool bHasPlayer();

	virtual void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);

	virtual void Movement();
	virtual void Attack();

	void TakeDamage(float _damage, bool _projectile = false);

private:
	bool m_bIsBoss = false;

	SLIMETYPE m_SlimeType = SLIMETYPE::GREEN;

	CPlayer* m_Player;

	bool m_bCanFallDamage = true;

	sf::Clock* m_DamageTimer;

};
#endif
