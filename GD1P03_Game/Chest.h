#pragma once
#ifndef _CHEST_H__
#define _CHEST_H__

#include <fstream>

#include "Block.h"



class CChest : public CBlock
{
public:
	CChest();
	CChest(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY);
	virtual ~CChest();

	void Update();

	int GetInventorySize();

	void AddItemToInventory(CBlock* _block, bool _canStack = true);
	bool IsBlockInInventory(CBlock* _block);
	void RemoveItemFromInventory(int _position);

	bool m_bIsOpen = false;
	std::map<int, CBlock> m_Inventory = {};
	std::map<int, int> m_InventoryStackValues = {};

private:
	CBlock* m_Block;

	int m_InventorySize = 0;
};

#endif

