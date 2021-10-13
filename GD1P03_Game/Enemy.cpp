//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CEnemy.cpp
// Description : CEnemy Implementation file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#include "Enemy.h"

/// <summary>
/// CEnemy Default Constructor
/// </summary>
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

/// <summary>
/// CEnemy Destructor
/// </summary>
CEnemy::~CEnemy()
{
	std::cout << "Enemy Destoryed" << std::endl;

	DestroyBody();

	m_Texture = nullptr;
	m_Body = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
	m_AudioManager = nullptr;
	m_Player = nullptr;
}

/// <summary>
/// CEnemy Update
/// </summary>
void CEnemy::Update()
{
	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

/// <summary>
/// CEnemy Render
/// </summary>
/// <param name="_shader"></param>
void CEnemy::Render(sf::Shader* _shader)
{
	m_RenderWindow->draw(m_Shape, _shader);
}

/// <summary>
/// Sets m_Health = _value
/// </summary>
/// <param name="_value"></param>
void CEnemy::SetHealth(int _value)
{
	m_Health = _value;
}

/// <summary>
/// Returns m_Health (int)
/// </summary>
/// <returns></returns>
int CEnemy::GetHealth()
{
	return m_Health;
}

/// <summary>
/// Sets the player pointer to _player
/// </summary>
/// <param name="_player"></param>
void CEnemy::SetPlayer(CPlayer* _player)
{
	if (m_Player != nullptr)
	{
		m_Player = _player;
	}
}

/// <summary>
/// Returns a copy of the enemies Shape
/// </summary>
/// <returns></returns>
sf::Sprite CEnemy::GetShape()
{
	return m_Shape;
}

/// <summary>
/// Makes use of the m_Damage timer to regulate taking damage
/// </summary>
/// <param name="_damage"></param>
/// <param name="_projectile"></param>
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

/// <summary>
/// Creates the b2Body (Generic Size is 100x100)
/// </summary>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
/// <param name="_type"></param>
/// <param name="_sensor"></param>
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

/// <summary>
/// Destroys the enemies box2d Body
/// </summary>
void CEnemy::DestroyBody()
{
	if (m_World != nullptr && m_Body != nullptr)
	{
		m_World->DestroyBody(m_Body);

		m_Body = nullptr;
	}
}

/// <summary>
/// Sets the position of the sfml shape to the box2d Body and orients its rotation.
/// This function will also set the origin of the enemies shape to the middle.
/// </summary>
void CEnemy::SetSFShapeToBody()
{
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

/// <summary>
/// Returns the Magnitude betweem two vectors x and y (float)
/// </summary>
/// <param name="_x1"></param>
/// <param name="_x2"></param>
/// <param name="_y1"></param>
/// <param name="_y2"></param>
/// <returns></returns>
float CEnemy::CalculateMag(float _x1, float _x2, float _y1, float _y2)
{
	return sqrt(((_x1 - _x2) * (_x1 - _x2)) + ((_y1 - _y2) * (_y1 - _y2)));
}

/// <summary>
/// Generic Melee Attack (Snowman Uses It As their main attack is snowball)
/// </summary>
/// <param name="DirectionToPlayer"></param>
void CEnemy::Melee(int DirectionToPlayer)
{
	m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, 0), true);
	if (m_Player != nullptr)
	{
		m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

		m_Player->TakeDamage(m_Damage);
	}
	m_AttackTimer.restart();
}

/// <summary>
/// Handles All World Contacts For The Enemy (Arrows e.t.c)
/// </summary>
void CEnemy::WorldContacts()
{
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
}
