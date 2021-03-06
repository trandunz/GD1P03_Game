//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CWorldManager.h
// Description : CWorldManager Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once
#ifndef _WORLDMANAGER_H__
#define _WORLDMANAGER_H__

#define _NOISEWIDTH_ 366
#define _NOISEHEIGHT_ 113
#define _PI_ 3.141592653589793

#define _CURRENT_TIME_ time(NULL)
#define _WORLDSTART_HEIGHT_ 42

#define _TREE_RARITY_ rand() % 6 <= 3 && x % 6 == 0
#define _CHEST_RARITY_ rand() % 9 == 0 && x % 19 == 0
#define _CHEST_RARITY_CLOUDS_ rand() % 3 == 0 && x % 19 == 0 && y == -42
#define _STONE_RARITY_ rand() % 2 == 0 && y > 4
#define _DIRT_RARITY_SURFACE_ rand() % 10 == 0 && y < 30
#define _DIRT_RARITY_UNDERGOUND_ rand() % 15 == 0 && y < 50 && y >= 30
#define _MOSSYBRICK_RARITY_SURFACE_ rand() % 50 == 0
#define _MOSSYBRICK_RARITY_UNDERGOUND_ rand() % 2 == 0

// DUNGEON BOSS ROOM LOCATION : 296	 113

// Non-Local Includes
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <box2d/b2_world_callbacks.h>
#include <fstream>
#include <algorithm>
#include <cmath>

// Local Includes
#include "Player.h"
#include "GUI.h"
#include "Spawner.h"

namespace Utils
{
	const int WINDOWWIDTH = 1920;
	const int WINDOWHEIGHT = 1080;
	const float m_Scale = 50.0f;
};

class CWorldManager
{
public:
	enum class WORLDTYPE
	{
		PLAINS,
		SAND,
		ICE,
		HELL
	};

	// Functions
	CWorldManager(sf::RenderWindow* _renderWindow, CPlayer* _player, b2World& _world, GUI* _gui, sf::Shader* _shader = NULL, sf::Shader* _surfaceShader = NULL, sf::Shader* _tourchShader = NULL);
	~CWorldManager();

	void Start(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners, WORLDTYPE _worldtype = WORLDTYPE::PLAINS);
	void Update(sf::Vector2f _mousePos, CTextureMaster* _textureMaster);
	void Render(sf::Shader* _defaultShader = NULL);

	void InitPointer(CPlayer* _player);
	void InitShaders(sf::Shader* _shader, sf::Shader* _surfaceshader, sf::Shader* _tourchshader);

	void CreateSkyChunk(CTextureMaster* _textureMaster);

	bool bIsBlockInRangeOfLightSource(std::list<CBlock>::iterator _it);
	bool bIsItemInRangeOfLightSource(sf::Sprite _shape);

	void CleanupAllLists();

	// Variables
	std::list<CBlock>* m_Chunk;
	std::list<CChest> m_Chests;
	std::list<CFurnace> m_Furnaces;
	std::list<CWorkBench> m_WorkBenches;
	std::list<CDoor> m_Doors;
	std::list<CBlock> m_Tourches;
	std::list<sf::RectangleShape> m_SkyChunk = {};

	int m_GenerateOffsetX = 296;
	int m_GenerateOffsetY = 113;

private:
	WORLDTYPE m_WorldType = CWorldManager::WORLDTYPE::PLAINS;

	// Variables
	sf::Shader* m_Shader;
	sf::Shader* m_SurfaceShader;
	sf::Shader* m_TourchShader;

	sf::RenderWindow* m_RenderWindow;
	b2WorldManifold m_WorldManifold;
	CPlayer* m_Player;
	b2World* m_World;
	GUI* m_GUI;

	sf::Sprite m_BGPlainsSurface;
	sf::Sprite m_BGSandSurface;
	sf::Sprite m_BGPlainsUnderGr;

	// Temp Pointers (Used To Create New Blocks)
	CBlock* m_Block;
	CChest* m_Chest;
	CPotion* m_Potion;

	// Threads
	sf::Mutex m_GlobalMutex;

	sf::Clock m_LoadTimer;
	bool m_bIsLoading = true;

	sf::Clock m_ParralaxClock;

	// Noise
	double m_Noise[_NOISEHEIGHT_][_NOISEWIDTH_] = {}; // Noise Array (0-1)

	double m_XPeriod = 2; // Defines Repetition Of Marble Lines In x
	double m_YPeriod = 2; // Defines Repetition Of Marble Lines In y
	double m_TurbPower = 1.5; // Makes Twists (turbPower = 0 : SinWave)
	double m_TurbSize = 36.0; // Initial Size Of Turbulence

	// Functions
	void GenerateNoise();

	double SmoothNoise(double x, double y);
	
	double Turbulence(double x, double y, double size);

	void CreateNoiseWorld(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners);
	void CreateSandNoiseWorld(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners);
	void CreateIceNoiseWorld(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners);
	void CreateHellNoiseWorld(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners);
	void CreateWorldBoundary(CTextureMaster* _textureMaster);
	void CreateClouds(CTextureMaster* _textureMaster);

	void OutPutWorldToFiles();
	void OutPutWorldToFiles(std::string _xPositions, std::string _yPositions);

	void InputWorldFromFiles(CTextureMaster* _textureMaster);

	void WorldBackGroundColourGradient();
	void DrawBackGround(sf::Shader* _defaultShader = NULL);

	bool IsObjectInBlock(sf::Sprite _shape);
	bool PositionIsBlock(sf::Vector2f _pos);

	void CreateDungeon(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners);

	void CreateSurfaceSpawners(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners);

	void CreateSandBackgrounds(CTextureMaster* _textureMaster);
	void CreatePlainsBackgrounds(CTextureMaster* _textureMaster);
	void CreateIceBackgrounds(CTextureMaster* _textureMaster);
	void CreateHellBackgrounds(CTextureMaster* _textureMaster);

	void ParralaxBackground();

	void CreateSlimeSurfaceSpawners(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners);
	void CreateSnowmanSurfaceSpawners(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners, bool _isCactus = false);

	void DeleteAllPointers();
	void ReleaseAllPointers();
	void UpdateGUIMousePosToBlocks(sf::Vector2f _mousePos);
	void UpdateGUIMousePosToDoor(sf::Vector2f _mousePos);
	void UpdateGUIMousePosToChest(sf::Vector2f _mousePos);
	void UpdateFurnaceCanSmeltAndMousePos(sf::Vector2f _mousePos);
	void UpdateWorkBenchCanCraftAndMousePos(sf::Vector2f _mousePos);
	void CalculateWorkBenchTypes();
	void SetShaderUniforms();
	void DrawBlocksInRangeOfPlayer(sf::Shader* _defaultShader = NULL);
	void RenderTourches(sf::Shader* _defaultShader = NULL);
	void RenderDoors(sf::Shader* _defaultShader = NULL);
	void RenderChests(sf::Shader* _defaultShader = NULL);
	void RenderFurnaces(sf::Shader* _defaultShader = NULL);
	void RenderWorkbenches(sf::Shader* _defaultShader = NULL);
};
#endif

