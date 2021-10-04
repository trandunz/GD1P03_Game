#include "Enemy.h"

CEnemy::CEnemy()
{
	m_Texture = nullptr;
	m_Body = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
	m_AudioManager = nullptr;
	m_Player = nullptr;
}

CEnemy::~CEnemy()
{
	DestroyBody();

	m_Texture = nullptr;
	m_Body = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
	m_AudioManager = nullptr;
	m_Player = nullptr;
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

void CEnemy::Render(sf::Shader* _shader)
{
	m_RenderWindow->draw(m_Shape, _shader);
}

void CEnemy::SetHealth(int _value)
{
	m_Health = _value;
}

int CEnemy::GetHealth()
{
	return m_Health;
}

void CEnemy::SetPlayer(CPlayer* _player)
{
	if (m_Player != nullptr)
	{
		m_Player = _player;
	}

}

sf::Sprite CEnemy::GetShape()
{
	return m_Shape;
}

void CEnemy::Movement()    
{
}

void CEnemy::Attack()
{
}

void CEnemy::TakeDamage(float _damage, bool _projectile)
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
