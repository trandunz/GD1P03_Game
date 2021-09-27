#include "Slime.h"

Slime::Slime(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CAudioManager& _audioManager)
{
	m_RenderWindow = _renderWindow;
	m_AudioManager = &_audioManager;
	m_Texture = new sf::Texture();

	srand(time(NULL));

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
		m_SlimeType = SLIMETYPE::BOSSGREEN;
	}
	else if (rand() % 5 == 4)
	{
		m_SlimeType = SLIMETYPE::BOSSBLUE;
	}
	else if (rand() % 5 == 4)
	{
		m_SlimeType = SLIMETYPE::BOSSRED;
	}
	else if (rand() % 5 == 4)
	{
		m_SlimeType = SLIMETYPE::BOSSPURPLE;
	}
	else if (rand() % 5 == 4)
	{
		m_SlimeType = SLIMETYPE::BOSSYELLOW;
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
		m_Health = 80;
		break;
	}
	case Slime::SLIMETYPE::RED:
	{
		m_Texture->loadFromFile("Images/RedSlime.png");
		m_Health = 200;
		break;
	}
	case Slime::SLIMETYPE::PURPLE:
	{
		m_Texture->loadFromFile("Images/PurpleSlime.png");
		m_Health = 80;
		break;
	}
	case Slime::SLIMETYPE::YELLOW:
	{
		m_Texture->loadFromFile("Images/YellowSlime.png");
		m_Health = 150;
		break;
	}
	case Slime::SLIMETYPE::BOSSGREEN:
	{
		m_Texture->loadFromFile("Images/BossSlimeGreen.png");
		m_bIsBoss = true;
		m_Health = 1000;
		break;
	}
	case Slime::SLIMETYPE::BOSSBLUE:
	{
		m_Texture->loadFromFile("Images/BossSlimeBlue.png");
		m_bIsBoss = true;
		m_Health = 800;
		break;
	}
	case Slime::SLIMETYPE::BOSSRED:
	{
		m_Texture->loadFromFile("Images/BossSlimeRed.png");
		m_bIsBoss = true;
		m_Health = 1400;
		break;
	}
	case Slime::SLIMETYPE::BOSSPURPLE:
	{
		m_Texture->loadFromFile("Images/BossSlimePurple.png");
		m_bIsBoss = true;
		m_Health = 900;
		break;
	}
	case Slime::SLIMETYPE::BOSSYELLOW:
	{
		m_Texture->loadFromFile("Images/BossSlimeYellow.png");
		m_bIsBoss = true;
		m_Health = 1200;
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

	Slime::CreateBody(_posX, _posY, b2_dynamicBody);
}

Slime::~Slime()
{
	m_TextureMaster = nullptr;

	delete m_DamageTimer;
	m_DamageTimer = nullptr;

	delete m_Texture;
	m_Player = nullptr;
}

void Slime::Start()
{
	m_DamageTimer = new sf::Clock();
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
				if (impactVelocity.y <= -40.5f)
				{
					//m_AudioManager->PlayPlayerDeath();
					TakeDamage(-impactVelocity.y);
				}

				m_bCanFallDamage = false;
			}

			if (a->GetBody()->GetFixtureList()->IsSensor() || b->GetBody()->GetFixtureList()->IsSensor())
			{
				int* damage = nullptr;
				if (a->GetBody()->GetFixtureList()->IsSensor() && a->GetBody()->GetUserData().pointer)
				{
					int* damage = (int*) a->GetBody()->GetUserData().pointer;
					TakeDamage(*damage, true);
					std::cout << "Damage: " << *damage << std::endl;
				}
				else if (b->GetBody()->GetUserData().pointer)
				{
					int* damage = (int*)b->GetBody()->GetUserData().pointer;
					TakeDamage(*damage, true);
					std::cout << "Damage: " << *damage << std::endl;
				}
				damage = nullptr;
			}
		}

		

		a = nullptr;
		b = nullptr;
	}
	contact = nullptr;
}

