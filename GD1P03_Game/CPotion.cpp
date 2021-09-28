#include "CPotion.h"

CPotion::CPotion(CBlock::POTIONTYPE _potionType)
{
	m_Scale = 50.0f;
	m_Body = nullptr;
	m_Type = BLOCKTYPE::POTION;
	m_Texture = nullptr;
	m_Texture = new sf::Texture();
	m_PotionType = _potionType;

	// Type Specific Data
	switch (_potionType)
	{
	case CBlock::POTIONTYPE::HPSMALL:
		m_Texture->loadFromFile("Images/HPSmall.png");
		break;
	case CBlock::POTIONTYPE::HPLARGE:
		m_Texture->loadFromFile("Images/HPLarge.png");
		break;
	default:
		break;
	}

	// Sprite
	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setScale(0.3f, 0.3f);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
}

CPotion::~CPotion()
{
	delete m_Texture;

	m_Texture = nullptr;
	m_World = nullptr;
}
