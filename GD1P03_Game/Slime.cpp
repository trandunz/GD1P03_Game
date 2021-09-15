#include "Slime.h"

Slime::Slime(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY)
{
	m_RenderWindow = _renderWindow;
	m_Texture = new sf::Texture();

	srand((unsigned)time(NULL));

	// Random Slime Type
	if (rand() % 5 == 0)
	{
		m_SlimeType = SLIMETYPE::GREEN;
	}
	else if (rand() % 5 == 1)
	{
		m_SlimeType = SLIMETYPE::RED;
	}
	else if (rand() % 5 == 2)
	{
		m_SlimeType = SLIMETYPE::BLUE;
	}
	else if (rand() % 5 == 3)
	{
		m_SlimeType = SLIMETYPE::YELLOW;
	}
	else if (rand() % 5 == 4)
	{
		m_SlimeType = SLIMETYPE::PURPLE;
	}
	else if (rand() % 5 == 4)
	{
		m_SlimeType = SLIMETYPE::BOSS;
	}
	else
	{
		m_SlimeType = SLIMETYPE::GREEN;
	}

	// Set Appropriate Texture
	switch (m_SlimeType)
	{
	case Slime::SLIMETYPE::GREEN:
	{
		m_Texture->loadFromFile("Images/Slime.png");
		break;
	}
	case Slime::SLIMETYPE::BLUE:
	{
		m_Texture->loadFromFile("Images/BlueSlime.png");
		break;
	}
	case Slime::SLIMETYPE::RED:
	{
		m_Texture->loadFromFile("Images/RedSlime.png");
		break;
	}
	case Slime::SLIMETYPE::PURPLE:
	{
		m_Texture->loadFromFile("Images/PurpleSlime.png");
		break;
	}
	case Slime::SLIMETYPE::YELLOW:
	{
		m_Texture->loadFromFile("Images/YellowSlime.png");
		break;
	}
	case Slime::SLIMETYPE::BOSS:
	{
		m_Texture->loadFromFile("Images/BossSlime.png");
		m_bIsBoss = true;
		break;
	}
	default:
	{
		m_Texture->loadFromFile("Images/Slime.png");
		break;
	}
	}
	
	m_World = &_world;
	m_Shape.setTexture(*m_Texture, true);
	m_Player = nullptr;

	CreateBody(_posX, _posY, b2_dynamicBody);
}

Slime::~Slime()
{
	//std::cout << "A Slime Died!" << std::endl;
	delete m_Texture;
	m_Player = nullptr;
}

void Slime::Start()
{
	
}

void Slime::Update()
{
	Movement();
	Attack();

	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);

	b2Contact* contact;
	for (contact = m_World->GetContactList(); contact; contact = contact->GetNext())
	{
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();
		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);

		b2Vec2 vel1 = m_Body->GetLinearVelocityFromWorldPoint(m_Body->GetPosition());
		b2Vec2 vel2 = b->GetBody()->GetLinearVelocityFromWorldPoint(worldManifold.points[0]);
		b2Vec2 impactVelocity = vel1 - vel2;
		b2Vec2 worldposition = { m_Shape.getPosition().x, m_Shape.getPosition().y };

		if (a->GetBody() == m_Body || b->GetBody() == m_Body)
		{
			// Can Take Fall Damage?
			if ((vel1.y > 0.0f))
			{
				m_bCanFallDamage = true;
			}

			// Fall Damage
			if (worldManifold.normal.y > 0 && m_bCanFallDamage)
			{
				//std::cout << "A Slime Landed" << "( Impact Velocity: " << impactVelocity.y << ")" << std::endl;
				//if (impactVelocity.y <= -25.5f)
				//{
				//	//m_AudioManager->PlayPlayerDeath();
				//	m_Health -= 300;
				//}

				m_bCanFallDamage = false;
			}
		}

		a = nullptr;
		b = nullptr;
	}
	contact = nullptr;
}

void Slime::Render()
{
	m_RenderWindow->draw(m_Shape);
}

void Slime::SetPlayer(CPlayer* _player)
{
	m_Player = _player;
}

void Slime::LoosePlayer()
{
	m_Player = nullptr;
}

