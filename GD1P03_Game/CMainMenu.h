//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CMainMenu.h
// Description : CMainMenu Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once
#ifndef _MAINMENU_H__
#define _MAINMENU_H__

// Non-Local Includes
#include <SFML/Graphics.hpp>

// Local Includes
#include "CButtons.h"

class CMainMenu : public sf::Drawable, sf::Transformable
{
public:
	CMainMenu(sf::RenderWindow* _renderWindow, sf::Font& _font);
	~CMainMenu();
	void Update(sf::Event& _event);

	CButtons* m_Play;
	CButtons* m_Options;
	CButtons* m_Exit;

private:
	void  Start();

	virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;

	void LoadImageOntoSprite(sf::Sprite& _sprite, sf::Texture& _texture, std::string _filePath);

	void CreateBackGroundImage();
	void CreateTitle();

	void RainbowTitle();
	void ParralaxBackground();

	void CreateButtons();

	void ButtonUpdates();

	void RenderButtons(sf::RenderTarget& _target, sf::RenderStates _states) const;

	void InitButtonTextures(std::string _texture1, std::string _texture2);

	sf::Text m_Title;
	sf::Text m_Credit;

	sf::RenderWindow* m_RenderWindow;

	sf::Texture m_MainMenuTexture;
	sf::Texture m_MainMenuButtonTexture;
	sf::Texture m_MainMenuButtonTexture_Hover;

	sf::Sprite m_MainMenuImage;

	sf::Font* m_Font;

	std::string m_TitleString;

	sf::Clock m_RainbowClock;
	sf::Clock m_ParralaxClock;

	sf::Vector2f m_MousePos;
};
#endif 

