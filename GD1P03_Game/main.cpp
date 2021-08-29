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

void CreateSurfaceLayer(int _posX, int _posY);
void CreateChunck(int _posX, int _posY);
void CreateMegaChunk(int _posX, int _posY);

void CenterViewTo(sf::RectangleShape _object);

void BodyUpdates(sf::Event& _event);

// Mouse
sf::Vector2f MousePos;
// 
// Textures
sf::Texture* m_MousePosTex;
sf::Texture* m_Dirt;
sf::Texture* m_Grass;

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
	m_Dirt->loadFromFile("Images/Dirt.png");
	m_Grass->loadFromFile("Images/Grass.png");
	m_MousePosTex->loadFromFile("Images/MousePos.png");

	Start();
	Update();


	std::list<CBlock>::iterator block = m_Chunk.begin();

	while (m_Chunk.size() > 0)
	{
		if (block != m_Chunk.end())
		{
			m_Chunk.erase(block);
			std::advance(block, 1);

		}
		m_Chunk.pop_front();

	}
	delete m_MousePosTex;
	delete m_Block;
	delete m_Dirt;
	delete m_Grass;
	delete m_Player;
	delete m_RenderWindow;
	m_MousePosTex = nullptr;
	m_Dirt = nullptr;
	m_Grass = nullptr;
	m_Block = nullptr;
	m_Player = nullptr;
	m_RenderWindow = nullptr;

	return 0;
}

void Start()
{
	// Player
	m_Player = new CPlayer(m_RenderWindow, m_World, Utils::m_Scale);

	m_MousePos.setTexture(*m_MousePosTex, true);
	m_MousePos.setOrigin(m_MousePos.getGlobalBounds().width / 2, m_MousePos.getGlobalBounds().height / 2);

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
		sf::Event Event;
		while (m_RenderWindow->pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
			{
				m_RenderWindow->close();
				
				std::list<CBlock>::iterator block = m_Chunk.begin();

				while (m_Chunk.size() > 0)
				{
					if (block != m_Chunk.end())
					{
						m_Chunk.erase(block);
						std::advance(block, 1);
						
					}
					m_Chunk.pop_front();

				}

				delete m_Block;
				delete m_Player;
				delete m_RenderWindow;
				m_Block = nullptr;
				m_Player = nullptr;
				m_RenderWindow = nullptr;
				break;
			}

			if (Event.type == sf::Event::KeyPressed)
			{
				// Body Updates
				BodyUpdates(Event);

			}

			
		}

		// Centre View To Player
		CenterViewTo(m_Player->GetShape());

		// World Step
		m_World.Step(1 / 60.0f, 60, 60);

		// Body Updates
		BodyUpdates(Event);
		
		MousePos = m_RenderWindow->mapPixelToCoords((sf::Mouse::getPosition(*m_RenderWindow)));
		

		// Render
		Render();
	}
}

void Render()
{
	m_RenderWindow->clear();

	
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

void CenterViewTo(sf::RectangleShape _object)
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
	for (CBlock& block : m_Chunk)
	{
		block.Update();
		if (block.GetShape().getGlobalBounds().contains(MousePos))
		{
			m_MousePos.setPosition(block.GetShape().getPosition());
		}
		else if (block.GetShape().getGlobalBounds().intersects(m_MousePos.getGlobalBounds()))
		{
			m_MousePos.setPosition(block.GetShape().getPosition());
		}
		/*else
		{

			m_MousePos.setPosition(MousePos);
		}*/
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
