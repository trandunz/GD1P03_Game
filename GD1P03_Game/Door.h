#pragma once
#ifndef _DOOR_H__
#define _DOOR_H__

#include "Block.h"



class CDoor : public CBlock
{
public:
	CDoor();
	CDoor(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY);
	virtual ~CDoor();

	void OCDoor(sf::Vector2f _playerPosition);
	virtual void SetSize(float _x, float _y);

	void Update();
	

	bool m_bOpen = false;
private:
	sf::Texture m_OpenLeftTex;
	sf::Texture m_OpenRightTex;
};

#endif

