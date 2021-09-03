#pragma once
#include "Block.h"
class CDoor : public CBlock
{
public:
	CDoor(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY);
	virtual ~CDoor();

	virtual void SetSize(float _x, float _y);

	void Update();
};

