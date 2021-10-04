#pragma once
#ifndef _SLIME_H__
#define _SLIME_H__

#include "Enemy.h"
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
		BOSSGREEN,
		BOSSBLUE,
		BOSSRED,
		BOSSPURPLE,
		BOSSYELLOW,
	};

	Slime(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CAudioManager& _audioManager, bool _boss = false);
	virtual ~Slime();

	virtual void Start();
	virtual void Update();
	void Render(sf::Shader* _shader = NULL);

	void SetPlayer(CPlayer* _player);
	void LoosePlayer();
	bool bHasPlayer();

	virtual void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);

	virtual void Movement();
	void SimpleMove();
	virtual void Attack();

	SLIMETYPE m_SlimeType = SLIMETYPE::GREEN;

	bool m_bIsBoss = false;
private:
	

	sf::Clock m_AttackTimer;

	CPlayer* m_Player;

	bool m_bCanFallDamage = true;
};
#endif
