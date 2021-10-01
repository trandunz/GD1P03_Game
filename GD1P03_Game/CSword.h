#pragma once
#ifndef _SWORD_H__
#define _SWORD_H__

#include "Block.h"

class CSword : public CBlock
{
public:
	CSword(SWORDTYPE _swordType = SWORDTYPE::WOOD);
	CSword(sf::RenderWindow* _renderWindow, const float& _scale, float _posX, float _posY, SWORDTYPE _swordType = SWORDTYPE::WOOD);
	virtual ~CSword();

	void FlipSprite(sf::Vector2f _playerPosition, sf::Sprite& _sprite, sf::Texture* _playerLeft, sf::Texture* _playerRight);

	void CreateSwordTrigger();

	void Update();
	void Render();

	float m_SwordPower = 1.0f;

	sf::Sprite* m_Sprite;
private:
	sf::Texture m_LeftTex;
	sf::Texture m_RightTex;

};
#endif
