#include "CSnowman.h"

CSnowman::CSnowman(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CAudioManager& _audioManager, bool _boss)
{
	m_RenderWindow = _renderWindow;
	m_AudioManager = &_audioManager;
	m_Texture = new sf::Texture();
	m_bIsBoss = _boss;

	m_World = &_world;
	srand(time(NULL));

	if (!_boss)
	{
		m_SnowmanType = SNOWMANTYPE::GREEN;
	}
	else
	{
		if (rand() % 5 == 1)
		{
			m_SnowmanType = SNOWMANTYPE::BOSSGREEN;
		}
		else if (rand() % 5 == 2)
		{
			m_SnowmanType = SNOWMANTYPE::BOSSBLUE;
		}
		else if (rand() % 5 == 3)
		{
			m_SnowmanType = SNOWMANTYPE::BOSSRED;
		}
		else if (rand() % 5 == 0)
		{
			m_SnowmanType = SNOWMANTYPE::BOSSYELLOW;
		}
		else
		{
			m_SnowmanType = SNOWMANTYPE::BOSSGREEN;
		}
	}

	// Set Appropriate Texture
	switch (m_SnowmanType)
	{
	case CSnowman::SNOWMANTYPE::BOSSGREEN:
	{
		m_Texture->loadFromFile("Images/BossSnowManGreen.png");
		m_bIsBoss = true;
		m_Health = 3000;
		break;
	}
	case CSnowman::SNOWMANTYPE::BOSSBLUE:
	{
		m_Texture->loadFromFile("Images/BossSnowManBlue.png");
		m_bIsBoss = true;
		m_Health = 2800;
		break;
	}
	case CSnowman::SNOWMANTYPE::BOSSRED:
	{
		m_Texture->loadFromFile("Images/BossSnowManRed.png");
		m_bIsBoss = true;
		m_Health = 3800;
		break;
	}
	case CSnowman::SNOWMANTYPE::BOSSYELLOW:
	{
		m_Texture->loadFromFile("Images/BossSnowManYellow.png");
		m_bIsBoss = true;
		m_Health = 3400;
		break;
	}
	default:
	{
		m_Texture->loadFromFile("Images/Snowman.png");
		m_Health = 150;
		break;
	}
	}
	m_Shape.setTexture(*m_Texture, true);
	m_Player = nullptr;

	CSnowman::CreateBody(_posX, _posY, b2_dynamicBody);
}

CSnowman::~CSnowman()
{
	for (std::list<CProjectile>::iterator it = m_Projectiles.begin(); it != m_Projectiles.end(); it++)
	{
		it = m_Projectiles.erase(it);
	}
	m_Projectiles.clear();

	m_TextureMaster = nullptr;

	delete m_Texture;
	m_Player = nullptr;
}

void CSnowman::Start()
{
	SimpleMove();

	// 3rd Level = Triple The Damage As First Level
	m_Damage *= 3;
}

void CSnowman::Update()
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

			if (a->GetBody()->GetFixtureList()->IsSensor() || b->GetBody()->GetFixtureList()->IsSensor())
			{
				if (a->GetBody()->GetFixtureList()->IsSensor() && a->GetBody()->GetUserData().pointer)
				{
					int* damage = (int*)a->GetBody()->GetUserData().pointer;
					TakeDamage(*damage, true);
					std::cout << "Damage: " << *damage << std::endl;
					damage = nullptr;
				}
				else if (b->GetBody()->GetUserData().pointer)
				{
					int* damage = (int*)b->GetBody()->GetUserData().pointer;
					TakeDamage(*damage, true);
					std::cout << "Damage: " << *damage << std::endl;
					damage = nullptr;
				}
			}
		}

		a = nullptr;
		b = nullptr;
	}
	contact = nullptr;

	for (CProjectile& projectile : m_Projectiles)
	{
		projectile.Update();
	}

	// Delete All Collided Projectiles
	std::list<CProjectile>::iterator pit = m_Projectiles.begin();
	while (pit != m_Projectiles.end())
	{
		if (pit->m_bMARKASDESTROY)
		{
			pit = m_Projectiles.erase(pit);
		}
		pit++;
	}
}

void CSnowman::Render(sf::Shader* _shader)
{
	if (m_Shape.getPosition().y < 1100)
	{
		m_RenderWindow->draw(m_Shape);
	}
	else
	{
		m_RenderWindow->draw(m_Shape, _shader);
	}

	sf::Shader* nullshader = NULL;

	// Draw All Projectiles
	for (CProjectile& projectile : m_Projectiles)
	{
		m_RenderWindow->draw(projectile.GetShape(), nullshader);
	}
	nullshader = nullptr;
}

void CSnowman::SetPlayer(CPlayer* _player)
{
	m_Player = _player;
}

void CSnowman::LoosePlayer()
{
	m_Player = nullptr;
}

bool CSnowman::bHasPlayer()
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

