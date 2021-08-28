#include "Player.h"

CPlayer::CPlayer(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = m_World;

	//falling object
	m_Shape.setSize(sf::Vector2f(100, 100));

	//falling object physics
	m_BodyDef.position = b2Vec2(100 / _scale, 100 / _scale);
	m_BodyDef.type = b2_dynamicBody;
	m_BodyDef.linearDamping = 0.1f;
	m_BodyDef.angularDamping = 0.1f;
	m_BodyDef.allowSleep = true;
	m_BodyDef.awake = true;
	m_Body = _world.CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((100.f / 2) / _scale, (100.f / 2) / _scale);

	m_FixtureDef.density = 1.f;
	m_FixtureDef.shape = &m_b2pShape;
	m_Body->CreateFixture(&m_FixtureDef);

	// Velocity
	m_Velocity = b2Vec2(0.0f, 0.0f);


}

CPlayer::~CPlayer()
{
	m_World->DestroyBody(m_Body);
	m_Body = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
}

void CPlayer::Start()
{
}

void CPlayer::Update(b2Body* _bodyIterator)
{
	Movement(_bodyIterator);

	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width/2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(_bodyIterator->GetPosition().x * m_Scale, _bodyIterator->GetPosition().y * m_Scale);
	m_Shape.setRotation(_bodyIterator->GetAngle() * 180 / b2_pi);


}

void CPlayer::Render()
{
	m_RenderWindow->draw(m_Shape);
}

sf::RectangleShape CPlayer::GetShape()
{
	return m_Shape;
}

void CPlayer::Movement(b2Body* _bodyIterator)
{
	int x = 0;
	int y = 0;

	// Up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		y--;
	}
	// Down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		y++;
	}
	// Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		x--;
	}
	// Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		x++;
	}

	float Mag = sqrt((x * x) + (y * y));

	m_Velocity = b2Vec2(x, y);

	if (Mag > 1)
	{
		m_Velocity.x *= 1 / Mag;
		m_Velocity.y *= 1 / Mag;
	}

	// Move Sprite And Body
	m_Shape.setPosition(_bodyIterator->GetPosition().x * m_Scale, _bodyIterator->GetPosition().y * m_Scale);
	m_Body->ApplyLinearImpulseToCenter(m_MoveSpeed * m_Velocity, true);
}
