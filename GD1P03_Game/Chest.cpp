//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CChest.cpp
// Description : CChest Implementation file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#include "Chest.h"

/// <summary>
/// Chest Constructor
/// </summary>
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
	m_Block = nullptr;
}

/// <summary>
/// Chest Contructor (Block)
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_world"></param>
/// <param name="_scale"></param>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
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

	CreateBody(_posX, _posY, b2_staticBody, true);
	m_Block = nullptr;
}

/// <summary>
/// CBlock Destructor
/// </summary>
CChest::~CChest()
{
	m_Inventory.clear();
	m_InventoryStackValues.clear();

	DestroyBody();
	delete m_Texture;
	m_Block = nullptr;
	m_Texture = nullptr;
	m_Body = nullptr;
	m_RenderWindow = nullptr;
	m_World = nullptr;
}

/// <summary>
/// Returns an int refering to the size of the chests inventory
/// </summary>
/// <returns></returns>
int CChest::GetInventorySize()
{
	return m_InventorySize;
}

/// <summary>
/// Adds an item to the chests inventory at the first free position
/// </summary>
/// <param name="_block"></param>
/// <param name="_canStack"></param>
void CChest::AddItemToInventory(CBlock* _block, bool _canStack)
{
	if (_canStack == true)
	{
		if (IsBlockInInventory(_block))
		{
		}
		else
		{
			//
			// Reading From File??
			//
			std::string line;
			std::ifstream myfile("Output/FirstEmptyChestSlot.txt");
			myfile.is_open();
			int firstEmpty = 0;
			myfile >> firstEmpty;

			_block->GetShape().setScale(0.4f, 0.4f);
			_block->GetShape().setOrigin(_block->GetShape().getGlobalBounds().width / 2, _block->GetShape().getGlobalBounds().height / 2);
			m_RenderWindow->mapCoordsToPixel(_block->GetShape().getPosition());
			m_InventorySize++;
			// increase number of that type
			m_InventoryStackValues[firstEmpty]++;
			_block->m_PositionInInventory = firstEmpty;
			m_Inventory.insert_or_assign(firstEmpty, *_block);
			//std::cout << "Added Item To Inventory - ";
			//std::cout << firstEmpty << std::endl;

			myfile.close();
		}
	}
	else
	{
		//
		// Reading From File??
		//
		std::string line;
		std::ifstream myfile("Output/FirstEmptyChestSlot.txt");
		myfile.is_open();
		int firstEmpty = 0;
		myfile >> firstEmpty;

		_block->GetShape().setScale(0.4f, 0.4f);
		_block->GetShape().setOrigin(_block->GetShape().getGlobalBounds().width / 2, _block->GetShape().getGlobalBounds().height / 2);
		m_RenderWindow->mapCoordsToPixel(_block->GetShape().getPosition());
		m_InventorySize++;
		// increase number of that type
		m_InventoryStackValues[firstEmpty]++;
		_block->m_PositionInInventory = firstEmpty;
		m_Inventory.insert_or_assign(firstEmpty, *_block);
		//std::cout << "Added Item To Inventory - ";
		std::cout << firstEmpty << std::endl;

		myfile.close();
	}
	
}

/// <summary>
/// Adds an item to the chests inventory at the specified position
/// </summary>
/// <param name="_block"></param>
/// <param name="_position"></param>
/// <param name="_canStack"></param>
void CChest::AddItemToInventory(CBlock* _block, int _position, bool _canStack)
{
	if (_canStack == true)
	{
		if (IsBlockInInventory(_block))
		{
		}
		else
		{
			_block->GetShape().setScale(0.4f, 0.4f);
			_block->GetShape().setOrigin(_block->GetShape().getGlobalBounds().width / 2, _block->GetShape().getGlobalBounds().height / 2);
			m_RenderWindow->mapCoordsToPixel(_block->GetShape().getPosition());
			m_InventorySize++;
			// increase number of that type
			m_InventoryStackValues[_position]++;
			_block->m_PositionInInventory = _position;
			m_Inventory.insert_or_assign(_position, *_block);
		}
	}
	else
	{
		_block->GetShape().setScale(0.4f, 0.4f);
		_block->GetShape().setOrigin(_block->GetShape().getGlobalBounds().width / 2, _block->GetShape().getGlobalBounds().height / 2);
		m_RenderWindow->mapCoordsToPixel(_block->GetShape().getPosition());
		m_InventorySize++;
		// increase number of that type
		m_InventoryStackValues[_position]++;
		_block->m_PositionInInventory = _position;
		m_Inventory.insert_or_assign(_position, *_block);
	}

}

/// <summary>
/// Returns a bool refering to weather or not the passed in blocktype exists in the chests inventory
/// </summary>
/// <param name="_block"></param>
/// <returns></returns>
bool CChest::IsBlockInInventory(CBlock* _block)
{
	std::map<int, CBlock>::iterator it;
	for (it = m_Inventory.begin(); it != m_Inventory.end(); it++)
	{
		if (it == m_Inventory.end())
		{
			break;
		}
		if (it->second.m_Type == _block->m_Type)
		{
			// increase number of that type
			m_InventoryStackValues[it->first]++;
			return true;
		}
	}
	return false;
}

/// <summary>
/// Removes an item from the chests inventory at _position
/// </summary>
/// <param name="_position"></param>
void CChest::RemoveItemFromInventory(int _position)
{
	std::map<int, CBlock>::iterator it = m_Inventory.begin();

	while (it != m_Inventory.end())
	{
		if (it == m_Inventory.end())
		{
			break;
		}
		if (it->first == _position)
		{

			while (m_InventoryStackValues[_position] > 0)
			{
				m_InventoryStackValues[_position]--;
			}
			m_InventorySize--;
			it = m_Inventory.erase(it);
			
			return;
		}
		else
		{
			it++;
		}
	}
}
