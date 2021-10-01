#include "CDebugWindow.h"

CDebugWindow::CDebugWindow(CTextureMaster* _textureMaster, CWorldManager* _worldManager, CPlayer* _player, std::list<Spawner>& _spawners)
{
	m_WorldManager = _worldManager;
	m_Player = _player;
	m_TextureMaster = _textureMaster;
	m_Spawners = &_spawners;

	// Texture
	m_PlayerPreviewTexture.loadFromFile("Images/PlayerIconRight.png");
	m_PlayerPreview.setTexture(m_PlayerPreviewTexture, true);

	// Player Stuff
	m_PlayerBackGround = sf::RectangleShape(sf::Vector2f(400, 600));
	m_PlayerBackGround.setFillColor(sf::Color::Green);
	m_PlayerStatsBackGround = sf::RectangleShape(sf::Vector2f(350, 275));
	m_PlayerStatsBackGround.setFillColor(sf::Color::Yellow);
	m_PlayerStatsBackGround.setPosition(25, 300);
	m_PlayerImageIconBackGround = sf::RectangleShape(sf::Vector2f(125, 125));
	m_PlayerImageIconBackGround.setFillColor(sf::Color::Magenta);
	m_PlayerImageIconBackGround.setPosition(50, 325);
	m_PlayerPreview.setPosition(50 + 12.5f, 325 + 12.5f);

	// Enemy Stuff
	m_EnemiesBackGround = sf::RectangleShape(sf::Vector2f(200, 300));
	m_EnemiesBackGround.setFillColor(sf::Color::Red);
	m_EnemiesBackGround.setPosition(400, 0);
	// World Stuff
	m_WorldBackGround = sf::RectangleShape(sf::Vector2f(200, 300));
	m_WorldBackGround.setFillColor(sf::Color::Blue);
	m_WorldBackGround.setPosition(400, 300);

	// Render Window Creation
	m_RenderWindow = new sf::RenderWindow(sf::VideoMode(600, 600), "DEBUG CONTROLS", (sf::Style::Titlebar));
}

CDebugWindow::~CDebugWindow()
{
	m_PlayerButtons.clear();
	m_ItemPreviews.clear();
	m_ItemListButtons.clear();

	delete m_RenderWindow;

	m_RenderWindow = nullptr;
	m_Spawners = nullptr;
}

void CDebugWindow::Start()
{
	CreateItemListButtons();
	CreateEnemyControlButtons();
	CreatePlayerControlButtons();
}

void CDebugWindow::Update()
{
	m_MousePos = m_RenderWindow->mapPixelToCoords((sf::Mouse::getPosition(*m_RenderWindow)));

	// bGodMode ? SetHP (Every Frame)
	GodMode();

	// Buttons (Polled Update)
	while (m_RenderWindow->pollEvent(m_Event))
	{
		for (int i = 0; i < m_ItemListButtons.size(); i++)
		{
			m_ItemListButtons[i].Update();
			if (m_Event.type == sf::Event::MouseButtonPressed && m_ItemListButtons[i].bIsinBounds(m_MousePos))
			{
				AddItemToInventory(i);
				break;
			}
		}

		for (int i = 0; i < m_EnemyButtons.size(); i++)
		{
			m_EnemyButtons[i].Update();
			if (m_Event.type == sf::Event::MouseButtonPressed && m_EnemyButtons[i].bIsinBounds(m_MousePos))
			{
				if (i == 0)
				{
					KillEnemies(CEnemy::ENEMYTYPE::SLIME);
				}
				if (i == 1)
				{
					KillEnemies();
				}
				break;
			}
		}

		for (int i = 0; i < m_PlayerButtons.size(); i++)
		{
			m_PlayerButtons[i].Update();
			if (m_Event.type == sf::Event::MouseButtonPressed && m_PlayerButtons[i].bIsinBounds(m_MousePos))
			{
				if (i == 0)
				{
					KillPlayer();
				}
				else if (i == 1)
				{
					m_PlayerButtons[i].SetState("Click");
					m_PlayerButtons[i].m_bIsPressed = !m_PlayerButtons[i].m_bIsPressed;
					ToggleGodMode();
				}
				break;
			}
		}
	}
}

