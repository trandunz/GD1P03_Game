#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <iostream>

class CPlayer
{
public:
	CPlayer(sf::RenderWindow* _renderWindow, b2World& m_World, const float& _scale);
	~CPlayer();

	void Start();
	void Update();
	void Render();

	sf::RectangleShape GetShape();

	void Movement(sf::Event& _event);
	 
private:
	sf::RenderWindow* m_RenderWindow;
	b2World* m_World;

	b2Vec2 m_Velocity;

	sf::RectangleShape m_Shape;
	sf::Texture m_Texture;

	b2BodyDef m_BodyDef;
	b2Body* m_Body;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;

	float m_Scale;
	const int m_MoveSpeed = 3;
};

