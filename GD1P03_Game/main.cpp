// Non-Local Includes
#include <iostream>

// Local Includes
#include "GUI.h"
#include "WorldManager.h"
#include "AudioManager.h"
#include "Spawner.h"

// Forward Declaration
void Start();
void Update();
void Render();

void InitWorldView();
void InitUI();

void CenterViewsToSprite(sf::Sprite _object);

void Test_AddItemToInv(CBlock::BLOCKTYPE _type);
void Test_AddItemToInv(CBlock::BLOCKTYPE _type, CBlock::BOWTYPE _bowType);
void Test_AddItemToInv(CBlock::BLOCKTYPE _type, CBlock::PROJECTILETYPE _ProjectileType);
bool PickupItemOnGround();

void InitShaders();


void InitGameOver();
void GameOverScreen();

// Mouse
sf::Vector2f MousePos;

// Font
sf::Font m_Font;

// Main Render Window
sf::RenderWindow* m_RenderWindow;
sf::Event m_Event;

// World
CWorldManager* m_WorldManager;

// b2World
b2Vec2 m_Gravity(0.0f, 20.0f);
b2World m_World(m_Gravity);

// Main View / Camera
sf::View m_WorldView;
sf::View m_UIView;

// UI
GUI* m_GUI;

// Player
CPlayer* m_Player;

// Close App?
bool m_bClose = false;

// Audio Manager
CAudioManager* m_AudioManager;

// Block Pointer If Needed To Make Stuff
CBlock* m_Block;

// Block Pointer If Needed To Make Stuff
CChest* m_Chest;

// Block Pointer If Needed To Make Stuff
CDoor* m_Door;

// Block Pointer If Needed To Make Stuff
CPickaxe* m_Pickaxe;

CTextureMaster* m_TextureMaster;

sf::Clock m_DeathTimer;
sf::Clock m_InventoryClickTimer;

Spawner* m_SlimeSpawner;
std::list<Spawner> m_SlimeSpawners;

sf::Shader m_CoreShader;
sf::Shader m_SurfaceShader;
sf::Shader m_ShaderMiniMap;
sf::Shader m_TourchShader;

Bow* m_Bow;

// Gameover screen variables
float m_PlayerRespawnTime = 5;
sf::Clock m_FadeTimer;
sf::RectangleShape m_FadeScreen;
sf::Text m_GameOverText;