void CDebugWindow::Render()
{
	// Clear Render Window
	m_RenderWindow->clear();

	m_RenderWindow->draw(m_PlayerBackGround);
	m_RenderWindow->draw(m_EnemiesBackGround);
	m_RenderWindow->draw(m_WorldBackGround);
	m_RenderWindow->draw(m_PlayerStatsBackGround);
	m_RenderWindow->draw(m_PlayerImageIconBackGround);
	m_RenderWindow->draw(m_PlayerPreview);

	// Item Buttons
	for (int i = 0; i < m_ItemListButtons.size(); i++)
	{
		m_ItemListButtons[i].Render();
	}

	for (int i = 0; i < m_ItemPreviews.size(); i++)
	{
		m_RenderWindow->draw(m_ItemPreviews[i].GetShape());
	}

	for (int i = 0; i < m_EnemyButtons.size(); i++)
	{
		m_RenderWindow->draw(m_EnemyButtons[i].Sprite);
	}

	for (int i = 0; i < m_PlayerButtons.size(); i++)
	{
		m_RenderWindow->draw(m_PlayerButtons[i].Sprite);
	}

	// Display
	m_RenderWindow->display();
}

void CDebugWindow::SetPlayer(CPlayer* _player)
{
	m_Player = _player;
}

void CDebugWindow::SetSpawners(std::list<Spawner>* _spawners)
{
	m_Spawners = _spawners;
}