bool Slime::bHasPlayer()
{
	if (m_Player == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Slime::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	//ground physics
	m_BodyDef.type = _type;
	m_BodyDef.fixedRotation = true;
	m_BodyDef.linearDamping = 0.6f;

	if (m_bIsBoss)
	{
		m_BodyDef.gravityScale = 3.0f;
	}
	else
	{
		m_BodyDef.gravityScale = 10.0f;
	}
	
	m_BodyDef.position = b2Vec2(_posX / m_Scale, (_posY / m_Scale));
	m_Body = m_World->CreateBody(&m_BodyDef);

	if (m_bIsBoss)
	{
		m_b2pShape.SetAsBox((300 / 2) / m_Scale, (300 / 2) / m_Scale);
	}
	else
	{
		m_b2pShape.SetAsBox((100 / 2) / m_Scale, (100 / 2) / m_Scale);
	}

	if (_sensor)
	{
		m_FixtureDef.isSensor = true;
	}
	m_FixtureDef.density = 2.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_FixtureDef.friction = 1.0f;
	m_FixtureDef.restitution = 0.1f;
	m_Body->CreateFixture(&m_FixtureDef);

	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

void Slime::Movement()
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

			switch (m_SlimeType)
			{
			case Slime::SLIMETYPE::GREEN:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 300, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				break;
			}
			case Slime::SLIMETYPE::BLUE:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 200, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 400), true);
				break;
			}
			case Slime::SLIMETYPE::RED:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 300, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				break;
			}
			case Slime::SLIMETYPE::PURPLE:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 400), true);
				break;
			}
			case Slime::SLIMETYPE::YELLOW:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 300, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				break;
			}
			case Slime::SLIMETYPE::BOSS:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 2400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 2400), true);
				break;
			}
			default:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 300, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				break;
			}
			}
		}
		else if (velocity.x < 1.0f && velocity.x > -1.0f && DirectionToPlayer > 0)
		{
			// Face Right
			m_Shape.setScale(-1, 1);

			// Move Right
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 400, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 400), true);
		}
	}
}

void Slime::Attack()
{
	float DistanceToPlayer;
	int DirectionToPlayer;

	if (bHasPlayer())
	{
		DistanceToPlayer = sqrt(((m_Player->m_Shape.getPosition().x - m_Shape.getPosition().x) * (m_Player->m_Shape.getPosition().x - m_Shape.getPosition().x)) + ((m_Player->m_Shape.getPosition().y - m_Shape.getPosition().y) * (m_Player->m_Shape.getPosition().y - m_Shape.getPosition().y)));
		DirectionToPlayer = m_Player->m_Shape.getPosition().x - m_Shape.getPosition().x ;

		if (DistanceToPlayer <= 350)
		{
			std::cout << "Attack Player" << std::endl;
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5,-200), true);
			m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1/2, 0), true);

			switch (m_SlimeType)
			{
			case Slime::SLIMETYPE::GREEN:
			{
				if (DistanceToPlayer <= 150)
				{
					m_Player->SetCurrentHP(m_Player->GetCurrentHP() - 12.25f);
				}
				break;
			}
			case Slime::SLIMETYPE::BLUE:
			{
				if (DistanceToPlayer <= 150)
				{
					m_Player->SetCurrentHP(m_Player->GetCurrentHP() - 10.0f);
				}
				break;
			}
			case Slime::SLIMETYPE::RED:
			{
				if (DistanceToPlayer <= 150)
				{
					m_Player->SetCurrentHP(m_Player->GetCurrentHP() - 15.0f);
				}
				break;
			}
			case Slime::SLIMETYPE::PURPLE:
			{
				if (DistanceToPlayer <= 150)
				{
					m_Player->SetCurrentHP(m_Player->GetCurrentHP() - 10.0f);
				}
				break;
			}
			case Slime::SLIMETYPE::YELLOW:
			{
				if (DistanceToPlayer <= 150)
				{
					m_Player->SetCurrentHP(m_Player->GetCurrentHP() - 13.25f);
				}
				break;
			}
			case Slime::SLIMETYPE::BOSS:
			{
				m_Player->SetCurrentHP(m_Player->GetCurrentHP() - 20.0f);
				break;
			}
			default:
			{
				if (DistanceToPlayer <= 150)
				{
					m_Player->SetCurrentHP(m_Player->GetCurrentHP() - 12.25f);
				}
				break;
			}
			}

			m_Player->m_AudioManager->PlayPlayerDamage();
		}
	}
}


