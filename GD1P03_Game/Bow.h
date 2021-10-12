#pragma once
#ifndef _BOW_H__
#define _BOW_H__

#include "Block.h"
class Bow : public CBlock
{
public:

	Bow(BOWTYPE _bowType);
	Bow(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, float _posX, float _posY, BOWTYPE _bowtype);
	virtual ~Bow();

	void FlipSprite(sf::Vector2f _playerPosition, sf::Sprite& _sprite, sf::Texture* _playerLeft, sf::Texture* _playerRight);

	void Update();
	void Render();

	sf::Sprite* m_Sprite;
	int m_BowPower = 1.0f;
private:
	void LoadTextureBasedOnType();
	void LoadTexturesAndBPBasedOnType();
	void InitSpriteTexture();

	sf::Texture m_LeftTex;
	sf::Texture m_RightTex;
};
#endif

