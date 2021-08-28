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
	const float m_Scale = 40.0f;
};

// Forward Declaration
void Start();
void Update();
void Render();
void InitView();

void CenterViewTo(sf::RectangleShape _object);

void BodyUpdates();

// Main Render Window
sf::RenderWindow* m_RenderWindow;

// Main View / Camera
sf::View m_View;

// b2World
b2Vec2 m_Gravity(0.0f, 10.0f);
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


	Start();
	Update();


	// Cleanup
	delete m_Block;
	delete m_Player;
	delete m_RenderWindow;
	m_Block = nullptr;
	m_Player = nullptr;
	m_RenderWindow = nullptr;

	return 0;
}

void Start()
{
	m_Player = new CPlayer(m_RenderWindow, m_World, Utils::m_Scale);
	m_Block = new CBlock(m_RenderWindow, m_World, Utils::m_Scale, 100, 400);
	m_Block->SetSize(2000, 400);

	InitView();
	CenterViewTo(m_Player->GetShape());
}

void Update()
{
	while (m_RenderWindow->isOpen())
	{
		sf::Event event;
		while (m_RenderWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_RenderWindow->close();
				// Cleanup
				delete m_Block;
				delete m_Player;
				delete m_RenderWindow;
				m_Block = nullptr;
				m_Player = nullptr;
				m_RenderWindow = nullptr;
				break;
			}
		}

		// Centre View To Player
		CenterViewTo(m_Player->GetShape());

		// World Step
		m_World.Step(1 / 60.f, 10, 30);

		// Body Updates
		BodyUpdates();

		// Render
		Render();
	}
}

void Render()
{
	m_RenderWindow->clear();

	// Bodies
	for (b2Body* BodyIterator = m_World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
	{
		// Dynamic
		if (BodyIterator->GetType() == b2_dynamicBody)
		{
			m_Player->Render();
		}

		// Static
		if (BodyIterator->GetType() == b2_staticBody)
		{
			m_Block->Render();

		}
	}

	m_RenderWindow->display();
}

void InitView()
{
	m_View = sf::View(sf::Vector2f(0.0f,0.0f), sf::Vector2f(m_RenderWindow->getSize().x, m_RenderWindow->getSize().y));
	m_View.zoom(4);
	m_RenderWindow->setView(m_View);
}

void CenterViewTo(sf::RectangleShape _object)
{
	m_View.setCenter(_object.getPosition());
	m_RenderWindow->setView(m_View);
}

void BodyUpdates()
{
	// Bodies
	for (b2Body* BodyIterator = m_World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
	{
		// Dynamic
		if (BodyIterator->GetType() == b2_dynamicBody)
		{
			m_Player->Update(BodyIterator);
		}

		// Static
		else if (BodyIterator->GetType() == b2_staticBody)
		{
			m_Block->Update(BodyIterator);

		}
	}
}
