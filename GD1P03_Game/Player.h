//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CPlayer.h
// Description : CPlayer Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once
#ifndef _PLAYER_H__
#define _PLAYER_H__

// #Defines
#define _PLAYER_FILTER_ 0x0002
#define _ENEMY_FILTER_ 0x0004
#define _WORLD_FILTER_ 0x0006
#define _PROJECTILE_FILTER_ 0x0008

#define _PLAYER_GROUPINDEX_ 1
#define _PROJECTILE_GROUPINDEX_ 2
#define _ENEMY_GROUPINDEX_ 3
#define _WORLD_GROUPINDEX_ 4

// Scene Values
#define _PLAINS_ 0
#define _ICE_ 1
#define _SAND_ 2 
#define _HELL_ 3

// Non-Local Includes
#include <vector>
#include <fstream>

// Local Includes
#include "CPickaxe.h"
#include "Chest.h"
#include "Door.h"
#include "AudioManager.h"
#include "CTextureMaster.h"
#include "CFurnace.h"
#include "CProjectile.h"
#include "Bow.h"
#include "CParticleSystem.h"
#include "CPotion.h"
#include "CWorkBench.h"
#include "CArmour.h"

class CPlayer
{
public:
	CPlayer(sf::RenderWindow* _renderWindow, b2World& m_World, const float& _scale, CAudioManager* _audioManager, CTextureMaster* _textureMaster, bool* _changeScenes, int* _sceneValue);
	~CPlayer();

	void Start();
	void Update(sf::Vector2f _mousePos, bool& _playerHitByProjectile);
	void Render(sf::Shader* _defaultShader = NULL);

	void Movement();
	void Movement(sf::Event& _event);

	void Interact(std::list<CFurnace>& m_Furnaces, std::list<CChest>& m_Chests, std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite, std::list<CWorkBench>& m_WorkBenches, std::list<CBlock>& m_Tourches);

	b2Body* GetBody();
	sf::Sprite& GetShape();

	CPickaxe* GetPickaxe();
	void SetPickaxe(CPickaxe* _pickaxe);

	Bow* GetBow();
	void SetBow(Bow* _bow);

	int GetCurrentHP();
	int GetMaxHP();

	void TakeDamage(float _damage);
	void Heal(float _amount);

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
	bool IsItemInInventory(CBlock::BLOCKTYPE _type);
	bool SelectedItemIsEmpty();

	int	GetPositionInInventory(CBlock::BLOCKTYPE _type);
	int IsItemInInventory(CBlock::BLOCKTYPE _type, bool _bReturnAmount);

	void AddItemToInventory(CBlock* _block, int _position, bool _canStack = true);
	void AddItemToInventory(CBlock* _block, bool _canStack = true);
	void RemoveItemFromInventory(int _position);

	void ToggleInventoryUI(std::list<CChest>& _chests);

	void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);
	void DestroyBody();

	void Lst_MoveToFront(std::list<CBlock>& list, std::list<CBlock>::iterator element);
	void Lst_MoveToFront(std::list<CDoor>& list, std::list<CDoor>::iterator element);
	void Lst_MoveToFront(std::list<CChest>& list, std::list<CChest>::iterator element);

	void ToggleGodMode();
	bool GetGodMode();

	b2World* GetWorld();

	void OutPutInventoryToFile();

	void SetArmour(int _value);
	int GetArmour();

	std::map<int, int> m_InventoryStackValues;
	std::map<int, CBlock> m_InventoryMap;

	sf::Sprite m_MapIcon;

	// Textures
	sf::Texture* m_PlayerRightTex;
	sf::Texture* m_PlayerLeftTex;

	// Mouse
	sf::Vector2f m_MousePos;

	sf::Clock m_HPPotionTimer;

	int m_InventorySize = -1;
	int m_CurrentItemIndex = 0;

	bool m_bInventoryOpen = false;
	bool m_bPlayerIsInChest = false;

	sf::Sprite m_HelmetShape;
	sf::Sprite m_ChesPlateShape;
	sf::Sprite m_LegsShape;

	float m_LightIntensity = 1.0f;


