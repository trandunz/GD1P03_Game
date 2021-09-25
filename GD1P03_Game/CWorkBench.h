#pragma once
#ifndef _WORKBENCH_H__
#define _WORKBENCH_H__

#include "Block.h"
class CWorkBench :	public CBlock
{
public:
	CWorkBench();
	CWorkBench(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY);
	virtual ~CWorkBench();

	void Update();

};
#endif

