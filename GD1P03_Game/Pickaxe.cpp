#include "Pickaxe.h"

CPickaxe::CPickaxe(PICKAXETYPE _pickType)
{
	m_Scale = 50.0f;
	m_Body = nullptr;
	CBlock::m_Type = BLOCKTYPE::PICKAXE;
	m_Sprite = new sf::Sprite();
	m_Sprite->setColor(sf::Color::White);
	m_Texture = nullptr;
	m_Texture = new sf::Texture();

	m_PickType = _pickType;

	switch (_pickType)
	{
	case CPickaxe::PICKAXETYPE::BASIC:
	{
		m_Texture->loadFromFile("Images/PickAxeRight.png");
		break;
	}
	case CPickaxe::PICKAXETYPE::IRON:
	{
		m_Texture->loadFromFile("Images/IronPickAxeRight.png");
		break;
	}
	case CPickaxe::PICKAXETYPE::GOLD:
	{
		m_Texture->loadFromFile("Images/GoldPickAxeRight.png");
		break;
	}
	case CPickaxe::PICKAXETYPE::DIAMOND:
	{
		m_Texture->loadFromFile("Images/DiamondPickAxeRight.png");
		break;
	}
	case CPickaxe::PICKAXETYPE::OBSIDIAN:
	{
		m_Texture->loadFromFile("Images/ObsidianPickAxeRight.png");
		break;
	}
	case CPickaxe::PICKAXETYPE::PURPLE:
	{
		m_Texture->loadFromFile("Images/PurplePickAxeRight.png");
		break;
	}
	case CPickaxe::PICKAXETYPE::GOLDEN:
	{
		m_Texture->loadFromFile("Images/GoldenPickAxeRight.png");
		break;
	}
	default:
	{
		m_Texture->loadFromFile("Images/PickAxeRight.png");
		break;
	}
	}

	m_Sprite->setTexture(*m_Texture, true);
	m_Sprite->setScale(0.2f, 0.2f);
	m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, m_Sprite->getGlobalBounds().height / 2);

	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setScale(0.2f, 0.2f);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2 , m_Shape.getGlobalBounds().height / 2);
}

CPickaxe::CPickaxe(sf::RenderWindow* _renderWindow, const float& _scale, float _posX, float _posY, PICKAXETYPE _pickType)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_PickType = _pickType;

	m_Sprite = new sf::Sprite();
	m_Sprite->setColor(sf::Color::White);

	switch (m_PickType)
	{
	case CPickaxe::PICKAXETYPE::BASIC:
	{
		m_LeftTex.loadFromFile("Images/PickAxeLeft.png");
		m_RightTex.loadFromFile("Images/PickAxeRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/PickAxeRight.png");

		m_PickaxePower = 1.0f;
		break;
	}
	case CPickaxe::PICKAXETYPE::IRON:
	{
		m_LeftTex.loadFromFile("Images/IronPickAxeLeft.png");
		m_RightTex.loadFromFile("Images/IronPickAxeRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/IronPickAxeRight.png");

		m_PickaxePower = 1.2f;
		break;
	}
	case CPickaxe::PICKAXETYPE::GOLD:
	{
		m_LeftTex.loadFromFile("Images/GoldPickAxeLeft.png");
		m_RightTex.loadFromFile("Images/GoldPickAxeRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/GoldPickAxeRight.png");

		m_PickaxePower = 1.4f;
		break;
	}
	case CPickaxe::PICKAXETYPE::DIAMOND:
	{
		m_LeftTex.loadFromFile("Images/DiamondPickAxeLeft.png");
		m_RightTex.loadFromFile("Images/DiamondPickAxeRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/DiamondPickAxeRight.png");

		m_PickaxePower = 1.6f;
		break;
	}
	case CPickaxe::PICKAXETYPE::PURPLE:
	{
		m_LeftTex.loadFromFile("Images/PurplePickAxeLeft.png");
		m_RightTex.loadFromFile("Images/PurplePickAxeRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/PurplePickAxeRight.png");

		m_PickaxePower = 1.8f;
		break;
	}
	case CPickaxe::PICKAXETYPE::GOLDEN:
	{
		m_LeftTex.loadFromFile("Images/GoldenPickAxeLeft.png");
		m_RightTex.loadFromFile("Images/GoldenPickAxeRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/GoldenPickAxeRight.png");

		m_PickaxePower = 2.0f;
		break;
	}
	case CPickaxe::PICKAXETYPE::OBSIDIAN:
	{
		m_LeftTex.loadFromFile("Images/ObsidianPickAxeLeft.png");
		m_RightTex.loadFromFile("Images/ObsidianPickAxeRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/ObsidianPickAxeRight.png");

		m_PickaxePower = 2.2f;
		break;
	}
	default:
	{
		m_LeftTex.loadFromFile("Images/PickAxeLeft.png");
		m_RightTex.loadFromFile("Images/PickAxeRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/PickAxeRight.png");

		m_PickaxePower = 1.0f;
		break;
	}
	}
	
	m_Sprite->setTexture(*m_Texture, true);
	m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, (m_Sprite->getGlobalBounds().height / 2));
	m_Sprite->setPosition(-10000, -10000);
	CBlock::m_Type = BLOCKTYPE::PICKAXE;
	m_Type = BLOCKTYPE::PICKAXE;
}

CPickaxe::~CPickaxe()
{
	std::cout << "Pickaxe Deleted!" << std::endl;
	delete m_Texture;
	delete m_Sprite;
	m_Sprite = nullptr;
	m_Body = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
}

void CPickaxe::FlipSprite(sf::Vector2f _playerPosition, sf::Sprite& _sprite, sf::Texture* _playerLeft, sf::Texture* _playerRight)
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

void CPickaxe::Update()
{
	CBlock::Update();
}

void CPickaxe::Render()
{
	m_RenderWindow->draw(*m_Sprite);
}
