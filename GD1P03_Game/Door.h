//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CDoor.h
// Description : CDoor Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

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
	virtual void SetSizeAndPos(float _currentPosX, float _currentPosY, float _x, float _y);

	bool m_bOpen = false;
private:
	sf::Texture m_OpenLeftTex;
	sf::Texture m_OpenRightTex;
};

#endif

