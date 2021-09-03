#pragma once
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

	void HealthUI(sf::RenderWindow* _renderWindow);
	sf::Text InitHealthUI();
	void MiniMapUI(sf::RenderWindow* _renderWindow, std::list<CBlock>& _chunk, std::list<sf::RectangleShape>& _skyChunk);
	void InitMiniMap(sf::RenderWindow* _renderWindow);
	void InventoryUI(sf::RenderWindow* _renderWindow);
	void InitInventoryUI();
	void ToggleInventoryUI();
	void CraftingUI(sf::RenderWindow* _renderWindow);

	void Render(sf::RenderWindow* _renderWindow);

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

	//Sprites
	sf::Sprite m_MousePos;

	// MiniMap
	sf::RenderTexture* m_miniMap;

private:
	CPlayer* m_Player;
	sf::Font m_Font;

	std::string m_HealthString;
	sf::Text m_HealthText;

	sf::RectangleShape m_MiniMapBG1;
	sf::RectangleShape m_MiniMapBG2;
	sf::RectangleShape m_MiniMapBG3;
	sf::RectangleShape m_MiniMapShape;

	sf::Sprite m_MiniMapWorldBackGround;

	std::map<int, sf::Sprite> m_InventorySlotMap;
	std::map<int, sf::Sprite> m_InventoryMap;
	

	bool m_bInventoryOpen = false;

	// Heart Containers
	sf::Sprite heart1;
	sf::Sprite heart2;
	sf::Sprite heart3;
	sf::Sprite heart4;
	sf::Sprite heart5;

};

