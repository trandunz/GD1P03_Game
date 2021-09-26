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
	enum class BLOCKTYPE
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
		IRONORE,
		GOLDORE,
		DIAMONDORE,
		COALORE,
		OBSIDIAN,
		BOW,
		IRONINGOT,
		GOLDINGOT,
		DIAMOND,
		CLOUD,
		WORKBENCH,
		PURPLEINGOT,
		GOLDENINGOT,
		ANVIL,
		TOURCH,
		PURPLEORE,
		GOLDENORE,
	};

	enum class PICKAXETYPE
	{
		BASIC,
		IRON,
		GOLD,
		DIAMOND,
		OBSIDIAN,
		PURPLE,
		GOLDEN
	};

	enum class BOWTYPE
	{
		DEFAULT,
		OBSIDIAN,
		ICE,
		BLOOD,
		IRON,
		GOLDEN,
		PURPLE,
		GREEN,
	};

	CBlock();
	CBlock(sf::Texture* _texture, BLOCKTYPE _type = BLOCKTYPE::BLOCK);
	CBlock(sf::RenderWindow* _renderWindow, b2World& _world, const sf::Texture* _texture, const float& _scale, float _posX, float _posY, bool _bWallpaper, BLOCKTYPE _type = BLOCKTYPE::BLOCK);
	virtual ~CBlock();

	void Start();
	virtual void Update();
	virtual void Render(sf::Shader* _shader = NULL);

	void SetPosition(float _x, float _y);
	sf::Vector2f GetPosition();

	virtual void SetSizeAndPos(float _currentPosX, float _currentPosY, float _x, float _y);
	sf::Vector2f GetSize();

	sf::Sprite& GetShape();
	b2Body* GetBody()
	{
		return m_Body;
	}

	void DestroyBody();
	virtual void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);

	float GetHeight(float x) 
	{
		return 0;//1 - cosf( x * 0.1f );
	}

	b2Body* m_Body;
	b2BodyDef m_BodyDef;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;

	BLOCKTYPE m_Type;


	bool MARKASDESTROY = false;

	long int m_ArrayIndex = -1;
	int m_PositionInInventory = -1;
	bool m_bIsMovingItemInInv = false;

	bool m_bIsItemAndSelected = false;

	float m_BlockStrength = 3;
	
	sf::Texture* m_Texture;

	bool m_bHasBeenSpotted = false;

	bool m_bCanCraft = false;

	PICKAXETYPE m_PickType = PICKAXETYPE::BASIC;
	BOWTYPE m_BowType = BOWTYPE::DEFAULT;
protected:
	sf::RenderWindow* m_RenderWindow;

	b2World* m_World;

	sf::Sprite m_Shape;

	float m_Scale;
	sf::Vector2f m_Size = sf::Vector2f(100, 100);
};
#endif

