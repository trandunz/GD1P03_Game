//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CSnowman.cpp
// Description : CSnowman Implementation file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#include "CSnowman.h"

/// <summary>
/// CSnowman Constructor.
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_world"></param>
/// <param name="_textureMaster"></param>
/// <param name="_scale"></param>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
/// <param name="_audioManager"></param>
/// <param name="_boss"></param>
CSnowman::CSnowman(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CAudioManager& _audioManager, bool _boss, bool _bIsCactus)
{
	m_RenderWindow = _renderWindow;
	m_AudioManager = &_audioManager;
	m_Texture = new sf::Texture();
	m_bIsBoss = _boss;
	m_World = &_world;
	m_bIsCactus = _bIsCactus;

	if (!_boss)
	{
		m_SnowmanType = SNOWMANTYPE::GREEN;
	}
	else
	{
		srand(time(NULL));
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
		if (m_bIsCactus)
		{
			m_Texture->loadFromFile("Images/BossCactusGreen.png");
		}
		else
		{
			m_Texture->loadFromFile("Images/BossSnowManGreen.png");
		}
		
		m_bIsBoss = true;
		m_Health = 3000;
		break;
	}
	case CSnowman::SNOWMANTYPE::BOSSBLUE:
	{
		if (m_bIsCactus)
		{
			m_Texture->loadFromFile("Images/BossCactusBlue.png");
		}
		else
		{
			m_Texture->loadFromFile("Images/BossSnowManBlue.png");
		}
		
		m_bIsBoss = true;
		m_Health = 2800;
		break;
	}
	case CSnowman::SNOWMANTYPE::BOSSRED:
	{
		if (m_bIsCactus)
		{
			m_Texture->loadFromFile("Images/BossCactusRed.png");
		}
		else
		{
			m_Texture->loadFromFile("Images/BossSnowManRed.png");
		}
		
		m_bIsBoss = true;
		m_Health = 3800;
		break;
	}
	case CSnowman::SNOWMANTYPE::BOSSYELLOW:
	{
		if (m_bIsCactus)
		{
			m_Texture->loadFromFile("Images/BossCactusYellow.png");
		}
		else
		{
			m_Texture->loadFromFile("Images/BossSnowManYellow.png");
		}
		
		m_bIsBoss = true;
		m_Health = 3400;
		break;
	}
	default:
	{
		if (m_bIsCactus)
		{
			m_Texture->loadFromFile("Images/CactusMob.png");
		}
		else
		{
			m_Texture->loadFromFile("Images/Snowman.png");
		}
		
		m_Health = 150;
		break;
	}
	}
	m_Shape.setTexture(*m_Texture, true);
	m_Player = nullptr;

	CSnowman::CreateBody(_posX, _posY, b2_dynamicBody);
}

/// <summary>
/// CSnowman Destructor
/// </summary>
CSnowman::~CSnowman()
{
	m_Projectiles.clear();

	m_TextureMaster = nullptr;

	if (m_Texture != nullptr)
	{
		delete m_Texture;
		m_Texture = nullptr;
	}
	m_Player = nullptr;
}

/// <summary>
/// CSnowman Start
/// </summary>
void CSnowman::Start()
{
	SimpleMove();

	// 3rd Level = Triple The Damage As First Level
	if (m_bIsCactus)
	{
		if (m_bIsBoss)
		{
			m_AttackSpeed = 0.7f;
		}
		else
		{
			m_AttackSpeed = 2.0f;
		}
		m_Damage *= 2;
		
	}
	else
	{
		m_Damage *= 3;
	}

}

/// <summary>
/// CSnowman Update
/// </summary>
void CSnowman::Update(bool& m_PlayerHitByProjectile)
{
	if (m_bIsCactus)
	{
		Attack();
	}
	else
	{
		Movement();
		Attack();
	}

	SetSFShapeToBody();

	WorldContacts();

	HandleProjectiles(m_PlayerHitByProjectile);
}

/// <summary>
/// CSnowman Render
/// </summary>
/// <param name="_shader"></param>
void CSnowman::Render(sf::Shader* _shader)
{
	if (m_Shape.getPosition().y < 1200)
	{
		m_RenderWindow->draw(m_Shape);
	}
	else
	{
		m_RenderWindow->draw(m_Shape, _shader);
	}

	DrawProjectiles();
}

/// <summary>
/// Set the pointer m_Player to _player.
/// </summary>
/// <param name="_player"></param>
void CSnowman::SetPlayer(CPlayer* _player)
{
	m_Player = _player;
}

/// <summary>
/// Set the pointer m_Player to nullptr
/// </summary>
void CSnowman::LoosePlayer()
{
	m_Player = nullptr;
}

/// <summary>
/// Returns a bool depending on m_Player = nullptr
/// </summary>
/// <returns></returns>
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

