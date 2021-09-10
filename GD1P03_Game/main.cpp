// Non-Local Includes
#include <iostream>

// Local Includes
#include "GUI.h"
#include "WorldManager.h"
#include "AudioManager.h"

// Forward Declaration
void Start();
void Update();
void Render();

void InitWorldView();
void InitUI();

void CenterViewsToSprite(sf::Sprite _object);

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

/// <summary>
/// 
/// </summary>
/// <returns></returns>
int main()
{
	// Render Window Settings
	sf::ContextSettings m_Settings;
	m_Settings.antialiasingLevel = 12;

	// Render Window Creation
	m_RenderWindow = new sf::RenderWindow(sf::VideoMode(Utils::WINDOWWIDTH, Utils::WINDOWHEIGHT), "BiomeWorks.exe", sf::Style::Default, m_Settings);
	m_RenderWindow->setFramerateLimit(120);
	/*m_RenderWindow->setVerticalSyncEnabled(true);*/
	m_RenderWindow->setKeyRepeatEnabled(false);

	// Window Icon
	sf::Image icon;
	icon.loadFromFile("Images/Chest.png");
	m_RenderWindow->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

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

	// Cleanup
	if (m_WorldManager->CleanUpBlocks() && m_WorldManager->CleanUpSky())
	{
		std::cout << "Cleanup Success" << std::endl;
	}
	delete m_AudioManager;
	delete m_GUI;
	delete m_Player;
	delete m_RenderWindow;
	delete m_WorldManager;
	m_Pickaxe = nullptr;
	m_Door = nullptr;
	m_WorldManager = nullptr;
	m_AudioManager = nullptr;
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
	m_Event = sf::Event();

	// UI
	m_GUI = new GUI();

	// Music
	m_AudioManager = new CAudioManager();
	m_AudioManager->PlayMusic();
	
	// Player
	m_Player = new CPlayer(m_RenderWindow, m_World, Utils::m_Scale, m_AudioManager);
	m_Player->Start();

	// Init UI
	InitUI();

	// Map
	m_WorldManager = new CWorldManager(m_RenderWindow, m_Player, m_World, m_GUI);
	m_WorldManager->Start();

	// Init World
	InitWorldView();

	// Center All Views To Player
	CenterViewsToSprite(m_Player->GetShape());

	// Debug Add Items To Inventory
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Dirt, CBlock::BLOCKTYPE::DIRT);
		m_Block->m_Type = m_Block->BLOCKTYPE::DIRT;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Planks, CBlock::BLOCKTYPE::PLANKS);
		m_Block->m_Type = m_Block->BLOCKTYPE::PLANKS;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	for (int i = 0; i < 1; i++)
	{
		m_Pickaxe = new CPickaxe();
		m_Player->AddItemToInventory(m_Pickaxe);
		m_Pickaxe = nullptr;
	}
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Grass, CBlock::BLOCKTYPE::GRASS);
		m_Block->m_Type = m_Block->BLOCKTYPE::GRASS;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Sand, CBlock::BLOCKTYPE::SAND);
		m_Block->m_Type = m_Block->BLOCKTYPE::SAND;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Wood, CBlock::BLOCKTYPE::WOOD);
		m_Block->m_Type = m_Block->BLOCKTYPE::WOOD;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Stone, CBlock::BLOCKTYPE::STONE);
		m_Block->m_Type = m_Block->BLOCKTYPE::STONE;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	for (int i = 0; i < 5; i++)
	{
		m_Block = new CBlock(m_GUI->m_MossyBrick, CBlock::BLOCKTYPE::MOSSYBRICK);
		m_Block->m_Type = m_Block->BLOCKTYPE::MOSSYBRICK;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	for (int i = 0; i < 5; i++)
	{
		m_Block = new CBlock(m_GUI->m_Chest, CBlock::BLOCKTYPE::CHEST);
		m_Block->m_Type = m_Block->BLOCKTYPE::CHEST;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	for (int i = 0; i < 6; i++)
	{
		m_Door = new CDoor();
		m_Door->m_Type = m_Block->BLOCKTYPE::DOOR;
		m_Player->AddItemToInventory(m_Door);
		m_Door = nullptr;
	}
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Leaves, CBlock::BLOCKTYPE::LEAVES);
		m_Block->m_Type = m_Block->BLOCKTYPE::LEAVES;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	
	m_Door = nullptr;
	m_Pickaxe = nullptr;
	m_Block = nullptr;
	m_bClose = false;

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
		while (m_RenderWindow->pollEvent(m_Event))
		{
			MousePos = m_RenderWindow->mapPixelToCoords((sf::Mouse::getPosition(*m_RenderWindow)), m_WorldView);

			// Regained Focus
			if (m_Event.type == sf::Event::GainedFocus)
			{
				m_Player->m_bCanMove = true;
			}

			// Click Out Of Window
			if (m_Event.type == sf::Event::LostFocus)
			{
				m_Player->m_bCanMove = false;
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
				/*m_GUI->ToggleInventoryUI();*/
			}

			// General Key Pressed
			if (m_Event.type == sf::Event::KeyPressed)
			{
				m_Player->Update(MousePos, m_Event);
				m_Player->Movement(m_Event);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
				{
					m_Player->ToggleInventoryUI();
				}
			}

			// Mouse Wheel Scroll
			if (m_Event.type == sf::Event::MouseWheelScrolled)
			{
				m_GUI->HotBarScrolling(m_Event, m_Player);
			}
			
			// Right On Press Mouse
			if (m_Player->m_bInventoryOpen && sf::Mouse::isButtonPressed(sf::Mouse::Right) && m_Event.type == sf::Event::MouseButtonPressed)
			{
				m_GUI->DropCurrentlyHeldItem(m_Player, m_Event);
			}

			// General On Press Mouse
			if (m_Event.type == sf::Event::MouseButtonPressed)
			{
				//m_Player->Movement(m_Event);
				m_Player->Interact(m_WorldManager->m_Chests,m_WorldManager->m_Doors, m_WorldManager->m_Chunk, m_Event, m_GUI->m_MousePos);
				
				// b2World Step & MousePosBox Position
				m_WorldManager->Update(m_Event, MousePos);
				break;
			}
		}

		//
		// UnPolled Update
		//
		if (!m_bClose)
		{
			// Player Exists
			if (m_Player != nullptr)
			{
				// Centre View To Player
				CenterViewsToSprite(m_Player->GetShape());

				// Player Update And Movement
				m_Player->Interact(m_WorldManager->m_Chests, m_WorldManager->m_Doors, m_WorldManager->m_Chunk, m_Event, m_GUI->m_MousePos);
				m_Player->Update(MousePos, m_Event);
				m_Player->Movement(m_Event);

				// Reset Players SFML Sprite To Box2D Body
				m_Player->ResetSpritePos();

				// b2World Step & MousePosBox Position
				m_WorldManager->Update(m_Event, MousePos);

				//
				// Main Render
				//
				Render();
			}
		}
		else if (m_bClose)
		{
			m_WorldManager->CleanUpBlocks();
			m_WorldManager->CleanUpSky();
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
	m_Player->Render();

	// UI
	m_RenderWindow->setView(m_UIView);
	/////////////////////////////////////

	m_GUI->HealthUI(m_RenderWindow, m_Player);
	m_GUI->MiniMapUI(m_RenderWindow, m_WorldManager->m_Chunk, m_WorldManager->m_SkyChunk, m_Player);

	m_GUI->InventoryUI(m_RenderWindow, m_Player, m_UIView, m_WorldView, m_Event);
	
	m_GUI->CraftingUI(m_RenderWindow, m_Player);
	m_GUI->Render(m_RenderWindow, m_Player, m_WorldView, m_UIView);

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
	m_GUI->InitTextureMaster();
	m_GUI->InitMiniMap(m_RenderWindow);
	m_GUI->InitHealthUI(m_Player);
	m_GUI->InitInventoryUI(m_Player, m_RenderWindow);

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



