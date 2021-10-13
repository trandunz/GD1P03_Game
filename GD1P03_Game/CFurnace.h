//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CFurnace.h
// Description : CFurnace Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once
#ifndef _FURNACE_H__
#define _FURNACE_H__

#include "Block.h"
class CFurnace : public CBlock
{
public:
	CFurnace();
	CFurnace(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY);
	virtual ~CFurnace();
};
#endif

