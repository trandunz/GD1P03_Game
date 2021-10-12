#include "Bow.h"

/// <summary>
/// Bow Constructor (Item In Inventory)
/// </summary>
/// <param name="_bowType"></param>
Bow::Bow(BOWTYPE _bowType)
{
	m_BowType = _bowType;
	m_Scale = 50.0f;
	m_Body = nullptr;
	m_Type = BLOCKTYPE::BOW;
	m_Sprite = new sf::Sprite();
	m_Sprite->setColor(sf::Color::White);

	m_Texture = nullptr;
	m_Texture = new sf::Texture();
	LoadTextureBasedOnType();
	
	InitSpriteTexture();
}

/// <summary>
/// Bow Contructor (Actual Item)
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_world"></param>
/// <param name="_scale"></param>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
/// <param name="_bowtype"></param>
Bow::Bow(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY, BOWTYPE _bowtype)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = &_world;
	m_Texture = nullptr;

	m_Sprite = new sf::Sprite();
	m_Sprite->setColor(sf::Color::White);

	m_BowType = _bowtype;

	m_Texture = new sf::Texture();

	LoadTexturesAndBPBasedOnType();

	m_Sprite->setTexture(*m_Texture, true);
	m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, (m_Sprite->getGlobalBounds().height / 2));
	m_Sprite->setPosition(-10000, -10000);
	CBlock::m_Type = BLOCKTYPE::BOW;
	m_Type = BLOCKTYPE::BOW;
}

/// <summary>
/// Bow Destructor
/// </summary>
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

/// <summary>
/// Reflects the bows position in the y axis based on the player sprite orientation
/// </summary>
/// <param name="_playerPosition"></param>
/// <param name="_sprite"></param>
/// <param name="_playerLeft"></param>
/// <param name="_playerRight"></param>
void Bow::FlipSprite(sf::Vector2f _playerPosition, sf::Sprite& _sprite, sf::Texture* _playerLeft, sf::Texture* _playerRight)
{
	if (_sprite.getTexture() == _playerLeft)
	{
		//std::cout << "Pickaxe Flipped!" << std::endl;
		m_Sprite->setTexture(m_LeftTex, true);
		m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, (m_Sprite->getGlobalBounds().height / 2));
		m_Sprite->setPosition(_playerPosition.x - 65, _playerPosition.y + 15);
	}
	else if (_sprite.getTexture() == _playerRight)
	{
		//std::cout << "Pickaxe Flipped!" << std::endl;
		m_Sprite->setTexture(m_RightTex, true);
		m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, (m_Sprite->getGlobalBounds().height / 2));
		m_Sprite->setPosition(_playerPosition.x + 65, _playerPosition.y + 15);
	}
}

/// <summary>
/// Bow Update
/// </summary>
void Bow::Update()
{
	CBlock::Update();
}

/// <summary>
/// Bow Render
/// </summary>
void Bow::Render()
{
	m_RenderWindow->draw(*m_Sprite);
}

/// <summary>
/// Loads the correct texture into m_Texture
/// </summary>
void Bow::LoadTextureBasedOnType()
{
	switch (m_BowType)
	{
	case CBlock::BOWTYPE::BASIC:
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

	case CBlock::BOWTYPE::IRONGUN:
		m_Texture->loadFromFile("Images/IronGunRight.png");
		break;
	case CBlock::BOWTYPE::GOLDGUN:
		m_Texture->loadFromFile("Images/GoldGunRight.png");
		break;
	case CBlock::BOWTYPE::PURPLEGUN:
		m_Texture->loadFromFile("Images/PurpleGunRight.png");
		break;
	case CBlock::BOWTYPE::GOLDENGUN:
		m_Texture->loadFromFile("Images/GoldenGunRight.png");
		break;
	case CBlock::BOWTYPE::GREENGUN:
		m_Texture->loadFromFile("Images/GreenGunRight.png");
		break;
	default:
		m_Texture->loadFromFile("Images/BowRight.png");
		break;
	}
}

