#pragma once
#ifndef _WORKBENCH_H__
#define _WORKBENCH_H__

#include "Block.h"
class CWorkBench :	public CBlock
{
public:
	CWorkBench(CBlock::WORKBENCHTYPE _wordBenchType);
	CWorkBench(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY, CBlock::WORKBENCHTYPE _wordBenchType);
	virtual ~CWorkBench();
};
#endif

