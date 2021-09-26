#include "Bow.h"

Bow::Bow(BOWTYPE _bowType)
{
	m_Scale = 50.0f;
	m_Body = nullptr;
	m_Type = BLOCKTYPE::BOW;
	m_Sprite = new sf::Sprite();
	m_Sprite->setColor(sf::Color::White);
	m_Texture = nullptr;
	m_Texture = new sf::Texture();

	m_BowType = _bowType;
	switch (m_BowType)
	{
	case CBlock::BOWTYPE::DEFAULT:
		m_Texture->loadFromFile("Images/BowRight.png");
		break;
	case CBlock::BOWTYPE::OBSIDIAN:
		m_Texture->loadFromFile("Images/ObsidianBowRight.png");
		break;
	case CBlock::BOWTYPE::ICE:
		m_Texture->loadFromFile("Images/IceBowRight.png");
		break;
	case CBlock::BOWTYPE::BLOOD:
		m_Texture->loadFromFile("Images/BloodBowRight.png");
		break;
	case CBlock::BOWTYPE::IRON:
		m_Texture->loadFromFile("Images/IronBowRight.png");
		break;
	case CBlock::BOWTYPE::GOLDEN:
		m_Texture->loadFromFile("Images/GoldenBowRight.png");
		break;
	case CBlock::BOWTYPE::PURPLE:
		m_Texture->loadFromFile("Images/PurpleBowRight.png");
		break;
	case CBlock::BOWTYPE::GREEN:
		m_Texture->loadFromFile("Images/GreenBowRight.png");
		break;
	default:
		m_Texture->loadFromFile("Images/BowRight.png");
		break;
	}
	
	m_Sprite->setTexture(*m_Texture, true);
	m_Sprite->setScale(0.2f, 0.2f);
	m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, m_Sprite->getGlobalBounds().height / 2);

	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setScale(0.3f, 0.3f);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
}

Bow::Bow(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY, BOWTYPE _bowtype)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = &_world;
	m_Texture = nullptr;

	m_Sprite = new sf::Sprite();
	m_Sprite->setColor(sf::Color::White);

	m_Texture = new sf::Texture();

	m_BowType = _bowtype;
	switch (m_BowType)
	{
	case CBlock::BOWTYPE::DEFAULT:
		m_LeftTex.loadFromFile("Images/BowLeft.png");
		m_RightTex.loadFromFile("Images/BowRight.png");

		m_Texture->loadFromFile("Images/BowRight.png");
		break;
	case CBlock::BOWTYPE::OBSIDIAN:
		m_LeftTex.loadFromFile("Images/ObsidianBowLeft.png");
		m_RightTex.loadFromFile("Images/ObsidianBowRight.png");

		m_Texture->loadFromFile("Images/ObsidianBowRight.png");
		break;
	case CBlock::BOWTYPE::ICE:
		m_LeftTex.loadFromFile("Images/IceBowLeft.png");
		m_RightTex.loadFromFile("Images/IceBowRight.png");

		m_Texture->loadFromFile("Images/IceBowRight.png");
		break;
	case CBlock::BOWTYPE::BLOOD:
		m_LeftTex.loadFromFile("Images/BloodBowLeft.png");
		m_RightTex.loadFromFile("Images/BloodBowRight.png");

		m_Texture->loadFromFile("Images/BloodBowRight.png");
		break;
	case CBlock::BOWTYPE::IRON:
		m_LeftTex.loadFromFile("Images/IronBowLeft.png");
		m_RightTex.loadFromFile("Images/IronBowRight.png");

		m_Texture->loadFromFile("Images/IronBowRight.png");
		break;
	case CBlock::BOWTYPE::GOLDEN:
		m_LeftTex.loadFromFile("Images/GoldenBowLeft.png");
		m_RightTex.loadFromFile("Images/GoldenBowRight.png");

		m_Texture->loadFromFile("Images/GoldenBowRight.png");
		break;
	case CBlock::BOWTYPE::PURPLE:
		m_LeftTex.loadFromFile("Images/PurpleBowLeft.png");
		m_RightTex.loadFromFile("Images/PurpleBowRight.png");

		m_Texture->loadFromFile("Images/PurpleBowRight.png");
		break;
	case CBlock::BOWTYPE::GREEN:
		m_LeftTex.loadFromFile("Images/GreenBowLeft.png");
		m_RightTex.loadFromFile("Images/GreenBowRight.png");

		m_Texture->loadFromFile("Images/GreenBowRight.png");
		break;
	default:
		m_LeftTex.loadFromFile("Images/BowLeft.png");
		m_RightTex.loadFromFile("Images/BowRight.png");
		
		m_Texture->loadFromFile("Images/BowRight.png");
		break;
	}

	m_Sprite->setTexture(*m_Texture, true);
	m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, (m_Sprite->getGlobalBounds().height / 2));
	m_Sprite->setPosition(-10000, -10000);
	CBlock::m_Type = BLOCKTYPE::BOW;
	m_Type = BLOCKTYPE::BOW;
}

Bow::~Bow()
{
	std::cout << "Bow Deleted!" << std::endl;
	delete m_Texture;
	delete m_Sprite;
	m_Sprite = nullptr;
	m_Body = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
}

void Bow::FlipSprite(sf::Vector2f _playerPosition, sf::Sprite& _sprite, sf::Texture* _playerLeft, sf::Texture* _playerRight)
{
	if (_sprite.getTexture() == _playerLeft)
	{
		//std::cout << "Pickaxe Flipped!" << std::endl;
		m_Sprite->setTexture(m_LeftTex, true);
		m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, (m_Sprite->getGlobalBounds().height / 2));
		m_Sprite->setPosition(_playerPosition.x - 60, _playerPosition.y + 15);
	}
	else if (_sprite.getTexture() == _playerRight)
	{
		//std::cout << "Pickaxe Flipped!" << std::endl;
		m_Sprite->setTexture(m_RightTex, true);
		m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, (m_Sprite->getGlobalBounds().height / 2));
		m_Sprite->setPosition(_playerPosition.x + 60, _playerPosition.y + 15);
	}
}

void Bow::Update()
{
	CBlock::Update();
}

void Bow::Render()
{
	m_RenderWindow->draw(*m_Sprite);
}
