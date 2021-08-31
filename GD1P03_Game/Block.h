#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

/// <summary>
/// // 100 x 100 = STANDARD BLOCK SIZING
/// </summary>

class CBlock
{
public:
	bool MARKASDESTROY = false;

	CBlock(sf::RenderWindow* _renderWindow, b2World& _world, sf::Texture* _texture, const float& _scale, float _posX, float _posY);
	~CBlock();

	void Start();
	void Update();
	void Render();

	void SetPosition(int _x, int _y);

	void SetSize(float _x, float _y);
	sf::Vector2f GetSize();

	sf::Sprite GetShape();
	b2Body* GetBody()
	{
		return m_Body;
	}
	b2Fixture* GetFixture()
	{
		return m_Fixture;
	}

private:
	sf::RenderWindow* m_RenderWindow;

	b2World* m_World;

	sf::Sprite m_Shape;
	sf::Texture m_Texture;

	b2Body* m_Body;
	b2Fixture* m_Fixture;
	b2BodyDef m_BodyDef;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;

	float m_Scale;
	sf::Vector2f m_Size = sf::Vector2f(100, 100);

	
};

