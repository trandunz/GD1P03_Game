//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : Slime.cpp
// Description : Slime Implementation file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#include "Slime.h"

/// <summary>
/// Slime Contructor
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_world"></param>
/// <param name="_textureMaster"></param>
/// <param name="_scale"></param>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
/// <param name="_audioManager"></param>
/// <param name="_boss"></param>
Slime::Slime(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CAudioManager& _audioManager, bool _boss)
{
	m_RenderWindow = _renderWindow;
	m_AudioManager = &_audioManager;
	m_Texture = new sf::Texture();

	RandomizeSlimeType(_boss);

	
	SetAppropriateSlimeTexture();
	m_World = &_world;
	m_Shape.setTexture(*m_Texture, true);
	m_Player = nullptr;

	Slime::CreateBody(_posX, _posY, b2_dynamicBody);
}

/// <summary>
/// Slime Destructor
/// </summary>
Slime::~Slime()
{
	m_TextureMaster = nullptr;

	delete m_Texture;
	m_Player = nullptr;
}

/// <summary>
/// Slime Start (SimpleMove())
/// </summary>
void Slime::Start()
{
	SimpleMove();
}

/// <summary>
/// Slime Update
/// </summary>
void Slime::Update()
{
	Movement();
	Attack();

	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);

	WorldContacts();
}

/// <summary>
/// Slime Render
/// </summary>
/// <param name="_shader"></param>
void Slime::Render(sf::Shader* _shader)
{
	if (m_Shape.getPosition().y < 1100)
	{
		m_RenderWindow->draw(m_Shape);
	}
	else
	{
		m_RenderWindow->draw(m_Shape, _shader);
	}
}

/// <summary>
/// Sets m_Player pointer to _player
/// </summary>
/// <param name="_player"></param>
void Slime::SetPlayer(CPlayer* _player)
{
	m_Player = _player;
}

/// <summary>
/// Sets m_Player pointer to nullptr
/// </summary>
void Slime::LoosePlayer()
{
	m_Player = nullptr;
}

/// <summary>
/// Returns a bool corresponding to the value of m_Player pointer
/// </summary>
/// <returns></returns>
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

/// <summary>
/// Creates the box2d Body
/// </summary>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
/// <param name="_type"></param>
/// <param name="_sensor"></param>
void Slime::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	// BodyDef
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
	Setb2pShapeSizeAppropriatly();

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
	m_FixtureDef.filter.groupIndex = -_PLAYER_GROUPINDEX_;
	m_Body->CreateFixture(&m_FixtureDef);

	SetSFShapeToBody();
}

/// <summary>
/// Handles Slime Movement (Jumping e.t.c)
/// </summary>
void Slime::Movement()
{
	b2Vec2 worldposition = { m_Shape.getPosition().x, m_Shape.getPosition().y };
	b2Vec2 velocity = m_Body->GetLinearVelocityFromWorldPoint(worldposition);

	float DistanceToPlayer;
	int DirectionToPlayer;
	if (bHasPlayer())
	{
		DistanceToPlayer = CalculateMag(m_Player->GetShape().getPosition().x, m_Shape.getPosition().x, m_Player->GetShape().getPosition().y, m_Shape.getPosition().y);
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
			MoveLeft(DistanceToPlayer);
		}
		// Move Right
		else if (velocity.x < 1.0f && velocity.x > -1.0f && DirectionToPlayer > 0)
		{
			MoveRight(DistanceToPlayer);
		}
	}
}

/// <summary>
/// Handles Slime's simple movement. (On Spawn e.t.c)
/// </summary>
void Slime::SimpleMove()
{
	b2Vec2 worldposition = { m_Shape.getPosition().x, m_Shape.getPosition().y };
	b2Vec2 velocity = m_Body->GetLinearVelocityFromWorldPoint(worldposition);

	float DistanceToPlayer;
	int DirectionToPlayer;
	if (bHasPlayer())
	{
		DistanceToPlayer = CalculateMag(m_Player->GetShape().getPosition().x, m_Shape.getPosition().x, m_Player->GetShape().getPosition().y, m_Shape.getPosition().y);
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
		SimpleMoveLeft();
	}
	// Move Right
	else if (velocity.x < 1.0f && velocity.x > -1.0f)
	{
		SimpleMoveRight();
	}
}

/// <summary>
/// Slime Attack (Melee)
/// </summary>
void Slime::Attack()
{
	float DistanceToPlayer = 0;
	int DirectionToPlayer = 0;

	if (bHasPlayer() && m_AttackTimer.getElapsedTime().asSeconds() >= m_AttackSpeed)
	{
		DistanceToPlayer = CalculateMag(m_Player->GetShape().getPosition().x, m_Shape.getPosition().x, m_Player->GetShape().getPosition().y, m_Shape.getPosition().y);
		DirectionToPlayer = m_Player->GetShape().getPosition().x - m_Shape.getPosition().x ;

		if (DistanceToPlayer <= 300)
		{
			AttackBasedOnAppropriateType(DirectionToPlayer, DistanceToPlayer);
		}
	}
}

