#pragma once
#ifndef _PROJECTILE_H__
#define _PROJECTILE_H__

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "Block.h"
#include "Bow.h"

class CProjectile : public CBlock
{
public:
	CProjectile(CBlock::PROJECTILETYPE _projtype = CBlock::PROJECTILETYPE::ARROW);
	CProjectile(b2World& _world, float _startPosX, float _startPosY, sf::Vector2f _mousPos, CBlock::PROJECTILETYPE _projtype = CBlock::PROJECTILETYPE::ARROW, Bow* _activeBow = nullptr, bool _friendly = true);
	~CProjectile();

	void Update();
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
