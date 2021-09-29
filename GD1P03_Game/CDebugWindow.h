#pragma once
#ifndef _DEBUGWINDOW_H_
#define _DEBUGWINDOW_H_

#include "GUI.h"
#include "WorldManager.h"
#include "AudioManager.h"
#include "Spawner.h"
#include "CInputField.h"
#include "CButtons.h"

class CDebugWindow
{
public:
	CDebugWindow(CTextureMaster* _textureMaster = nullptr, CWorldManager* _worldManager = nullptr, CPlayer* _player = nullptr);
	~CDebugWindow();

	void Start();
	void Update();
	void Render();

	void SetPlayer(CPlayer* _player);

	void CreateItemListButtons();

	void Close();

	void ResetAllButtons();

	void AddItemToInventory(int _itemIndexValue);

	bool m_InFocus = true;

private:
	sf::Event m_Event;

	sf::Vector2f m_MousePos;

	sf::RenderWindow* m_RenderWindow = nullptr;
	CWorldManager* m_WorldManager = nullptr;
	CPlayer* m_Player = nullptr;
	CTextureMaster* m_TextureMaster = nullptr;

	std::map<int, CButtons> m_ItemListButtons;
	std::map<int, CBlock> m_ItemPreviews;

	sf::RectangleShape m_BackGround;
};
#endif

