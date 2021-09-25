#pragma once
#ifndef _WORLDMANAGER_H__
#define _WORLDMANAGER_H__
#define _NOISEWIDTH_ 366
#define _NOISEHEIGHT_ 113
#define _PI_ 3.141592653589793

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
	void Update(sf::Vector2f _mousePos);
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

	void InputWorldFromFiles();

	void WorldBackGroundColourGradient();
	void DrawBackGround(sf::Shader* _defaultShader = NULL);
	
	// Variables
	std::list<CBlock> m_Chunk = {};
	std::list<CChest> m_Chests = {};
	std::list<CFurnace> m_Furnaces = {};
	std::list<CWorkBench> m_WorkBenches = {};
	std::list<CDoor> m_Doors = {};
	std::list<sf::RectangleShape> m_SkyChunk = {};

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

	// Threads
	sf::Mutex m_GlobalMutex;

	// Noise
	double m_Noise[_NOISEHEIGHT_][_NOISEWIDTH_] = {}; // Noise Array (0-1)

	double m_XPeriod = 1; // Defines Repetition Of Marble Lines In x
	double m_YPeriod = 2; // Defines Repetition Of Marble Lines In y
	double m_TurbPower = 2.0; // Makes Twists (turbPower = 0 : SinWave)
	double m_TurbSize = 32.0; // Initial Size Of Turbulence

	// Functions
	void GenerateNoise();

	double SmoothNoise(double x, double y);
	
	double Turbulence(double x, double y, double size);
};
#endif