/// <summary>
/// Creates the box2d body
/// </summary>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
/// <param name="_type"></param>
/// <param name="_sensor"></param>
void CSnowman::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	// Body Def
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

	// b2Shape
	if (m_bIsBoss)
	{
		m_b2pShape.SetAsBox((300 / 2) / m_Scale, (600 / 2) / m_Scale);
	}
	else
	{
		m_b2pShape.SetAsBox((100 / 2) / m_Scale, (200 / 2) / m_Scale);
	}

	// Fixture Def
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
	m_FixtureDef.filter.groupIndex = -_ENEMY_GROUPINDEX_;
	m_Body->CreateFixture(&m_FixtureDef);

	SetSFShapeToBody();
}

/// <summary>
/// CSnowman Movement
/// </summary>
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
			Moveleft(DistanceToPlayer);
		}
		// Move Right
		else if (velocity.x < 1.0f && velocity.x > -1.0f && DirectionToPlayer > 0)
		{
			MoveRight(DistanceToPlayer);
		}
	}
}

/// <summary>
/// CSnowman Simple Movement
/// </summary>
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

/// <summary>
/// CSnowman Attack (Snowball and Melee)
/// </summary>
void CSnowman::Attack()
{
	float DistanceToPlayer = 0;
	int DirectionToPlayer = 0;

	if (bHasPlayer() && m_AttackTimer.getElapsedTime().asSeconds() >= m_AttackSpeed)
	{
		DistanceToPlayer = CalculateMag(m_Player->GetShape().getPosition().x, m_Shape.getPosition().x, m_Player->GetShape().getPosition().y, m_Shape.getPosition().y);
		DirectionToPlayer = m_Player->GetShape().getPosition().x - m_Shape.getPosition().x;
		float UnitDirecton = DirectionToPlayer / sqrt(((DirectionToPlayer) * (DirectionToPlayer)));

		// Melee
		if (DistanceToPlayer <= 130)
		{
			Melee(UnitDirecton);
		}
		// Snowball
		else if (m_Player != nullptr && DistanceToPlayer <= 2200)
		{
			if (m_bIsCactus && !m_bIsBoss)
			{
				if (DistanceToPlayer <= 1000)
				{
					FireSnowBall(UnitDirecton);
				}
			}
			else
			{
				FireSnowBall(UnitDirecton);
			}
		}
	}
}

/// <summary>
/// Handles All the snowmens projectiles (Snowballs) and deletes any whomb are marked.
/// </summary>
void CSnowman::HandleProjectiles(bool& m_PlayerHitByProjectile)
{
	for (CProjectile& projectile : m_Projectiles)
	{
		projectile.Update(m_PlayerHitByProjectile);
	}

	DeleteAllProjectiles();
}

/// <summary>
/// Draw All Snowmens Projectiles (Snowballs)
/// </summary>
/// <param name="nullshader"></param>
void CSnowman::DrawProjectiles(sf::Shader* nullshader)
{
	// Draw All Projectiles
	for (CProjectile& projectile : m_Projectiles)
	{
		m_RenderWindow->draw(projectile.GetShape(), nullshader);
	}
}

/// <summary>
/// Deletes all snowballs
/// Sceptical if these functions even work with lists?
/// </summary>
void CSnowman::DeleteAllProjectiles()
{
	// Delete All Collided Projectiles
	std::list<CProjectile>::iterator pit = m_Projectiles.begin();
	while (pit != m_Projectiles.end())
	{
		if (pit == m_Projectiles.end())
		{
			break;
		}
		if (pit->m_bMARKASDESTROY)
		{
			pit->DestroyBody();
			pit = m_Projectiles.erase(pit);
			if (m_Projectiles.size() <= 0)
			{
				return;
			}
		}
		else
		{
			pit++;
		}
		
	}
}

/// <summary>
/// Move Left
/// </summary>
/// <param name="DistanceToPlayer"></param>
void CSnowman::Moveleft(int DistanceToPlayer)
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

/// <summary>
/// Move Right
/// </summary>
/// <param name="DistanceToPlayer"></param>
void CSnowman::MoveRight(int DistanceToPlayer)
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

/// <summary>
/// Fires A Snowball at the Centre of the render window (Players Position)
/// </summary>
void CSnowman::FireSnowBall(float UnitDirection)
{
	if (m_SnowballTimer.getElapsedTime().asSeconds() >= 2.0f)
	{
		std::cout << "Snowman Fired Snowball!" << std::endl;

		CProjectile* m_Projectile;
		// throw snowball
		if (m_bIsCactus)
		{
			m_Projectile = new CProjectile(*m_World, m_Shape.getPosition().x + 20 * UnitDirection, m_Shape.getPosition().y - 50, m_RenderWindow->getView().getCenter(), CBlock::PROJECTILETYPE::TUMBLEWEED, nullptr, false);
		}
		else
		{
			m_Projectile = new CProjectile(*m_World, m_Shape.getPosition().x + 20 * UnitDirection, m_Shape.getPosition().y - 50, m_RenderWindow->getView().getCenter(), CBlock::PROJECTILETYPE::SNOWBALL, nullptr, false);
		}
		
		m_Projectiles.push_back(*m_Projectile);
		m_Projectile = nullptr;

		m_SnowballTimer.restart();
	}
}