void CSnowman::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
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
		m_b2pShape.SetAsBox((300 / 2) / m_Scale, (600 / 2) / m_Scale);
	}
	else
	{
		m_b2pShape.SetAsBox((100 / 2) / m_Scale, (200 / 2) / m_Scale);
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
	m_FixtureDef.filter.maskBits = _WORLD_FILTER_;
	m_FixtureDef.filter.groupIndex = -_PLAYER_GROUPINDEX_;
	m_Body->CreateFixture(&m_FixtureDef);

	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

void CSnowman::Movement()
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
	if (DistanceToPlayer > 100.0f)
	{
		// Move Left
		if (velocity.x < 1.0f && velocity.x > -1.0f && DirectionToPlayer < 0)
		{
			// Face Left
			m_Shape.setScale(1, 1);

			switch (m_SnowmanType)
			{
			case CSnowman::SNOWMANTYPE::GREEN:
			{
				if (DistanceToPlayer < m_DetectionRange)
				{
					// Move Left
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % -300, 0.0f), true);
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				}

				break;
			}
			case CSnowman::SNOWMANTYPE::BLUE:
			{
				if (DistanceToPlayer < m_DetectionRange)
				{
					// Move Left
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 200, 0.0f), true);
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 400), true);
				}

				break;
			}
			case CSnowman::SNOWMANTYPE::RED:
			{
				if (DistanceToPlayer < m_DetectionRange)
				{
					// Move Left
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 300, 0.0f), true);
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				}

				break;
			}
			case CSnowman::SNOWMANTYPE::YELLOW:
			{
				if (DistanceToPlayer < m_DetectionRange)
				{
					// Move Left
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 300, 0.0f), true);
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				}

				break;
			}
			case CSnowman::SNOWMANTYPE::BOSSGREEN:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-3000 - rand() % 4400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 3400), true);
				break;
			}
			case CSnowman::SNOWMANTYPE::BOSSBLUE:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-3000 - rand() % 4400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 3400), true);
				break;
			}
			case CSnowman::SNOWMANTYPE::BOSSRED:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-3000 - rand() % 4000, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 3000), true);
				break;
			}
			case CSnowman::SNOWMANTYPE::BOSSYELLOW:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(-2000 - rand() % 4400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 3400), true);
				break;
			}
			default:
			{
				if (DistanceToPlayer < m_DetectionRange)
				{
					// Move Left
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(-1000 - rand() % 300, 0.0f), true);
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				}
				break;
			}
			}
		}
		// Move Right
		else if (velocity.x < 1.0f && velocity.x > -1.0f && DirectionToPlayer > 0)
		{
			// Face Right
			m_Shape.setScale(-1, 1);

			switch (m_SnowmanType)
			{
			case CSnowman::SNOWMANTYPE::GREEN:
			{
				if (DistanceToPlayer < m_DetectionRange)
				{
					// Move Left
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 300, 0.0f), true);
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -200 - rand() % 300), true);
				}

				break;
			}
			case CSnowman::SNOWMANTYPE::BLUE:
			{
				if (DistanceToPlayer < m_DetectionRange)
				{
					// Move Left
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 200, 0.0f), true);
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 400), true);
				}
				break;
			}
			case CSnowman::SNOWMANTYPE::RED:
			{
				if (DistanceToPlayer < m_DetectionRange)
				{
					// Move Left
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 300, 0.0f), true);
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				}
				break;
			}
			case CSnowman::SNOWMANTYPE::YELLOW:
			{
				if (DistanceToPlayer < m_DetectionRange)
				{
					// Move Left
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 300, 0.0f), true);
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				}
				break;
			}
			case CSnowman::SNOWMANTYPE::BOSSGREEN:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(3000 + rand() % 4400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 3400), true);
				break;
			}
			case CSnowman::SNOWMANTYPE::BOSSBLUE:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(3000 + rand() % 4400, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 3400), true);
				break;
			}
			case CSnowman::SNOWMANTYPE::BOSSRED:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(3000 + rand() % 4000, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 3000), true);
				break;
			}
			case CSnowman::SNOWMANTYPE::BOSSYELLOW:
			{
				// Move Left
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(3000 + rand() % 4800, 0.0f), true);
				m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 3800), true);
				break;
			}
			default:
			{
				if (DistanceToPlayer < m_DetectionRange)
				{
					// Move Left
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 300, 0.0f), true);
					m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
				}
				break;
			}
			}
		}
	}
}

void CSnowman::SimpleMove()
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

	// Move Left
	if (velocity.x < 1.0f && velocity.x > -1.0f)
	{

		// Face Left
		m_Shape.setScale(1, 1);

		// Move Left
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 300, 0.0f), true);
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
	}
	// Move Right
	else if (velocity.x < 1.0f && velocity.x > -1.0f)
	{
		// Face Right
		m_Shape.setScale(-1, 1);

		// Move Left
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 300, 0.0f), true);
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
	}
}

void CSnowman::Attack()
{
	float DistanceToPlayer = 0;
	int DirectionToPlayer = 0;

	if (bHasPlayer() && m_AttackTimer.getElapsedTime().asSeconds() >= m_AttackSpeed)
	{
		DistanceToPlayer = sqrt(((m_Player->GetShape().getPosition().x - m_Shape.getPosition().x) * (m_Player->GetShape().getPosition().x - m_Shape.getPosition().x)) + ((m_Player->GetShape().getPosition().y - m_Shape.getPosition().y) * (m_Player->GetShape().getPosition().y - m_Shape.getPosition().y)));
		DirectionToPlayer = m_Player->GetShape().getPosition().x - m_Shape.getPosition().x;
		float UnitDirecton = DirectionToPlayer / sqrt(((DirectionToPlayer) * (DirectionToPlayer)));

		// Melee
		if (DistanceToPlayer <= 130)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, 0), true);
			m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

			m_Player->TakeDamage(m_Damage);
			m_AttackTimer.restart();
		}
		// Snowball
		else if (m_Player != nullptr)
		{
			if (m_SnowballTimer.getElapsedTime().asSeconds() >= 2.0f)
			{
				std::cout << "Snowman Fired Snowball!" << std::endl;

				// throw snowball
				CProjectile* m_Projectile = new CProjectile(*m_World, m_Shape.getPosition().x + 20 * UnitDirecton, m_Shape.getPosition().y - 50, m_RenderWindow->getView().getCenter(), CBlock::PROJECTILETYPE::SNOWBALL, nullptr, false);
				m_Projectiles.push_back(*m_Projectile);
				m_Projectile = nullptr;

				m_SnowballTimer.restart();
			}
			
		}
	}
}