/// <summary>
/// 
/// </summary>
/// <returns></returns>
int main()
{
	// Render Window Creation
	if (m_RenderWindow == nullptr)
	{
		// Render Window Settings
		sf::ContextSettings m_Settings;
		m_Settings.antialiasingLevel = 2;

		m_RenderWindow = new sf::RenderWindow(sf::VideoMode(Utils::WINDOWWIDTH, Utils::WINDOWHEIGHT), "Planetary", sf::Style::Default, m_Settings);
		m_RenderWindow->setVerticalSyncEnabled(true);
		m_RenderWindow->setKeyRepeatEnabled(false);

		// Window Icon
		sf::Image icon;
		icon.loadFromFile("Images/Chest.png");
		m_RenderWindow->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
	
	//
	Start();
	Update();
	//

	//// Save
	//std::list<CBlock>::iterator itr;
	//std::ofstream output("ChunkPositions.txt");
	//output.open("ChunkPositions.txt");

	//for (itr = m_Chunk.begin(); itr != m_Chunk.end(); itr++)
	//{
	//	output << itr->GetShape().getPosition().x;
	//	output << ",";
	//	output << itr->GetShape().getPosition().y;
	//	output << std::endl;
	//}
	//output.close();

	m_SlimeSpawners.clear();

	delete m_TextureMaster;
	delete m_GUI;
	delete m_Player;
	delete m_RenderWindow;
	delete m_WorldManager;
	delete m_AudioManager;
	m_Chest = nullptr;
	m_AudioManager = nullptr;
	m_SlimeSpawner = nullptr;
	m_TextureMaster = nullptr;
	m_Pickaxe = nullptr;
	m_Door = nullptr;
	m_WorldManager = nullptr;
	m_GUI = nullptr;
	m_Player = nullptr;
	m_RenderWindow = nullptr;
	m_Block = nullptr;
	m_Bow = nullptr;

	//
	std::cout << "Return Main" << std::endl;
	return 0;
	//
}

/// <summary>
/// 
/// </summary>
void Start()
{
	InitShaders();
	InitGameOver();

	m_TextureMaster = new CTextureMaster();
	m_Event = sf::Event();

	// UI
	m_GUI = new GUI();

	// Music
	m_AudioManager = new CAudioManager();

	// Player
	m_Player = new CPlayer(m_RenderWindow, m_World, Utils::m_Scale, m_AudioManager, m_TextureMaster);
	m_Player->Start();

	// Map
	m_WorldManager = new CWorldManager(m_RenderWindow, m_Player, m_World, m_GUI, &m_CoreShader, &m_SurfaceShader, &m_TourchShader);
	m_WorldManager->Start(m_TextureMaster);

	m_SlimeSpawner = new Spawner(m_AudioManager, m_RenderWindow, m_World, m_TextureMaster, Utils::m_Scale, 16400,-3100, m_Player, CEnemy::ENEMYTYPE::SLIME, &m_CoreShader, &m_TourchShader, m_WorldManager);
	m_SlimeSpawner->ToggleSpawning();
	m_SlimeSpawner->SetSpawnCount(2);
	m_SlimeSpawners.push_back(*m_SlimeSpawner);
	m_SlimeSpawner = nullptr;

	m_SlimeSpawner = new Spawner(m_AudioManager, m_RenderWindow, m_World, m_TextureMaster, Utils::m_Scale, -16400, -3100, m_Player, CEnemy::ENEMYTYPE::SLIME, &m_CoreShader, &m_TourchShader, m_WorldManager);
	m_SlimeSpawner->ToggleSpawning();
	m_SlimeSpawner->SetSpawnCount(2);
	m_SlimeSpawners.push_back(*m_SlimeSpawner);
	m_SlimeSpawner = nullptr;

	m_SlimeSpawner = new Spawner(m_AudioManager, m_RenderWindow, m_World, m_TextureMaster, Utils::m_Scale, 0, -3100, m_Player, CEnemy::ENEMYTYPE::SLIME, &m_CoreShader, &m_TourchShader, m_WorldManager);
	m_SlimeSpawner->ToggleSpawning();
	m_SlimeSpawner->SetSpawnCount(2);
	m_SlimeSpawners.push_back(*m_SlimeSpawner);
	m_SlimeSpawner = nullptr;

	// Init UI
	InitUI();

	// Init World
	InitWorldView();

	// Center All Views To Player
	CenterViewsToSprite(m_Player->GetShape());
	
	m_Door = nullptr;
	m_Pickaxe = nullptr;
	m_Block = nullptr;
	m_bClose = false;

	// Already Has A Pickaxe Somehow?
	m_GUI->InitHotBarScrolling(m_Player);
}

/// <summary>
/// 
/// </summary>
void Update()
{
	while (m_RenderWindow->isOpen())
	{
		MousePos = m_RenderWindow->mapPixelToCoords((sf::Mouse::getPosition(*m_RenderWindow)), m_WorldView);
		
		//
		// Polled Update
		//
		while (m_RenderWindow->pollEvent(m_Event))
		{
			// Regained Focus
			if (m_Event.type == sf::Event::GainedFocus)
			{
				break;
			}

			// Click Out Of Window
			if (m_Event.type == sf::Event::LostFocus)
			{
				if (m_Player != nullptr)
				{
					if (!m_Player->m_bInventoryOpen)
					{
						m_Player->ToggleInventoryUI(m_WorldManager->m_Chests);
					}
				}
				break;
			}

			// Exit
			if (m_Event.type == sf::Event::Closed)
			{
				if (m_Player != nullptr)
				{
					m_Player->OutPutInventoryToFile();
				}

				m_RenderWindow->close();
				m_bClose = true;
				break;
			}

			// Resize
			if (m_Event.type == sf::Event::Resized)
			{
				break;
			}

			// General Key Pressed
			if (m_Event.type == sf::Event::KeyPressed)
			{
				if (m_Player != nullptr)
				{
					m_Player->Movement(m_Event);

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && !m_GUI->bPlayerIsMovingAnItem(m_Player))
					{
						m_Player->ToggleInventoryUI(m_WorldManager->m_Chests);
					}

					// NUMPAD INVENTORYCONTROL
					// Numpad9 = BOW			// Numpad3 = DIAMOND 		// Numpad6 = DIAMOND ORE
					// Numpad1 = IRON INGOT		// Numpad4 = IRON ORE		// Numpad7 = WOOD
					// Numpad2 = GOLD INGOT		// Numpad5 = GOLD ORE		// Numpad8 = STONE
					if (true)
					{
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0))
						{
							Test_AddItemToInv(CBlock::BLOCKTYPE::PURPLEINGOT);
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
						{
							Test_AddItemToInv(CBlock::BLOCKTYPE::WORKBENCH);
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
						{
							Test_AddItemToInv(CBlock::BLOCKTYPE::GOLDENINGOT);
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
						{
							Test_AddItemToInv(CBlock::BLOCKTYPE::COALORE);
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
						{
							Test_AddItemToInv(CBlock::BLOCKTYPE::IRONORE);
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
						{
							Test_AddItemToInv(CBlock::BLOCKTYPE::GOLDORE);
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
						{
							Test_AddItemToInv(CBlock::BLOCKTYPE::DIAMONDORE);
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7))
						{
							Test_AddItemToInv(CBlock::BLOCKTYPE::PROJECTILE, CBlock::PROJECTILETYPE::CURSEDARROW);
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
						{
							Test_AddItemToInv(CBlock::BLOCKTYPE::BOW, CBlock::BOWTYPE::GREEN);
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9))
						{
							Test_AddItemToInv(CBlock::BLOCKTYPE::BOW, CBlock::BOWTYPE::OBSIDIAN);
						}
					}

					break;
				}
			}

			// Mouse Wheel Scroll
			if (m_Event.type == sf::Event::MouseWheelScrolled)
			{
				if (m_Player != nullptr)
				{
					m_GUI->HotBarScrolling(m_Event, m_Player);
					break;
				}
			}
			
			// Right On Press Mouse
			if (m_Player != nullptr)
			{
				if (m_Player->bInventoryOpen() && sf::Mouse::isButtonPressed(sf::Mouse::Right) && m_Event.type == sf::Event::MouseButtonPressed)
				{
					m_GUI->DropCurrentlyHeldItem(m_Player, m_Event);
					break;
				}
			}

			// General On Press Mouse
			if (m_Event.type == sf::Event::MouseButtonPressed)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					if (m_GUI != nullptr && m_SlimeSpawners.size() > 0)
					{
						for (std::list<Spawner>::iterator spit = m_SlimeSpawners.begin(); spit != m_SlimeSpawners.end(); spit++)
						{
							if (spit->m_Shape.getGlobalBounds().contains(MousePos))
							{
								spit->ToggleSpawning();
								break;
							}
						}
					}
				}

				m_GUI->ClickedItemInInventory(m_Event, m_Player, m_WorldManager->m_Chests);

				break;
			}

			if (m_Player != nullptr)
			{
				m_GUI->LetGoOfItemInInventory(m_RenderWindow, m_UIView, m_WorldView, m_Event, m_Player);
			}

			break;
		}

		//
		// UnPolled Update
		//
		if (!m_bClose)
		{
			for (Spawner& spawner : m_SlimeSpawners)
			{
				spawner.Update();
			}

			if (m_AudioManager != nullptr)
			{
				m_AudioManager->PlayMusic();
				m_AudioManager->PlayUnderGroundMusic();
			}

			// Player Exists
			if (m_Player != nullptr && m_WorldManager != nullptr && m_GUI != nullptr)
			{
				// Centre View To Player
				CenterViewsToSprite(m_Player->GetShape());

				if (m_AudioManager != nullptr)
				{
					if (m_Player->GetShape().getPosition().y >= 1000 && m_Player->GetShape().getPosition().y < 1200)
					{
						m_AudioManager->m_MusicLevel = 4;
					}
					else if (m_Player->GetShape().getPosition().y >= 1200 && m_Player->GetShape().getPosition().y < 1400)
					{
						m_AudioManager->m_MusicLevel = 3;
					}
					else if (m_Player->GetShape().getPosition().y >= 1400 && m_Player->GetShape().getPosition().y < 1600)
					{
						m_AudioManager->m_MusicLevel = 2;
					}
					else if (m_Player->GetShape().getPosition().y >= 1600)
					{
						m_AudioManager->m_MusicLevel = 1;
					}
					else
					{
						m_AudioManager->m_MusicLevel = 5;
					}
				}

				// Player Update And Movement
				m_Player->Interact(m_WorldManager->m_Furnaces, m_WorldManager->m_Chests, m_WorldManager->m_Doors, m_WorldManager->m_Chunk, m_Event, m_GUI->m_MousePos, m_WorldManager->m_WorkBenches);
				m_Player->Update(MousePos);
				m_Player->Movement();

				// Player Dies
				if (m_Player->GetCurrentHP() <= 0.0f && m_Player != nullptr)
				{
					m_AudioManager->PlayPlayerDeath();

					// Drop Chest
					if (true)
					{
						m_Chest = new CChest(m_RenderWindow, m_World, Utils::m_Scale, m_Player->GetShape().getPosition().x, m_Player->GetShape().getPosition().y);
						
						m_Chest->m_Inventory = m_Player->m_InventoryMap;
						m_Chest->m_InventoryStackValues = m_Player->m_InventoryStackValues;
						m_WorldManager->m_Chests.push_back(*m_Chest);
						m_Chest = nullptr;
					}

					delete m_Player;
					m_Player = nullptr;
					m_WorldManager->InitPointer(m_Player);
					for (Spawner& spawner : m_SlimeSpawners)
					{
						spawner.LoosePlayer();
					}
					

					m_FadeScreen.setPosition(m_WorldView.getCenter());
					m_GameOverText.setPosition(m_WorldView.getCenter());
					m_FadeTimer.restart();
					m_DeathTimer.restart();
				}
			}

			// Players Dead
			else if (m_Player == nullptr)
			{
				GameOverScreen();

				if (m_DeathTimer.getElapsedTime().asSeconds() >= m_PlayerRespawnTime)
				{
					// Player
					m_Player = new CPlayer(m_RenderWindow, m_World, Utils::m_Scale, m_AudioManager, m_TextureMaster);
					m_Player->Start();

					m_WorldManager->InitPointer(m_Player);

					// Already Has A Pickaxe Selected
					m_GUI->InitHotBarScrolling(m_Player);

					//m_WorldManager = new CWorldManager(m_RenderWindow, m_Player, m_World,m_GUI, &m_CoreShader, &m_SurfaceShader);
					//m_WorldManager->Start(m_TextureMaster);

					for (Spawner& spawner : m_SlimeSpawners)
					{
						spawner.SetPlayer(m_Player);
					}

					
				}
			}

			if (m_WorldManager != nullptr)
			{
				// MousePosBox Position
				m_WorldManager->Update(MousePos);

				// World Step
				m_World.Step(1 / 60.0f, 60, 60);
			}

			//
			// Main Render
			//
			Render();
		}
	}
}

/// <summary>
/// 
/// </summary>
void Render()
{
	m_RenderWindow->clear();
	/////////////////////////////////////

	// Sky	
	m_RenderWindow->setView(m_WorldView);

	if (m_WorldManager != nullptr)
	{
		m_WorldManager->CreateSkyChunk(m_TextureMaster);
		m_WorldManager->Render();
	}

	if (m_SlimeSpawners.size() > 0)
	{
		for (Spawner& spawner : m_SlimeSpawners)
		{
			spawner.Render();
		}
	}

	// Player
	if (m_Player != nullptr && m_WorldManager != nullptr)
	{
		m_Player->Render();

		// UI
		m_RenderWindow->setView(m_UIView);
		/////////////////////////////////////

		m_GUI->HealthUI(m_RenderWindow, m_Player, m_TextureMaster);
		m_GUI->MiniMapUI(m_RenderWindow, m_WorldManager->m_Chunk, m_WorldManager->m_SkyChunk, m_Player, &m_SurfaceShader);

		m_GUI->CraftingUI(m_RenderWindow, m_Player, m_TextureMaster, m_UIView);
		m_GUI->InventoryUI(m_RenderWindow, m_Player, m_UIView, m_WorldView, m_Event, m_TextureMaster, m_WorldManager->m_Chests);
		m_GUI->ChestUI(m_RenderWindow, m_Player, m_UIView, m_WorldView, m_Event, m_TextureMaster, m_WorldManager->m_Chests);

		m_GUI->Render(m_RenderWindow, m_Player, m_WorldView, m_UIView, m_WorldManager->m_Chests);
	}

	m_RenderWindow->draw(m_FadeScreen);
	m_RenderWindow->draw(m_GameOverText);

	/////////////////////////////////////
	m_RenderWindow->display();
	m_RenderWindow->setView(m_WorldView);
}

/// <summary>
/// 
/// </summary>
void InitWorldView()
{
	m_WorldView = sf::View(sf::Vector2f(0.0f,0.0f), sf::Vector2f(m_RenderWindow->getSize().x, m_RenderWindow->getSize().y));
	m_WorldView.zoom(2.5f);
	m_RenderWindow->setView(m_WorldView);
}

/// <summary>
/// 
/// </summary>
void InitUI()
{
	// GUI
	m_GUI->SetPlayer(m_Player);
	m_GUI->InitMousePosSprite(m_TextureMaster);
	m_GUI->InitMiniMap(m_RenderWindow, m_TextureMaster);
	m_GUI->InitHealthUI(m_Player);
	m_GUI->InitCraftingUI(m_TextureMaster);
	m_GUI->InitInventoryUI(m_Player, m_RenderWindow, m_TextureMaster);
	m_GUI->InitChestUI(m_RenderWindow,m_TextureMaster);

	// UI View
	m_UIView = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(m_RenderWindow->getSize().x, m_RenderWindow->getSize().y));
	m_RenderWindow->setView(m_UIView);
}

