#include "Door.h"

CDoor::CDoor()
{
	m_Scale = 50.0f;
	m_Body = nullptr;
	m_Type = BLOCKTYPE::DOOR;

	m_Texture.loadFromFile("Images/DoorOpenLeft.png");
	m_Shape.setTexture(m_Texture, true);
	m_Shape.setScale(0.2f, 0.2f);
	m_Shape.setOrigin(0.0f, m_Shape.getGlobalBounds().height / 2 + m_Shape.getGlobalBounds().height / 3);
}

CDoor::CDoor(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = &_world;

	m_OpenLeftTex.loadFromFile("Images/DoorOpenLeft.png");
	m_OpenRightTex.loadFromFile("Images/DoorOpenRight.png");
	m_Texture.loadFromFile("Images/Door.png");
	m_Shape.setTexture(m_Texture, true);

	m_BodyDef.position = b2Vec2(_posX / m_Scale, _posY / m_Scale);

	CBlock::m_Type = BLOCKTYPE::DOOR;
	m_Type = BLOCKTYPE::DOOR;
}

CDoor::~CDoor()
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

void CDoor::OCDoor(sf::Vector2f _playerPosition)
{
	m_bOpen = !m_bOpen;

	if (m_bOpen)
	{
		if (m_Shape.getPosition().x - _playerPosition.x < 0)
		{
			m_Shape.setTexture(m_OpenLeftTex, true);
			m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, (m_Shape.getGlobalBounds().height / 2));
			m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
			m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
		}
		else if (m_Shape.getPosition().x - _playerPosition.x > 0)
		{
			m_Shape.setTexture(m_OpenRightTex, true);
			m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, (m_Shape.getGlobalBounds().height / 2));
			m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
			m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
		}
		

		if (m_World != nullptr && m_Body != nullptr)
		{
			m_World->DestroyBody(m_Body);
			m_Body = nullptr;
			//std::cout << "Destructing Block" << std::endl;
		}
	}
	else if (!m_bOpen)
	{
		m_Shape.setTexture(m_Texture, true);
		m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, (m_Shape.getGlobalBounds().height / 2));
			
		if (m_World != nullptr && m_Body == nullptr)
		{
			m_BodyDef.type = b2_staticBody;
			m_BodyDef.position = b2Vec2(m_Shape.getPosition().x / m_Scale, (m_Shape.getPosition().y / m_Scale));
			m_Body = m_World->CreateBody(&m_BodyDef);

			m_b2pShape.SetAsBox((m_Size.x / 2) / m_Scale, (m_Size.y / 2) / m_Scale);

			m_FixtureDef.density = 1.0f;
			m_FixtureDef.shape = &m_b2pShape;
			m_Body->CreateFixture(&m_FixtureDef);
		}

		m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
		m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
	}

}

void CDoor::SetSize(float _x, float _y)
{
	m_Size.x = _x;
	m_Size.y = _y;

	/*m_Shape.setSize(m_Size);*/
	m_BodyDef.position = b2Vec2(m_BodyDef.position.x, m_BodyDef.position.y - 1);

	//ground physics
	m_BodyDef.type = b2_staticBody;
	m_Body = m_World->CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((m_Size.x / 2) / m_Scale, (m_Size.y / 2) / m_Scale);

	m_FixtureDef.density = 1.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_Body->CreateFixture(&m_FixtureDef);

	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, (m_Shape.getGlobalBounds().height / 2));
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);

	Render();
}

void CDoor::Update()
{
	CBlock::Update();
}

