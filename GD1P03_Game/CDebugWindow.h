#pragma once
#ifndef _DEBUGWINDOW_H__
#define _DEBUGWINDOW_H__

#include "GUI.h"
#include "WorldManager.h"
#include "AudioManager.h"
#include "Spawner.h"
#include "CInputField.h"
#include "CButtons.h"

class CDebugWindow
{
public:
	CDebugWindow(CTextureMaster* _textureMaster, CWorldManager* _worldManager, CPlayer* _player, std::list<Spawner>& _spawners);
	~CDebugWindow();

	void Start();
	void Update();
	void Render();

	void Close();

	void SetPlayer(CPlayer* _player);
	void SetSpawners(std::list<Spawner>* _spawners);

	void CreateItemListButtons();
	void CreateEnemyControlButtons();
	void CreatePlayerControlButtons();

	void ResetAllButtons();

	void KillEnemies();
	void KillEnemies(CEnemy::ENEMYTYPE _type);

	void KillPlayer();

	void GodMode();
	void ToggleGodMode();
	void SetGodMode(bool _value);

	void AddItemToInventory(int _itemIndexValue);
	void ClearPlayerInventory(bool _giveStarterItems = false);

	bool m_InFocus = true;

private:
	std::list<Spawner>* m_Spawners;

	sf::Event m_Event;

	sf::Vector2f m_MousePos;

	sf::RenderWindow* m_RenderWindow = nullptr;
	CWorldManager* m_WorldManager = nullptr;
	CPlayer* m_Player = nullptr;
	CTextureMaster* m_TextureMaster = nullptr;

	std::map<int, CButtons> m_ItemListButtons;
	std::map<int, CBlock> m_ItemPreviews;

	std::map<int, CButtons> m_PlayerButtons;
	std::map<int, CButtons> m_EnemyButtons;

	sf::RectangleShape m_PlayerBackGround;
	sf::RectangleShape m_PlayerStatsBackGround;
	sf::RectangleShape m_PlayerImageIconBackGround;

	sf::RectangleShape m_EnemiesBackGround;

	sf::RectangleShape m_WorldBackGround;

	sf::Sprite m_PlayerPreview;
	sf::Texture m_PlayerPreviewTexture;

	bool m_bGodMode = false;
};
#endif

