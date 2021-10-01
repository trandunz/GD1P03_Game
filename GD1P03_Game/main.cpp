// Non-Local Includes
#include <iostream>

// Local Includes
#include "CDebugWindow.h"

// Forward Declaration
void Start();
void Update();
void Render();

void InitWorldView();
void InitUI();

void CenterViewsToSprite(sf::Sprite _object);

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
std::list<Spawner> m_Spawners;

sf::Shader m_CoreShader;
sf::Shader m_SurfaceShader;
sf::Shader m_ShaderMiniMap;
sf::Shader m_TourchShader;

Bow* m_Bow;
CPotion* m_Potion;

// Gameover screen variables
float m_PlayerRespawnTime = 5;
sf::Clock m_FadeTimer;
sf::RectangleShape m_FadeScreen;
sf::Text m_GameOverText;

CDebugWindow* m_DebugWindow = nullptr;
bool m_DebugInFocus = true;

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

	m_Spawners.clear();

	delete m_DebugWindow;
	delete m_TextureMaster;
	delete m_GUI;
	delete m_Player;
	delete m_RenderWindow;
	delete m_WorldManager;
	delete m_AudioManager;
	m_Potion = nullptr;
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
	m_DebugWindow = nullptr;

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

	m_SlimeSpawner = new Spawner(m_AudioManager, m_RenderWindow, m_World, m_TextureMaster, Utils::m_Scale, 0, -400, m_Player, CEnemy::ENEMYTYPE::SLIME, &m_CoreShader, &m_TourchShader, m_WorldManager, true);
	m_SlimeSpawner->ToggleSpawning();
	m_SlimeSpawner->SetSpawnCount(2);
	m_Spawners.push_back(*m_SlimeSpawner);
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

	m_DebugWindow = new CDebugWindow(m_TextureMaster, m_WorldManager, m_Player, m_Spawners);
	m_DebugWindow->Start();
}

/// <summary>
/// 
/// </summary>
void Update()
{ 
	while (m_RenderWindow->isOpen())
	{
		MousePos = m_RenderWindow->mapPixelToCoords((sf::Mouse::getPosition(*m_RenderWindow)), m_WorldView);

		// PopOutMenu Update
		if (m_DebugWindow != nullptr)
		{
			m_DebugWindow->Update();
		}

		//
		// Polled Update
		//
		while (m_RenderWindow->pollEvent(m_Event))
		{
			// Regained Focus
			if (m_Event.type == sf::Event::GainedFocus)
			{
				m_DebugInFocus = false;
				break;
			}

			// Click Out Of Window
			if (m_Event.type == sf::Event::LostFocus)
			{
				m_DebugInFocus = true;
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

				m_DebugWindow->Close();
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

					break;
				}
			}

			// Mouse Wheel Scroll
			if (m_Event.type == sf::Event::MouseWheelScrolled)
			{
				if (m_Player != nullptr)
				{
					m_GUI->HotBarScrolling(m_Event, m_Player);
				}
			}
			
			// Right On Press Mouse
			if (m_Player != nullptr)
			{
				if (m_Player->bInventoryOpen() && sf::Mouse::isButtonPressed(sf::Mouse::Right) && m_Event.type == sf::Event::MouseButtonPressed)
				{
					m_GUI->DropCurrentlyHeldItem(m_Player, m_Event);
				}
			}

			// General On Press Mouse
			if (m_Event.type == sf::Event::MouseButtonPressed)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					if (m_GUI != nullptr && m_Spawners.size() > 0)
					{
						for (std::list<Spawner>::iterator spit = m_Spawners.begin(); spit != m_Spawners.end(); spit++)
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
			}
			else if (m_Player != nullptr && m_Event.type == sf::Event::MouseButtonReleased)
			{
				m_GUI->LetGoOfItemInInventory(m_RenderWindow, m_UIView, m_WorldView, m_Event, m_Player);
			}
		}

		//
		// UnPolled Update
		//
		if (!m_bClose)
		{
			for (Spawner& spawner : m_Spawners)
			{
				spawner.Update(m_WorldManager);
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
				m_Player->Interact(m_WorldManager->m_Furnaces, m_WorldManager->m_Chests, m_WorldManager->m_Doors, m_WorldManager->m_Chunk, m_Event, m_GUI->m_MousePos, m_WorldManager->m_WorkBenches, m_WorldManager->m_Tourches);
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

					// Clears The saved Inventory
					m_Player->m_InventoryMap.clear();
					m_Player->m_InventoryStackValues.clear();
					m_Player->OutPutInventoryToFile();

					delete m_Player;
					m_Player = nullptr;
					m_WorldManager->InitPointer(m_Player);
					m_DebugWindow->SetPlayer(m_Player);
					for (Spawner& spawner : m_Spawners)
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
					m_DebugWindow->SetPlayer(m_Player);

					// Already Has A Pickaxe Selected
					m_GUI->InitHotBarScrolling(m_Player);

					//m_WorldManager = new CWorldManager(m_RenderWindow, m_Player, m_World,m_GUI, &m_CoreShader, &m_SurfaceShader);
					//m_WorldManager->Start(m_TextureMaster);

					for (Spawner& spawner : m_Spawners)
					{
						spawner.SetPlayer(m_Player);
					}

					
				}
			}

			if (m_WorldManager != nullptr)
			{
				// MousePosBox Position
				m_WorldManager->Update(MousePos, m_TextureMaster);

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

	m_DebugWindow->Render();

	// Sky	
	m_RenderWindow->setView(m_WorldView);

	if (m_WorldManager != nullptr)
	{
		m_WorldManager->CreateSkyChunk(m_TextureMaster);
		m_WorldManager->Render();
	}

	if (m_Spawners.size() > 0)
	{
		for (Spawner& spawner : m_Spawners)
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
		m_GUI->StatusEffectUI(m_RenderWindow, m_Player);
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
	m_GUI->InitStatusEffectUI(m_Player);

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
	if (_object.getPosition().x < -34200 || _object.getPosition().x > 34200)
	{
		if (_object.getPosition().y < -4500 || _object.getPosition().y > 10000)
		{
			m_WorldView.setCenter(_object.getPosition().x,m_WorldView.getCenter().y);
		}
		else
		{

			m_WorldView.setCenter(m_WorldView.getCenter().x, _object.getPosition().y);
		}
	}
	else
	{
		if (_object.getPosition().y < -4500 || _object.getPosition().y > 10000)
		{
			m_WorldView.setCenter(_object.getPosition().x, m_WorldView.getCenter().y);
		}
		else
		{

			m_WorldView.setCenter(_object.getPosition());
		}
	}

	m_UIView.setCenter(_object.getPosition());
	m_RenderWindow->setView(m_WorldView);
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





