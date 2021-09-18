#include "Enemy.h"

CEnemy::CEnemy()
{
	m_Texture = nullptr;
	m_Body = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
}

CEnemy::~CEnemy()
{
	DestroyBody();

	m_Texture = nullptr;
	m_Body = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
}

void CEnemy::Start()
{
}

void CEnemy::Update()
{
	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

void CEnemy::Render()
{
	m_RenderWindow->draw(m_Shape);
}

void CEnemy::SetHealth(int _value)
{
	m_Health = _value;
}

int CEnemy::GetHealth()
{
	return m_Health;
}

void CEnemy::Movement()    
{
}

void CEnemy::Attack()
{
}

void CEnemy::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	//ground physics
	m_BodyDef.type = _type;
	m_BodyDef.fixedRotation = true;
	m_BodyDef.position = b2Vec2(_posX / m_Scale, (_posY / m_Scale));
	m_Body = m_World->CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((100 / 2) / m_Scale, (100 / 2) / m_Scale);

	if (_sensor)
	{
		m_FixtureDef.isSensor = true;
	}
	m_FixtureDef.density = 1.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_Body->CreateFixture(&m_FixtureDef);

	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

void CEnemy::DestroyBody()
{
	if (m_World != nullptr && m_Body != nullptr)
	{
		m_World->DestroyBody(m_Body);
		m_Body = nullptr;
	}
}
