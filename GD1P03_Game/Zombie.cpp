#include "Zombie.h"

Zombie::Zombie(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY)
{
	m_RenderWindow = _renderWindow;
	m_Texture = new sf::Texture();
	m_Texture->loadFromFile("Images/ZombieLeft.png");
	m_World = &_world;
	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setTextureRect(sf::IntRect(0, 0, 100, 200));

	CreateBody(_posX, _posY, b2_dynamicBody);
}

Zombie::~Zombie()
{
}

void Zombie::Start()
{
}

void Zombie::Update()
{
	Movement();

	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

void Zombie::Render()
{
	m_RenderWindow->draw(m_Shape);
}

void Zombie::Movement()
{
	b2Vec2 worldposition = { m_Shape.getPosition().x, m_Shape.getPosition().y };
	b2Vec2 velocity = m_Body->GetLinearVelocityFromWorldPoint(worldposition);
	
	// Input Audio Right
	if (velocity.x > 1.0f)
	{
		if (m_AnimationTimer.getElapsedTime().asSeconds() >= 0.2f)
		{
			if (m_Shape.getTextureRect().left < 200)
			{
				m_Shape.setTextureRect(sf::IntRect(m_Shape.getTextureRect().left + 100, 0, 100, 200));
			}
			else if (m_Shape.getTextureRect() == sf::IntRect(200, 0, 100, 200))
			{
				m_Shape.setTextureRect(sf::IntRect(0, 0, 100, 200));
			}

			std::cout << velocity.x<< std::endl;
			m_AnimationTimer.restart();
		}
		
	}
	// Input Audio Left
	else if (velocity.x < -1.0f)
	{
		if (m_AnimationTimer.getElapsedTime().asSeconds() >= 0.2f)
		{
			if (m_Shape.getTextureRect().left < 200)
			{
				m_Shape.setTextureRect(sf::IntRect(m_Shape.getTextureRect().left + 100, 0, 100, 200));
			}
			else if (m_Shape.getTextureRect() == sf::IntRect(200, 0, 100, 200))
			{
				m_Shape.setTextureRect(sf::IntRect(0, 0, 100, 200));
			}

			std::cout << velocity.x << std::endl;
			m_AnimationTimer.restart();
		}
		
		
	}
}
