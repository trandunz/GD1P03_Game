//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CButtons.h
// Description : CButtons Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once
#ifndef _BUTTONS_H__
#define _BUTTONS_H__

// Non-Local Includes
#include <SFML/Graphics.hpp>

class CButtons
{
public:
	enum STATE
	{
		Idle,
		Hover,
		Click,
	};

	bool m_bIsHovering = false;
	bool m_bIsPressed = false;
	bool m_bIsClicked = false;

	std::string m_Label;
	std::string m_OnClickLabel;
	std::string m_HoverLabel;

	sf::Sprite Sprite;

	sf::Text m_tLabel;

	CButtons();
	CButtons(sf::RenderWindow* _renderWindow);
	~CButtons();

	void Update();
	void Render();

	void RenderOnlyLabel();

	void SetMousePosition(sf::Vector2f _position);
	sf::Vector2f GetMousePosition();

	void SetPosition(float _newX, float _newY);
	sf::Vector2f GetPosition();

	void SetLabel(std::string _newLabel);
	void SetLabel(std::string _newLabel, float _fontSize);
	 
	void SetState(std::string type);

	bool bIsinBounds(sf::Vector2f _vMousePosition);

	void SetHoverTex(sf::Texture _newTexture);
	void SetIdleTex(sf::Texture _newTexture);
	void SetClickTex(sf::Texture _newTexture);

	void SetFontSize(float _size);

	inline sf::Texture GetHoverText()
	{
		return m_HoverTexture;
	}
	inline sf::Texture GetClickText()
	{
		return m_ClickTexture;
	}

	int GetHeight();
	int GetWidth();

	inline void SetSpritePos(sf::Vector2f _pos) { Sprite.setPosition(_pos); };
	inline void SetSpritePos(float _x, float _y) { Sprite.setPosition(_x, _y); };

private:
	sf::RenderWindow* m_RenderWindow;

	sf::Texture m_IdleTexture;
	sf::Texture m_HoverTexture;
	sf::Texture m_ClickTexture;

	sf::Font m_Font;

	sf::Vector2f m_vMousePosition;
};
#endif
