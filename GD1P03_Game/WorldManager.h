#pragma once
#ifndef _WORLDMANAGER_H__
#define _WORLDMANAGER_H__

#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <box2d/b2_world_callbacks.h>
#include <fstream>
#include <algorithm>
#include <cmath>

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

	void UpdateWorldTexture(sf::View& _view);

	bool CleanUpBlocks();
	bool CleanUpSky();

	void InitPointer(CPlayer* _player);

	void OutPutWorldToFiles();
	void OutPutWorldToFiles(std::string _xPositions, std::string _yPositions);

	std::list<CBlock> m_Chunk = {};
	std::list<CChest> m_Chests = {};
	std::list<CFurnace> m_Furnaces = {};
	std::list<CDoor> m_Doors = {};
	std::list<sf::RectangleShape> m_SkyChunk = {};

private:
	sf::Shader* m_Shader;
	sf::Shader* m_SurfaceShader;
	sf::Shader* m_TourchShader;

	sf::RenderWindow* m_RenderWindow;
	CPlayer* m_Player;
	b2World* m_World;
	GUI* m_GUI;

	// Temp Block Pointer (Used To Create New Blocks During World Generation)
	CBlock* m_Block;

	CChest* m_Chest;

	sf::Sprite m_BGPlainsSurface;
	sf::Sprite m_BGPlainsUnderGr;

	b2WorldManifold m_WorldManifold;

	// Blocks / Tiles
	sf::RenderTexture m_WorldTexture;
	sf::RectangleShape m_WorldSprite;

	// Threads
	sf::Mutex GlobalMutex;

    /**/
    // Noise
    //
//#define noiseWidth 166
//#define noiseHeight 113
#define noiseWidth 366
#define noiseHeight 113

	//xPeriod and yPeriod together define the angle of the lines
  //xPeriod and yPeriod both 0 ==> it becomes a normal clouds or turbulence pattern
	double xPeriod = 1; //defines repetition of marble lines in x direction
	double yPeriod = 2; //defines repetition of marble lines in y direction
	//turbPower = 0 ==> it becomes a normal sine pattern
	double turbPower = 2.0; //makes twists
	double turbSize = 32.0; //initial size of the turbulence

	double noise[noiseHeight][noiseWidth]; // Noise Array (0-1)

	void generateNoise()
	{
		for (int y = 0; y < noiseHeight; y++)
			for (int x = 0; x < noiseWidth; x++)
			{
				noise[y][x] = (rand() % 32768) / 32768.0; // Random 0-1
			}
	}
	double smoothNoise(double x, double y)
	{
		//Get Fractional Part of x and y
		double fractX = x - int(x);
		double fractY = y - int(y);

		//wrap around
		int x1 = (int(x) + noiseWidth) % noiseWidth;
		int y1 = (int(y) + noiseHeight) % noiseHeight;

		//neighbor values
		int x2 = (x1 + noiseWidth - 1) % noiseWidth;
		int y2 = (y1 + noiseHeight - 1) % noiseHeight;

		//smooth the noise with bilinear interpolation
		double value = 0.0;
		value += fractX * fractY * noise[y1][x1];
		value += (1 - fractX) * fractY * noise[y1][x2];
		value += fractX * (1 - fractY) * noise[y2][x1];
		value += (1 - fractX) * (1 - fractY) * noise[y2][x2];

		return value;
	}
	double turbulence(double x, double y, double size)
	{
		double value = 0.0, initialSize = size;

		while (size >= 1)
		{
			value += smoothNoise(x / size, y / size) * size;
			size /= 2.0;
		}

		return(128.0 * value / initialSize);
	}
    //
    // Noise
    /**/

};
#endif

