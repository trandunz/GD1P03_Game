#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>

class CProjectile
{
public:

	CProjectile(b2World& _world, float _startPosX, float _startPosY, sf::Vector2f _mousPos);
	~CProjectile();

	void Update();
	void Render();

	void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);
	void DestroyBody();

	bool m_bMARKASDESTROY = false;

	float m_Weight = 0.8f;

	sf::Sprite m_Shape;
private:
	sf::Texture* m_Texture;
	b2World* m_World;
	

	// Box2d
	b2BodyDef m_BodyDef;
	b2Body* m_Body;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;

};

