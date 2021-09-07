#pragma once
#ifndef _GUI_H__
#define _GUI_H__

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <list>
#include <set>
#include <map>
#include <fstream>
#include "Player.h"



class GUI
{
public:
	GUI();
	~GUI();

	void SetPlayer(CPlayer* _player);
	
	CPlayer* GetPlayer();

	void SetFont(sf::Font& _font);

	void HealthUI(sf::RenderWindow* _renderWindow, CPlayer* _player);
	sf::Text InitHealthUI(CPlayer* _player);
	void MiniMapUI(sf::RenderWindow* _renderWindow, std::list<CBlock>& _chunk, std::list<sf::RectangleShape>& _skyChunk, CPlayer* _player);
	void InitMiniMap(sf::RenderWindow* _renderWindow);
	void InventoryUI(sf::RenderWindow* _renderWindow, CPlayer* _player, sf::View& _uiView, sf::View& _worldView, sf::Event& _event);
	void InitInventoryUI(CPlayer* _player, sf::RenderWindow* _renderWindow);
	void CraftingUI(sf::RenderWindow* _renderWindow, CPlayer* _player);

	void Render(sf::RenderWindow* _renderWindow, CPlayer* _player, sf::View& _worldView, sf::View& _uiView);

	std::string ToString(int32 integer);

	// Texture Master
	void InitTextureMaster();

	// Textures
	sf::Texture* m_MousePosTex;
	sf::Texture* m_Dirt;
	sf::Texture* m_Grass;
	sf::Texture* m_Sky;
	sf::Texture* m_ItemSpacer;
	sf::Texture* m_HeartFull;
	sf::Texture* m_HeartEmpty;
	sf::Texture* m_CIITexture;
	sf::Texture* m_Planks;
	sf::Texture* m_Stone;
	sf::Texture* m_Wood;
	sf::Texture* m_Sand;
	sf::Texture* m_MossyBrick;
	sf::Texture* m_MousePointerTex;

	//Sprites
	sf::Sprite m_MousePos;

	// MiniMap
	sf::RenderTexture* m_miniMap;

	std::map<int, sf::Sprite> m_InventorySlotMap;

private:
	sf::Font m_Font;

	std::string m_HealthString;
	sf::Text m_HealthText;

	sf::RectangleShape m_MiniMapBG1;
	sf::RectangleShape m_MiniMapBG2;
	sf::RectangleShape m_MiniMapBG3;
	sf::RectangleShape m_MiniMapShape;

	sf::Sprite m_MiniMapWorldBackGround;

	std::map<int, sf::Text> m_InventoryItemStackCounters;
	

	// Mouse Pointer
	sf::Sprite m_MousePointer;


	// Heart Containers
	sf::Sprite heart1;
	sf::Sprite heart2;
	sf::Sprite heart3;
	sf::Sprite heart4;
	sf::Sprite heart5;

};
#endif

