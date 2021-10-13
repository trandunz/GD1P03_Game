//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CBlock.cpp
// Description : CBlock Implementation file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#include "Block.h"

/// <summary>
/// Default Block Constructor
/// </summary>
CBlock::CBlock()
{
	m_RenderWindow = nullptr;
	m_Scale = 50.0f;
	m_World = nullptr;
	m_Body = nullptr;
	m_Type = BLOCKTYPE::MOSSYBRICK;
	m_Texture = nullptr;

	// Optimization
	m_BodyDef = nullptr;
	m_b2pShape = nullptr;
	m_FixtureDef = nullptr;

	Start();
}

/// <summary>
/// CBlock Item Constructor
/// </summary>
/// <param name="_texture"></param>
/// <param name="_type"></param>
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

	m_Shape.setScale(0.2f, 0.2f);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);

	Start();
}

/// <summary>
/// CBlock Object/Block Contructor
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_world"></param>
/// <param name="_texture"></param>
/// <param name="_scale"></param>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
/// <param name="_bWallpaper"></param>
/// <param name="_type"></param>
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
		CreateBody(_posX, _posY, b2_staticBody);
	}
	else if (_bWallpaper)
	{
		m_Shape.setPosition(_posX, _posY);
	}
	
	Start();
	Render();
}

/// <summary>
/// CBlock Destructor
/// </summary>
CBlock::~CBlock()
{
	//std::cout << "Block Destoryed" << std::endl;

	DestroyBody();

	m_Texture = nullptr;
	m_Body = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
	m_BodyDef = nullptr;
	m_b2pShape = nullptr;
	m_FixtureDef = nullptr;
}

/// <summary>
/// CBlock Start
/// </summary>
void CBlock::Start()
{
	CalculateBlockStrength();
}

/// <summary>
/// CBlock Update
/// </summary>
void CBlock::Update()
{
	// Set SFML Shape Transform To Box 2D Body Transform
	SetSFShapeToBody();
}

/// <summary>
/// CBlock Render
/// </summary>
/// <param name="_shader"></param>
void CBlock::Render(sf::Shader* _shader)
{
	m_RenderWindow->draw(m_Shape, _shader);
}

/// <summary>
/// Sets the size and position of the block / placed item
/// </summary>
/// <param name="_currentPosX"></param>
/// <param name="_currentPosY"></param>
/// <param name="_x"></param>
/// <param name="_y"></param>
void CBlock::SetSizeAndPos(float _currentPosX, float _currentPosY, float _x, float _y)
{
	m_Size.x = _x;
	m_Size.y = _y;

	DestroyBody();
	CreateBody(_currentPosX, _currentPosY, b2_staticBody, true);
}

/// <summary>
/// Destroys the blocks box2d body
/// </summary>
void CBlock::DestroyBody()
{
	if (m_World != nullptr && m_Body != nullptr)
	{
		m_World->DestroyBody(m_Body);

		if (m_BodyDef != nullptr)
		{
			delete m_BodyDef;
			m_BodyDef = nullptr;
		}

		if (m_b2pShape != nullptr)
		{
			delete m_b2pShape;
			m_b2pShape = nullptr;
		}

		if (m_FixtureDef != nullptr)
		{
			delete m_FixtureDef;
			m_FixtureDef = nullptr;
		}

		m_Body = nullptr;
	}
}

/// <summary>
/// Creates the box2d Body
/// </summary>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
/// <param name="_type"></param>
/// <param name="_sensor"></param>
void CBlock::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	// Body Def
	m_BodyDef = new b2BodyDef();
	m_BodyDef->type = _type;
	m_BodyDef->position = b2Vec2(_posX / m_Scale, (_posY / m_Scale));
	m_BodyDef->allowSleep = true;
	m_BodyDef->awake = false;
	m_Body = m_World->CreateBody(m_BodyDef);
	m_Body->SetAwake(false);

	// b2Shape
	m_b2pShape = new b2PolygonShape();
	m_b2pShape->SetAsBox((m_Size.x / 2) / m_Scale, (m_Size.y / 2) / m_Scale);

	m_FixtureDef = new b2FixtureDef();
	if (_sensor)
	{
		m_FixtureDef->isSensor = true;
	}

	// Fixture Def
	m_FixtureDef->density = 1.0f;
	m_FixtureDef->shape = m_b2pShape;
	m_FixtureDef->filter.categoryBits = _WORLD_FILTER_;
	m_FixtureDef->filter.groupIndex = -_WORLD_GROUPINDEX_;
	m_Body->CreateFixture(m_FixtureDef);

	SetSFShapeToBody();
}

