#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <list>

#include "Chest.h"
#include "Door.h"

class CPlayer
{
public:
	CPlayer(sf::RenderWindow* _renderWindow, b2World& m_World, const float& _scale);
	~CPlayer();

	void Start();
	void Update(sf::Vector2f _mousePos);
	void Render();

	void Movement(sf::Event& _event);
	void ResetSpritePos();
	
	void PlaceBlocks(std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite);
	
	b2Body* GetBody();
	sf::Sprite GetShape();
	
	int GetCurrentHP();
	int GetMaxHP();

	 
	std::list<CBlock> m_Inventory;

	sf::Sprite m_MapIcon;

private:
	// Essentials
	sf::RenderWindow* m_RenderWindow;
	b2World* m_World;
	float m_Scale;

	// Mouse
	sf::Vector2f MousePos;

	// Player
	int m_MaxHP = 100;
	int m_HPRegenRate_s = 1;

	int m_InteractionRange = 10;


	int m_Health = 100;
	const int m_MoveSpeed = 3;
	
	b2Vec2 m_Velocity;

	// Textures
	sf::Sprite m_Shape;
	sf::Texture* m_PlayerRightTex;
	sf::Texture* m_PlayerLeftTex;
	sf::Texture* m_MapIconText;

	// Box2d
	b2BodyDef m_BodyDef;
	b2Body* m_Body;
	b2PolygonShape m_b2pShape;
	b2FixtureDef m_FixtureDef;
	b2Filter m_PlayerFilter;

	// Temp Block Pointer (Used To Create New Blocks During Block Placement)
	CBlock* m_Block;

	
};