void CDebugWindow::CreateItemListButtons()
{
	// Row 1 Buttons
	for (int i = 0; i < 10; i++)
	{
		CButtons* tempbutton = new CButtons(m_RenderWindow);
		tempbutton->SetPosition(10 + (i * 30), (1 * 30) - 20);
		tempbutton->SetLabel("");
		m_ItemListButtons.insert_or_assign(i, *tempbutton);
		tempbutton = nullptr;
	}
	// Row 2 Buttons
	for (int i = 10; i < 20; i++)
	{
		CButtons* tempbutton = new CButtons(m_RenderWindow);
		tempbutton->SetPosition(10 + ((i - 10) * 30), (2 * 30) - 20);
		tempbutton->SetLabel("");
		m_ItemListButtons.insert_or_assign(i, *tempbutton);
		tempbutton = nullptr;
	}
	// Row 3 Buttons
	for (int i = 20; i < 30; i++)
	{
		CButtons* tempbutton = new CButtons(m_RenderWindow);
		tempbutton->SetPosition(10 + ((i - 20) * 30), (3 * 30) - 20);
		tempbutton->SetLabel("");
		m_ItemListButtons.insert_or_assign(i, *tempbutton);
		tempbutton = nullptr;
	}
	// Row 4 Buttons
	for (int i = 30; i < 40; i++)
	{
		CButtons* tempbutton = new CButtons(m_RenderWindow);
		tempbutton->SetPosition(10 + ((i - 30) * 30), (4 * 30) - 20);
		tempbutton->SetLabel("");
		m_ItemListButtons.insert_or_assign(i, *tempbutton);
		tempbutton = nullptr;
	}
	// Row 5 Buttons
	for (int i = 40; i < 50; i++)
	{
		CButtons* tempbutton = new CButtons(m_RenderWindow);
		tempbutton->SetPosition(10 + ((i - 40) * 30), (5 * 30) - 20);
		tempbutton->SetLabel("");
		m_ItemListButtons.insert_or_assign(i, *tempbutton);
		tempbutton = nullptr;
	}
	// Row 6 Buttons
	for (int i = 50; i < 60; i++)
	{
		CButtons* tempbutton = new CButtons(m_RenderWindow);
		tempbutton->SetPosition(10 + ((i - 50) * 30), (6 * 30) - 20);
		tempbutton->SetLabel("");
		m_ItemListButtons.insert_or_assign(i, *tempbutton);
		tempbutton = nullptr;
	}

	// Item Icons / Previews
	for (int i = 0; i < m_ItemListButtons.size(); i++)
	{
		switch (i)
		{
		case 0:
		{
			CPickaxe* pickaxe = new CPickaxe();
			
			m_ItemPreviews.insert_or_assign(i, *pickaxe);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			pickaxe = nullptr;
			break;
		}
		case 1:
		{
			CPickaxe* pickaxe = new CPickaxe(CBlock::PICKAXETYPE::IRON);
			m_ItemPreviews.insert_or_assign(i, *pickaxe);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			pickaxe = nullptr;
			break;
		}
		case 2:
		{
			CPickaxe* pickaxe = new CPickaxe(CBlock::PICKAXETYPE::GOLD);
			m_ItemPreviews.insert_or_assign(i, *pickaxe);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			pickaxe = nullptr;
			break;
		}
		case 3:
		{
			CPickaxe* pickaxe = new CPickaxe(CBlock::PICKAXETYPE::DIAMOND);
			m_ItemPreviews.insert_or_assign(i, *pickaxe);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			pickaxe = nullptr;
			break;
		}
		case 4:
		{
			CPickaxe* pickaxe = new CPickaxe(CBlock::PICKAXETYPE::PURPLE);

			m_ItemPreviews.insert_or_assign(i, *pickaxe);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			pickaxe = nullptr;
			break;
		}
		case 5:
		{
			CPickaxe* pickaxe = new CPickaxe(CBlock::PICKAXETYPE::GOLDEN);
			m_ItemPreviews.insert_or_assign(i, *pickaxe);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			pickaxe = nullptr;
			break;
		}
		case 6:
		{
			CPickaxe* pickaxe = new CPickaxe(CBlock::PICKAXETYPE::OBSIDIAN);
			m_ItemPreviews.insert_or_assign(i, *pickaxe);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			pickaxe = nullptr;

			break;
		}
		case 7:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_Dirt, CBlock::BLOCKTYPE::DIRT);
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 8:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_Stone, CBlock::BLOCKTYPE::STONE);
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 9:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_Wood, CBlock::BLOCKTYPE::WOOD);
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 10:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_Planks, CBlock::BLOCKTYPE::PLANKS);
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 11:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_Sand, CBlock::BLOCKTYPE::SAND);
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 12:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_MossyBrick, CBlock::BLOCKTYPE::MOSSYBRICK);
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 13:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_Grass, CBlock::BLOCKTYPE::GRASS);
			m_ItemPreviews.insert_or_assign(i, *block);

			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 14:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_Leaves, CBlock::BLOCKTYPE::LEAVES);
			m_ItemPreviews.insert_or_assign(i, *block);

			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 15:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_IronOre, CBlock::BLOCKTYPE::IRONORE);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);

			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 16:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_GoldOre, CBlock::BLOCKTYPE::GOLDORE);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);

			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 17:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_DiamondOre, CBlock::BLOCKTYPE::DIAMONDORE);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 18:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_PurpleOre, CBlock::BLOCKTYPE::PURPLEORE);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 19:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_GoldenOre, CBlock::BLOCKTYPE::GOLDENORE);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 20:
		{
			CDoor* block = new CDoor();
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.1f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 21:
		{
			CFurnace* block = new CFurnace();
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 22:
		{
			CChest* block = new CChest();
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 23:
		{
			CWorkBench* block = new CWorkBench();
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 24:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 25:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 26:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 27:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_PurpleIngot, CBlock::BLOCKTYPE::PURPLEINGOT);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 28:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_GoldenIngot, CBlock::BLOCKTYPE::GOLDENINGOT);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 29:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_Tourch, CBlock::BLOCKTYPE::TOURCH);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 30:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_Dirt, CBlock::BLOCKTYPE::BROKENSWORD);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 31:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_RedSlime, CBlock::BLOCKTYPE::REDSLIME);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 32:
		{
			CBlock* block = new CBlock(m_TextureMaster->m_GlassBeaker, CBlock::BLOCKTYPE::EMPTYBEAKER);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 33:
		{
			CPotion* block = new CPotion(CBlock::POTIONTYPE::HPSMALL);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 34:
		{
			CPotion* block = new CPotion(CBlock::POTIONTYPE::HPLARGE);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 35:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::BASIC);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 36:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::IRON);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 37:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::PURPLE);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 38:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::GOLDEN);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 39:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::GREEN);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 40:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::ICE);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 41:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::BLOOD);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 42:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::OBSIDIAN);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 43:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::IRONGUN);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 44:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::GOLDGUN);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 45:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::PURPLEGUN);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 46:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::GOLDENGUN);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 47:
		{
			Bow* block = new Bow(CBlock::BOWTYPE::GREENGUN);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 48:
		{
			CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::ARROW);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 49:
		{
			CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::FIREARROW);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 50:
		{
			CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::CURSEDARROW);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 51:
		{
			CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::POISONARROW);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 52:
		{
			CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::IRONBULLET);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 53:
		{
			CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::GOLDBULLET);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 54:
		{
			CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::PURPLEBULLET);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		case 55:
		{
			CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::GOLDENBULLET);
			block->GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			m_ItemPreviews.insert_or_assign(i, *block);
			m_ItemPreviews[i].GetShape().setScale(0.2f, 0.2f);
			m_ItemPreviews[i].GetShape().setOrigin(m_ItemPreviews[i].GetShape().getGlobalBounds().width / 2, m_ItemPreviews[i].GetShape().getGlobalBounds().height / 2);
			m_ItemPreviews[i].GetShape().setPosition(m_ItemListButtons[i].GetPosition());
			block = nullptr;
			break;
		}
		default:
			break;
		}
	}

	// Item Previews Updates
	for (int i = 0; i < m_ItemPreviews.size(); i++)
	{
		m_ItemPreviews[i].Update();
	}
}

