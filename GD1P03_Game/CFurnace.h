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

	void Update();
};
#endif

