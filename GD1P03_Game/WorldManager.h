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
#include "Zombie.h"

namespace Utils
{
	const int WINDOWWIDTH = 1920;
	const int WINDOWHEIGHT = 1080;
	const float m_Scale = 50.0f;
};

class CWorldManager
{
public:
	// Functions
	CWorldManager(sf::RenderWindow* _renderWindow, CPlayer* _player, b2World& _world, GUI* _gui, sf::Shader* _shader = NULL, sf::Shader* _surfaceShader = NULL, sf::Shader* _tourchShader = NULL);
	~CWorldManager();

	void Start(CTextureMaster* _textureMaster);
	void Update(sf::Vector2f _mousePos, CTextureMaster* _textureMaster);
	void Render(sf::Shader* _defaultShader = NULL);

	void CreateSkyChunk(CTextureMaster* _textureMaster);
    void CreateNoiseWorld(CTextureMaster* _textureMaster);
	void CreateWorldBoundary(CTextureMaster* _textureMaster);
	void CreateClouds(CTextureMaster* _textureMaster);

	bool bIsBlockInRangeOfLightSource(std::list<CBlock>::iterator _it);
	bool bIsItemInRangeOfLightSource(sf::Sprite _shape);

	bool CleanUpBlocks();
	bool CleanUpSky();

	void InitPointer(CPlayer* _player);

	void OutPutWorldToFiles();
	void OutPutWorldToFiles(std::string _xPositions, std::string _yPositions);

	void InputWorldFromFiles(CTextureMaster* _textureMaster);

	void WorldBackGroundColourGradient();
	void DrawBackGround(sf::Shader* _defaultShader = NULL);

	bool IsObjectInBlock(sf::Sprite _shape);
	bool PositionIsBlock(sf::Vector2f _pos);

	void CreateDungeon(CTextureMaster* _textureMaster);
	
	// Variables
	std::list<CBlock> m_Chunk = {};
	std::list<CChest> m_Chests = {};
	std::list<CFurnace> m_Furnaces = {};
	std::list<CWorkBench> m_WorkBenches = {};
	std::list<CDoor> m_Doors = {};
	std::list<sf::RectangleShape> m_SkyChunk = {};

	std::vector<CBlock> m_TestChunk = {};

	int m_GenerateOffsetX = 296;
	int m_GenerateOffsetY = 113;
private:
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
	sf::Sprite m_BGPlainsUnderGr;

	// Temp Pointers (Used To Create New Blocks)
	CBlock* m_Block;
	CChest* m_Chest;
	CPotion* m_Potion;

	// Threads
	sf::Mutex m_GlobalMutex;

	sf::Clock m_LoadTimer;
	bool m_bIsLoading = true;

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
};
#endif

