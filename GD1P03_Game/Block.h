#pragma once
#ifndef _BLOCK_H__
#define _BLOCK_H__

#define _PLAYER_FILTER_ 0x0002
#define _ENEMY_FILTER_ 0x0004
#define _WORLD_FILTER_ 0x0006
#define _PROJECTILE_FILTER_ 0x0008

#define _PLAYER_GROUPINDEX_ 1
#define _PROJECTILE_GROUPINDEX_ 2
#define _ENEMY_GROUPINDEX_ 3
#define _WORLD_GROUPINDEX_ 4

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
		DOOR = 1,
		CHEST = 2,
		DIRT = 3,
		STONE = 4,
		WOOD = 5,
		PLANKS = 6,
		SAND = 7,
		MOSSYBRICK = 8,
		GRASS = 9,
		BLOCK = 10,
		LEAVES = 11,
		FURNACE = 12,
		IRONORE = 13,
		GOLDORE = 14,
		DIAMONDORE = 15,
		COALORE = 16,
		OBSIDIAN = 17,
		BOW = 18,
		IRONINGOT = 19,
		GOLDINGOT = 20,
		DIAMOND = 21,
		CLOUD = 22,
		WORKBENCH = 23,
		PURPLEINGOT = 24,
		GOLDENINGOT = 25,
		ANVIL = 26,
		TOURCH = 27,
		PURPLEORE = 28,
		GOLDENORE = 29,
		PROJECTILE = 30,
		POTION = 31,
		EMPTYBEAKER = 32,
		REDSLIME = 33,
		SWORD = 34,
		BROKENSWORD = 35,
	};

	enum class PICKAXETYPE
	{
		BASIC = 0,
		IRON,
		GOLD,
		DIAMOND,
		OBSIDIAN,
		PURPLE,
		GOLDEN
	};

	enum class BOWTYPE
	{
		BASIC = 0,
		OBSIDIAN,
		ICE,
		BLOOD,
		IRON,
		GOLDEN,
		PURPLE,
		GREEN,
		
		IRONGUN,
		GOLDGUN,
		PURPLEGUN,
		GOLDENGUN,
		GREENGUN,
	};

	enum class PROJECTILETYPE
	{
		ARROW = 0,
		FIREARROW,
		CURSEDARROW,
		POISONARROW,

		IRONBULLET,
		GOLDBULLET,
		PURPLEBULLET,
		GOLDENBULLET,
	};

	enum class POTIONTYPE
	{
		HPSMALL = 0,
		HPLARGE,
	};

	enum class SWORDTYPE
	{
		WOOD = 0,
		ANCIENT,
		FLAME,
		GOD,
		GOLDEN,
		GREEN,
		PURPLE,
		SLIME,
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

	virtual void DestroyBody();
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
	BOWTYPE m_BowType = BOWTYPE::BASIC;
	PROJECTILETYPE m_ProjectileType = PROJECTILETYPE::ARROW;
	POTIONTYPE m_PotionType = POTIONTYPE::HPSMALL;
	SWORDTYPE m_SwordType = SWORDTYPE::WOOD;
protected:
	sf::RenderWindow* m_RenderWindow;

	b2World* m_World;

	sf::Sprite m_Shape;

	float m_Scale;
	sf::Vector2f m_Size = sf::Vector2f(100, 100);
};
#endif

