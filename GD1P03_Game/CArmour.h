//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CArmour.h
// Description : CArmour Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once
#ifndef _ARMOUR_H__
#define _ARMOUR_H__

#include "Block.h"
class CArmour :  public CBlock
{
public:
	CArmour(CBlock::BLOCKTYPE _itemType, CBlock::ARMOURTYPE _armourtype);
	~CArmour();

private:
	void LoadTextureBasedOnItemType(CBlock::BLOCKTYPE _itemType, CBlock::ARMOURTYPE _armourType);

	void CheckHelmetTypeAndAssignTexture(CBlock::ARMOURTYPE _armourType);
	void CheckChestPlateTypeAndAssignTexture(CBlock::ARMOURTYPE _armourType);
	void CheckLeggingTypeAndAssignTexture(CBlock::ARMOURTYPE _armourType);
};
#endif

