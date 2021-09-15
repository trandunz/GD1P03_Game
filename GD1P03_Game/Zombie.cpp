#include "Zombie.h"

Zombie::Zombie(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY)
{
	m_RenderWindow = _renderWindow;
	m_Texture = new sf::Texture();
	m_Texture->loadFromFile("Images/ZombieLeft.png");
	m_World = &_world;
	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setTextureRect(sf::IntRect(0, 0, 100, 200));
	m_Player = nullptr;

	CreateBody(_posX, _posY, b2_dynamicBody);
}

Zombie::~Zombie()
{
	m_Player = nullptr;
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

void Zombie::SetPlayer(CPlayer* _player)
{
	m_Player = _player;
}

void Zombie::LoosePlayer()
{
	m_Player = nullptr;
}

bool Zombie::bHasPlayer()
{
	if(m_Player == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Zombie::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	//ground physics
	m_BodyDef.type = _type;
	m_BodyDef.fixedRotation = true;
	m_BodyDef.linearDamping = 0.6f;
	m_BodyDef.gravityScale = 10.0f;
	m_BodyDef.position = b2Vec2(_posX / m_Scale, (_posY / m_Scale));
	m_Body = m_World->CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((100 / 2) / m_Scale, (200 / 2) / m_Scale);

	if (_sensor)
	{
		m_FixtureDef.isSensor = true;
	}
	m_FixtureDef.density = 2.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_FixtureDef.friction = 1.0f;
	m_FixtureDef.restitution = 0.3f;
	m_Body->CreateFixture(&m_FixtureDef);

	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

void Zombie::Movement()
{
	b2Vec2 worldposition = { m_Shape.getPosition().x, m_Shape.getPosition().y };
	b2Vec2 velocity = m_Body->GetLinearVelocityFromWorldPoint(worldposition);

	float DistanceToPlayer;
	int DirectionToPlayer;
	if (bHasPlayer())
	{
		DistanceToPlayer = sqrt(((m_Player->m_Shape.getPosition().x - m_Shape.getPosition().x) * (m_Player->m_Shape.getPosition().x - m_Shape.getPosition().x)) + ((m_Player->m_Shape.getPosition().y - m_Shape.getPosition().y) * (m_Player->m_Shape.getPosition().y - m_Shape.getPosition().y)));
		DirectionToPlayer = m_Player->m_Shape.getPosition().x - m_Shape.getPosition().x;
	}
	else
	{
		DistanceToPlayer = 0.0f;
		DirectionToPlayer = 0;
	}

	// Movement
	if (DistanceToPlayer > 100.0f)
	{
		if (velocity.x < 1.0f && velocity.x > -1.0f && DirectionToPlayer < 0)
		{
			// Face Left
			m_Shape.setScale(1, 1);

			// Move Left
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(-300.0f, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -400.0f), true);
		}
		else if (velocity.x < 1.0f && velocity.x > -1.0f && DirectionToPlayer > 0)
		{
			// Face Right
			m_Shape.setScale(-1, 1);

			// Move Right
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(300.0f, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -400.0f), true);
		}
	}

	//
	// Animation
	//
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

			m_AnimationTimer.restart();
		}
		
		
	}
	else
	{
		m_Shape.setTextureRect(sf::IntRect(0, 0, 100, 200));
	}
}
