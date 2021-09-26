#include "CProjectile.h"

CProjectile::CProjectile(b2World& _world, float _startPosX, float _startPosY, sf::Vector2f _mousPos, PROJECTILETYPE _type)
{
	m_World = &_world;
	m_Texture = new sf::Texture();
	m_Type = _type;
	switch (m_Type)
	{
	case CProjectile::PROJECTILETYPE::ARROW:
		m_Texture->loadFromFile("Images/Arrow.png");
		break;
	case CProjectile::PROJECTILETYPE::FIREARROW:
		m_Texture->loadFromFile("Images/FireArrow.png");
		break;
	case CProjectile::PROJECTILETYPE::CURSEDARROW:
		m_Texture->loadFromFile("Images/CursedArrow.png");
		break;
	case CProjectile::PROJECTILETYPE::POISONARROW:
		m_Texture->loadFromFile("Images/PoisonArrow.png");
		break;
	default:
		m_Texture->loadFromFile("Images/Arrow.png");
		break;
	}
	
	m_Shape.setTexture(*m_Texture, true);
	CreateBody(_startPosX, _startPosY, b2_dynamicBody, true);

	float d = _mousPos.x - m_Shape.getPosition().x;
	float dy = _mousPos.y - m_Shape.getPosition().y;

	m_Body->SetLinearVelocity(b2Vec2(d  / 25 / m_Weight, dy / 25 / m_Weight));
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
	b2Contact* contact;
	for (contact = m_World->GetContactList(); contact; contact = contact->GetNext())
	{
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();

		if (a->GetBody() == m_Body || b->GetBody() == m_Body)
		{
			m_bMARKASDESTROY = true;
		}

		a = nullptr;
		b = nullptr;
	}
	contact = nullptr;

	float rot = atan2(m_Body->GetLinearVelocity().y, m_Body->GetLinearVelocity().x);
	m_Body->SetTransform(m_Body->GetPosition(), rot);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * 50.0f, m_Body->GetPosition().y * 50.0f);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

void CProjectile::Render()
{
}

void CProjectile::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	//ground physics
	m_BodyDef.type = _type;
	m_BodyDef.fixedRotation = false;
	m_BodyDef.position = b2Vec2(_posX / 50.0f, (_posY / 50.0f));
	m_BodyDef.bullet = true;
	m_BodyDef.gravityScale = 4;
	//
	m_Body = m_World->CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((30 / 2) / 50.0f, (10 / 2) / 50.0f);

	if (_sensor)
	{
		m_FixtureDef.isSensor = true;
	}
	m_FixtureDef.density = 1.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_FixtureDef.filter.categoryBits = 0x0002;
	m_FixtureDef.filter.maskBits = 0x0004;
	m_FixtureDef.filter.maskBits = 0x0006;
	m_FixtureDef.filter.groupIndex = -2;
	m_Body->CreateFixture(&m_FixtureDef);

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
