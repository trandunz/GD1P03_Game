#include "CSword.h"

CSword::CSword(SWORDTYPE _swordType)
{
	m_Scale = 50.0f;
	m_Body = nullptr;
	CBlock::m_Type = BLOCKTYPE::SWORD;
	m_Sprite = new sf::Sprite();
	m_Sprite->setColor(sf::Color::White);
	m_Texture = nullptr;
	m_Texture = new sf::Texture();

	m_SwordType = _swordType;

	switch (m_SwordType)
	{
	case CBlock::SWORDTYPE::WOOD:
		m_Texture->loadFromFile("Images/WoodSwordRight.png");
		break;
	case CBlock::SWORDTYPE::ANCIENT:
		m_Texture->loadFromFile("Images/AncientSwordRight.png");
		break;
	case CBlock::SWORDTYPE::FLAME:
		m_Texture->loadFromFile("Images/FlameSwordRight.png");
		break;
	case CBlock::SWORDTYPE::GOD:
		m_Texture->loadFromFile("Images/GodSwordRight.png");
		break;
	case CBlock::SWORDTYPE::GOLDEN:
		m_Texture->loadFromFile("Images/GoldenSwordRight.png");
		break;
	case CBlock::SWORDTYPE::GREEN:
		m_Texture->loadFromFile("Images/GreenSwordRight.png");
		break;
	case CBlock::SWORDTYPE::PURPLE:
		m_Texture->loadFromFile("Images/PurpleSwordRight.png");
		break;
	case CBlock::SWORDTYPE::SLIME:
		m_Texture->loadFromFile("Images/SlimeSwordRight.png");
		break;
	default:
		m_Texture->loadFromFile("Images/WoodSwordRight.png");
		break;
	}

	m_Sprite->setTexture(*m_Texture, true);
	m_Sprite->setScale(0.2f, 0.2f);
	m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, m_Sprite->getGlobalBounds().height / 2);

	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setScale(0.3f, 0.3f);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
}

CSword::CSword(sf::RenderWindow* _renderWindow, const float& _scale, float _posX, float _posY, SWORDTYPE _swordType)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_SwordType = _swordType;

	m_Sprite = new sf::Sprite();
	m_Sprite->setColor(sf::Color::White);

	switch (m_SwordType)
	{
	case CBlock::SWORDTYPE::WOOD:
		m_LeftTex.loadFromFile("Images/WoodSwordLeft.png");
		m_RightTex.loadFromFile("Images/WoodSwordRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/WoodSwordRight.png");

		m_SwordPower = 1.0f;
		break;
	case CBlock::SWORDTYPE::ANCIENT:
		m_LeftTex.loadFromFile("Images/AncientSwordLeft.png");
		m_RightTex.loadFromFile("Images/AncientSwordRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/AncientSwordRight.png");

		m_SwordPower = 1.0f;
		break;
	case CBlock::SWORDTYPE::FLAME:
		m_LeftTex.loadFromFile("Images/FlameSwordLeft.png");
		m_RightTex.loadFromFile("Images/FlameSwordRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/FlameSwordRight.png");

		m_SwordPower = 1.0f;
		break;
	case CBlock::SWORDTYPE::GOD:
		m_LeftTex.loadFromFile("Images/GodSwordLeft.png");
		m_RightTex.loadFromFile("Images/GodSwordRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/GodSwordRight.png");

		m_SwordPower = 1.0f;
		break;
	case CBlock::SWORDTYPE::GOLDEN:
		m_LeftTex.loadFromFile("Images/GoldenSwordLeft.png");
		m_RightTex.loadFromFile("Images/GoldenSwordRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/GoldenSwordRight.png");

		m_SwordPower = 1.0f;
		break;
	case CBlock::SWORDTYPE::GREEN:
		m_LeftTex.loadFromFile("Images/GreenSwordLeft.png");
		m_RightTex.loadFromFile("Images/GreenSwordRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/GreenSwordRight.png");

		m_SwordPower = 1.0f;
		break;
	case CBlock::SWORDTYPE::PURPLE:
		m_LeftTex.loadFromFile("Images/PurpleSwordLeft.png");
		m_RightTex.loadFromFile("Images/PurpleSwordRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/PurpleSwordRight.png");

		m_SwordPower = 1.0f;
		break;
	case CBlock::SWORDTYPE::SLIME:
		m_LeftTex.loadFromFile("Images/SlimeSwordLeft.png");
		m_RightTex.loadFromFile("Images/SlimeSwordRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/SlimeSwordRight.png");

		m_SwordPower = 1.0f;
		break;
	default:
		m_LeftTex.loadFromFile("Images/WoodSwordLeft.png");
		m_RightTex.loadFromFile("Images/WoodSwordRight.png");

		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/WoodSwordRight.png");

		m_SwordPower = 1.0f;
		break;
	}

	m_Sprite->setTexture(*m_Texture, true);
	m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, (m_Sprite->getGlobalBounds().height / 2));
	m_Sprite->setPosition(-10000, -10000);
	CBlock::m_Type = BLOCKTYPE::SWORD;
	m_Type = BLOCKTYPE::SWORD;
}

CSword::~CSword()
{
	std::cout << "Sword Deleted!" << std::endl;
	delete m_Texture;
	delete m_Sprite;
	m_Sprite = nullptr;
	m_Body = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
}

void CSword::FlipSprite(sf::Vector2f _playerPosition, sf::Sprite& _sprite, sf::Texture* _playerLeft, sf::Texture* _playerRight)
{
	if (_sprite.getTexture() == _playerLeft)
	{
		//std::cout << "Pickaxe Flipped!" << std::endl;
		m_Sprite->setTexture(m_LeftTex, true);
		m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, (m_Sprite->getGlobalBounds().height / 2));
		m_Sprite->setPosition(_playerPosition.x - 62, _playerPosition.y + 13);
	}
	else if (_sprite.getTexture() == _playerRight)
	{
		//std::cout << "Pickaxe Flipped!" << std::endl;
		m_Sprite->setTexture(m_RightTex, true);
		m_Sprite->setOrigin(m_Sprite->getGlobalBounds().width / 2, (m_Sprite->getGlobalBounds().height / 2));
		m_Sprite->setPosition(_playerPosition.x + 62, _playerPosition.y + 13);
	}
}

void CSword::CreateSwordTrigger()
{
	// Body
	m_BodyDef.type = b2_staticBody;
	m_BodyDef.fixedRotation = false;
	m_BodyDef.position = b2Vec2(m_Sprite->getPosition().x / 50.0f, (m_Sprite->getPosition().y / 50.0f));
	m_BodyDef.bullet = true;

	int* damage = new int(m_SwordPower);
	m_BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(damage);
	damage = nullptr;

	//
	m_Body = m_World->CreateBody(&m_BodyDef);

	// Shape
	m_b2pShape.m_radius = 2.0f;

	// Fixture
	m_FixtureDef.isSensor = true;
	m_FixtureDef.density = 0.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_FixtureDef.filter.categoryBits = _PLAYER_FILTER_;
	m_FixtureDef.filter.maskBits = _ENEMY_FILTER_;
	m_FixtureDef.filter.maskBits = _WORLD_FILTER_;
	m_FixtureDef.filter.groupIndex = -_PLAYER_GROUPINDEX_;
	m_Body->CreateFixture(&m_FixtureDef);
}

void CSword::Update()
{
	CBlock::Update();
}

void CSword::Render()
{
	m_RenderWindow->draw(*m_Sprite);
}
