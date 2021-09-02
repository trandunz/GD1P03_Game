#pragma once
#include "Block.h"
class CChest : public CBlock
{
public:
	CChest(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY);
	virtual ~CChest();

	void Update();
};