void CDebugWindow::CreateEnemyControlButtons()
{
	// kill all slimes
	if (true)
	{
		CButtons* tempbutton = new CButtons(m_RenderWindow);
		tempbutton->SetPosition(20 + (13 * 30), (1 * 30) - 20);
		tempbutton->SetLabel("Kill All Slimes");

		sf::Texture Hover;
		Hover.loadFromFile("Images/KillAllSlimesButton-Hover.png");
		tempbutton->SetHoverTex(Hover);

		sf::Texture Pressed;
		Pressed.loadFromFile("Images/KillAllSlimesButton.png");
		tempbutton->SetClickTex(Pressed);

		sf::Texture Idle;
		Idle.loadFromFile("Images/KillAllSlimesButton.png");
		tempbutton->SetIdleTex(Idle);

		m_EnemyButtons.insert_or_assign(0, *tempbutton);
		tempbutton = nullptr;
	}
	
	// kill all Enemies
	if (true)
	{
		CButtons* tempbutton = new CButtons(m_RenderWindow);
		tempbutton->SetPosition(20 + (14 * 30), (1 * 30) - 20);
		tempbutton->SetLabel("Kill All Enemies");

		sf::Texture Hover;
		Hover.loadFromFile("Images/KillAllButton-Hover.png");
		tempbutton->SetHoverTex(Hover);

		sf::Texture Pressed;
		Pressed.loadFromFile("Images/KillAllButton.png");
		tempbutton->SetClickTex(Pressed);

		sf::Texture Idle;
		Idle.loadFromFile("Images/KillAllButton.png");
		tempbutton->SetIdleTex(Idle);

		m_EnemyButtons.insert_or_assign(1, *tempbutton);
		tempbutton = nullptr;
	}
}

