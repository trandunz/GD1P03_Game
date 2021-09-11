#include "Block.h"

CBlock::CBlock()
{
	m_RenderWindow = nullptr;
	m_Scale = 50.0f;
	m_World = nullptr;
	m_Body = nullptr;
	m_Type = BLOCK;
	m_Texture = nullptr;

	Start();
}

CBlock::CBlock(sf::Texture* _texture, BLOCKTYPE _type)
{
	m_RenderWindow = nullptr;
	m_Scale = 50.0f;
	m_World = nullptr;
	m_Scale = 50.0f;
	m_Body = nullptr;
	m_Type = _type;
	m_Texture = _texture;
	m_Shape.setTexture(*_texture, true);
	m_Shape.setScale(0.3f, 0.3f);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);

	Start();
}

CBlock::CBlock(sf::RenderWindow* _renderWindow, b2World& _world, const sf::Texture* _texture, const float& _scale, float _posX, float _posY, bool _bWallpaper, BLOCKTYPE _type)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = &_world;
	m_Type = _type;
	m_Texture = const_cast<sf::Texture*>(_texture);

	m_Shape.setTexture(*_texture, true);

	//Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);

	if (!_bWallpaper)
	{
		//ground physics
		m_BodyDef.type = b2_staticBody;
		m_BodyDef.position = b2Vec2(_posX / m_Scale, (_posY / m_Scale));
		m_Body = m_World->CreateBody(&m_BodyDef);

		m_b2pShape.SetAsBox((m_Size.x / 2) / m_Scale, (m_Size.y / 2) / m_Scale);

		m_FixtureDef.density = 1.0f;
		m_FixtureDef.shape = &m_b2pShape;
		m_Body->CreateFixture(&m_FixtureDef);

		m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
		m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
	}
	else if (_bWallpaper)
	{
		m_Shape.setPosition(_posX, _posY);
	}
	
	
	
	

	Start();
	Render();
}

CBlock::~CBlock()
{
	if (m_World != nullptr && m_Body != nullptr)
	{
		m_World->DestroyBody(m_Body);
		//std::cout << "Destructing Block" << std::endl;
	}

	m_Texture = nullptr;
	m_Body = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
}

void CBlock::Start()
{
	switch (m_Type)
	{
	case CBlock::DOOR:
	{
		m_BlockStrength = 5;
		break;
	}
	case CBlock::CHEST:
	{
		m_BlockStrength = 5;
		break;
	}
	case CBlock::DIRT:
	{
		m_BlockStrength = 2;
		break;
	}
	case CBlock::STONE:
	{
		m_BlockStrength = 3;
		break;
	}
	case CBlock::WOOD:
	{
		m_BlockStrength = 4;
		break;
	}
	case CBlock::PLANKS:
	{
		m_BlockStrength = 3;
		break;
	}
	case CBlock::SAND:
	{
		m_BlockStrength = 2;
		break;
	}
	case CBlock::MOSSYBRICK:
	{
		m_BlockStrength = 6;
		break;
	}
	case CBlock::GRASS:
	{
		m_BlockStrength = 3;
		break;
	}
	case CBlock::LEAVES:
	{
		m_BlockStrength = 1;
		break;
	}
	case CBlock::OBSIDIAN:
	{
		m_BlockStrength = 100;
		break;
	}
	case CBlock::IRON:
	{
		m_BlockStrength = 5;
		break;
	}
	case CBlock::COAL:
	{
		m_BlockStrength = 5;
		break;
	}
	case CBlock::GOLD:
	{
		m_BlockStrength = 6;
		break;
	}
	case CBlock::DIAMOND:
	{
		m_BlockStrength = 6;
		break;
	}
		
	default:
	{
		m_BlockStrength = 3;
		break;
	}
		
	}
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
