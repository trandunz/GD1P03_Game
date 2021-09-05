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

// Garbage Filter
b2Filter* m_GarbageFilter;
int m_NumberOfBlocksToClean = 0;

// Close App?
bool m_bClose = false;

// Audio Manager
CAudioManager* m_AudioManager;

// Block Pointer If Needed To Make Stuff
CBlock* m_Block;

// Block Pointer If Needed To Make Stuff
CDoor* m_Door;

/// <summary>
/// 
/// </summary>
/// <returns></returns>
int main()
{
	// Render Window Settings
	sf::ContextSettings m_Settings;
	m_Settings.antialiasingLevel = 8;

	// Render Window Creation
	m_RenderWindow = new sf::RenderWindow(sf::VideoMode(Utils::WINDOWWIDTH, Utils::WINDOWHEIGHT), "BiomeWorks.exe", sf::Style::Default, m_Settings);
	m_RenderWindow->setFramerateLimit(60);
	/*m_RenderWindow->setVerticalSyncEnabled(true);*/
	m_RenderWindow->setKeyRepeatEnabled(false);

	// Window Icon
	sf::Image icon;
	icon.loadFromFile("Images/Chest.png");
	m_RenderWindow->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	m_GarbageFilter = new b2Filter();
	m_GarbageFilter->categoryBits = 1;
	m_GarbageFilter->maskBits = 0;
	m_GarbageFilter->groupIndex = 0;

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
	delete m_GarbageFilter;
	delete m_Player;
	delete m_RenderWindow;
	delete m_WorldManager;
	m_Door = nullptr;
	m_WorldManager = nullptr;
	m_AudioManager = nullptr;
	m_GUI = nullptr;
	m_GarbageFilter = nullptr;
	m_Player = nullptr;
	m_RenderWindow = nullptr;
	m_Block = nullptr;

	//
	return 0;
	//
}

/// <summary>
/// 
/// </summary>
void Start()
{
	// UI
	m_GUI = new GUI();

	// Music
	m_AudioManager = new CAudioManager();
	m_AudioManager->PlayMusic();
	
	// Player
	m_Player = new CPlayer(m_RenderWindow, m_World, Utils::m_Scale, m_AudioManager);
	m_Player->Start();

	InitUI();

	// Map
	m_WorldManager = new CWorldManager(m_RenderWindow, m_Player, m_World, m_GUI);
	m_WorldManager->Start();

	InitWorldView();
	CenterViewsToSprite(m_Player->GetShape());

	
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Dirt);
		m_Block->m_Type = m_Block->BLOCKTYPE::DIRT;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Planks);
		m_Block->m_Type = m_Block->BLOCKTYPE::PLANKS;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	
	for (int i = 0; i < 1337; i++)
	{
		m_Door = new CDoor();
		m_Player->AddItemToInventory(m_Door);
		m_Door = nullptr;
	}
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Grass);
		m_Block->m_Type = m_Block->BLOCKTYPE::GRASS;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Sand);
		m_Block->m_Type = m_Block->BLOCKTYPE::SAND;
		m_Player->AddItemToInventory(m_Block);

		m_Block = nullptr;
	}
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Wood);
		m_Block->m_Type = m_Block->BLOCKTYPE::WOOD;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	for (int i = 0; i < 1337; i++)
	{
		m_Block = new CBlock(m_GUI->m_Stone);
		m_Block->m_Type = m_Block->BLOCKTYPE::STONE;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	for (int i = 0; i < 5; i++)
	{
		m_Block = new CBlock(m_GUI->m_MossyBrick);
		m_Block->m_Type = m_Block->BLOCKTYPE::MOSSYBRICK;
		m_Player->AddItemToInventory(m_Block);
		m_Block = nullptr;
	}
	


	m_Block = nullptr;
	m_Block = nullptr;


	Render();

	
	

	m_bClose = false;

	
}

/// <summary>
/// 
/// </summary>
void Update()
{
	while (m_RenderWindow->isOpen())
	{
		m_Event = sf::Event();
		while (m_RenderWindow->pollEvent(m_Event))
		{
			if (m_Event.type == sf::Event::GainedFocus)
			{

				m_Player->m_bCanMove = true;
			}

			if (m_Event.type == sf::Event::LostFocus)
			{
				m_Player->m_bCanMove = false;
			}

			if (m_Event.type == sf::Event::Closed)
			{
				m_RenderWindow->close();
				m_bClose = true;
				break;
			}

			if (m_Event.type == sf::Event::Resized)
			{
				/*m_GUI->ToggleInventoryUI();*/
			}

			if (m_Event.type == sf::Event::KeyPressed)
			{
				// Body Updates
				m_WorldManager->Update(m_Event, MousePos);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
				{
					m_Player->ToggleInventoryUI();
				}
			}

			
		}

		MousePos = m_RenderWindow->mapPixelToCoords((sf::Mouse::getPosition(*m_RenderWindow)));
		
		if (!m_bClose)
		{
			if (m_Player != nullptr)
			{
				// Centre View To Player
				CenterViewsToSprite(m_Player->GetShape());

				// Body Updates
				m_WorldManager->Update(m_Event, MousePos);

				// Reset Players SFML Sprite To Box2D Body
				m_Player->ResetSpritePos();

				// Render
				Render();
			}

			// Block Placing
			if (m_Event.type == sf::Event::MouseButtonPressed && m_Player->m_bCanPlace)
			{
				m_Player->PlaceBlocks(m_WorldManager->m_Doors, m_WorldManager->m_Chunk, m_Event, m_GUI->m_MousePos);
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
	m_RenderWindow->draw(m_GUI->m_MousePos);

	// UI
	m_RenderWindow->setView(m_UIView);
	/////////////////////////////////////

	m_GUI->HealthUI(m_RenderWindow, m_Player);
	m_GUI->InventoryUI(m_RenderWindow, m_Player, m_UIView, m_WorldView, m_Event);
	m_GUI->MiniMapUI(m_RenderWindow, m_WorldManager->m_Chunk, m_WorldManager->m_SkyChunk, m_Player);
	m_GUI->CraftingUI(m_RenderWindow, m_Player);
	m_GUI->Render(m_RenderWindow, m_Player);

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



