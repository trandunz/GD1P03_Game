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
		SLIME
	};

	CEnemy();
	virtual ~CEnemy();

	virtual void Start();
	virtual void Update();
	virtual void Render();

	void SetHealth(int _value);
	int GetHealth();

	sf::Sprite GetShape();

	virtual void Movement();
	virtual void Attack();

	virtual void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);
	void DestroyBody();

	ENEMYTYPE m_Type = ENEMYTYPE::DEFAULT;

	bool m_MARKASDESTORY = false;

protected:
	// Essentials
	CAudioManager* m_AudioManager;
	sf::RenderWindow* m_RenderWindow;
	CTextureMaster* m_TextureMaster;
	b2World* m_World;
	float m_Scale = 50.0f;
	sf::Sprite m_Shape;

	int m_Health = 100;
	int m_MaxHealth = 100;
	int m_MoveSpeed = 50;
	float m_AttackSpeed = 1.0f;

	// Box2d
	b2BodyDef m_BodyDef;
	b2Body* m_Body;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;

	sf::Texture* m_Texture;
};
#endif

