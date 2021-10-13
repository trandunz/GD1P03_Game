//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CSnowman.h
// Description : CSnowman Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once
#ifndef _SNOWMAN_H__
#define _SNOWMAN_H__

// Local Includes
#include "Enemy.h"

class CSnowman : public CEnemy
{
public:
	enum class SNOWMANTYPE
	{
		GREEN,
		BLUE,
		RED,
		YELLOW,
		BOSSGREEN,
		BOSSBLUE,
		BOSSRED,
		BOSSYELLOW,
	};

	CSnowman(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CAudioManager& _audioManager, bool _boss = false, bool _bIsCactus = false);
	virtual ~CSnowman();

	virtual void Start();
	virtual void Update(bool& m_PlayerHitByProjectile);
	virtual void Render(sf::Shader* _shader = NULL);

	void SetPlayer(CPlayer* _player);
	void LoosePlayer();
	bool bHasPlayer();

	bool m_bIsBoss = false;

	SNOWMANTYPE m_SnowmanType = SNOWMANTYPE::GREEN;

	bool m_bIsCactus = false;
protected :

	virtual void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);

	virtual void Movement();
	void SimpleMove();
	virtual void Attack();

	void HandleProjectiles(bool& m_PlayerHitByProjectile);
	void DrawProjectiles(sf::Shader* nullshader = NULL);
	void DeleteAllProjectiles();

	void Moveleft(int DistanceToPlayer);
	void MoveRight(int DistanceToPlayer);

	virtual void FireSnowBall(float UnitDirection);
	
	sf::Clock m_SnowballTimer;

	bool m_bCanFallDamage = true;

	std::list<CProjectile> m_Projectiles;

	float m_DetectionRange = 1400.0f;
};
#endif

