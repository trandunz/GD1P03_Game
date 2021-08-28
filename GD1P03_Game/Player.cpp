#include "Player.h"

CPlayer::CPlayer(sf::RenderWindow* _renderWindow, b2World& m_World, const float& _scale)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;

	//falling object
	m_Shape.setSize(sf::Vector2f(100, 100));

	//falling object physics
	m_BodyDef.position = b2Vec2(100 / _scale, 100 / _scale);
	m_BodyDef.type = b2_dynamicBody;
	b2Body* boxBody = m_World.CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((100.f / 2) / _scale, (100.f / 2) / _scale);

	m_FixtureDef.density = 1.f;
	m_FixtureDef.shape = &m_b2pShape;
	boxBody->CreateFixture(&m_FixtureDef);
}

CPlayer::~CPlayer()
{
	m_RenderWindow = nullptr;
}

void CPlayer::Start()
{
}

void CPlayer::Update(b2Body* _bodyIterator)
{
	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(50, 50);
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

void CPlayer::Movement()
{
}
