#include "Door.h"

/// <summary>
/// CDoor Contructor
/// </summary>
CDoor::CDoor()
{
	m_Scale = 50.0f;
	m_Body = nullptr;
	CBlock::m_Type = BLOCKTYPE::DOOR;
	m_Type = BLOCKTYPE::DOOR;

	m_BlockStrength = 5;
	m_Texture = new sf::Texture();
	m_Texture->loadFromFile("Images/DoorOpenLeft.png");

	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setScale(0.2f, 0.2f);
	m_Shape.setOrigin(0.0f, m_Shape.getGlobalBounds().height / 2 + m_Shape.getGlobalBounds().height / 3);
}

/// <summary>
/// CDoor Constructor (Block)
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_world"></param>
/// <param name="_scale"></param>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
CDoor::CDoor(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = &_world;

	m_OpenLeftTex.loadFromFile("Images/DoorOpenLeft.png");
	m_OpenRightTex.loadFromFile("Images/DoorOpenRight.png");
	m_Texture = new sf::Texture();
	m_Texture->loadFromFile("Images/Door.png");
	m_Shape.setTexture(*m_Texture, true);

	CreateBody(_posX, _posY, b2_staticBody);

	CBlock::m_Type = BLOCKTYPE::DOOR;
	m_Type = BLOCKTYPE::DOOR;

	m_BlockStrength = 5;
}

/// <summary>
/// CDoor Destructor
/// </summary>
CDoor::~CDoor()
{
	DestroyBody();
	delete m_Texture;
	m_Texture = nullptr;
	m_Body = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
}

/// <summary>
/// Handles Opening and closing the door along with the orientation of the sprite
/// </summary>
/// <param name="_playerPosition"></param>
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
		
		DestroyBody();
	}
	else if (!m_bOpen)
	{
		m_Shape.setTexture(*m_Texture, true);
		m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, (m_Shape.getGlobalBounds().height / 2));
			
		if (m_World != nullptr && m_Body == nullptr)
		{
			CreateBody(m_Shape.getPosition().x, m_Shape.getPosition().y, b2_staticBody);
		}

		m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
		m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
	}

}

/// <summary>
/// Sets the doors box2d body size and position 
/// </summary>
/// <param name="_currentPosX"></param>
/// <param name="_currentPosY"></param>
/// <param name="_x"></param>
/// <param name="_y"></param>
void CDoor::SetSizeAndPos(float _currentPosX, float _currentPosY, float _x, float _y)
{
	m_Size.x = _x;
	m_Size.y = _y;

	DestroyBody();
	CreateBody(_currentPosX, _currentPosY - m_Shape.getGlobalBounds().height / 4, b2_staticBody);
}