/// <summary>
/// Randomizes the Slime type with the current system time
/// </summary>
/// <param name="_boss"></param>
void Slime::RandomizeSlimeType(bool _boss)
{
	srand(time(NULL));

	if (!_boss)
	{
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
		else
		{
			m_SlimeType = SLIMETYPE::GREEN;
		}
	}
	else
	{
		if (rand() % 5 == 1)
		{
			m_SlimeType = SLIMETYPE::BOSSGREEN;
		}
		else if (rand() % 5 == 2)
		{
			m_SlimeType = SLIMETYPE::BOSSBLUE;
		}
		else if (rand() % 5 == 3)
		{
			m_SlimeType = SLIMETYPE::BOSSRED;
		}
		else if (rand() % 5 == 4)
		{
			m_SlimeType = SLIMETYPE::BOSSPURPLE;
		}
		else if (rand() % 5 == 0)
		{
			m_SlimeType = SLIMETYPE::BOSSYELLOW;
		}
		else
		{
			m_SlimeType = SLIMETYPE::BOSSGREEN;
		}
	}
}

/// <summary>
/// Sets the appropriate slime texture (m_Texture)
/// </summary>
void Slime::SetAppropriateSlimeTexture()
{
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
		m_Health = 3000;
		break;
	}
	case Slime::SLIMETYPE::BOSSBLUE:
	{
		m_Texture->loadFromFile("Images/BossSlimeBlue.png");
		m_bIsBoss = true;
		m_Health = 2800;
		break;
	}
	case Slime::SLIMETYPE::BOSSRED:
	{
		m_Texture->loadFromFile("Images/BossSlimeRed.png");
		m_bIsBoss = true;
		m_Health = 3800;
		break;
	}
	case Slime::SLIMETYPE::BOSSPURPLE:
	{
		m_Texture->loadFromFile("Images/BossSlimePurple.png");
		m_bIsBoss = true;
		m_Health = 3000;
		break;
	}
	case Slime::SLIMETYPE::BOSSYELLOW:
	{
		m_Texture->loadFromFile("Images/BossSlimeYellow.png");
		m_bIsBoss = true;
		m_Health = 3400;
		break;
	}
	default:
	{
		m_Texture->loadFromFile("Images/Slime.png");
		break;
	}
	}
}

/// <summary>
/// Sets the size of m_b2pShape Appopriatly based on the slime type
/// </summary>
void Slime::Setb2pShapeSizeAppropriatly()
{
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
}

