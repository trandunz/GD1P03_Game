#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <iostream>
#include <list>
#include "Block.h"

class CPlayer
{
public:
	CPlayer(sf::RenderWindow* _renderWindow, b2World& m_World, const float& _scale);
	~CPlayer();

	void Start();
	void Update();
	void Render();

	sf::Sprite GetShape();

	void Movement(sf::Event& _event);
	void PlaceBlocks(std::list<CBlock>& m_Chunk, sf::Event& _event);
	b2Body* GetBody()
	{
		return m_Body;
	}
	 
private:
	sf::RenderWindow* m_RenderWindow;
	b2World* m_World;

	// Mouse
	sf::Vector2f MousePos;

	b2Vec2 m_Velocity;

	sf::Sprite m_Shape;
	sf::Texture* m_PlayerRightTex;
	sf::Texture* m_PlayerLeftTex;

	b2BodyDef m_BodyDef;
	b2Body* m_Body;
	
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;

	float m_Scale;
	const int m_MoveSpeed = 3;

	CBlock* m_Block;

	b2Filter m_PlayerFilter;
};

