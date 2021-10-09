#pragma once
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