/// <summary>
/// Handles leftward slime movement
/// </summary>
/// <param name="DistanceToPlayer"></param>
void Slime::MoveLeft(float DistanceToPlayer)
{
	// Face Left
	m_Shape.setScale(1, 1);

	switch (m_SlimeType)
	{
	case Slime::SLIMETYPE::GREEN:
	{
		if (DistanceToPlayer < 1400.0f)
		{
			// Move Left
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % -300, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
		}

		break;
	}
	case Slime::SLIMETYPE::BLUE:
	{
		if (DistanceToPlayer < 1400.0f)
		{
			// Move Left
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 200, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 400), true);
		}

		break;
	}
	case Slime::SLIMETYPE::RED:
	{
		if (DistanceToPlayer < 1400.0f)
		{
			// Move Left
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 300, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
		}

		break;
	}
	case Slime::SLIMETYPE::PURPLE:
	{
		if (DistanceToPlayer < 1400.0f)
		{
			// Move Left
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 400, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 400), true);
		}

		break;
	}
	case Slime::SLIMETYPE::YELLOW:
	{
		if (DistanceToPlayer < 1400.0f)
		{
			// Move Left
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(-100 - rand() % 300, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
		}

		break;
	}
	case Slime::SLIMETYPE::BOSSGREEN:
	{
		// Move Left
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(-3000 - rand() % 4400, 0.0f), true);
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 6400), true);
		break;
	}
	case Slime::SLIMETYPE::BOSSBLUE:
	{
		// Move Left
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(-3000 - rand() % 4400, 0.0f), true);
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 6400), true);
		break;
	}
	case Slime::SLIMETYPE::BOSSRED:
	{
		// Move Left
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(-3000 - rand() % 4000, 0.0f), true);
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 6000), true);
		break;
	}
	case Slime::SLIMETYPE::BOSSPURPLE:
	{
		// Move Left
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(-3000 - rand() % 4400, 0.0f), true);
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 6400), true);
		break;
	}
	case Slime::SLIMETYPE::BOSSYELLOW:
	{
		// Move Left
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(-3000 - rand() % 4400, 0.0f), true);
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 6400), true);
		break;
	}
	default:
	{
		if (DistanceToPlayer < 1400.0f)
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
/// Handles Rightward slime Movement
/// </summary>
/// <param name="DistanceToPlayer"></param>
void Slime::MoveRight(float DistanceToPlayer)
{
	// Face Right
	m_Shape.setScale(-1, 1);

	switch (m_SlimeType)
	{
	case Slime::SLIMETYPE::GREEN:
	{
		if (DistanceToPlayer < 1400.0f)
		{
			// Move Left
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 300, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -200 - rand() % 300), true);
		}

		break;
	}
	case Slime::SLIMETYPE::BLUE:
	{
		if (DistanceToPlayer < 1400.0f)
		{
			// Move Left
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 200, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 400), true);
		}
		break;
	}
	case Slime::SLIMETYPE::RED:
	{
		if (DistanceToPlayer < 1400.0f)
		{
			// Move Left
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 300, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
		}
		break;
	}
	case Slime::SLIMETYPE::PURPLE:
	{
		if (DistanceToPlayer < 1400.0f)
		{
			// Move Left
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 400, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 400), true);
		}
		break;
	}
	case Slime::SLIMETYPE::YELLOW:
	{
		if (DistanceToPlayer < 1400.0f)
		{
			// Move Left
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(100 + rand() % 300, 0.0f), true);
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100 - rand() % 300), true);
		}
		break;
	}
	case Slime::SLIMETYPE::BOSSGREEN:
	{
		// Move Left
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(3000 + rand() % 4400, 0.0f), true);
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 2400), true);
		break;
	}
	case Slime::SLIMETYPE::BOSSBLUE:
	{
		// Move Left
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(3000 + rand() % 4400, 0.0f), true);
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 3400), true);
		break;
	}
	case Slime::SLIMETYPE::BOSSRED:
	{
		// Move Left
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(3000 + rand() % 4000, 0.0f), true);
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 2000), true);
		break;
	}
	case Slime::SLIMETYPE::BOSSPURPLE:
	{
		// Move Left
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(3000 + rand() % 4400, 0.0f), true);
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 3400), true);
		break;
	}
	case Slime::SLIMETYPE::BOSSYELLOW:
	{
		// Move Left
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(3000 + rand() % 4800, 0.0f), true);
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -1000 - rand() % 2800), true);
		break;
	}
	default:
	{
		if (DistanceToPlayer < 1400.0f)
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
/// Handles Simple Slime movement Left
/// </summary>
void Slime::SimpleMoveLeft()
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

/// <summary>
/// Handles Simple Slime movement Right
/// </summary>
void Slime::SimpleMoveRight()
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

/// <summary>
/// Attacks based on the appropriate slime type
/// </summary>
/// <param name="DirectionToPlayer"></param>
/// <param name="DistanceToPlayer"></param>
void Slime::AttackBasedOnAppropriateType(int DirectionToPlayer, float DistanceToPlayer)
{
	switch (m_SlimeType)
	{
	case Slime::SLIMETYPE::GREEN:
	{
		if (DistanceToPlayer <= 140)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -10, 0), true);
			m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

			m_Player->TakeDamage(12.25f);
			m_AttackTimer.restart();
		}
		break;
	}
	case Slime::SLIMETYPE::BLUE:
	{
		if (DistanceToPlayer <= 130)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -10, 0), true);
			m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

			m_Player->TakeDamage(10.0f);
			m_AttackTimer.restart();
		}
		break;
	}
	case Slime::SLIMETYPE::RED:
	{
		if (DistanceToPlayer <= 160)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -10, 0), true);
			m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

			m_Player->TakeDamage(15.0f);
			m_AttackTimer.restart();
		}
		break;
	}
	case Slime::SLIMETYPE::PURPLE:
	{
		if (DistanceToPlayer <= 130)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -10, 0), true);
			m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

			m_Player->TakeDamage(10.0f);
			m_AttackTimer.restart();
		}
		break;
	}
	case Slime::SLIMETYPE::YELLOW:
	{
		if (DistanceToPlayer <= 140)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -10, 0), true);
			m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

			m_Player->TakeDamage(13.25f);
			m_AttackTimer.restart();
		}
		break;
	}
	case Slime::SLIMETYPE::BOSSGREEN:
	{
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -50, 0), true);
		m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

		m_Player->TakeDamage(20.0f);
		m_AttackTimer.restart();
		break;
	}
	case Slime::SLIMETYPE::BOSSBLUE:
	{
		if (DistanceToPlayer <= 280)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -50, 0), true);
			m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

			m_Player->TakeDamage(20.0f);
			m_AttackTimer.restart();
		}
		break;
	}
	case Slime::SLIMETYPE::BOSSRED:
	{
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -50, 0), true);
		m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

		m_Player->TakeDamage(30.0f);
		m_AttackTimer.restart();
		break;
	}
	case Slime::SLIMETYPE::BOSSPURPLE:
	{
		if (DistanceToPlayer <= 280)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -50, 0), true);
			m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

			m_Player->TakeDamage(25.0f);
			m_AttackTimer.restart();
		}
		break;
	}
	case Slime::SLIMETYPE::BOSSYELLOW:
	{
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -50, 0), true);
		m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

		m_Player->TakeDamage(25.0f);
		m_AttackTimer.restart();
		break;
	}
	default:
	{
		if (DistanceToPlayer <= 140)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * -5, 0), true);
			m_Player->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(DirectionToPlayer * 1 / 2, 0), true);

			m_Player->TakeDamage(12.25f);
			m_AttackTimer.restart();
		}
		break;
	}
	}
}

