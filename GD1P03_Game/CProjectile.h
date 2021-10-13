//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CProjectile.h
// Description : CProjectile Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//


#pragma once
#ifndef _PROJECTILE_H__
#define _PROJECTILE_H__

// Non-Local Includes
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>

// Local Includes
#include "Block.h"
#include "Bow.h"

class CProjectile : public CBlock
{
public:
	CProjectile(CBlock::PROJECTILETYPE _projtype = CBlock::PROJECTILETYPE::ARROW);
	CProjectile(b2World& _world, float _startPosX, float _startPosY, sf::Vector2f _mousPos, CBlock::PROJECTILETYPE _projtype = CBlock::PROJECTILETYPE::ARROW, Bow* _activeBow = nullptr, bool _friendly = true);
	~CProjectile();

	void Update(bool& _hitPlayer);
	void Render();

	void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);
	void DestroyBody();

	bool m_bMARKASDESTROY = false;

	float m_Weight = 0.8f;

private:
	int m_Damage = 25.0f;
	bool m_Friendly = true;

	bool m_Bullet = false;

};
#endif