void CDebugWindow::CreatePlayerControlButtons()
{
	// Kill Player Button
	if (true)
	{
		CButtons* tempbutton = new CButtons(m_RenderWindow);
		tempbutton->SetPosition(10 + (0 * 30), (8 * 30) - 20);
		tempbutton->SetLabel("Kill Player");

		sf::Texture Hover;
		Hover.loadFromFile("Images/KillPlayerButton-Hover.png");
		tempbutton->SetHoverTex(Hover);

		sf::Texture Pressed;
		Pressed.loadFromFile("Images/KillPlayerButton.png");
		tempbutton->SetClickTex(Pressed);

		sf::Texture Idle;
		Idle.loadFromFile("Images/KillPlayerButton.png");
		tempbutton->SetIdleTex(Idle);

		m_PlayerButtons.insert_or_assign(0, *tempbutton);
		tempbutton = nullptr;
	}

	// GodMode Bool Button
	if (true)
	{
		CButtons* tempbutton = new CButtons(m_RenderWindow);
		tempbutton->SetPosition(10 + (1 * 30), (8 * 30) - 20);

		sf::Texture Hover;
		Hover.loadFromFile("Images/GodMode.png");
		tempbutton->SetHoverTex(Hover);

		sf::Texture Pressed;
		Pressed.loadFromFile("Images/GodModeToggled.png");
		tempbutton->SetClickTex(Pressed);

		sf::Texture Idle;
		Idle.loadFromFile("Images/GodMode.png");
		tempbutton->SetIdleTex(Idle);

		m_PlayerButtons.insert_or_assign(1, *tempbutton);
		tempbutton = nullptr;
	}
}

void CDebugWindow::Close()
{
	m_RenderWindow->close();
}

void CDebugWindow::ResetAllButtons()
{
	for (int i = 0; i < m_ItemListButtons.size(); i++)
	{
		m_ItemListButtons[i].m_bIsPressed = false;
	}

	for (int i = 0; i < m_EnemyButtons.size(); i++)
	{
		m_EnemyButtons[i].m_bIsPressed = false;
	}

	for (int i = 0; i < m_PlayerButtons.size(); i++)
	{
		m_PlayerButtons[i].m_bIsPressed = false;
	}
}

void CDebugWindow::KillEnemies()
{
	std::list<Spawner>::iterator spawner = m_Spawners->begin();
	for (spawner; spawner != m_Spawners->end(); spawner++)
	{
		spawner->m_Slimes.clear();
		spawner->m_Zombies.clear();
	}
}

void CDebugWindow::KillEnemies(CEnemy::ENEMYTYPE _type)
{
	if (_type == CEnemy::ENEMYTYPE::SLIME)
	{
		std::list<Spawner>::iterator spawner = m_Spawners->begin();
		for (spawner; spawner != m_Spawners->end(); spawner++)
		{
			spawner->m_Slimes.clear();
		}
	}
}

void CDebugWindow::KillPlayer()
{
	if (m_Player != nullptr)
	{
		m_Player->SetCurrentHP(0);
	}
}

void CDebugWindow::ToggleGodMode()
{
	m_bGodMode = !m_bGodMode;

	if (m_Player != nullptr)
	{
		m_Player->ToggleGodMode();
	}
}

void CDebugWindow::SetGodMode(bool _value)
{
	m_bGodMode = _value;
}

void CDebugWindow::GodMode()
{
	// God Mode
	if (m_bGodMode && m_Player != nullptr)
	{
		m_Player->SetCurrentHP(m_Player->GetMaxHP());
	}
	else if (m_Player == nullptr)
	{
		ResetAllButtons();
		SetGodMode(false);
	}
}

