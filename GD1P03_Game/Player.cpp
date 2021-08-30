#include "Player.h"

CPlayer::CPlayer(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = m_World;
	m_Block = nullptr;
	// Textures
	m_PlayerRightTex = new sf::Texture();
	m_PlayerLeftTex = new sf::Texture();
	if (m_PlayerRightTex->loadFromFile("Images/Player.png"))
	{
		std::cout << "Loaded Player" << std::endl;
	}
	if (m_PlayerLeftTex->loadFromFile("Images/PlayerLeft.png"))
	{
		std::cout << "Loaded Player" << std::endl;
	}
	m_Shape = sf::Sprite();
	m_Shape.setTexture(*m_PlayerRightTex, true);
	

	////falling object
	/*m_Shape.setSize(sf::Vector2f(100, 200));*/
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width/2, m_Shape.getGlobalBounds().height / 2);

	//falling object physics
	m_BodyDef = b2BodyDef();
	m_BodyDef.position = b2Vec2(m_Shape.getPosition().x / _scale, m_Shape.getPosition().y  / _scale);
	m_BodyDef.type = b2_dynamicBody;
	m_BodyDef.linearDamping = 0.1f;
	m_BodyDef.angularDamping = 0.1f;
	m_BodyDef.fixedRotation = true;
	m_BodyDef.allowSleep = false;
	m_BodyDef.awake = true;
	m_BodyDef.gravityScale = 10.0f;
	m_Body = _world.CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((100/2) / _scale, (200 / 2) / _scale);

	m_FixtureDef.density = 1.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_FixtureDef.friction = 0.1f;
	m_Body->CreateFixture(&m_FixtureDef);

	// Velocity
	m_Velocity = b2Vec2(0.0f, 0.0f);


}

CPlayer::~CPlayer()
{
	
	delete m_PlayerRightTex;
	m_PlayerRightTex = nullptr;
	delete m_PlayerLeftTex;
	delete m_Block;
	m_Block = nullptr;
	m_PlayerLeftTex = nullptr;
	m_Body = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
}

void CPlayer::Start()
{
}

void CPlayer::Update()
{
	MousePos = m_RenderWindow->mapPixelToCoords((sf::Mouse::getPosition(*m_RenderWindow)));
	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

void CPlayer::Render()
{
	m_RenderWindow->draw(m_Shape);
}

sf::Sprite CPlayer::GetShape()
{
	return m_Shape;
}

void CPlayer::Movement(sf::Event& _event)
{
	int x = 0;
	int y = 0;
		
	// Up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -100.0f), true);
	}
	// Down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		y++;
	}
	// Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		x = -m_MoveSpeed;
	}
	// Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		x = m_MoveSpeed;
	}

	float Mag = sqrt((x * x) + (y * y));

	m_Velocity = b2Vec2(x, y);

	if (Mag <= m_MoveSpeed)
	{
		m_Body->ApplyLinearImpulseToCenter(m_MoveSpeed * m_Velocity, true);
	}
	else if (Mag > m_MoveSpeed)
	{
		m_Velocity.x *= 1 / Mag;
		m_Velocity.y *= 1 / Mag;
	}

	if (m_Velocity.x > 0)
	{
		m_Shape.setTexture(*m_PlayerRightTex, true);
	}
	else if (m_Velocity.x < 0)
	{
		m_Shape.setTexture(*m_PlayerLeftTex, true);
	}

	
	// Move Sprite And Body
	/*m_Body->SetLinearVelocity(m_MoveSpeed * m_Velocity);*/
	
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	std::cout << m_Shape.getPosition().x << std::endl;
	std::cout << m_Shape.getPosition().y << std::endl;
	
}

void CPlayer::PlaceBlocks(std::list<CBlock>& m_Chunk, sf::Event& _event)
{
	if (_event.type == sf::Event::MouseButtonPressed)
	{
		m_Block = nullptr;
		m_Block = new CBlock(m_RenderWindow, *m_World, m_PlayerRightTex, m_Scale, MousePos.x, MousePos.y);
		m_Block->SetSize(100, 100);
		m_Chunk.push_front(*m_Block);
	}
	
}
