#include "CFurnace.h"

CFurnace::CFurnace()
{
	m_Scale = 50.0f;
	m_Body = nullptr;
	CBlock::m_Type = BLOCKTYPE::FURNACE;
	m_Type = BLOCKTYPE::FURNACE;

	m_BlockStrength = 5;
	m_Texture = new sf::Texture();
	m_Texture->loadFromFile("Images/Furnace.png");
	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setScale(0.3f, 0.3f);
	m_Shape.setOrigin(0.0f, m_Shape.getGlobalBounds().height / 2 + m_Shape.getGlobalBounds().height / 3);
}

CFurnace::CFurnace(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = &_world;
	CBlock::m_Type = BLOCKTYPE::FURNACE;
	m_Type = BLOCKTYPE::FURNACE;

	m_BlockStrength = 5;
	m_Texture = nullptr;
	m_Texture = new sf::Texture();
	m_Texture->loadFromFile("Images/Furnace.png");
	m_Shape.setTexture(*m_Texture, true);


	CreateBody(_posX, _posY, b2_staticBody);
}

CFurnace::~CFurnace()
{
	delete m_Texture;
	m_Texture = nullptr;
	m_Body = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
}

void CFurnace::Update()
{
	CBlock::Update();
}