void CDebugWindow::AddItemToInventory(int _itemIndexValue)
{
	if (m_Player != nullptr && m_TextureMaster != nullptr)
	{
		if (m_Player->m_InventorySize < 49)
		{
			switch (_itemIndexValue)
			{
			case 0:
			{
				CPickaxe* pickaxe = new CPickaxe();
				m_Player->AddItemToInventory(pickaxe, false);
				pickaxe = nullptr;
				break;
			}
			case 1:
			{
				CPickaxe* pickaxe = new CPickaxe(CBlock::PICKAXETYPE::IRON);
				m_Player->AddItemToInventory(pickaxe, false);
				pickaxe = nullptr;
				break;
			}
			case 2:
			{
				CPickaxe* pickaxe = new CPickaxe(CBlock::PICKAXETYPE::GOLD);
				m_Player->AddItemToInventory(pickaxe, false);
				pickaxe = nullptr;
				break;
			}
			case 3:
			{
				CPickaxe* pickaxe = new CPickaxe(CBlock::PICKAXETYPE::DIAMOND);
				m_Player->AddItemToInventory(pickaxe, false);
				pickaxe = nullptr;
				break;
			}
			case 4:
			{
				CPickaxe* pickaxe = new CPickaxe(CBlock::PICKAXETYPE::PURPLE);
				m_Player->AddItemToInventory(pickaxe, false);
				pickaxe = nullptr;
				break;
			}
			case 5:
			{
				CPickaxe* pickaxe = new CPickaxe(CBlock::PICKAXETYPE::GOLDEN);
				m_Player->AddItemToInventory(pickaxe, false);
				pickaxe = nullptr;
				break;
			}
			case 6:
			{
				CPickaxe* pickaxe = new CPickaxe(CBlock::PICKAXETYPE::OBSIDIAN);
				m_Player->AddItemToInventory(pickaxe, false);
				pickaxe = nullptr;

				break;
			}
			case 7:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_Dirt, CBlock::BLOCKTYPE::DIRT);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 8:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_Stone, CBlock::BLOCKTYPE::STONE);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 9:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_Wood, CBlock::BLOCKTYPE::WOOD);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 10:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_Planks, CBlock::BLOCKTYPE::PLANKS);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 11:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_Sand, CBlock::BLOCKTYPE::SAND);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 12:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_MossyBrick, CBlock::BLOCKTYPE::MOSSYBRICK);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 13:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_Grass, CBlock::BLOCKTYPE::GRASS);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 14:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_Leaves, CBlock::BLOCKTYPE::LEAVES);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 15:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_IronOre, CBlock::BLOCKTYPE::IRONORE);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 16:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_GoldOre, CBlock::BLOCKTYPE::GOLDORE);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 17:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_DiamondOre, CBlock::BLOCKTYPE::DIAMONDORE);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 18:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_PurpleOre, CBlock::BLOCKTYPE::PURPLEORE);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 19:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_GoldenOre, CBlock::BLOCKTYPE::GOLDENORE);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 20:
			{
				CDoor* block = new CDoor();
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 21:
			{
				CFurnace* block = new CFurnace();
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 22:
			{
				CChest* block = new CChest();
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 23:
			{
				CWorkBench* block = new CWorkBench();
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 24:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 25:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 26:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 27:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_PurpleIngot, CBlock::BLOCKTYPE::PURPLEINGOT);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 28:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_GoldenIngot, CBlock::BLOCKTYPE::GOLDENINGOT);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 29:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_Tourch, CBlock::BLOCKTYPE::TOURCH);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 30:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_Dirt, CBlock::BLOCKTYPE::BROKENSWORD);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 31:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_RedSlime, CBlock::BLOCKTYPE::REDSLIME);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 32:
			{
				CBlock* block = new CBlock(m_TextureMaster->m_GlassBeaker, CBlock::BLOCKTYPE::EMPTYBEAKER);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 33:
			{
				CPotion* block = new CPotion(CBlock::POTIONTYPE::HPSMALL);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 34:
			{
				CPotion* block = new CPotion(CBlock::POTIONTYPE::HPLARGE);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 35:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::BASIC);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 36:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::IRON);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 37:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::PURPLE);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 38:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::GOLDEN);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 39:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::GREEN);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 40:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::ICE);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 41:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::BLOOD);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 42:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::OBSIDIAN);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 43:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::IRONGUN);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 44:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::GOLDGUN);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 45:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::PURPLEGUN);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 46:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::GOLDENGUN);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 47:
			{
				Bow* block = new Bow(CBlock::BOWTYPE::GREENGUN);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, false);
				block = nullptr;
				break;
			}
			case 48:
			{
				CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::ARROW);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 49:
			{
				CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::FIREARROW);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 50:
			{
				CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::CURSEDARROW);
				block->GetShape().setScale(0.1, 0.1);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 51:
			{
				CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::POISONARROW);
				block->GetShape().setScale(0.2, 0.2);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 52:
			{
				CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::IRONBULLET);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 53:
			{
				CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::GOLDBULLET);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 54:
			{
				CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::PURPLEBULLET);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			case 55:
			{
				CProjectile* block = new CProjectile(CBlock::PROJECTILETYPE::GOLDENBULLET);
				m_Player->AddItemToInventory(block, true);
				block = nullptr;
				break;
			}
			default:
				break;
			}
		}
	}
}

