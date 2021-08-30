// Non-Local Includes
#include <SFML/Graphics.hpp>
#include <box2d\box2D.h>
#include <iostream>
#include <list>

// Local Includes
#include "Player.h"
#include "Block.h"

namespace Utils
{
	const int WINDOWWIDTH = 640;
	const int WINDOWHEIGHT = 640;
	const float m_Scale = 50.0f;
};

// Forward Declaration
void Start();
void Update();
void Render();
void InitView();

void CreateSkyChunk();
void CreateSurfaceLayer(int _posX, int _posY);
void CreateChunck(int _posX, int _posY);
void CreateMegaChunk(int _posX, int _posY);

void CenterViewTo(sf::Sprite _object);

void moveToFront(std::list<CBlock>& list, std::list<CBlock>::iterator element);

void BodyUpdates(sf::Event& _event);

bool CleanUpBlocks();

// Mouse
sf::Vector2f MousePos;
// 
// Textures
sf::Texture* m_MousePosTex;
sf::Texture* m_Dirt;
sf::Texture* m_Grass;
sf::Texture* m_Sky;

//Sprites
sf::Sprite m_MousePos;

// Main Render Window
sf::RenderWindow* m_RenderWindow;

std::list<CBlock> m_Chunk = {};
std::list<sf::Sprite> m_SkyChunk = {};

// Main View / Camera
sf::View m_View;

// b2World
b2Vec2 m_Gravity(0.0f, 40.0f);
b2World m_World(m_Gravity);

// Player
CPlayer* m_Player;

// Ground
CBlock* m_Block;

bool m_bClose = false;
int m_NumberOfBlocksToClean = 0;

int main()
{
	// Render Window Settings
	sf::ContextSettings m_Settings;
	m_Settings.antialiasingLevel = 8;

	// Render Window Creation
	m_RenderWindow = new sf::RenderWindow(sf::VideoMode(Utils::WINDOWWIDTH, Utils::WINDOWHEIGHT), "SFML and box2D works!", sf::Style::Default, m_Settings);
	m_RenderWindow->setFramerateLimit(60);

	//Textures
	m_Dirt = new sf::Texture();
	m_Grass = new sf::Texture();
	m_MousePosTex = new sf::Texture();
	m_Sky = new sf::Texture();
	m_Dirt->loadFromFile("Images/Dirt.png");
	m_Grass->loadFromFile("Images/Grass.png");
	m_MousePosTex->loadFromFile("Images/MousePos.png");
	m_Sky->loadFromFile("Images/Sky.png");

	Start();
	Update();


	std::list<CBlock>::iterator block = m_Chunk.begin();

	while (m_Chunk.size() > 0)
	{
		m_Chunk.pop_front();
	}
	std::list<sf::Sprite>::iterator sky = m_SkyChunk.begin();
	while (m_SkyChunk.size() > 0)
	{
		m_SkyChunk.pop_front();
	}
	delete m_Sky;
	delete m_MousePosTex;
	delete m_Dirt;
	delete m_Grass;
	delete m_Player;
	delete m_RenderWindow;
	m_Sky = nullptr;
	m_MousePosTex = nullptr;
	m_Dirt = nullptr;
	m_Grass = nullptr;
	m_Block = nullptr;
	m_Player = nullptr;
	m_RenderWindow = nullptr;

	return NULL;
}

void Start()
{
	// Player
	m_Player = new CPlayer(m_RenderWindow, m_World, Utils::m_Scale);

	m_MousePos.setTexture(*m_MousePosTex, true);
	m_MousePos.setOrigin(m_MousePos.getGlobalBounds().width / 2, m_MousePos.getGlobalBounds().height / 2);

	CreateSkyChunk();

	// Map
	CreateSurfaceLayer(0, 0);
	CreateMegaChunk(0, 0);
	CreateMegaChunk(0, 3200);
	CreateMegaChunk(4500, 0);
	CreateMegaChunk(4500, 3200);
	CreateMegaChunk(9000, 0);
	CreateMegaChunk(9000, 3200);
	CreateMegaChunk(-4500, 0);
	CreateMegaChunk(-4500, 3200);
	CreateMegaChunk(-9000, 0);
	CreateMegaChunk(-9000, 3200);

	
	InitView();
	CenterViewTo(m_Player->GetShape());
}

