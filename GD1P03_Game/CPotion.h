#pragma once
#include "Block.h"
class CPotion : public CBlock
{
public:
	CPotion(CBlock::POTIONTYPE _potionType);
	~CPotion();
};

