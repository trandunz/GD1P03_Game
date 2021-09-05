#include "Block.h"

CBlock::CBlock()
{
	m_RenderWindow = nullptr;
	m_Scale = 50.0f;
	m_World = nullptr;
	m_Scale = 50.0f;
	m_Body = nullptr;
	m_Type = BLOCKTYPE::BLOCK;
}

CBlock::CBlock(sf::Texture* _texture)
{
	m_RenderWindow = nullptr;
	m_Scale = 50.0f;
	m_World = nullptr;
	m_Scale = 50.0f;
	m_Body = nullptr;
	m_Type = BLOCKTYPE::BLOCK;
	m_Shape.setTexture(*_texture, true);
	m_Shape.setScale(0.3f, 0.3f);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
}

CBlock::CBlock(sf::RenderWindow* _renderWindow, b2World& _world, const sf::Texture* _texture, const float& _scale, float _posX, float _posY)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = &_world;

	m_Shape.setTexture(*_texture, true);

	//ground physics
	
	m_BodyDef.type = b2_staticBody;
	m_BodyDef.position = b2Vec2(_posX / m_Scale, (_posY / m_Scale));
	m_Body = m_World->CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((m_Size.x / 2) / m_Scale, (m_Size.y / 2) / m_Scale);

	m_FixtureDef.density = 1.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_Body->CreateFixture(&m_FixtureDef);
	
	//Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);

	Render();
}

CBlock::~CBlock()
{
	if (m_World != nullptr && m_Body != nullptr)
	{
		m_World->DestroyBody(m_Body);
		//std::cout << "Destructing Block" << std::endl;
	}

	m_Body = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
}

void CBlock::Start()
{
	
}

void CBlock::Update()
{
	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	if (m_Body != nullptr)
	{
		m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, (m_Body->GetPosition().y * m_Scale));
		m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
	}
}



void CBlock::Render()
{
	m_RenderWindow->draw(m_Shape);
}

void CBlock::Destroy()
{
	//// remove sprite
	//m_Shape = sf::Sprite();

	//// remove physical body
	//m_World->DestroyBody(m_Body);
}

void CBlock::SetPosition(float _x, float _y)
{
	m_Shape.setPosition(_x, _y);
}

sf::Vector2f CBlock::GetPosition()
{
	return m_Shape.getPosition();
}

void CBlock::SetSize(float _x, float _y)
{
	m_Size.x = _x;
	m_Size.y = _y;

	/*m_Shape.setSize(m_Size);*/

	//ground physics
	m_BodyDef.type = b2_staticBody;
	m_Body = m_World->CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((m_Size.x / 2) / m_Scale, (m_Size.y / 2) / m_Scale);

	m_FixtureDef.density = 1.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_Body->CreateFixture(&m_FixtureDef);

	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);

	Render();
}

sf::Vector2f CBlock::GetSize()
{
	return m_Size;
}

sf::Sprite CBlock::GetShape()
{
	return m_Shape;
}