void Update()
{
	while (m_RenderWindow->isOpen())
	{
		MousePos = m_RenderWindow->mapPixelToCoords((sf::Mouse::getPosition(*m_RenderWindow)));

		sf::Event Event;
		while (m_RenderWindow->pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
			{
				m_RenderWindow->close();
				m_bClose = true;
				break;
			}

			if (Event.type == sf::Event::KeyPressed)
			{
				// Body Updates
				BodyUpdates(Event);

			}
		}

		if (m_Player != nullptr)
		{
			

			// Centre View To Player
			CenterViewTo(m_Player->GetShape());

			// Body Updates
			BodyUpdates(Event);

			
			// World Step
			m_World.Step(1 / 60.0f, 60, 60);
			
			// Render
			Render();

			
		}
		
		

		
		// Block Placing
		if (Event.type == sf::Event::MouseButtonPressed)
		{
			float Mag = sqrt(((m_MousePos.getPosition().x - m_Player->GetShape().getPosition().x) * (m_MousePos.getPosition().x - m_Player->GetShape().getPosition().x)) + ((m_MousePos.getPosition().y - m_Player->GetShape().getPosition().y) * (m_MousePos.getPosition().y - m_Player->GetShape().getPosition().y)));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && Mag > 200)
			{
				m_Block = nullptr;
				m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, m_MousePos.getPosition().x, m_MousePos.getPosition().y);
				m_Block->SetSize(100, 100);
				m_Chunk.push_back(*m_Block);
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && Mag > 200)
			{
				std::cout << "Mouse Right" << std::endl;
				std::list<CBlock>::iterator element = m_Chunk.begin();
				while (element != m_Chunk.end())
				{
					if (element->GetShape().getPosition() == m_MousePos.getPosition())
					{
						moveToFront(m_Chunk, element);
						m_Chunk.pop_front();

						break;
					}

					element++;
				}
			}

		}
		
	}
}

bool CleanUpBlocks()
{
	std::list<CBlock>::iterator element = m_Chunk.begin();
	while (element != m_Chunk.end())
	{
		if (element->MARKASDESTROY)
		{
			m_Chunk.erase(element);

			m_Chunk.pop_front();
			return true;
		}
		
		element++;
	}
	return false;
}

void Render()
{
	m_RenderWindow->clear();

	for (sf::Sprite& sky : m_SkyChunk)
	{
		m_RenderWindow->draw(sky);
	}
	// Shapes
	for (CBlock& block : m_Chunk)
	{
		block.Render();
	}
	m_Player->Render();
	m_RenderWindow->draw(m_MousePos);

	m_RenderWindow->display();
}

void InitView()
{
	m_View = sf::View(sf::Vector2f(0.0f,0.0f), sf::Vector2f(m_RenderWindow->getSize().x, m_RenderWindow->getSize().y));
	m_View.zoom(5.0f);
	m_RenderWindow->setView(m_View);
}

void CreateSurfaceLayer(int _posX, int _posY)
{
	for (int i = 10; i < 11400; i += 100)
	{
		m_Block = nullptr;
		m_Block = new CBlock(m_RenderWindow, m_World, m_Grass, Utils::m_Scale, i + _posX, 400 + _posY);
		m_Block->SetSize(100, 100);
		m_Chunk.push_front(*m_Block);
	}
	for (int i = -90; i > -11100; i -= 100)
	{
		m_Block = nullptr;
		m_Block = new CBlock(m_RenderWindow, m_World, m_Grass, Utils::m_Scale, i + _posX, 400 + _posY);
		m_Block->SetSize(100, 100);
		m_Chunk.push_front(*m_Block);
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

void CreateSkyChunk()
{
	for (int i = 0; i < 10000; i += 100)
	{
		for (int j = 0; j < 10000; j += 100)
		{
			sf::Sprite sky = sf::Sprite(*m_Sky);
			sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
			sky.setPosition(sf::Vector2f(i+10, j));
			m_SkyChunk.push_front(sky);
		}
		for (int j = 0; j > -10000; j -= 100)
		{
			sf::Sprite sky = sf::Sprite(*m_Sky);
			sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
			sky.setPosition(sf::Vector2f(i + 10, j));
			m_SkyChunk.push_front(sky);
		}
	}
	for (int i = 0; i > -10000; i -= 100)
	{
		for (int j = 0; j > -10000; j -= 100)
		{
			sf::Sprite sky = sf::Sprite(*m_Sky);
			sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
			sky.setPosition(sf::Vector2f(i + 10, j));
			m_SkyChunk.push_front(sky);
		}
		for (int j = 0; j < 10000; j += 100)
		{
			sf::Sprite sky = sf::Sprite(*m_Sky);
			sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
			sky.setPosition(sf::Vector2f(i + 10, j));
			m_SkyChunk.push_front(sky);
		}
	}
}

void CreateChunck(int _posX, int _posY)
{
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 110 + _posX, 500 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 210 + _posX, 500 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 310 + _posX, 500 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 410 + _posX, 500 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 510 + _posX, 500 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 10 + _posX, 500 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -90 + _posX, 500 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -190 + _posX, 500 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -290 + _posX, 500 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);

	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 110 + _posX, 600 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 210 + _posX, 600 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 310 + _posX, 600 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 410 + _posX, 600 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 510 + _posX, 600 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 10 + _posX, 600 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -90 + _posX, 600 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -190 + _posX, 600 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -290 + _posX, 600 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);

	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 110 + _posX, 700 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 210 + _posX, 700 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 310 + _posX, 700 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 410 + _posX, 700 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 510 + _posX, 700 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 10 + _posX, 700 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -90 + _posX, 700 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -190 + _posX, 700 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -290 + _posX, 700 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);

	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 110 + _posX, 800 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 210 + _posX, 800 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 310 + _posX, 800 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 410 + _posX, 800 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 510 + _posX, 800 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 10 + _posX, 800 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -90 + _posX, 800 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -190 + _posX, 800 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -290 + _posX, 800 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);

	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 110 + _posX, 900 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 210 + _posX, 900 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 310 + _posX, 900 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 410 + _posX, 900 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 510 + _posX, 900 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 10 + _posX, 900 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -90 + _posX, 900 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -190 + _posX, 900 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -290 + _posX, 900 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);

	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 110 + _posX, 1000 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 210 + _posX, 1000 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 310 + _posX, 1000 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 410 + _posX, 1000 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 510 + _posX, 1000 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 10 + _posX, 1000 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -90 + _posX, 1000 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -190 + _posX, 1000 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -290 + _posX, 1000 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);

	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 110 + _posX, 1100 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 210 + _posX, 1100 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 310 + _posX, 1100 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 410 + _posX, 1100 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 510 + _posX, 1100 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, 10 + _posX, 1100 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -90 + _posX, 1100 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -190 + _posX, 1100 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;
	m_Block = new CBlock(m_RenderWindow, m_World, m_Dirt, Utils::m_Scale, -290 + _posX, 1100 + _posY);
	m_Block->SetSize(100, 100);
	m_Chunk.push_front(*m_Block);
	m_Block = nullptr;

}