/// <summary>
/// Loads the correct texture into all textures
/// Note : This function also sets m_BowPower Appropriatly
/// </summary>
void Bow::LoadTexturesAndBPBasedOnType()
{
	switch (m_BowType)
	{
	case CBlock::BOWTYPE::BASIC:
		m_LeftTex.loadFromFile("Images/BowLeft.png");
		m_RightTex.loadFromFile("Images/BowRight.png");

		m_Texture->loadFromFile("Images/BowRight.png");
		m_BowPower = 1;
		break;
	case CBlock::BOWTYPE::OBSIDIAN:
		m_LeftTex.loadFromFile("Images/ObsidianBowLeft.png");
		m_RightTex.loadFromFile("Images/ObsidianBowRight.png");

		m_Texture->loadFromFile("Images/ObsidianBowRight.png");
		m_BowPower = 8;
		break;
	case CBlock::BOWTYPE::ICE:
		m_LeftTex.loadFromFile("Images/IceBowLeft.png");
		m_RightTex.loadFromFile("Images/IceBowRight.png");

		m_Texture->loadFromFile("Images/IceBowRight.png");
		m_BowPower = 5;
		break;
	case CBlock::BOWTYPE::BLOOD:
		m_LeftTex.loadFromFile("Images/BloodBowLeft.png");
		m_RightTex.loadFromFile("Images/BloodBowRight.png");

		m_Texture->loadFromFile("Images/BloodBowRight.png");
		m_BowPower = 6;
		break;
	case CBlock::BOWTYPE::IRON:
		m_LeftTex.loadFromFile("Images/IronBowLeft.png");
		m_RightTex.loadFromFile("Images/IronBowRight.png");

		m_Texture->loadFromFile("Images/IronBowRight.png");
		m_BowPower = 2;
		break;
	case CBlock::BOWTYPE::GOLDEN:
		m_LeftTex.loadFromFile("Images/GoldenBowLeft.png");
		m_RightTex.loadFromFile("Images/GoldenBowRight.png");

		m_Texture->loadFromFile("Images/GoldenBowRight.png");
		m_BowPower = 4;
		break;
	case CBlock::BOWTYPE::PURPLE:
		m_LeftTex.loadFromFile("Images/PurpleBowLeft.png");
		m_RightTex.loadFromFile("Images/PurpleBowRight.png");

		m_Texture->loadFromFile("Images/PurpleBowRight.png");
		m_BowPower = 3;
		break;
	case CBlock::BOWTYPE::GREEN:
		m_LeftTex.loadFromFile("Images/GreenBowLeft.png");
		m_RightTex.loadFromFile("Images/GreenBowRight.png");

		m_Texture->loadFromFile("Images/GreenBowRight.png");

		m_BowPower = 7;
		break;

	case CBlock::BOWTYPE::IRONGUN:
		m_LeftTex.loadFromFile("Images/IronGunLeft.png");
		m_RightTex.loadFromFile("Images/IronGunRight.png");

		m_Texture->loadFromFile("Images/IronGunRight.png");
		break;
	case CBlock::BOWTYPE::GOLDGUN:
		m_LeftTex.loadFromFile("Images/GoldGunLeft.png");
		m_RightTex.loadFromFile("Images/GoldGunRight.png");

		m_Texture->loadFromFile("Images/GoldGunRight.png");
		break;
	case CBlock::BOWTYPE::PURPLEGUN:
		m_LeftTex.loadFromFile("Images/PurpleGunLeft.png");
		m_RightTex.loadFromFile("Images/PurpleGunRight.png");

		m_Texture->loadFromFile("Images/PurpleGunRight.png");
		break;
	case CBlock::BOWTYPE::GOLDENGUN:
		m_LeftTex.loadFromFile("Images/GoldenGunLeft.png");
		m_RightTex.loadFromFile("Images/GoldenGunRight.png");

		m_Texture->loadFromFile("Images/GoldenGunRight.png");
		break;
	case CBlock::BOWTYPE::GREENGUN:
		m_LeftTex.loadFromFile("Images/GreenGunLeft.png");
		m_RightTex.loadFromFile("Images/GreenGunRight.png");

		m_Texture->loadFromFile("Images/GreenGunRight.png");
		break;
	default:
		m_LeftTex.loadFromFile("Images/BowLeft.png");
		m_RightTex.loadFromFile("Images/BowRight.png");

		m_Texture->loadFromFile("Images/BowRight.png");
		break;
	}
}

/// <summary>
/// Initializes the sprite textures, sets their scale and their origins
/// </summary>
void Bow::InitSpriteTexture()
{
	m_Sprite->setTexture(*m_Texture, true);
	m_Sprite->setScale(0.43f, 0.43f);
	m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, m_Sprite->getGlobalBounds().height / 2);

	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setScale(0.43f, 0.43f);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
}
