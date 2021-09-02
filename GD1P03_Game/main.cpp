// Non-Local Includes
#include <iostream>

// Local Includes
#include "GUI.h"
#include "Block.h"


namespace Utils
{
	const int WINDOWWIDTH = 1920;
	const int WINDOWHEIGHT = 1080;
	const float m_Scale = 50.0f;
};

// Forward Declaration
void Start();
void Update();
void Render();

void InitWorldView();
void InitUI();

void CreateSkyChunk();
void CreateSurfaceLayer(int _posX, int _posY);

void CenterViewsToSprite(sf::Sprite _object);

void Lst_MoveToFront(std::list<CBlock>& list, std::list<CBlock>::iterator element);

void b2BodyUpdates(sf::Event& _event);

bool CleanUpBlocks();
bool CleanUpSky();

// Mouse
sf::Vector2f MousePos;
// 


// Font
sf::Font m_Font;



// Main Render Window
sf::RenderWindow* m_RenderWindow;

// Blocks / Tiles
std::list<CBlock> m_Chunk = {};
std::list<sf::Sprite> m_SkyChunk = {};

// Main View / Camera
sf::View m_WorldView;
sf::View m_UIView;

// UI
GUI* m_GUI;

// b2World
b2Vec2 m_Gravity(0.0f, 40.0f);
b2World m_World(m_Gravity);

// Player
CPlayer* m_Player;

// Temp Block Pointer (Used To Create New Blocks During World Generation)
CBlock* m_Block;

// Garbage Filter
b2Filter* m_GarbageFilter;
int m_NumberOfBlocksToClean = 0;

// Close App?
bool m_bClose = false;

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
	m_RenderWindow = new sf::RenderWindow(sf::VideoMode(Utils::WINDOWWIDTH, Utils::WINDOWHEIGHT), "SFML and box2D works!", sf::Style::Default, m_Settings);
	m_RenderWindow->setFramerateLimit(120);
	m_RenderWindow->setKeyRepeatEnabled(false);

	m_GarbageFilter = new b2Filter();
	m_GarbageFilter->categoryBits = 1;
	m_GarbageFilter->maskBits = 0;
	m_GarbageFilter->groupIndex = 0;

	

	//
	Start();
	Update();
	//

	// Cleanup
	if (CleanUpBlocks() && CleanUpSky())
	{
		std::cout << "Cleanup Success" << std::endl;

		m_Chunk.clear();
		m_SkyChunk.clear();
	}
	delete m_GUI;
	delete m_GarbageFilter;
	delete m_Player;
	delete m_RenderWindow;
	m_GUI = nullptr;
	m_GarbageFilter = nullptr;
	m_Block = nullptr;
	m_Player = nullptr;
	m_RenderWindow = nullptr;

	//
	return 0;
	//
}

