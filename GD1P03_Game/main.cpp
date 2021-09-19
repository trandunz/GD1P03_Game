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

void Test_AddDirtToInv();
bool PickupItemOnGround();

void InitShaders();

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
CDoor* m_Door;

// Block Pointer If Needed To Make Stuff
CPickaxe* m_Pickaxe;

CTextureMaster* m_TextureMaster;

sf::Clock DeathTimer;

Spawner* m_SlimeSpawner;
std::list<Spawner> m_SlimeSpawners;

sf::Shader m_CoreShader;
sf::Shader m_SurfaceShader;
sf::Shader m_ShaderMiniMap;

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
		m_Settings.antialiasingLevel = 8;

		m_RenderWindow = new sf::RenderWindow(sf::VideoMode(Utils::WINDOWWIDTH, Utils::WINDOWHEIGHT), "Planetsrary", sf::Style::Default, m_Settings);
		m_RenderWindow->setFramerateLimit(144);
		//m_RenderWindow->setVerticalSyncEnabled(true);
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

	m_TextureMaster = new CTextureMaster();
	m_Event = sf::Event();

	// UI
	m_GUI = new GUI();

	// Music
	m_AudioManager = new CAudioManager();
	m_AudioManager->PlayMusic();

	// Player
	m_Player = new CPlayer(m_RenderWindow, m_World, Utils::m_Scale, m_AudioManager, m_TextureMaster);
	m_Player->Start();

	// Map
	m_WorldManager = new CWorldManager(m_RenderWindow, m_Player, m_World, m_GUI, &m_CoreShader, &m_SurfaceShader);
	m_WorldManager->Start(m_TextureMaster);

	m_SlimeSpawner = new Spawner(m_AudioManager, m_RenderWindow, m_World, m_TextureMaster, Utils::m_Scale, 9900, 9700, m_Player, CEnemy::ENEMYTYPE::SLIME, &m_CoreShader);
	m_SlimeSpawner->ToggleSpawning();
	m_SlimeSpawner->SetSpawnCount(20);
	m_SlimeSpawners.push_back(*m_SlimeSpawner);
	m_SlimeSpawner = nullptr;

	m_SlimeSpawner = new Spawner(m_AudioManager, m_RenderWindow, m_World, m_TextureMaster, Utils::m_Scale, 5400,-4000, m_Player, CEnemy::ENEMYTYPE::SLIME, &m_CoreShader);
	m_SlimeSpawner->ToggleSpawning();
	m_SlimeSpawner->SetSpawnCount(5);
	m_SlimeSpawners.push_back(*m_SlimeSpawner);
	m_SlimeSpawner = nullptr;

	m_SlimeSpawner = new Spawner(m_AudioManager, m_RenderWindow, m_World, m_TextureMaster, Utils::m_Scale, -5400, -4000, m_Player, CEnemy::ENEMYTYPE::SLIME, &m_CoreShader);
	m_SlimeSpawner->ToggleSpawning();
	m_SlimeSpawner->SetSpawnCount(5);
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
	m_GUI->InitHotBarScrolling(m_Event, m_Player);

	// Render
	Render();
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
			}

			// Click Out Of Window
			if (m_Event.type == sf::Event::LostFocus)
			{
				if (m_Player != nullptr)
				{
					m_Player->bCanMove(false);
				}
			}

			// Exit
			if (m_Event.type == sf::Event::Closed)
			{
				m_RenderWindow->close();
				m_bClose = true;
				break;
			}

			// Resize
			if (m_Event.type == sf::Event::Resized)
			{
			}

			// General Key Pressed
			if (m_Event.type == sf::Event::KeyPressed)
			{
				if (m_Player != nullptr)
				{
					m_Player->Movement(m_Event);

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
					{
						m_Player->ToggleInventoryUI();
					}
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
				// b2World Step & MousePosBox Position
				m_WorldManager->Update(m_Event, MousePos);
			}
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

			// Player Exists
			if (m_Player != nullptr)
			{
				// Centre View To Player
				CenterViewsToSprite(m_Player->GetShape());

				// Player Update And Movement
				m_Player->Interact(m_WorldManager->m_Furnaces, m_WorldManager->m_Chests, m_WorldManager->m_Doors, m_WorldManager->m_Chunk, m_Event, m_GUI->m_MousePos);
				m_Player->Update(MousePos);
				m_Player->Movement();

				// Reset Players SFML Sprite To Box2D Body
				m_Player->ResetSpritePos();

				// b2World Step & MousePosBox Position
				m_WorldManager->Update(m_Event, MousePos);

				if (m_Player->GetCurrentHP() <= 0.0f && m_Player != nullptr)
				{
					m_AudioManager->PlayPlayerDeath();
					DeathTimer.restart();
					delete m_Player;
					m_Player = nullptr;

					for (Spawner& spawner : m_SlimeSpawners)
					{
						spawner.LoosePlayer();
					}
					m_WorldManager->InitPointer(nullptr);
					//m_bClose = true;
				}

				if (m_WorldManager != nullptr)
				{
					// World Step
					m_World.Step(1 / 60.0f, 60, 60);
 
					//
					// Main Render
					//
					Render();
				}
			}
			else if (m_Player == nullptr)
			{
				if (DeathTimer.getElapsedTime().asSeconds() >= 3)
				{
					// Player
					m_Player = new CPlayer(m_RenderWindow, m_World, Utils::m_Scale, m_AudioManager, m_TextureMaster);
					m_Player->Start();
					m_WorldManager->InitPointer(m_Player);

					for (Spawner& spawner : m_SlimeSpawners)
					{
						spawner.SetPlayer(m_Player);
					}

					// Already Has A Pickaxe Somehow?
					m_GUI->InitHotBarScrolling(m_Event, m_Player);
				}
			}
			
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

	m_WorldManager->CreateSkyChunk();

	m_WorldManager->Render();

	// Player
	if (m_Player != nullptr)
	{
		for (Spawner& spawner : m_SlimeSpawners)
		{
			spawner.Render();
		}

		m_Player->Render();

		// UI
		m_RenderWindow->setView(m_UIView);
		/////////////////////////////////////

		m_GUI->HealthUI(m_RenderWindow, m_Player, m_TextureMaster);
		m_GUI->MiniMapUI(m_RenderWindow, m_WorldManager->m_Chunk, m_WorldManager->m_SkyChunk, m_Player, &m_SurfaceShader, &m_ShaderMiniMap);

		m_GUI->CraftingUI(m_RenderWindow, m_Player, m_TextureMaster, m_UIView);
		m_GUI->InventoryUI(m_RenderWindow, m_Player, m_UIView, m_WorldView, m_Event, m_TextureMaster);
		
		m_GUI->Render(m_RenderWindow, m_Player, m_WorldView, m_UIView);
	}

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
	m_WorldView.zoom(3.0f);
	m_RenderWindow->setView(m_WorldView);

	sf::View m_BoardView = sf::View(sf::Vector2f(0, 0), sf::Vector2f(640, 640));
	m_BoardView.setCenter(320, 320);
	m_RenderWindow->setView(m_BoardView);
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

void Test_AddDirtToInv()
{
	m_Pickaxe = new CPickaxe();
	m_Pickaxe->m_Type = CBlock::BLOCKTYPE::PICKAXE;
	m_Player->AddItemToInventory(m_Pickaxe, 5);
	m_Pickaxe = nullptr;
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
	
}





