#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "CTextureMaster.h"
class CEnemy
{
public:
	CEnemy();
	virtual ~CEnemy();

	virtual void Start();
	virtual void Update();
	virtual void Render();

	virtual void Movement();

protected:
	// Essentials
	sf::RenderWindow* m_RenderWindow;
	CTextureMaster* m_TextureMaster;
	b2World* m_World;
	float m_Scale = 50.0f;
	sf::Sprite m_Shape;

	int m_Health = 100;
	int m_MaxHealth = 100;
	int m_MoveSpeed = 50;

	// Box2d
	b2BodyDef m_BodyDef;
	b2Body* m_Body;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;
	


};