void CreateMegaChunk(int _posX, int _posY)
{
	CreateChunck(0 + _posX, 0 + _posY);
	CreateChunck(900 + _posX, 0 + _posY);
	CreateChunck(-900 + _posX, 0 + _posY);
	CreateChunck(1800 + _posX, 0 + _posY);
	CreateChunck(-1800 + _posX, 0 + _posY);

	CreateChunck(0 + _posX, 500 + _posY);
	CreateChunck(900 + _posX, 500 + _posY);
	CreateChunck(-900 + _posX, 500 + _posY);
	CreateChunck(1800 + _posX, 500 + _posY);
	CreateChunck(-1800 + _posX, 500 + _posY);

	CreateChunck(0 + _posX, 500 + 500 + _posY);
	CreateChunck(900 + _posX, 500 + 500 + _posY);
	CreateChunck(-900 + _posX, 500 + 500 + _posY);
	CreateChunck(1800 + _posX, 500 + 500 + _posY);
	CreateChunck(-1800 + _posX, 500 + 500 + _posY);

	CreateChunck(0 + _posX, 500 + 1000 + _posY);
	CreateChunck(900 + _posX, 500 + 1000 + _posY);
	CreateChunck(-900 + _posX, 500 + 1000 + _posY);
	CreateChunck(1800 + _posX, 500 + 1000 + _posY);
	CreateChunck(-1800 + _posX, 500 + 1000 + _posY);

	CreateChunck(0 + _posX, 500 + 1500 + _posY);
	CreateChunck(900 + _posX, 500 + 1500 + _posY);
	CreateChunck(-900 + _posX, 500 + 1500 + _posY);
	CreateChunck(1800 + _posX, 500 + 1500 + _posY);
	CreateChunck(-1800 + _posX, 500 + 1500 + _posY);

	CreateChunck(0 + _posX, 500 + 2000 + _posY);
	CreateChunck(900 + _posX, 500 + 2000 + _posY);
	CreateChunck(-900 + _posX, 500 + 2000 + _posY);
	CreateChunck(1800 + _posX, 500 + 2000 + _posY);
	CreateChunck(-1800 + _posX, 500 + 2000 + _posY);
}

void CenterViewTo(sf::Sprite _object)
{
	m_View.setCenter(_object.getPosition());
	m_RenderWindow->setView(m_View);
}

void BodyUpdates(sf::Event& _event)
{
	// Bodies
	std::list<CBlock>::iterator it = m_Chunk.begin();
	// Shapes
	m_Player->Update();
	m_Player->Movement(_event);
	
	for (sf::Sprite& sky : m_SkyChunk)
	{
		if (sky.getGlobalBounds().contains(MousePos))
		{
			m_MousePos.setPosition(sky.getPosition());
		}
		if (sky.getGlobalBounds().intersects(m_MousePos.getGlobalBounds()))
		{
			m_MousePos.setPosition(sky.getPosition());
		}
	}
	for (CBlock& block : m_Chunk)
	{
		block.Update();
		if (block.GetShape().getGlobalBounds().contains(MousePos))
		{
			m_MousePos.setPosition(block.GetShape().getPosition());
		}
	}
}

void CreateSky(sf::Color _colour)
{
	sf::Sprite SkyBlock;
	SkyBlock.setTextureRect(sf::IntRect(0,0,100,100));
	SkyBlock.setColor(sf::Color::Blue);
	for (int i = 10; i < 11400; i += 100)
	{
		m_SkyChunk.push_front(SkyBlock);
	}
	for (int i = -90; i > -11100; i -= 100)
	{
		m_SkyChunk.push_front(SkyBlock);
	}
}

void moveToFront(std::list<CBlock>& list, std::list<CBlock>::iterator element)
{
	if (element != list.begin()) 
	{
		list.splice(list.begin(), list, element, std::next(element));
	}
}
