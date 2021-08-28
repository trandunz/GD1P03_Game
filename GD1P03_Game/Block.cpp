#include "Block.h"

CBlock::CBlock(sf::RenderWindow* _renderWindow, b2World& m_World, const float& _scale)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;

	//ground
	m_Shape.setSize(sf::Vector2f(800, 100));
	m_Shape.setFillColor(sf::Color::Green);

	//ground physics
	m_BodyDef.position = b2Vec2(0 / m_Scale, 600 / m_Scale);
	m_BodyDef.type = b2_staticBody;
	b2Body* gBody = m_World.CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((800 / 2) / m_Scale, (100 / 2) / m_Scale);

	m_FixtureDef.density = 0.f;
	m_FixtureDef.shape = &m_b2pShape;
	gBody->CreateFixture(&m_FixtureDef);
}

CBlock::~CBlock()
{
	m_RenderWindow = nullptr;
}

void CBlock::Start()
{
}

void CBlock::Update(b2Body* _bodyIterator)
{
	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(50, 50);
	m_Shape.setPosition(_bodyIterator->GetPosition().x * m_Scale, _bodyIterator->GetPosition().y * m_Scale);
	m_Shape.setRotation(_bodyIterator->GetAngle() * 180 / b2_pi);

}

void CBlock::Render()
{
	m_RenderWindow->draw(m_Shape);
}

sf::RectangleShape CBlock::GetShape()
{
	return m_Shape;
}
