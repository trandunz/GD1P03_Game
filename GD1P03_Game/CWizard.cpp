#include "CWizard.h"

CWizard::CWizard(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CAudioManager& _audioManager)
{
	m_RenderWindow = _renderWindow;
	m_AudioManager = &_audioManager;
	m_Texture = new sf::Texture();
	m_Texture->loadFromFile("Images/PlayerLeft.png");

	srand(time(NULL));

	m_World = &_world;
	m_Shape.setTexture(*m_Texture, true);

	CWizard::CreateBody(_posX, _posY, b2_dynamicBody);
}

CWizard::~CWizard()
{
	DestroyBody();

	m_TextureMaster = nullptr;

	delete m_Texture;
	m_Texture = nullptr;
}

void CWizard::Update()
{
	Movement();

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
				int* damage = nullptr;
				if (a->GetBody()->GetFixtureList()->IsSensor() && a->GetBody()->GetUserData().pointer)
				{
					int* damage = (int*)a->GetBody()->GetUserData().pointer;
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

void CWizard::Movement()
{
	// Move Left
	m_Body->ApplyLinearImpulseToCenter(b2Vec2(-3000 - rand() % 4400, 0.0f), true);
	m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 6400), true);
}

void CWizard::Attack()
{
}

void CWizard::TakeDamage(float _damage, bool _projectile)
{
	if (m_DamageTimer.getElapsedTime().asSeconds() >= 0.5f)
	{
		if (_projectile)
		{
			//std::cout << "Slime Damaged! " << "(" << _damage << ")" << std::endl;
		}

		m_Health -= _damage;
		m_DamageTimer.restart();

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

void CWizard::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	//ground physics
	m_BodyDef.type = _type;
	m_BodyDef.fixedRotation = true;
	m_BodyDef.linearDamping = 0.6f;

	m_BodyDef.position = b2Vec2(_posX / m_Scale, (_posY / m_Scale));
	m_BodyDef.bullet = false;
	m_Body = m_World->CreateBody(&m_BodyDef);

	if (_sensor)
	{
		m_FixtureDef.isSensor = true;
	}

	m_b2pShape.SetAsBox(100/m_Scale, 200 / m_Scale);

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
