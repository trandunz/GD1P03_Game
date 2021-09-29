#pragma once
#ifndef  _SPAWNER_H
#define _SPAWNER_H


#include <SFML/Graphics.hpp>
#include <list>
#include "CParticleSystem.h"
#include "Slime.h"
#include "Zombie.h"
#include "WorldManager.h"

class Spawner
{
public:
	Spawner(CAudioManager* _audioManager, sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CPlayer*_player, CEnemy::ENEMYTYPE _type, sf::Shader* _shader, sf::Shader* _tourchShader, CWorldManager* _worldManager, bool _sprite = true);
	
	void Start();
	void Update(CWorldManager* _worldManager);
	void Render();

	void LoosePlayer();
	void SetPlayer(CPlayer* _player);

	void SetSpawnCount(int _amount);
	int GetSpawnCount();

	void ToggleSpawning();

	void KillAllChilderan();

	~Spawner();

	std::list<Slime> m_Slimes;
	std::list<Zombie> m_Zombies;

	sf::Sprite m_Shape;
private:
	sf::Shader* m_Shader;
	sf::Shader* m_TourchShader;

	CAudioManager* m_AudioManager;

	sf::Texture* m_Texture;

	CEnemy::ENEMYTYPE m_Type;

	bool m_bSpawn = false;

	int m_SpawnCount = 0;

	float m_SpawnFrequency = 4.0f;

	sf::RenderWindow* m_RenderWindow;
	b2World* m_World;
	CTextureMaster* m_TextureMaster;
	CPlayer* m_Player;

	float m_Scale = 50.0f;

	sf::Clock* m_SpawnTimer;

	Slime* m_Slimeptr;
	Zombie* m_Zombieptr;

	sf::Clock m_DeathParticleTimer;

	CParticleSystem* m_DeathParticles;

	int m_iBossCount = 0;

	CWorldManager* m_WorldManager;
};
#endif