/// <summary>
/// 
/// </summary>
/// <param name="_object"></param>
void CenterViewsToSprite(sf::Sprite _object)
{
	m_WorldView.setCenter(_object.getPosition());
	m_UIView.setCenter(_object.getPosition());
	m_RenderWindow->setView(m_WorldView);
}

void Test_AddItemToInv(CBlock::BLOCKTYPE _type)
{
	switch (_type)
	{
	case CBlock::BLOCKTYPE::PICKAXE:
	{
		m_Block = new CPickaxe();
		m_Pickaxe->m_Type = CBlock::BLOCKTYPE::PICKAXE;
		m_Pickaxe->m_PickType = CBlock::PICKAXETYPE::DIAMOND;
		m_Player->AddItemToInventory(m_Pickaxe, false);
		m_Pickaxe = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::DOOR:
	{
		m_Door = new CDoor();
		m_Player->AddItemToInventory(m_Door, false);
		m_Door = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::CHEST:
	{
		m_Block = new CBlock(m_TextureMaster->m_Chest, CBlock::BLOCKTYPE::CHEST);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::DIRT:
	{
		m_Block = new CBlock(m_TextureMaster->m_Dirt, CBlock::BLOCKTYPE::DIRT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::STONE:
	{
		m_Block = new CBlock(m_TextureMaster->m_Stone, CBlock::BLOCKTYPE::STONE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::WOOD:
	{
		m_Block = new CBlock(m_TextureMaster->m_Wood, CBlock::BLOCKTYPE::WOOD);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::PLANKS:
	{
		m_Block = new CBlock(m_TextureMaster->m_Planks, CBlock::BLOCKTYPE::PLANKS);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::SAND:
	{
		m_Block = new CBlock(m_TextureMaster->m_Sand, CBlock::BLOCKTYPE::SAND);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::MOSSYBRICK:
	{
		m_Block = new CBlock(m_TextureMaster->m_MossyBrick, CBlock::BLOCKTYPE::MOSSYBRICK);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GRASS:
	{
		m_Block = new CBlock(m_TextureMaster->m_Grass, CBlock::BLOCKTYPE::GRASS);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::LEAVES:
	{
		m_Block = new CBlock(m_TextureMaster->m_Leaves, CBlock::BLOCKTYPE::LEAVES);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::FURNACE:
	{
		m_Block = new CBlock(m_TextureMaster->m_Furnace, CBlock::BLOCKTYPE::FURNACE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::IRONORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_IronOre, CBlock::BLOCKTYPE::IRONORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_GoldOre, CBlock::BLOCKTYPE::GOLDORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::DIAMONDORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_DiamondOre, CBlock::BLOCKTYPE::DIAMONDORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::PURPLEORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_PurpleOre, CBlock::BLOCKTYPE::PURPLEORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDENORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_GoldenOre, CBlock::BLOCKTYPE::GOLDENORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::COALORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_Coal, CBlock::BLOCKTYPE::COALORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::OBSIDIAN:
	{
		m_Block = new CBlock(m_TextureMaster->m_Obsidian, CBlock::BLOCKTYPE::OBSIDIAN);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::BOW:
	{
		m_Bow = new Bow(CBlock::BOWTYPE::BASIC);
		m_Player->AddItemToInventory(m_Bow, false);
		m_Bow = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::IRONINGOT:
	{
		m_Block = new CBlock(m_TextureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDINGOT:
	{
		m_Block = new CBlock(m_TextureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::DIAMOND:
	{
		m_Block = new CBlock(m_TextureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::PURPLEINGOT:
	{
		m_Block = new CBlock(m_TextureMaster->m_PurpleIngot, CBlock::BLOCKTYPE::PURPLEINGOT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDENINGOT:
	{
		m_Block = new CBlock(m_TextureMaster->m_GoldenIngot, CBlock::BLOCKTYPE::GOLDENINGOT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::WORKBENCH:
	{
		m_Block = new CBlock(m_TextureMaster->m_WorkBench, CBlock::BLOCKTYPE::WORKBENCH);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	default:
	{
		m_Block = new CBlock(m_TextureMaster->m_Dirt, CBlock::BLOCKTYPE::DIRT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	}
}

void Test_AddItemToInv(CBlock::BLOCKTYPE _type, CBlock::BOWTYPE _bowType)
{
	switch (_type)
	{
	case CBlock::BLOCKTYPE::PICKAXE:
	{
		m_Block = new CPickaxe();
		m_Pickaxe->m_Type = CBlock::BLOCKTYPE::PICKAXE;
		m_Pickaxe->m_PickType = CBlock::PICKAXETYPE::DIAMOND;
		m_Player->AddItemToInventory(m_Pickaxe, false);
		m_Pickaxe = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::DOOR:
	{
		m_Door = new CDoor();
		m_Player->AddItemToInventory(m_Door, false);
		m_Door = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::CHEST:
	{
		m_Block = new CBlock(m_TextureMaster->m_Chest, CBlock::BLOCKTYPE::CHEST);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::DIRT:
	{
		m_Block = new CBlock(m_TextureMaster->m_Dirt, CBlock::BLOCKTYPE::DIRT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::STONE:
	{
		m_Block = new CBlock(m_TextureMaster->m_Stone, CBlock::BLOCKTYPE::STONE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::WOOD:
	{
		m_Block = new CBlock(m_TextureMaster->m_Wood, CBlock::BLOCKTYPE::WOOD);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::PLANKS:
	{
		m_Block = new CBlock(m_TextureMaster->m_Planks, CBlock::BLOCKTYPE::PLANKS);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::SAND:
	{
		m_Block = new CBlock(m_TextureMaster->m_Sand, CBlock::BLOCKTYPE::SAND);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::MOSSYBRICK:
	{
		m_Block = new CBlock(m_TextureMaster->m_MossyBrick, CBlock::BLOCKTYPE::MOSSYBRICK);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GRASS:
	{
		m_Block = new CBlock(m_TextureMaster->m_Grass, CBlock::BLOCKTYPE::GRASS);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::LEAVES:
	{
		m_Block = new CBlock(m_TextureMaster->m_Leaves, CBlock::BLOCKTYPE::LEAVES);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::FURNACE:
	{
		m_Block = new CBlock(m_TextureMaster->m_Furnace, CBlock::BLOCKTYPE::FURNACE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::IRONORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_IronOre, CBlock::BLOCKTYPE::IRONORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_GoldOre, CBlock::BLOCKTYPE::GOLDORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::DIAMONDORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_DiamondOre, CBlock::BLOCKTYPE::DIAMONDORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::PURPLEORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_PurpleOre, CBlock::BLOCKTYPE::PURPLEORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDENORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_GoldenOre, CBlock::BLOCKTYPE::GOLDENORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::COALORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_Coal, CBlock::BLOCKTYPE::COALORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::OBSIDIAN:
	{
		m_Block = new CBlock(m_TextureMaster->m_Obsidian, CBlock::BLOCKTYPE::OBSIDIAN);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::BOW:
	{
		switch (_bowType)
		{
		case CBlock::BOWTYPE::BASIC:
			m_Bow = new Bow(CBlock::BOWTYPE::BASIC);
			m_Player->AddItemToInventory(m_Bow, false);
			m_Bow = nullptr;
			break;
		case CBlock::BOWTYPE::OBSIDIAN:
			m_Bow = new Bow(CBlock::BOWTYPE::OBSIDIAN);
			m_Player->AddItemToInventory(m_Bow, false);
			m_Bow = nullptr;
			break;
		case CBlock::BOWTYPE::ICE:
			m_Bow = new Bow(CBlock::BOWTYPE::ICE);
			m_Player->AddItemToInventory(m_Bow, false);
			m_Bow = nullptr;
			break;
		case CBlock::BOWTYPE::BLOOD:
			m_Bow = new Bow(CBlock::BOWTYPE::BLOOD);
			m_Player->AddItemToInventory(m_Bow, false);
			m_Bow = nullptr;
			break;
		case CBlock::BOWTYPE::IRON:
			m_Bow = new Bow(CBlock::BOWTYPE::IRON);
			m_Player->AddItemToInventory(m_Bow, false);
			m_Bow = nullptr;
			break;
		case CBlock::BOWTYPE::GOLDEN:
			m_Bow = new Bow(CBlock::BOWTYPE::GOLDEN);
			m_Player->AddItemToInventory(m_Bow, false);
			m_Bow = nullptr;
			break;
		case CBlock::BOWTYPE::PURPLE:
			m_Bow = new Bow(CBlock::BOWTYPE::PURPLE);
			m_Player->AddItemToInventory(m_Bow, false);
			m_Bow = nullptr;
			break;
		case CBlock::BOWTYPE::GREEN:
			m_Bow = new Bow(CBlock::BOWTYPE::GREEN);
			m_Player->AddItemToInventory(m_Bow, false);
			m_Bow = nullptr;
			break;
		default:
			m_Bow = new Bow(CBlock::BOWTYPE::BASIC);
			m_Player->AddItemToInventory(m_Bow, false);
			m_Bow = nullptr;
			break;
		}
		break;
	}
	case CBlock::BLOCKTYPE::IRONINGOT:
	{
		m_Block = new CBlock(m_TextureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDINGOT:
	{
		m_Block = new CBlock(m_TextureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::DIAMOND:
	{
		m_Block = new CBlock(m_TextureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::PURPLEINGOT:
	{
		m_Block = new CBlock(m_TextureMaster->m_PurpleIngot, CBlock::BLOCKTYPE::PURPLEINGOT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDENINGOT:
	{
		m_Block = new CBlock(m_TextureMaster->m_GoldenIngot, CBlock::BLOCKTYPE::GOLDENINGOT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::WORKBENCH:
	{
		m_Block = new CBlock(m_TextureMaster->m_WorkBench, CBlock::BLOCKTYPE::WORKBENCH);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	default:
	{
		m_Block = new CBlock(m_TextureMaster->m_Dirt, CBlock::BLOCKTYPE::DIRT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	}
}

void Test_AddItemToInv(CBlock::BLOCKTYPE _type, CBlock::PROJECTILETYPE _ProjectileType)
{
	switch (_type)
	{
	case CBlock::BLOCKTYPE::PICKAXE:
	{
		m_Block = new CPickaxe();
		m_Pickaxe->m_Type = CBlock::BLOCKTYPE::PICKAXE;
		m_Pickaxe->m_PickType = CBlock::PICKAXETYPE::DIAMOND;
		m_Player->AddItemToInventory(m_Pickaxe, false);
		m_Pickaxe = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::DOOR:
	{
		m_Door = new CDoor();
		m_Player->AddItemToInventory(m_Door, false);
		m_Door = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::CHEST:
	{
		m_Block = new CBlock(m_TextureMaster->m_Chest, CBlock::BLOCKTYPE::CHEST);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::DIRT:
	{
		m_Block = new CBlock(m_TextureMaster->m_Dirt, CBlock::BLOCKTYPE::DIRT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::STONE:
	{
		m_Block = new CBlock(m_TextureMaster->m_Stone, CBlock::BLOCKTYPE::STONE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::WOOD:
	{
		m_Block = new CBlock(m_TextureMaster->m_Wood, CBlock::BLOCKTYPE::WOOD);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::PLANKS:
	{
		m_Block = new CBlock(m_TextureMaster->m_Planks, CBlock::BLOCKTYPE::PLANKS);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::SAND:
	{
		m_Block = new CBlock(m_TextureMaster->m_Sand, CBlock::BLOCKTYPE::SAND);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::MOSSYBRICK:
	{
		m_Block = new CBlock(m_TextureMaster->m_MossyBrick, CBlock::BLOCKTYPE::MOSSYBRICK);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GRASS:
	{
		m_Block = new CBlock(m_TextureMaster->m_Grass, CBlock::BLOCKTYPE::GRASS);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::LEAVES:
	{
		m_Block = new CBlock(m_TextureMaster->m_Leaves, CBlock::BLOCKTYPE::LEAVES);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::FURNACE:
	{
		m_Block = new CBlock(m_TextureMaster->m_Furnace, CBlock::BLOCKTYPE::FURNACE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::IRONORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_IronOre, CBlock::BLOCKTYPE::IRONORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_GoldOre, CBlock::BLOCKTYPE::GOLDORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::DIAMONDORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_DiamondOre, CBlock::BLOCKTYPE::DIAMONDORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::PURPLEORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_PurpleOre, CBlock::BLOCKTYPE::PURPLEORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDENORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_GoldenOre, CBlock::BLOCKTYPE::GOLDENORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::COALORE:
	{
		m_Block = new CBlock(m_TextureMaster->m_Coal, CBlock::BLOCKTYPE::COALORE);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::OBSIDIAN:
	{
		m_Block = new CBlock(m_TextureMaster->m_Obsidian, CBlock::BLOCKTYPE::OBSIDIAN);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::BOW:
	{
		m_Bow = new Bow(CBlock::BOWTYPE::BASIC);
		m_Player->AddItemToInventory(m_Bow, false);
		m_Bow = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::IRONINGOT:
	{
		m_Block = new CBlock(m_TextureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDINGOT:
	{
		m_Block = new CBlock(m_TextureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::DIAMOND:
	{
		m_Block = new CBlock(m_TextureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::PURPLEINGOT:
	{
		m_Block = new CBlock(m_TextureMaster->m_PurpleIngot, CBlock::BLOCKTYPE::PURPLEINGOT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDENINGOT:
	{
		m_Block = new CBlock(m_TextureMaster->m_GoldenIngot, CBlock::BLOCKTYPE::GOLDENINGOT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::WORKBENCH:
	{
		m_Block = new CBlock(m_TextureMaster->m_WorkBench, CBlock::BLOCKTYPE::WORKBENCH);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	case CBlock::BLOCKTYPE::PROJECTILE:
	{
		CProjectile* temp = nullptr;
		switch (_ProjectileType)
		{
		case CBlock::PROJECTILETYPE::ARROW:
			temp = new CProjectile(CBlock::PROJECTILETYPE::ARROW);
			m_Player->AddItemToInventory(temp);
			break;
		case CBlock::PROJECTILETYPE::FIREARROW:
			temp = new CProjectile(CBlock::PROJECTILETYPE::FIREARROW);
			m_Player->AddItemToInventory(temp);
			break;
		case CBlock::PROJECTILETYPE::CURSEDARROW:
			temp = new CProjectile(CBlock::PROJECTILETYPE::CURSEDARROW);
			m_Player->AddItemToInventory(temp);
			break;
		case CBlock::PROJECTILETYPE::POISONARROW:
			temp = new CProjectile(CBlock::PROJECTILETYPE::POISONARROW);
			m_Player->AddItemToInventory(temp);
			break;
		default:
			temp = new CProjectile(CBlock::PROJECTILETYPE::ARROW);
			m_Player->AddItemToInventory(temp);
			break;
		}

		temp = nullptr;
		break;
	}
	default:
	{
		m_Block = new CBlock(m_TextureMaster->m_Dirt, CBlock::BLOCKTYPE::DIRT);
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
		break;
	}
	}
}

bool PickupItemOnGround()
{
	//b2Contact* contact;
	//bool sensorA;
	//bool sensorB;
	//for (contact = m_World.GetContactList(); contact; contact = contact->GetNext())
	//{
	//	b2Fixture* fixtureA = contact->GetFixtureA();
	//	b2Fixture* fixtureB = contact->GetFixtureB();

	//	//make sure only one of the fixtures was a sensor
	//	sensorA = fixtureA->IsSensor();
	//	sensorB = fixtureB->IsSensor();
	//	if ((sensorA || sensorB) && (fixtureA->GetBody() == m_Player->m_Body || fixtureB->GetBody() == m_Player->m_Body))
	//	{
	//		std::list<Items>::iterator it;
	//		for (it = m_Items->begin(); it != m_Items->end(); it++)
	//		{
	//			if (it->GetBody() == fixtureA || it->GetBody() == fixtureB)
	//			{
	//				break;
	//			}
	//		}
	//		m_Player->AddItemToInventory(it);

	//		fixtureA = nullptr;
	//		fixtureB = nullptr;
	//		contact = nullptr;
	//		return true;
	//	}

	//	fixtureA = nullptr;
	//	fixtureB = nullptr;
	//}
	//contact = nullptr;

	//return false;
}

void InitShaders()
{
	if (!m_CoreShader.loadFromFile("Shaders/vertex_shader.vert", "Shaders/fragment_shader.frag"))
	{
		std::cout << "Core Shader Failed To Load!" << std::endl;
	}

	if (!m_SurfaceShader.loadFromFile("Shaders/vertex_shader_surface.vert", "Shaders/fragment_shader_surface.frag"))
	{
		std::cout << "Surface Shader Failed To Load!" << std::endl;
	}

	if (!m_ShaderMiniMap.loadFromFile("Shaders/vertex_shader_UI.vert", "Shaders/fragment_shader_UI.frag"))
	{
		std::cout << "Black Shader Failed To Load!" << std::endl;
	}

	if (!m_TourchShader.loadFromFile("Shaders/vertex_shader_Tourch.vert", "Shaders/fragment_shader_Tourch.frag"))
	{
		std::cout << "Tourch Shader Failed To Load!" << std::endl;
	}
	
}

void GameOverScreen()
{
	float elapsedtime = m_FadeTimer.getElapsedTime().asSeconds() / m_PlayerRespawnTime;

	if (m_DeathTimer.getElapsedTime().asSeconds() >= m_PlayerRespawnTime)
	{
		m_FadeScreen.setFillColor(sf::Color::Transparent);
		m_GameOverText.setFillColor(sf::Color::Transparent);

	}
	else if (m_DeathTimer.getElapsedTime().asSeconds() >= m_PlayerRespawnTime / 1.5)
	{
		m_FadeScreen.setFillColor(sf::Color(0, 0, 0, 255));
		m_GameOverText.setFillColor(sf::Color(255, 0, 0, 255));

	}
	else
	{
		m_FadeScreen.setFillColor(sf::Color(0, 0, 0, elapsedtime * 255 * 1.5));
		m_GameOverText.setFillColor(sf::Color(255, 0, 0, elapsedtime * 255 * 1.5));
	}
}

void InitGameOver()
{
	m_Font.loadFromFile("Fonts/ANDYB.TTF");
	m_FadeScreen.setSize(sf::Vector2f(30000, 30000));
	m_FadeScreen.setOrigin(sf::Vector2f(15000, 15000));
	m_FadeScreen.setFillColor(sf::Color::Transparent);
	m_GameOverText.setCharacterSize(400);
	m_GameOverText.setString("You Died");
	m_GameOverText.setFont(m_Font);
	m_GameOverText.setOrigin(m_GameOverText.getGlobalBounds().width / 2, m_GameOverText.getGlobalBounds().height / 2);
	m_GameOverText.setFillColor(sf::Color::Transparent);
	m_GameOverText.setOutlineColor(sf::Color::Transparent);
}





