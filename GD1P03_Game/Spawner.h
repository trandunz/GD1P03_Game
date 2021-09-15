#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "Slime.h"
#include "Zombie.h"

class Spawner
{
public:
	Spawner(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CPlayer*_player);
	
	void Start();
	void Update();
	void Render();

	void LoosePlayer();
	void SetPlayer(CPlayer* _player);

	void ToggleSpawning();

	~Spawner();

	std::list<Slime> m_Slimes;
	std::list<Zombie> m_Zombies;
private:
	bool m_bSpawn = false;

	int m_SpawnCount = 0;

	sf::RenderWindow* m_RenderWindow;
	b2World* m_World;
	CTextureMaster* m_TextureMaster;
	CPlayer* m_Player;

	float m_Scale = 50.0f;

	sf::Clock m_SpawnTimer;

	Slime* m_Slimeptr = {};
	Zombie* m_Zombieptr = {};
};
