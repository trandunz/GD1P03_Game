#pragma once
#ifndef _PLAYER_H__
#define _PLAYER_H__

#include <vector>

#include "Chest.h"
#include "Door.h"
#include "AudioManager.h"
#include "Pickaxe.h"
#include "CTextureMaster.h"
#include "CFurnace.h"


class CPlayer
{
public:
	CPlayer(sf::RenderWindow* _renderWindow, b2World& m_World, const float& _scale, CAudioManager* _audioManager, CTextureMaster* _textureMaster);
	~CPlayer();

	void Start();
	void Update(sf::Vector2f _mousePos, sf::Event& _event);
	void Render();

	void Movement(sf::Event& _event);
	void ResetSpritePos();
	
	void Interact(std::list<CFurnace>& m_Furnaces, std::list<CChest>& m_Chests, std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite);
	
	b2Body* GetBody();
	sf::Sprite GetShape();
	
	int GetCurrentHP();
	int GetMaxHP();

	bool bMouseNotOverBlock(std::list<CBlock>& m_Chunk, sf::Sprite& _mousePositionSprite);
	bool bMouseNotOverDoor(std::list<CDoor>& m_Doors, sf::Sprite& _mousePositionSprite);
	bool bMouseNotOverChest(std::list<CChest>& m_Chests, sf::Sprite& _mousePositionSprite);
	bool bMouseNotOverFurnace(std::list<CFurnace>& m_Furnaces, sf::Sprite& _mousePositionSprite);

	bool bMouseOverIventoryItem(std::map<int, CBlock>& m_Inventory, sf::Sprite& _mousePositionSprite);

	void Lst_MoveToFront(std::list<CBlock>& list, std::list<CBlock>::iterator element);

	void AddItemToInventory(CBlock* _block);
	void AddItemToInventory(CDoor* _door);
	void RemoveItemFromInventory(int _position);

	void ToggleInventoryUI();

	bool SelectedItemIsEmpty();

	bool IsBlockInInventory(CBlock* _block);
	bool IsDoorInInventory(CDoor* _door);

	void Lst_MoveToFront(std::list<CDoor>& list, std::list<CDoor>::iterator element);
	void Lst_MoveToFront(std::list<CChest>& list, std::list<CChest>::iterator element);

	void Mine(std::list<CBlock>& m_Chunk, std::list<CBlock>::iterator _block);
	void MineDoor(std::list<CDoor>& m_Doors, std::list<CDoor>::iterator _door);
	void MineChest(std::list<CChest>& m_Chests, std::list<CChest>::iterator _chest);
	void PlaceBlock(std::list<CBlock>& m_Chunk, sf::Sprite& _mousePositionSprite);
	void PlaceDoor(std::list<CDoor>& m_Doors, sf::Sprite& _mousePositionSprite);
	void PlaceChest(std::list<CChest>& m_Chests, sf::Sprite& _mousePositionSprite);
	void PlaceFurnace(std::list<CFurnace>& m_Chests, sf::Sprite& _mousePositionSprite);

	void MineFurnace(std::list<CFurnace>& m_Furnaces, std::list<CFurnace>::iterator _furnace);

	std::map<int, int> m_InventoryStackValues;

	sf::Sprite m_MapIcon;

	// Mouse
	sf::Vector2f m_MousePos;

	bool m_bCanPlace = true;

	std::map<int, CBlock> m_InventoryMap;

	int m_CurrentItemIndex = 0;

	bool m_bInventoryOpen = false;

	bool m_bCanMove = true;

	// Textures
	sf::Texture* m_PlayerRightTex;
	sf::Texture* m_PlayerLeftTex;

	int m_Health = 100;

	CPickaxe* m_Pickaxe;

	// Box2d
	b2BodyDef m_BodyDef;
	b2Body* m_Body;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;
	b2Filter m_PlayerFilter;
private:
	// Essentials
	sf::RenderWindow* m_RenderWindow;
	b2World* m_World;
	float m_Scale;
	CAudioManager* m_AudioManager;
	sf::Clock* m_AnimationTimer;
	sf::Clock* m_MineTimer;
	CTextureMaster* m_TextureMaster;
	int m_InventorySize = -1;

	// Player
	int m_MaxHP = 100;
	int m_HPRegenRate_s = 1;

	int m_InteractionRange = 10;


	
	const int m_MoveSpeed = 50;
	
	b2Vec2 m_Velocity;

	// Textures
	sf::Sprite m_Shape;
	sf::Texture* m_MapIconTex;
	sf::Texture* m_MapIconTexRight;

	

	// Temp Block Pointer (Used To Create New Blocks During Block Placement)
	CBlock* m_Block;

	// Temp Door Pointer
	CDoor* m_Door;

	

	CChest* m_Chest;

	CFurnace* m_Furnace;

	
};
#endif

