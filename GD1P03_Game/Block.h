#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class CBlock
{
public:
	CBlock(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY);
	~CBlock();

	void Start();
	void Update(b2Body* _bodyIterator);
	void Render();

	void SetPosition(int _x, int _y);

	void SetSize(float _x, float _y);
	sf::Vector2f GetSize();

	sf::RectangleShape GetShape();

private:
	sf::RenderWindow* m_RenderWindow;

	b2World* m_World;

	sf::RectangleShape m_Shape;
	b2Body* m_Body;
	b2BodyDef m_BodyDef;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;

	float m_Scale;
	sf::Vector2f m_Size = sf::Vector2f(100, 100);
};

