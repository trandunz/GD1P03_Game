#pragma once
#ifndef _PLAYER_H__
#define _PLAYER_H__

#define _PLAYER_FILTER_ 0x0002;
#define _ENEMY_FILTER_ 0x0004;
#define _WORLD_FILTER_ 0x0006;

#define _PLAYER_GROUPINDEX_ 1;
#define _PROJECTILE_GROUPINDEX_ 2;
#define _ENEMY_GROUPINDEX_ 3;
#define _WORLD_GROUPINDEX_ 4;

#include <vector>
#include <fstream>

#include "Chest.h"
#include "Door.h"
#include "AudioManager.h"
#include "Pickaxe.h"
#include "CTextureMaster.h"
#include "CFurnace.h"
#include "CProjectile.h"
#include "Bow.h"
#include "CParticleSystem.h"
#include "CPotion.h"
#include "CWorkBench.h"

class CPlayer
{
public:
	CPlayer(sf::RenderWindow* _renderWindow, b2World& m_World, const float& _scale, CAudioManager* _audioManager, CTextureMaster* _textureMaster);
	~CPlayer();

	void Start();
	void Update(sf::Vector2f _mousePos);
	void Render(sf::Shader* _defaultShader = NULL);

	void Movement();
	void Movement(sf::Event& _event);

	void Interact(std::list<CFurnace>& m_Furnaces, std::list<CChest>& m_Chests, std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite, std::list<CWorkBench>& m_WorkBenches);
	
	void Attack();

	void InitInventory();

	b2Body* GetBody();

	CPickaxe* GetPickaxe();
	void SetPickaxe(CPickaxe* _pickaxe);

	Bow* GetBow();
	void SetBow(Bow* _bow);

	int GetCurrentHP();
	int GetMaxHP();
	void TakeDamage(float _damage, bool _fallDamage = false);
	void Heal(float _amount);

	sf::Sprite& GetShape();

	void SetCurrentHP(int _amount);
	void SetMaxHP(int _amount);
	void SetMousePos(sf::Vector2f _position);

	bool bCanMove();
	void bCanMove(bool _bcanMove);
	bool bCanPlace();
	void bCanPlace(bool _bcanPlace);
	bool bInventoryOpen();
	void bInventoryOpen(bool _bInventoryOpen);

	void ResetSpritePos();

	template <class T>
	bool bMouseNotOver(std::list<T>& m_Chunk, sf::Sprite& _mousePositionSprite);
	bool bMouseOverIventoryItem(std::map<int, CBlock>& m_Inventory, sf::Sprite& _mousePositionSprite);
	
	bool IsBlockInInventory(CBlock* _block);

	bool IsItemInventory(CBlock::BLOCKTYPE _type);
	int	GetPositionInInventory(CBlock::BLOCKTYPE _type);
	int IsItemInventory(CBlock::BLOCKTYPE _type, bool _bReturnAmount);

	void AddItemToInventory(CBlock* _block, int _position, bool _canStack = true);
	void AddItemToInventory(CBlock* _block, bool _canStack = true);
	void RemoveItemFromInventory(int _position);

	void ToggleInventoryUI(std::list<CChest>& _chests);

	bool SelectedItemIsEmpty();

	template <typename T>
	int Mine(std::list<T>& m_Chunk, sf::Sprite& _mousePositionSprite);

	void PlaceBlock(std::list<CBlock>& m_Chunk, sf::Sprite& _mousePositionSprite);
	void PlaceDoor(std::list<CDoor>& m_Doors, sf::Sprite& _mousePositionSprite);
	void PlaceChest(std::list<CChest>& m_Chests, sf::Sprite& _mousePositionSprite);
	void PlaceFurnace(std::list<CFurnace>& m_Chests, sf::Sprite& _mousePositionSprite);
	void PlaceWorkBench(std::list<CWorkBench>& m_WorkBenches, sf::Sprite& _mousePositionSprite);

	void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);
	void DestroyBody();

	void Lst_MoveToFront(std::list<CBlock>& list, std::list<CBlock>::iterator element);
	void Lst_MoveToFront(std::list<CDoor>& list, std::list<CDoor>::iterator element);
	void Lst_MoveToFront(std::list<CChest>& list, std::list<CChest>::iterator element);

	void OutPutInventoryToFile();
	void InputInventoryToFile();

	b2World* GetWorld();


	std::map<int, int> m_InventoryStackValues;
	std::map<int, CBlock> m_InventoryMap;

	sf::Sprite m_MapIcon;

	int m_CurrentItemIndex = 0;

	// Textures
	sf::Texture* m_PlayerRightTex;
	sf::Texture* m_PlayerLeftTex;

	CPickaxe* m_Pickaxe;
	Bow* m_Bow;

	// Box2d
	b2BodyDef m_BodyDef;
	b2Body* m_Body;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;

	CAudioManager* m_AudioManager;

	CParticleSystem* m_TestParticles;

	bool m_bInventoryOpen = false;
	bool m_bPlayerIsInChest = false;


	// Mouse
	sf::Vector2f m_MousePos;

	sf::Clock m_HPPotionTimer;
private:
	

	sf::Sprite m_Shape;

	// Player
	b2Vec2 m_Velocity;
	const int m_MoveSpeed = 30;
	int m_MaxHP = 100;
	int m_Health = 100;
	float m_HeathRegenSpeed = 1;
	float m_HeathRegenAmount = 1;
	int m_InteractionRange = 10;
	float m_AttackSpeed = 0.7f;

	bool m_bCanPlace = true;
	
	bool m_bCanMove = true;
	bool m_bCanJump = false;
	bool m_bCanFallDamage = true;

	// Essentials
	sf::RenderWindow* m_RenderWindow;
	b2World* m_World;
	float m_Scale;
	sf::Clock* m_AnimationTimer;
	sf::Clock* m_MineTimer;
	sf::Clock* m_DamageTimer;
	sf::Clock* m_DamageIndicatorTimer;
	CTextureMaster* m_TextureMaster;
	int m_InventorySize = -1;

	// Textures
	sf::Texture* m_MapIconTex;
	sf::Texture* m_MapIconTexRight;

	// Temp Block Pointer (Used To Create New Blocks During Block Placement)
	CBlock* m_Block;

	// Temp Door Pointer
	CDoor* m_Door;

	CChest* m_Chest;

	CFurnace* m_Furnace;

	CWorkBench* m_WorkBench;

	CProjectile* m_Projectile;
	CPotion* m_Potion = nullptr;

	std::list<CProjectile> m_Projectiles;

	sf::Clock m_AttackTimer;

	sf::Clock m_ParticleTimer;

	sf::Clock m_HealthRegenTimer;

};
#endif

