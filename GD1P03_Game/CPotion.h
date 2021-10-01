#pragma once
#ifndef _POTION_H__
#define _POTION_H__

#include "Block.h"
class CPotion : public CBlock
{
public:
	CPotion(CBlock::POTIONTYPE _potionType);
	~CPotion();
};
#endif