void Slime::Render(sf::Shader* _shader)
{
	float DistanceToPlayer;
	if (m_Player != nullptr)
	{
		DistanceToPlayer = sqrt(((m_Player->GetShape().getPosition().x - m_Shape.getPosition().x) * (m_Player->GetShape().getPosition().x - m_Shape.getPosition().x)) + ((m_Player->GetShape().getPosition().y - m_Shape.getPosition().y) * (m_Player->GetShape().getPosition().y - m_Shape.getPosition().y)));
	}
	else
	{
		DistanceToPlayer = 0;
	}
	
	if (DistanceToPlayer > 1920 * 1.8)
	{
	}
	else if (DistanceToPlayer <= 1920 * 1.8 && m_Shape.getPosition().y < 1100)
	{
		m_RenderWindow->draw(m_Shape);
	}
	else if (DistanceToPlayer <= 1920 * 1.8 && m_Shape.getPosition().y > 1100 && m_Player != nullptr)
	{
		_shader->setUniform("hasTexture", true);
		_shader->setUniform("lightPos", m_Player->GetShape().getPosition());
		m_RenderWindow->draw(m_Shape, _shader);
	}
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
	m_BodyDef.bullet = false;
	m_Body = m_World->CreateBody(&m_BodyDef);

	if (m_bIsBoss)
	{
		if (m_SlimeType == SLIMETYPE::BOSSBLUE)
		{
			m_b2pShape.SetAsBox((250 / 2) / m_Scale, (250 / 2) / m_Scale);
		}
		else if (m_SlimeType == SLIMETYPE::BOSSPURPLE)
		{
			m_b2pShape.SetAsBox((250 / 2) / m_Scale, (250 / 2) / m_Scale);
		}
		else if (m_SlimeType == SLIMETYPE::BOSSRED)
		{
			m_b2pShape.SetAsBox((350 / 2) / m_Scale, (350 / 2) / m_Scale);
		}
		else
		{
			m_b2pShape.SetAsBox((300 / 2) / m_Scale, (300 / 2) / m_Scale);
		}
		
	}
	else
	{
		if (m_SlimeType == SLIMETYPE::RED)
		{
			m_b2pShape.SetAsBox((120 / 2) / m_Scale, (120 / 2) / m_Scale);
		}
		else if (m_SlimeType == SLIMETYPE::PURPLE)
		{
			m_b2pShape.SetAsBox((80 / 2) / m_Scale, (80 / 2) / m_Scale);
		}
		else if (m_SlimeType == SLIMETYPE::BLUE)
		{
			m_b2pShape.SetAsBox((80 / 2) / m_Scale, (80 / 2) / m_Scale);
		}
		else
		{
			m_b2pShape.SetAsBox((100 / 2) / m_Scale, (100 / 2) / m_Scale);
		}
	}

	if (_sensor)
	{
		m_FixtureDef.isSensor = true;
	}
	m_FixtureDef.density = 2.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_FixtureDef.friction = 1.0f;
	m_FixtureDef.restitution = 0.1f;
	m_FixtureDef.filter.categoryBits = _ENEMY_FILTER_;
	m_FixtureDef.filter.maskBits = _PLAYER_FILTER_;
	m_FixtureDef.filter.maskBits = _WORLD_FILTER_;
	m_FixtureDef.filter.groupIndex = -_ENEMY_GROUPINDEX_;
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
		DistanceToPlayer = sqrt(((m_Player->GetShape().getPosition().x - m_Shape.getPosition().x) * (m_Player->GetShape().getPosition().x - m_Shape.getPosition().x)) + ((m_Player->GetShape().getPosition().y - m_Shape.getPosition().y) * (m_Player->GetShape().getPosition().y - m_Shape.getPosition().y)));
		DirectionToPlayer = m_Player->GetShape().getPosition().x - m_Shape.getPosition().x;
	}
	else
	{
		DistanceToPlayer = 0.0f;
		DirectionToPlayer = 0;
	}

	// Movement
	if (DistanceToPlayer > 150.0f)
	{
		// Move Left
		if (velocity.x < 1.0f && velocity.x > -1.0f && DirectionToPlayer < 0)
		{

			// Face Left
			m_Shape.setScale(1, 1);

			switch (m_SlimeType)
			{
			case Slime::SLIMETYPE::GREEN:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % -300, 0.0f), true);
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
			case Slime::SLIMETYPE::BOSSGREEN:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 2400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 2400), true);
				break;
			}
			case Slime::SLIMETYPE::BOSSBLUE:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 3400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 3400), true);
				break;
			}
			case Slime::SLIMETYPE::BOSSRED:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 2000, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 2000), true);
				break;
			}
			case Slime::SLIMETYPE::BOSSPURPLE:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 3400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 3400), true);
				break;
			}
			case Slime::SLIMETYPE::BOSSYELLOW:
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
		// Move Right
		else if (velocity.x < 1.0f && velocity.x > -1.0f && DirectionToPlayer > 0)
		{
			// Face Right
			m_Shape.setScale(-1, 1);

			switch (m_SlimeType)
			{
			case Slime::SLIMETYPE::GREEN:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 300, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -200 - rand() % 300), true);
				break;
			}
			case Slime::SLIMETYPE::BLUE:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 200, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 400), true);
				break;
			}
			case Slime::SLIMETYPE::RED:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 300, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				break;
			}
			case Slime::SLIMETYPE::PURPLE:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 400), true);
				break;
			}
			case Slime::SLIMETYPE::YELLOW:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 300, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				break;
			}
			case Slime::SLIMETYPE::BOSSGREEN:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 2400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 2400), true);
				break;
			}
			case Slime::SLIMETYPE::BOSSBLUE:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 3400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 3400), true);
				break;
			}
			case Slime::SLIMETYPE::BOSSRED:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 2000, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 2000), true);
				break;
			}
			case Slime::SLIMETYPE::BOSSPURPLE:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 3400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 3400), true);
				break;
			}
			case Slime::SLIMETYPE::BOSSYELLOW:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 2800, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 2800), true);
				break;
			}
			default:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 300, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				break;
			}
			}
		}
	}
}

