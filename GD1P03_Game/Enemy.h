//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CEnemy.h
// Description : CEnemy Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once
#ifndef _ENEMY_H__
#define _ENEMY_H__

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "CTextureMaster.h"
#include "Player.h"

class CEnemy
{
public:
	enum class ENEMYTYPE
	{
		DEFAULT = 0,
		ZOMBIE,
		SLIME,
		NPC,
		SNOWMAN,
		CACTUS
	};

	CEnemy();
	virtual ~CEnemy();

	virtual void Start() = 0;
	virtual void Update();
	virtual void Render(sf::Shader* _shader = NULL);

	void SetHealth(int _value);
	int GetHealth();

	void SetPlayer(CPlayer* _player);

	sf::Sprite GetShape();

	virtual void Melee(int DirectionToPlayer);

	ENEMYTYPE m_Type = ENEMYTYPE::DEFAULT;

	bool m_MARKASDESTORY = false;
protected:
	virtual void Movement() = 0;
	virtual void Attack() = 0;

	virtual void TakeDamage(float _damage, bool _projectile = false);

	virtual void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);
	void DestroyBody();

	void SetSFShapeToBody();

	float CalculateMag(float _x1, float _x2, float _y1, float _y2);

	virtual void WorldContacts();

	// Essentials
	CAudioManager* m_AudioManager;
	sf::RenderWindow* m_RenderWindow;
	CTextureMaster* m_TextureMaster;
	b2World* m_World;
	float m_Scale = 50.0f;
	sf::Sprite m_Shape;
	CPlayer* m_Player;
	sf::Clock m_AttackTimer;

	int m_Health = 100;
	int m_MaxHealth = 100;
	int m_MoveSpeed = 50;
	float m_AttackSpeed = 0.5f;

	// Box2d
	b2BodyDef m_BodyDef;
	b2Body* m_Body;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;

	sf::Texture* m_Texture;

	sf::Clock m_DamageTimer;

	float m_Damage = 12.25f;
	float m_AttackRange = 140;
};
#endif

