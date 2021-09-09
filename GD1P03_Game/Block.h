#pragma once
#ifndef _Block_H__
#define _Block_H__

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include <cmath>
#include <list>



/// <summary>
/// // 100 x 100 = STANDARD BLOCK SIZING
/// </summary>

class CBlock
{
public:
	enum BLOCKTYPE
	{
		PICKAXE = 0,
		DOOR,
		CHEST,
		DIRT,
		STONE,
		WOOD,
		PLANKS,
		SAND,
		MOSSYBRICK,
		GRASS,
		BLOCK,
		LEAVES,
		FURNACE,
		IRON,
		GOLD,
		DIAMOND,
		COAL
	};

	bool MARKASDESTROY = false;

	CBlock();
	CBlock(sf::Texture* _texture, BLOCKTYPE _type = BLOCK);
	CBlock(sf::RenderWindow* _renderWindow, b2World& _world, const sf::Texture* _texture, const float& _scale, float _posX, float _posY, BLOCKTYPE _type = BLOCK);
	virtual ~CBlock();

	void Start();
	virtual void Update();
	void Render();

	void SetPosition(float _x, float _y);
	sf::Vector2f GetPosition();

	virtual void SetSize(float _x, float _y);
	sf::Vector2f GetSize();

	sf::Sprite GetShape();
	b2Body* GetBody()
	{
		return m_Body;
	}

	b2Body* m_Body;

	BLOCKTYPE m_Type;

	long int m_ArrayIndex = -1;
	int m_PositionInInventory = -1;
	bool m_bIsMovingItemInInv = false;

	bool m_bIsItemAndSelected = false;

	int m_BlockStrength = 3;
	
	
protected:
	sf::RenderWindow* m_RenderWindow;

	b2World* m_World;

	sf::Sprite m_Shape;

	sf::Texture m_Texture;

	b2BodyDef m_BodyDef;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;

	float m_Scale;
	sf::Vector2f m_Size = sf::Vector2f(100, 100);

	

	
};
#endif

