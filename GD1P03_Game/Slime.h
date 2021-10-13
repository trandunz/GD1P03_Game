//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : Slime.h
// Description : Slime Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

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
	void RandomizeSlimeType(bool _boss);
	void SetAppropriateSlimeTexture();
	void Setb2pShapeSizeAppropriatly();

	void MoveLeft(float DistanceToPlayer);
	void MoveRight(float DistanceToPlayer);

	void SimpleMoveLeft();
	void SimpleMoveRight();

	void AttackBasedOnAppropriateType(int DirectionToPlayer, float DistanceToPlayer);

	sf::Clock m_AttackTimer;

	bool m_bCanFallDamage = true;
};
#endif
