//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CWorkBench.cpp
// Description : CWorkBench Implementation file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#include "CWorkBench.h"

/// <summary>
/// CWorkbench Contructor
/// Note : Should be used to spawn Anvil
/// </summary>
/// <param name="_wordBenchType"></param>
CWorkBench::CWorkBench(CBlock::WORKBENCHTYPE _wordBenchType)
{
	m_Scale = 50.0f;
	m_Body = nullptr;
	m_WorkBenchType = _wordBenchType;

	m_BlockStrength = 5;
	m_Texture = new sf::Texture();

	switch (m_WorkBenchType)
	{
	case CBlock::WORKBENCHTYPE::WORKBENCH:
		m_Type = BLOCKTYPE::WORKBENCH;
		m_Texture->loadFromFile("Images/WorkBench.png");
		break;
	case CBlock::WORKBENCHTYPE::ANVIL:
		m_Type = BLOCKTYPE::ANVIL;
		m_Texture->loadFromFile("Images/Anvil.png");
		break;
	default:
		break;
	}
	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setScale(0.3f, 0.3f);
	m_Shape.setOrigin(0.0f, m_Shape.getGlobalBounds().height / 2 + m_Shape.getGlobalBounds().height / 3);
}

/// <summary>
/// CWorkbench Contructor
/// Note : Should be used to spawn anvil
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_world"></param>
/// <param name="_scale"></param>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
/// <param name="_wordBenchType"></param>
CWorkBench::CWorkBench(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY, CBlock::WORKBENCHTYPE _wordBenchType)
{
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = &_world;
	m_WorkBenchType = _wordBenchType;

	m_BlockStrength = 5;
	m_Texture = nullptr;
	m_Texture = new sf::Texture();
	switch (m_WorkBenchType)
	{
	case CBlock::WORKBENCHTYPE::WORKBENCH:
		m_Type = BLOCKTYPE::WORKBENCH;
		m_Texture->loadFromFile("Images/WorkBench.png");
		break;
	case CBlock::WORKBENCHTYPE::ANVIL:
		m_Type = BLOCKTYPE::ANVIL;
		m_Texture->loadFromFile("Images/Anvil.png");
		break;
	default:
		break;
	}
	m_Shape.setTexture(*m_Texture, true);


	CreateBody(_posX, _posY, b2_staticBody, true);
}

/// <summary>
/// CWorkbench Destructor
/// </summary>
CWorkBench::~CWorkBench()
{
	DestroyBody();
	delete m_Texture;
	m_Texture = nullptr;
	m_Body = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
}
