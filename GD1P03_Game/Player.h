#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class CPlayer
{
public:
	CPlayer(sf::RenderWindow* _renderWindow, b2World& m_World, const float& _scale);
	~CPlayer();

	void Start();
	void Update(b2Body* _bodyIterator);
	void Render();

	sf::RectangleShape GetShape();

	void Movement();

private:
	sf::RenderWindow* m_RenderWindow;

	sf::RectangleShape m_Shape;
	b2BodyDef m_BodyDef;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;

	float m_Scale;
};

