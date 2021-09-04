#pragma once
#ifndef _WORLDMANAGER_H__
#define _WORLDMANAGER_H__

#include "Player.h"
#include "GUI.h"



namespace Utils
{
	const int WINDOWWIDTH = 1920;
	const int WINDOWHEIGHT = 1080;
	const float m_Scale = 50.0f;
};

class CWorldManager
{
public:
	CWorldManager(sf::RenderWindow* _renderWindow, CPlayer* _player, b2World& _world, GUI* _gui);
	~CWorldManager();

	void Start();
	void Update(sf::Event& _event, sf::Vector2f _mousePos);
	void Render();

	void CreateSkyChunk();
	void CreateSurfaceLayerPart1();
	void CreateSurfaceLayerPart2();

	void UpdateWorldTexture(sf::View& _view);

	bool CleanUpBlocks();
	bool CleanUpSky();


	std::list<CBlock> m_Chunk = {};
	std::list<CChest> m_Chests = {};
	std::list<CDoor> m_Doors = {};
	std::list<sf::RectangleShape> m_SkyChunk = {};

private:

	sf::RenderWindow* m_RenderWindow;
	CPlayer* m_Player;
	b2World* m_World;
	GUI* m_GUI;

	// Temp Block Pointer (Used To Create New Blocks During World Generation)
	CBlock* m_Block;

	sf::Sprite m_WorldBackGround;

	// Blocks / Tiles
	sf::RenderTexture m_WorldTexture;
	sf::RectangleShape m_WorldSprite;

	// Threads
	sf::Mutex GlobalMutex;
};
#endif

