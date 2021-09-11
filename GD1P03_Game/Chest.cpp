#include "Chest.h"

CChest::CChest()
{
	m_Scale = 50.0f;
	m_Body = nullptr;
	CBlock::m_Type = BLOCKTYPE::CHEST;
	m_Type = BLOCKTYPE::CHEST;

	m_BlockStrength = 5;
	m_Texture = new sf::Texture();
	m_Texture->loadFromFile("Images/Chest.png");
	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setScale(0.3f, 0.3f);
	m_Shape.setOrigin(0.0f, m_Shape.getGlobalBounds().height / 2 + m_Shape.getGlobalBounds().height / 3);
}

CChest::CChest(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = &_world;
	CBlock::m_Type = BLOCKTYPE::CHEST;
	m_Type = BLOCKTYPE::CHEST;

	m_BlockStrength = 5;
	m_Texture = nullptr;
	m_Texture = new sf::Texture();
	m_Texture->loadFromFile("Images/Chest.png");
	m_Shape.setTexture(*m_Texture, true);


	//ground physics
	m_BodyDef.position = b2Vec2(_posX / m_Scale, (_posY / m_Scale));
	m_BodyDef.type = b2_staticBody;
	m_Body = _world.CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((m_Size.x / 2) / m_Scale, (m_Size.y / 2) / m_Scale);

	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

CChest::~CChest()
{
	if (m_World != nullptr && m_Body != nullptr)
	{
		m_World->DestroyBody(m_Body);
		//std::cout << "Destructing Block" << std::endl;
	}
	delete m_Texture;
	m_Texture = nullptr;
	m_Body = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
}

void CChest::Update()
{
	CBlock::Update();
}