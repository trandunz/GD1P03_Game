#include "CProjectile.h"

CProjectile::CProjectile(CBlock::PROJECTILETYPE _projtype)
{
	m_Scale = 50.0f;
	m_Body = nullptr;
	m_Type = BLOCKTYPE::PROJECTILE;
	m_Texture = nullptr;
	m_Texture = new sf::Texture();
	m_Type = CBlock::BLOCKTYPE::PROJECTILE;
	m_ProjectileType = _projtype;

	// Type Specific Data
	switch (m_ProjectileType)
	{
	case CBlock::PROJECTILETYPE::ARROW:
		m_Texture->loadFromFile("Images/Arrow.png");
		m_Damage = 50.0f;
		break;
	case CBlock::PROJECTILETYPE::FIREARROW:
		m_Texture->loadFromFile("Images/FireArrow.png");
		m_Damage = 60.0f;
		break;
	case CBlock::PROJECTILETYPE::POISONARROW:
		m_Texture->loadFromFile("Images/PoisonArrow.png");
		70.0f;
		break;
	case CBlock::PROJECTILETYPE::CURSEDARROW:
		m_Texture->loadFromFile("Images/CursedArrow.png");
		m_Damage = 80.0f;
		break;

	case CBlock::PROJECTILETYPE::IRONBULLET:
		m_Texture->loadFromFile("Images/IronBullet.png");
		m_Damage = 90.0f;
		m_Bullet = true;
		break;
	case CBlock::PROJECTILETYPE::GOLDBULLET:
		m_Texture->loadFromFile("Images/GoldBullet.png");
		m_Damage = 100.0f;
		m_Bullet = true;
		break;
	case CBlock::PROJECTILETYPE::PURPLEBULLET:
		m_Texture->loadFromFile("Images/PurpleBullet.png");
		m_Damage = 110.0f;
		m_Bullet = true;
		break;
	case CBlock::PROJECTILETYPE::GOLDENBULLET:
		m_Texture->loadFromFile("Images/GoldenBullet.png");
		m_Damage = 120.0f;
		m_Bullet = true;
		break;
	default:
		m_Texture->loadFromFile("Images/Arrow.png");
		m_Damage = 50.0f;
		break;
	}

	// Sprite
	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setScale(0.3f, 0.3f);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
}

CProjectile::CProjectile(b2World& _world, float _startPosX, float _startPosY, sf::Vector2f _mousPos, CBlock::PROJECTILETYPE _projtype, Bow* _activeBow)
{
	m_World = &_world;
	m_Texture = new sf::Texture();
	m_ProjectileType = _projtype;
	m_Type = CBlock::BLOCKTYPE::PROJECTILE;

	// Type Specific Data
	switch (m_ProjectileType)
	{
	case CBlock::PROJECTILETYPE::ARROW:
		m_Texture->loadFromFile("Images/Arrow.png");
		m_Damage = 25.0f;
		break;
	case CBlock::PROJECTILETYPE::FIREARROW:
		m_Texture->loadFromFile("Images/FireArrow.png");
		m_Damage = 50.0f;
		break;
	case CBlock::PROJECTILETYPE::POISONARROW:
		m_Texture->loadFromFile("Images/PoisonArrow.png");
		m_Damage = 50.0f;
		break;
	case CBlock::PROJECTILETYPE::CURSEDARROW:
		m_Texture->loadFromFile("Images/CursedArrow.png");
		m_Damage = 75.0f;
		break;

	case CBlock::PROJECTILETYPE::IRONBULLET:
		m_Texture->loadFromFile("Images/IronBullet.png");
		m_Damage = 100.0f;
		m_Bullet = true;
		break;
	case CBlock::PROJECTILETYPE::GOLDBULLET:
		m_Texture->loadFromFile("Images/GoldBullet.png");
		m_Damage = 125.0f;
		m_Bullet = true;
		break;
	case CBlock::PROJECTILETYPE::PURPLEBULLET:
		m_Texture->loadFromFile("Images/PurpleBullet.png");
		m_Damage = 150.0f;
		m_Bullet = true;
		break;
	case CBlock::PROJECTILETYPE::GOLDENBULLET:
		m_Texture->loadFromFile("Images/GoldenBullet.png");
		m_Damage = 175.0f;
		m_Bullet = true;
		break;
	default:
		m_Texture->loadFromFile("Images/Arrow.png");
		m_Damage = 25.0f;
		break;
	}
	
	// Sprite
	m_Shape.setTexture(*m_Texture, true);

	// b2Body
	CreateBody(_startPosX, _startPosY, b2_dynamicBody, true);

	// Apple Force
	if (_activeBow != nullptr)
	{
		float dx = _mousPos.x - m_Shape.getPosition().x;
		float dy = _mousPos.y - m_Shape.getPosition().y;
		m_Body->SetLinearVelocity(b2Vec2(dx / 25 / m_Weight * _activeBow->m_BowPower, dy / 25 / m_Weight * _activeBow->m_BowPower));
	}
	else
	{
		float dx = _mousPos.x - m_Shape.getPosition().x;
		float dy = _mousPos.y - m_Shape.getPosition().y;

		m_Body->SetLinearVelocity(b2Vec2(dx, dy));
		
		std::cout << dx << " (:) " << dy << std::endl;
	}
}