void Slime::Attack()
{
	float DistanceToPlayer;
	int DirectionToPlayer;

	if (bHasPlayer())
	{
		DistanceToPlayer = sqrt(((m_Player->GetShape().getPosition().x - m_Shape.getPosition().x) * (m_Player->GetShape().getPosition().x - m_Shape.getPosition().x)) + ((m_Player->GetShape().getPosition().y - m_Shape.getPosition().y) * (m_Player->GetShape().getPosition().y - m_Shape.getPosition().y)));
		DirectionToPlayer = m_Player->GetShape().getPosition().x - m_Shape.getPosition().x ;

		if (DistanceToPlayer <= 310)
		{
			switch (m_SlimeType)
			{
			case Slime::SLIMETYPE::GREEN:
			{
				if (DistanceToPlayer <= 130)
				{
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, -200), true);
					m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

					m_Player->TakeDamage(12.25f);
				}
				break;
			}
			case Slime::SLIMETYPE::BLUE:
			{
				if (DistanceToPlayer <= 110)
				{
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, -200), true);
					m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

					m_Player->TakeDamage(10.0f);
				}
				break;
			}
			case Slime::SLIMETYPE::RED:
			{
				if (DistanceToPlayer <= 150)
				{
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, -200), true);
					m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

					m_Player->TakeDamage(15.0f);
				}
				break;
			}
			case Slime::SLIMETYPE::PURPLE:
			{
				if (DistanceToPlayer <= 110)
				{
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, -200), true);
					m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

					m_Player->TakeDamage(10.0f);
				}
				break;
			}
			case Slime::SLIMETYPE::YELLOW:
			{
				if (DistanceToPlayer <= 130)
				{
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, -200), true);
					m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

					m_Player->TakeDamage(13.25f);
				}
				break;
			}
			case Slime::SLIMETYPE::BOSSGREEN:
			{
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, -200), true);
				m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

				m_Player->TakeDamage(20.0f);
				break;
			}
			case Slime::SLIMETYPE::BOSSBLUE:
			{
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, -200), true);
				m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

				m_Player->TakeDamage(20.0f);
				break;
			}
			case Slime::SLIMETYPE::BOSSRED:
			{
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, -200), true);
				m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

				m_Player->TakeDamage(30.0f);
				break;
			}
			case Slime::SLIMETYPE::BOSSPURPLE:
			{
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, -200), true);
				m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

				m_Player->TakeDamage(25.0f);
				break;
			}
			case Slime::SLIMETYPE::BOSSYELLOW:
			{
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, -200), true);
				m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

				m_Player->TakeDamage(25.0f);
				break;
			}
			default:
			{
				if (DistanceToPlayer <= 130)
				{
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, -200), true);
					m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

					m_Player->TakeDamage(12.25f);
				}
				break;
			}
			}
		}
	}
}

void Slime::TakeDamage(float _damage, bool _projectile)
{
	if (m_DamageTimer->getElapsedTime().asSeconds() >= 0.5f)
	{
		if (_projectile)
		{
			//std::cout << "Slime Damaged! " << "(" << _damage << ")" << std::endl;
		}
		
		m_Health -= _damage;
		m_DamageTimer->restart();

		if (m_Player != nullptr)
		{
			// Distance Based Sound
			float Mag1 = sqrt(((m_Shape.getPosition().x - m_Player->GetShape().getPosition().x) * (m_Shape.getPosition().x - m_Player->GetShape().getPosition().x)) + ((m_Shape.getPosition().y - m_Player->GetShape().getPosition().y) * (m_Shape.getPosition().y - m_Player->GetShape().getPosition().y)));
			if (Mag1 <= 1920 * 1.8f && Mag1 <= 520 * 1.8f)
			{
				m_AudioManager->PlaySlimeDamage(30);
			}
			else if (Mag1 <= 1920 * 1.8f && Mag1 <= 720 * 1.8f)
			{
				m_AudioManager->PlaySlimeDamage(25);
			}
			else if (Mag1 <= 1920 * 1.8f && Mag1 <= 920 * 1.8f)
			{
				m_AudioManager->PlaySlimeDamage(20);
			}
			else if (Mag1 <= 1920 * 1.8f && Mag1 <= 1120 * 1.8f)
			{
				m_AudioManager->PlaySlimeDamage(15);
			}
			else if (Mag1 <= 1920 * 1.8f && Mag1 <= 1320 * 1.8f)
			{
				m_AudioManager->PlaySlimeDamage(10);
			}
			else if (Mag1 <= 1920 * 1.8f && Mag1 <= 1520 * 1.8f)
			{
				m_AudioManager->PlaySlimeDamage(5);
			}
			else if (Mag1 <= 1920 * 1.8f && Mag1 <= 1720 * 1.8f)
			{
				m_AudioManager->PlaySlimeDamage(3);
			}
			else if (Mag1 <= 1920 * 1.8f)
			{
				m_AudioManager->PlaySlimeDamage(1);
			}
		}
	}
}


