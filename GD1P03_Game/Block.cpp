#include "Block.h"

CBlock::CBlock(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = &_world;

	//ground
	m_Shape.setSize(m_Size);
	m_Shape.setFillColor(sf::Color::Green);

	//ground physics
	m_BodyDef.position = b2Vec2(_posX / m_Scale, _posY / m_Scale);
	m_BodyDef.type = b2_staticBody;
	m_Body = _world.CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((m_Size.x / 2) / m_Scale, (m_Size.y / 2) / m_Scale);

	m_FixtureDef.density = 0.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_Body->CreateFixture(&m_FixtureDef);

	m_Body = nullptr;
}

CBlock::~CBlock()
{
	m_World->DestroyBody(m_Body);
	m_Body = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
}

void CBlock::Start()
{
}

void CBlock::Update(b2Body* _bodyIterator)
{
	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width/2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(_bodyIterator->GetPosition().x * m_Scale, _bodyIterator->GetPosition().y * m_Scale);
	m_Shape.setRotation(_bodyIterator->GetAngle() * 180 / b2_pi);

}



void CBlock::Render()
{
	m_RenderWindow->draw(m_Shape);
}

void CBlock::SetPosition(int _x, int _y)
{
}

void CBlock::SetSize(float _x, float _y)
{
	m_Size.x = _x;
	m_Size.y = _y;

	m_Shape.setSize(m_Size);

	//ground physics
	m_BodyDef.type = b2_staticBody;
	m_Body = m_World->CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((m_Size.x / 2) / m_Scale, (m_Size.y / 2) / m_Scale);

	m_FixtureDef.density = 0.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_Body->CreateFixture(&m_FixtureDef);

	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);

	Render();
}

sf::Vector2f CBlock::GetSize()
{
	return m_Size;
}

sf::RectangleShape CBlock::GetShape()
{
	return m_Shape;
}
