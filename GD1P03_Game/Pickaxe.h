#pragma once
#ifndef _PICK_H__
#define _PICK_H__

#include "Block.h"
class CPickaxe : public CBlock
{
public:
	CPickaxe();
	CPickaxe(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY);
	virtual ~CPickaxe();

	void FlipSprite(sf::Vector2f _playerPosition, sf::Sprite& _sprite, sf::Texture* _playerLeft, sf::Texture* _playerRight);

	void Update();
	void Render();


	bool m_bOpen = false;

	float m_PickaxePower = 1.2f;

	sf::Sprite* m_Sprite;
private:
	sf::Texture m_LeftTex;
	sf::Texture m_RightTex;

	
};
#endif