CProjectile::~CProjectile()
{
	DestroyBody();
	delete m_Texture;

	m_Texture = nullptr;
	m_World = nullptr;
}

void CProjectile::Update()
{
	// Contacts
	b2Contact* contact;
	for (contact = m_World->GetContactList(); contact; contact = contact->GetNext())
	{
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();

		if (a->GetBody() == m_Body || b->GetBody() == m_Body)
		{
			if (a->GetBody() != m_Body && a->GetBody()->GetFixtureList()->IsSensor())
			{

			}
			else if (b->GetBody() != m_Body && b->GetBody()->GetFixtureList()->IsSensor())
			{

			}
			else if (b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 0x0004 || a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 0x0004) // _ENEMY_FILTER_
			{
				m_bMARKASDESTROY = true;
			}
			else if (b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 0x0006 || a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 0x0006) // _WORLD_FILTER_
			{
				m_bMARKASDESTROY = true;
			}
		}

		a = nullptr;
		b = nullptr;
	}
	contact = nullptr;

	// Rotation
	if (!m_Bullet)
	{
		float rot = atan2(m_Body->GetLinearVelocity().y, m_Body->GetLinearVelocity().x);
		m_Body->SetTransform(m_Body->GetPosition(), rot);
	}

	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * 50.0f, m_Body->GetPosition().y * 50.0f);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

void CProjectile::Render()
{
}

void CProjectile::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	// Body
	m_BodyDef.type = _type;
	m_BodyDef.fixedRotation = false;
	m_BodyDef.position = b2Vec2(_posX / 50.0f, (_posY / 50.0f));
	m_BodyDef.bullet = true;
	m_BodyDef.gravityScale = 4;
	int* damage = new int(m_Damage);
	m_BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(damage);
	damage = nullptr;
	//
	m_Body = m_World->CreateBody(&m_BodyDef);

	// Bullet
	if (m_Bullet)
	{
		m_b2pShape.SetAsBox((10 / 2) / 50.0f, (10 / 2) / 50.0f);
		m_Body->SetGravityScale(0);
	}
	// Arrow
	else
	{
		m_b2pShape.SetAsBox((10 / 2) / 50.0f, (10 / 2) / 50.0f);
	}


	// Fixture
	if (_sensor)
	{
		m_FixtureDef.isSensor = true;
	}
	m_FixtureDef.density = 1.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_FixtureDef.filter.categoryBits = _PLAYER_FILTER_;
	m_FixtureDef.filter.maskBits = _ENEMY_FILTER_;
	m_FixtureDef.filter.maskBits = _WORLD_FILTER_;
	m_FixtureDef.filter.groupIndex = _ENEMY_GROUPINDEX_;
	m_Body->CreateFixture(&m_FixtureDef);

	// Sprite
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * 50.0f, m_Body->GetPosition().y * 50.0f);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

void CProjectile::DestroyBody()
{
	if (m_World != nullptr && m_Body != nullptr)
	{
		m_World->DestroyBody(m_Body);
		m_Body = nullptr;
	}
}
