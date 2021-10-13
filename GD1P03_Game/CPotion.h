//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CPotion.h
// Description : CPotion Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

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