void CDebugWindow::ClearPlayerInventory(bool _giveStarterItems)
{
	if (m_Player != nullptr)
	{
		if (m_Player->GetPickaxe() != nullptr)
		{
			delete m_Player->GetPickaxe();
		}
		m_Player->SetPickaxe(nullptr);
		if (m_Player->GetBow() != nullptr)
		{
			delete m_Player->GetBow();
		}
		m_Player->SetBow(nullptr);

		m_Player->m_InventoryMap.clear();
		m_Player->m_InventoryStackValues.clear();

		for (int i = 0; i < 50; i++)
		{
			m_Player->m_InventoryMap[i];
			m_Player->m_InventoryStackValues[i];
		}

		if (_giveStarterItems)
		{
			if (!m_Player->IsItemInInventory(CBlock::BLOCKTYPE::PICKAXE))
			{
				//Starting Items
				for (int i = 0; i < 50; i++)
				{
					if (m_Player->m_InventoryStackValues[i] == 0)
					{
						CPickaxe* m_Pickaxe = new CPickaxe();
						m_Player->AddItemToInventory(m_Pickaxe, 0, false);
						m_Pickaxe = nullptr;
						break;
					}
				}
			}
			if (!m_Player->IsItemInInventory(CBlock::BLOCKTYPE::BOW))
			{
				//Starting Items
				for (int i = 0; i < 50; i++)
				{
					if (m_Player->m_InventoryStackValues[i] == 0)
					{
						Bow* m_Bow = new Bow(CBlock::BOWTYPE::BASIC);
						m_Player->AddItemToInventory(m_Bow, 1, false);
						m_Bow = nullptr;
						break;
					}
				}
			}
			//Starting Items
			for (int i = 0; i < 50; i++)
			{
				if (m_Player->m_InventoryStackValues[i] == 0)
				{
					CPotion* m_Potion = new CPotion(CBlock::POTIONTYPE::HPSMALL);
					m_Player->AddItemToInventory(m_Potion, 2);
					m_Potion = nullptr;
					break;
				}
			}

			//Starting Items
			for (int i = 0; i < 50; i++)
			{
				if (m_Player->m_InventoryStackValues[i] == 0)
				{
					for (int i = 0; i < 15; i++)
					{
						CProjectile* temp = new CProjectile(CBlock::PROJECTILETYPE::ARROW);
						m_Player->AddItemToInventory(temp, 3);
						temp = nullptr;
					}

					break;
				}
			}
		}
	}
}
