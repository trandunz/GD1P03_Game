#pragma once
#ifndef _PLAYER_H__
#define _PLAYER_H__

#include <vector>

#include "Chest.h"
#include "Door.h"


class CPlayer
{
public:
	CPlayer(sf::RenderWindow* _renderWindow, b2World& m_World, const float& _scale);
	~CPlayer();

	void Start();
	void Update(sf::Vector2f _mousePos);
	void Render();

	void Movement(sf::Event& _event);
	void ResetSpritePos();
	
	void PlaceBlocks(std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite, sf::Texture* _texture);
	
	b2Body* GetBody();
	sf::Sprite GetShape();
	
	int GetCurrentHP();
	int GetMaxHP();

	bool bMouseNotOverBlock(std::list<CBlock>& m_Chunk, sf::Sprite& _mousePositionSprite);
	bool bMouseNotOverDoor(std::list<CDoor>& m_Doors, sf::Sprite& _mousePositionSprite);

	void Lst_MoveToFront(std::list<CBlock>& list, std::list<CBlock>::iterator element);

	void AddItemToInventory(CBlock* _block);
	void AddItemToInventory(CDoor _door);
	void RemoveItemToInventory();

	void ToggleInventoryUI();
	 
	std::map<int, CBlock> m_Inventory;

	sf::Sprite m_MapIcon;

	// Mouse
	sf::Vector2f m_MousePos;

	bool m_bCanPlace = true;

	std::map<int, CBlock> m_InventoryMap;

	bool m_bInventoryOpen = false;

private:
	// Essentials
	sf::RenderWindow* m_RenderWindow;
	b2World* m_World;
	float m_Scale;

	// Player
	int m_MaxHP = 100;
	int m_HPRegenRate_s = 1;

	int m_InteractionRange = 10;


	int m_Health = 100;
	const int m_MoveSpeed = 3;
	
	b2Vec2 m_Velocity;

	// Textures
	sf::Sprite m_Shape;
	sf::Texture* m_PlayerRightTex;
	sf::Texture* m_PlayerLeftTex;
	sf::Texture* m_MapIconTex;
	sf::Texture* m_MapIconTexRight;

	// Box2d
	b2BodyDef m_BodyDef;
	b2Body* m_Body;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;
	b2Filter m_PlayerFilter;

	// Temp Block Pointer (Used To Create New Blocks During Block Placement)
	CBlock* m_Block;

	// Temp Door Pointer
	CDoor* m_Door;

	
};
#endif