private:

	void Attack(CBlock* _item);

	void InitInventory();

	void InputInventoryToFile();

	template <typename T>
	void Mine(std::list<T>& m_Chunk, sf::Sprite& _mousePositionSprite);

	void PlaceBlock(std::list<CBlock>& m_Chunk, sf::Sprite& _mousePositionSprite);
	void PlaceDoor(std::list<CDoor>& m_Doors, sf::Sprite& _mousePositionSprite);
	void PlaceChest(std::list<CChest>& m_Chests, sf::Sprite& _mousePositionSprite);
	void PlaceFurnace(std::list<CFurnace>& m_Chests, sf::Sprite& _mousePositionSprite);
	void PlaceWorkBench(std::list<CWorkBench>& m_WorkBenches, sf::Sprite& _mousePositionSprite);
	void PlaceTourch(std::list<CBlock>& m_Tourches, sf::Sprite& _mousePositionSprite);
	void PlaceAnvil(std::list<CWorkBench>& m_WorkBenches, sf::Sprite& _mousePositionSprite);

	void CalculateAndAddPicktypes(int _array[60], int _iterator);
	void CalculateAndAddBowtypes(int _array[60], int _iterator);
	void CalculateAndAddProjectileTypes(int _array[60], int _iterator);
	void CalculateAndAddPotionTypes(int _array[60], int _iterator);
	void CalculateAndAddHelmetTypes(int _array[60], int _iterator);
	void CalculateAndAddChestPlateTypes(int _array[60], int _iterator);
	void CalculateAndAddLeggingsTypes(int _array[60], int _iterator);
	void CalculateAndAddWorkBenchTypes(int _array[60], int _iterator);

	void GrabAllSavedValues(int types[60], int stackvalues[60], int bowtypes[60], int swordtypes[60], int picktypes[60], int projtypes[60], int potiontypes[60], int armourtypes[60], int workbenchtypes[60]);

	void CheckForHoldableItem();
	void UpdateProjectiles(bool m_PlayerHitByProjectile = false);
	void CheckForDestroyedProjectiles();
	void HPRegen();

	void SetArmourPosititions();

	void PlayerCollisionContacts();

	void CheckForBasicPickaxe();

	void CleanupLists();

	void DeleteAndReleaseAllPointers();

	void LoadPlayerTextures();
	void InitShape();

	void InitMapIcon();

	void RenderHoldables();
	void DrawProjectiles(sf::Shader* _defaultShader = NULL);
	void DrawArmour(sf::Shader* _defaultShader = NULL);

	void SADInput(int& _x, int& _y);

	void AnimationAndAudio(const b2Vec2 _worldPosition);

	float CalculateMag(float _x1, float _x2, float _y1, float _y2);

	void PickaxeInteraction(std::list<CFurnace>& m_Furnaces, std::list<CChest>& m_Chests, std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite, std::list<CWorkBench>& m_WorkBenches, std::list<CBlock>& m_Tourches);

	void PotionInteraction();

	void EmptySpaceInteraction(std::list<CFurnace>& m_Furnaces, std::list<CChest>& m_Chests, std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite, std::list<CWorkBench>& m_WorkBenches, std::list<CBlock>& m_Tourches);

	void PortalKeyInteraction(int _sceneValue);

	void OpenChest(std::list<CFurnace>& m_Furnaces, std::list<CChest>& m_Chests, std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite, std::list<CWorkBench>& m_WorkBenches, std::list<CBlock>& m_Tourches);
	void OpenFurnace(std::list<CFurnace>& m_Furnaces, std::list<CChest>& m_Chests, std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite, std::list<CWorkBench>& m_WorkBenches, std::list<CBlock>& m_Tourches);
	void OpenWorkBench(std::list<CFurnace>& m_Furnaces, std::list<CChest>& m_Chests, std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite, std::list<CWorkBench>& m_WorkBenches, std::list<CBlock>& m_Tourches);

	void GunAttackSpeedCheck(CBlock* _item);

	void BowAndGunAttack(CBlock* _item);
	void FireBullet(int _positionInInventory);
	void FireArrow(int _positionInInventory);

	void SetShapePositionToBody();

	void DoorOpeningAndClosing(std::list<CFurnace>& m_Furnaces, std::list<CChest>& m_Chests, std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite, std::list<CWorkBench>& m_WorkBenches, std::list<CBlock>& m_Tourches);

	void InsertNewItemToInventory(CBlock* _block, int _position, bool _canStack);
	void InsertNewItemToInventory(CBlock* _block, bool _canStack);

	// Essentials
	sf::RenderWindow* m_RenderWindow;
	b2World* m_World;
	float m_Scale;
	sf::Clock* m_AnimationTimer;
	sf::Clock* m_MineTimer;
	sf::Clock* m_DamageTimer;
	sf::Clock* m_DamageIndicatorTimer;
	CTextureMaster* m_TextureMaster;
	CAudioManager* m_AudioManager;
	bool* m_bChangeScenes;
	int* m_SceneValue;

	// Player
	sf::Sprite m_Shape;
	b2Vec2 m_Velocity;

	int m_MoveSpeed = 30;
	int m_JumpMulti = 1.8;
	int m_MaxHP = 100;
	int m_Health = 100;
	int m_InteractionRange = 10;
	int m_Armour = 0;

	float m_HeathRegenSpeed = 1;
	float m_HeathRegenAmount = 1;
	float m_AttackSpeed = 0.7f;
	
	bool m_bGodMode = false;
	bool m_bCanPlace = true;
	bool m_bCanMove = true;
	bool m_bCanJump = false;
	bool m_bCanFallDamage = true;

	// Textures
	sf::Texture* m_MapIconTex;
	sf::Texture* m_MapIconTexRight;

	// Temp Pointers
	CBlock* m_Block;
	CDoor* m_Door;
	CChest* m_Chest;
	CFurnace* m_Furnace;
	CWorkBench* m_WorkBench;
	CPickaxe* m_Pickaxe;
	Bow* m_Bow;
	CParticleSystem* m_MineParticles;
	CProjectile* m_Projectile;
	CPotion* m_Potion;

	// Projectiles And Attacks
	std::list<CProjectile> m_Projectiles;
	sf::Clock m_AttackTimer;
	sf::Clock m_ParticleTimer;
	sf::Clock m_HealthRegenTimer;

	// Box2d
	b2BodyDef m_BodyDef;
	b2Body* m_Body;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;
};
#endif

