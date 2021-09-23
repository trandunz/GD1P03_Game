#pragma once
#ifndef _GUI_H__
#define _GUI_H__

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <cstddef>        // std::size_t

#include "Player.h"
#include "CTextureMaster.h"



class GUI
{
public:
	GUI();
	~GUI();

	void SetPlayer(CPlayer* _player);
	
	CPlayer* GetPlayer();

	void SetFont(sf::Font& _font);

	void HealthUI(sf::RenderWindow* _renderWindow, CPlayer* _player, CTextureMaster* _textureMaster);
	sf::Text InitHealthUI(CPlayer* _player);
	void MiniMapUI(sf::RenderWindow* _renderWindow, std::list<CBlock>& _chunk, std::list<sf::Sprite>& _skyChunk, CPlayer* _player, sf::Shader* _shader, sf::Shader* _shaderBlack);
	void InitMiniMap(sf::RenderWindow* _renderWindow, CTextureMaster* _textureMaster);
	void InventoryUI(sf::RenderWindow* _renderWindow, CPlayer* _player, sf::View& _uiView, sf::View& _worldView, sf::Event& _event, CTextureMaster* _textureMaster);
	void InitInventoryUI(CPlayer* _player, sf::RenderWindow* _renderWindow, CTextureMaster* _textureMaster);
	void CraftingUI(sf::RenderWindow* _renderWindow, CPlayer* _player, CTextureMaster* _textureMaster, sf::View& _uiView);
	void InitCraftingUI(CTextureMaster* _textureMaster);
	bool bIsCraftingSpaceEmpty(int _position);

	void Render(sf::RenderWindow* _renderWindow, CPlayer* _player, sf::View& _worldView, sf::View& _uiView, sf::Shader* _defaultShader = NULL);

	std::string ToString(int32 integer);

	// Texture Master
	void InitMousePosSprite(CTextureMaster* _textureMaster);

	void HotBarScrolling(sf::Event& _event, CPlayer* _player);
	void InitHotBarScrolling(CPlayer* _player);
	void LetGoOfItemInInventory(sf::RenderWindow* _renderWindow, sf::View& _uiView, sf::View& _worldView, sf::Event& _event, CPlayer* _player);
	void ClickedItemInInventory(sf::Event& _event, CPlayer* _player);
	void HoldItemInInventory(CPlayer* _player);
	void DropCurrentlyHeldItem(CPlayer* _player, sf::Event& _event);
	bool MousePointerOverSlot();

	bool bPlayerIsMovingAnItem(CPlayer* _player);
	bool bPlayerIsMovingAnItem(CPlayer* _player, int _iterator);
	int bGetPositionOfMovingItem(CPlayer* _player);

	int FindFirstEmptyInventorySlot(CPlayer* _player);

	
	

	//Sprites
	sf::Sprite m_MousePos;

	// MiniMap
	sf::RenderTexture* m_miniMap;

	std::map<int, sf::Sprite> m_InventorySlotMap;
	std::map<int, sf::Sprite> m_CraftingSlots;
	std::vector<CBlock> m_CraftList;

	// Mouse Pointer
	sf::Sprite m_MousePointer;

	unsigned int m_CurrentSeed = 0;

	bool m_bCanSmelt;

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

	// Heart Containers
	sf::Sprite heart1;
	sf::Sprite heart2;
	sf::Sprite heart3;
	sf::Sprite heart4;
	sf::Sprite heart5;

	sf::Clock m_DrawTimer;
	sf::Clock* m_CraftTimer;
	sf::Clock* m_FirstEmpySlotTimer;

	CBlock* m_TempBlock;




};
#endif

