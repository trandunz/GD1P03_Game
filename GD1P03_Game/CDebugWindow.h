#pragma once
#ifndef _DEBUGWINDOW_H__
#define _DEBUGWINDOW_H__

// Local Includes
#include "GUI.h"
#include "WorldManager.h"
#include "AudioManager.h"
#include "Spawner.h"
#include "CInputField.h"
#include "CButtons.h"
#include "CArmour.h"

class CDebugWindow
{
public:
	CDebugWindow(CTextureMaster* _textureMaster, CWorldManager* _worldManager, CPlayer* _player, std::list<Spawner>& _spawners, bool* _bChangeScenes, int* _sceneValue, int& _numberOfLeakes);
	~CDebugWindow();

	void Start();
	void Update();
	void Render();

	void Close();

	void SetPlayer(CPlayer* _player);
	void SetSpawners(std::list<Spawner>* _spawners);

	bool m_InFocus = true;

private:
	void CreateItemListButtons();
	void CreateEnemyControlButtons();
	void CreateWorldControlButtons();
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

	std::list<Spawner>* m_Spawners;

	sf::Event m_Event;

	sf::Vector2f m_MousePos;

	sf::RenderWindow* m_RenderWindow;
	CWorldManager* m_WorldManager;
	CPlayer* m_Player = nullptr;
	CTextureMaster* m_TextureMaster;

	std::map<int, CButtons> m_ItemListButtons;
	std::map<int, CBlock> m_ItemPreviews;

	std::map<int, CButtons> m_PlayerButtons;
	std::map<int, CButtons> m_EnemyButtons;
	std::map<int, CButtons> m_WorldButtons;

	sf::RectangleShape m_PlayerBackGround;
	sf::RectangleShape m_PlayerStatsBackGround;
	sf::RectangleShape m_PlayerImageIconBackGround;

	sf::RectangleShape m_EnemiesBackGround;

	sf::RectangleShape m_WorldBackGround;

	sf::Sprite m_PlayerPreview;
	sf::Texture m_PlayerPreviewTexture;

	bool m_bGodMode = false;

	bool* m_bChangeScenes;
	int* m_SceneValue;

	int* m_NumberOFLeakes;
};
#endif