/// <summary>
/// Calculates and sets m_BlockStrength based on m_Type (BLOCKTYPE)
/// </summary>
void CBlock::CalculateBlockStrength()
{
	switch (m_Type)
	{
	case CBlock::BLOCKTYPE::DOOR:
	{
		m_BlockStrength = 5;
		break;
	}
	case CBlock::BLOCKTYPE::CHEST:
	{
		m_BlockStrength = 5;
		break;
	}
	case CBlock::BLOCKTYPE::DIRT:
	{
		m_BlockStrength = 2;
		break;
	}
	case CBlock::BLOCKTYPE::STONE:
	{
		m_BlockStrength = 3;
		break;
	}
	case CBlock::BLOCKTYPE::WOOD:
	{
		m_BlockStrength = 4;
		break;
	}
	case CBlock::BLOCKTYPE::PLANKS:
	{
		m_BlockStrength = 3;
		break;
	}
	case CBlock::BLOCKTYPE::SAND:
	{
		m_BlockStrength = 2;
		break;
	}
	case CBlock::BLOCKTYPE::MOSSYBRICK:
	{
		m_BlockStrength = 6;
		break;
	}
	case CBlock::BLOCKTYPE::GRASS:
	{
		m_BlockStrength = 3;
		break;
	}
	case CBlock::BLOCKTYPE::LEAVES:
	{
		m_BlockStrength = 1;
		break;
	}
	case CBlock::BLOCKTYPE::OBSIDIAN:
	{
		m_BlockStrength = 100;
		break;
	}
	case CBlock::BLOCKTYPE::IRONORE:
	{
		m_BlockStrength = 5;
		break;
	}
	case CBlock::BLOCKTYPE::COALORE:
	{
		m_BlockStrength = 5;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDORE:
	{
		m_BlockStrength = 6;
		break;
	}
	case CBlock::BLOCKTYPE::DIAMONDORE:
	{
		m_BlockStrength = 7;
		break;
	}
	case CBlock::BLOCKTYPE::PURPLEORE:
	{
		m_BlockStrength = 8;
		break;
	}
	case CBlock::BLOCKTYPE::GOLDENORE:
	{
		m_BlockStrength = 9;
		break;
	}
	case CBlock::BLOCKTYPE::CLOUD:
	{
		m_BlockStrength = 1;
		break;
	}
	case CBlock::BLOCKTYPE::SANDSTONE:
	{
		m_BlockStrength = 3;
		break;
	}
	case CBlock::BLOCKTYPE::HARDSANDSTONE:
	{
		m_BlockStrength = 6;
		break;
	}
	case CBlock::BLOCKTYPE::CACTUS:
	{
		m_BlockStrength = 1;
		break;
	}
	case CBlock::BLOCKTYPE::SNOW:
	{
		m_BlockStrength = 2;
		break;
	}
	case CBlock::BLOCKTYPE::DIRTHELL:
	{
		m_BlockStrength = 2;
		break;
	}
	case CBlock::BLOCKTYPE::STONEHELL:
	{
		m_BlockStrength = 3;
		break;
	}
	case CBlock::BLOCKTYPE::BRICKHELL:
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

/// <summary>
/// Sets the sfml shape's position and rotation to that of the box2d bodies
/// </summary>
void CBlock::SetSFShapeToBody()
{
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	if (m_Body != nullptr)
	{
		m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, (m_Body->GetPosition().y * m_Scale));
		m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
	}
}

void CBlock::DeleteThis()
{
	delete this;
}
