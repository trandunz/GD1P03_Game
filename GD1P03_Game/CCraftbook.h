#pragma once
#ifndef _CRAFTBOOK_H__
#define _CRAFTBOOK_H__

// Non-Local Includes
#include <SFML/Graphics.hpp>

class CCraftbook : public sf::Drawable, public sf::Transformable
{
public:
	CCraftbook(sf::RenderWindow* _renderWindow, sf::Font& _font);
	~CCraftbook();
	void Update(sf::Event& _event);

private:
	void  Start();

	virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;

	sf::RectangleShape m_CraftbookBG;
};
#endif