/// <summary>
/// 
/// </summary>
void Start()
{
	// Player
	m_Player = new CPlayer(m_RenderWindow, m_World, Utils::m_Scale);

	// UI
	InitUI();
	InitWorldView();
	CenterViewsToSprite(m_Player->GetShape());

	// Map
	CreateSkyChunk();
	CreateSurfaceLayer(0, 0);

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
		sf::Event Event = sf::Event();
		while (m_RenderWindow->pollEvent(Event))
		{
			if (Event.type == sf::Event::GainedFocus)
			{
				InitUI();
				InitWorldView();
			}

			if (Event.type == sf::Event::Closed)
			{
				m_RenderWindow->close();
				m_bClose = true;
				break;
			}

			if (Event.type == sf::Event::Resized)
			{
				InitUI();
				InitWorldView();

				m_GUI->ToggleInventoryUI();
			}

			if (Event.type == sf::Event::KeyPressed)
			{
				// Body Updates
				b2BodyUpdates(Event);

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
				{
					m_GUI->ToggleInventoryUI();
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
				b2BodyUpdates(Event);

				// Reset Players SFML Sprite To Box2D Body
				m_Player->ResetSpritePos();

				// Render
				Render();
			}

			// World Step
			m_World.Step(1 / 60.0f, 60, 60);

			// Block Placing
			if (Event.type == sf::Event::MouseButtonPressed)
			{
				m_Player->PlaceBlocks(m_Chunk, Event, m_GUI->m_MousePos);
			}
		}
		else if (m_bClose)
		{
			CleanUpBlocks();
			CleanUpSky();
		}
	}
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool CleanUpBlocks()
{
	// Shapes
	while (m_Chunk.size() > 0)
	{
		m_Chunk.pop_front();
	}
	return true;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool CleanUpSky()
{
	while (m_SkyChunk.size() > 0)
	{
		m_SkyChunk.pop_front();
	}
	return true;
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
	for (sf::Sprite& sky : m_SkyChunk)
	{
		/*if (sky.getPosition().x > m_WorldView.getCenter().x && sky.getPosition().x < m_WorldView.getSize().x/2
			&& sky.getPosition().y > m_WorldView.getCenter().y && sky.getPosition().y < m_WorldView.getSize().y / 2)
		{
			m_RenderWindow->draw(sky);
		}*/
		m_RenderWindow->draw(sky);
	}

	// Blocks
	for (CBlock& block : m_Chunk)
	{
		if (block.GetShape().getPosition().x > m_WorldView.getCenter().x && block.GetShape().getPosition().x < (m_WorldView.getSize().x / 2 + m_WorldView.getCenter().x)
			&& block.GetShape().getPosition().y > m_WorldView.getCenter().y && block.GetShape().getPosition().y < (m_WorldView.getSize().y / 2 + m_WorldView.getCenter().y))
		{
			
		}
		block.Render();
		/*if (block.GetShape().getPosition().x < m_WorldView.getCenter().x && block.GetShape().getPosition().x > (m_WorldView.getSize().x / 2 - m_WorldView.getCenter().x)
			&& block.GetShape().getPosition().y < m_WorldView.getCenter().y && block.GetShape().getPosition().y > (m_WorldView.getSize().y / 2 - m_WorldView.getCenter().y))
		{
			
		}
		block.Render();*/
	}

	// Player
	m_Player->Render();
	m_RenderWindow->draw(m_GUI->m_MousePos);

	// UI
	m_RenderWindow->setView(m_UIView);
	/////////////////////////////////////

	m_GUI->HealthUI(m_RenderWindow);
	m_GUI->InventoryUI(m_RenderWindow);
	m_GUI->MiniMapUI(m_RenderWindow, m_Chunk, m_SkyChunk);
	m_GUI->CraftingUI(m_RenderWindow);
	m_GUI->Render(m_RenderWindow);

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
	m_GUI = new GUI();
	m_GUI->InitTextureMaster();
	m_GUI->SetPlayer(m_Player);

	// UI View
	m_UIView = sf::View(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(m_RenderWindow->getSize().x, m_RenderWindow->getSize().y));
	m_RenderWindow->setView(m_UIView);
}

/// <summary>
/// 
/// </summary>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
void CreateSurfaceLayer(int _posX, int _posY)
{
	for (int i = 10; i < 11400 / 1.9f; i += 100)
	{
		m_Block = nullptr;
		m_Block = new CBlock(m_RenderWindow, m_World, m_GUI->m_Grass, Utils::m_Scale, i + _posX, 400 + _posY);
		m_Block->SetSize(100, 100);
		m_Chunk.push_front(*m_Block);
		m_Block = nullptr;

		for (int j = 500; j < 11400 / 1.9f; j += 100)
		{
			m_Block = nullptr;
			m_Block = new CBlock(m_RenderWindow, m_World, m_GUI->m_Dirt, Utils::m_Scale, i + _posX, j + _posY);
			m_Block->SetSize(100, 100);
			m_Chunk.push_front(*m_Block);
			m_Block = nullptr;
		}
	}
	for (int i = -90; i > -11100 / 1.9f; i -= 100)
	{
		m_Block = nullptr;
		m_Block = new CBlock(m_RenderWindow, m_World, m_GUI->m_Grass, Utils::m_Scale, i + _posX, 400 + _posY);
		m_Block->SetSize(100, 100);
		m_Chunk.push_front(*m_Block);
		m_Block = nullptr;

		for (int j = 500; j < 11400 / 1.9f; j += 100)
		{
			m_Block = nullptr;
			m_Block = new CBlock(m_RenderWindow, m_World, m_GUI->m_Dirt, Utils::m_Scale, i + _posX, j + _posY);
			m_Block->SetSize(100, 100);
			m_Chunk.push_front(*m_Block);
			m_Block = nullptr;
		}
	}
	//// Layer 1
	//m_Block = nullptr;
	//m_Block = new CBlock(m_RenderWindow, m_World, m_Grass, Utils::m_Scale, 110 + _posX, 400 + _posY);
	//m_Block->SetSize(100, 100);
	//m_Chunk.push_front(*m_Block);
	//m_Block = nullptr;
	//m_Block = new CBlock(m_RenderWindow, m_World, m_Grass, Utils::m_Scale, 210 + _posX, 400 + _posY);
	//m_Block->SetSize(100, 100);
	//m_Chunk.push_front(*m_Block);
	//m_Block = nullptr;
	//m_Block = new CBlock(m_RenderWindow, m_World, m_Grass, Utils::m_Scale, 310 + _posX, 400 + _posY);
	//m_Block->SetSize(100, 100);
	//m_Chunk.push_front(*m_Block);
	//m_Block = nullptr;
	//m_Block = new CBlock(m_RenderWindow, m_World, m_Grass, Utils::m_Scale, 410 + _posX, 400 + _posY);
	//m_Block->SetSize(100, 100);
	//m_Chunk.push_front(*m_Block);
	//m_Block = nullptr;
	//m_Block = new CBlock(m_RenderWindow, m_World, m_Grass, Utils::m_Scale, 510 + _posX, 400 + _posY);
	//m_Block->SetSize(100, 100);
	//m_Chunk.push_front(*m_Block);
	//m_Block = nullptr;
	//m_Block = new CBlock(m_RenderWindow, m_World, m_Grass, Utils::m_Scale, 10 + _posX, 400 + _posY);
	//m_Block->SetSize(100, 100);
	//m_Chunk.push_front(*m_Block);
	//m_Block = nullptr;
	//m_Block = new CBlock(m_RenderWindow, m_World, m_Grass, Utils::m_Scale, -90 + _posX, 400 + _posY);
	//m_Block->SetSize(100, 100);
	//m_Chunk.push_front(*m_Block);
	//m_Block = nullptr;
	//m_Block = new CBlock(m_RenderWindow, m_World, m_Grass, Utils::m_Scale, -190 + _posX, 400 + _posY);
	//m_Block->SetSize(100, 100);
	//m_Chunk.push_front(*m_Block);
	//m_Block = nullptr;
	//m_Block = new CBlock(m_RenderWindow, m_World, m_Grass, Utils::m_Scale, -290 + _posX, 400 + _posY);
	//m_Block->SetSize(100, 100);
	//m_Chunk.push_front(*m_Block);
}

/// <summary>
/// 
/// </summary>
void CreateSkyChunk()
{
	for (int i = 0; i < 10000; i += 100)
	{
		for (int j = 0; j < 10000; j += 100)
		{
			sf::Sprite sky = sf::Sprite(*m_GUI->m_Sky);
			sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
			sky.setPosition(sf::Vector2f(i+10, j));
			m_SkyChunk.push_front(sky);
		}
		for (int j = 0; j > -10000; j -= 100)
		{
			sf::Sprite sky = sf::Sprite(*m_GUI->m_Sky);
			sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
			sky.setPosition(sf::Vector2f(i + 10, j));
			m_SkyChunk.push_front(sky);
		}
	}
	for (int i = 0; i > -10000; i -= 100)
	{
		for (int j = 0; j > -10000; j -= 100)
		{
			sf::Sprite sky = sf::Sprite(*m_GUI->m_Sky);
			sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
			sky.setPosition(sf::Vector2f(i + 10, j));
			m_SkyChunk.push_front(sky);
		}
		for (int j = 0; j < 10000; j += 100)
		{
			sf::Sprite sky = sf::Sprite(*m_GUI->m_Sky);
			sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
			sky.setPosition(sf::Vector2f(i + 10, j));
			m_SkyChunk.push_front(sky);
		}
	}
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

/// <summary>
/// 
/// </summary>
/// <param name="_event"></param>
void b2BodyUpdates(sf::Event& _event)
{
	// Bodies
	std::list<CBlock>::iterator it = m_Chunk.begin();
	// Shapes
	m_Player->Update(MousePos);
	m_Player->Movement(_event);
	
	for (sf::Sprite& sky : m_SkyChunk)
	{
		if (sky.getGlobalBounds().contains(MousePos))
		{
			m_GUI->m_MousePos.setPosition(sky.getPosition());
		}
		if (sky.getGlobalBounds().intersects(m_GUI->m_MousePos.getGlobalBounds()))
		{
			m_GUI->m_MousePos.setPosition(sky.getPosition());
		}
	}
	for (CBlock& block : m_Chunk)
	{
		block.Update();
		if (block.GetShape().getGlobalBounds().contains(MousePos))
		{
			m_GUI->m_MousePos.setPosition(block.GetShape().getPosition());
		}
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="list"></param>
/// <param name="element"></param>
void Lst_MoveToFront(std::list<CBlock>& list, std::list<CBlock>::iterator element)
{
	if (element != list.begin()) 
	{
		list.splice(list.begin(), list, element, std::next(element));
	}
}

