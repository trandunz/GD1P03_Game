//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CPickaxe.h
// Description : CPickaxe Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once
#ifndef _PICK_H__
#define _PICK_H__

#include "Block.h"
class CPickaxe : public CBlock
{
public:

	CPickaxe(PICKAXETYPE _pickType = PICKAXETYPE::BASIC);
	CPickaxe(sf::RenderWindow* _renderWindow, const float& _scale, float _posX, float _posY, PICKAXETYPE _pickType = PICKAXETYPE::BASIC);
	virtual ~CPickaxe();

	void FlipSprite(sf::Vector2f _playerPosition, sf::Sprite& _sprite, sf::Texture* _playerLeft, sf::Texture* _playerRight);

	void Render();

	float m_PickaxePower = 1.0f;

	sf::Sprite* m_Sprite;
private:
	void LoadAppropriateItemTexture();
	void LoadAppropriateWorldTextures();

	void InitializeSpriteTextures();

	sf::Texture m_LeftTex;
	sf::Texture m_RightTex;


};
#endif

