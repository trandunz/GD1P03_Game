//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : Spawner.h
// Description : Spawner Header file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#pragma once
#ifndef  _SPAWNER_H__
#define _SPAWNER_H__


#include <SFML/Graphics.hpp>
#include <list>
#include "CParticleSystem.h"
#include "Slime.h"
#include "CSnowman.h"

class Spawner
{
public:
	Spawner(CAudioManager* _audioManager, sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CPlayer*_player, CEnemy::ENEMYTYPE _type, sf::Shader* _shader, sf::Shader* _tourchShader, bool _sprite = true);
	
	void Start();
	void Update(bool& m_PlayerHitByProjectile);
	void Render(sf::Shader* _tourchshader, bool _isInRangeOfLightSource);

	void LoosePlayer();
	void SetPlayer(CPlayer* _player);

	void SetSpawnCount(int _amount);
	int GetSpawnCount();

	void ToggleSpawning();

	void KillAllChilderan();

	~Spawner();

	std::list<Slime> m_Slimes;
	std::list<CSnowman> m_Snowmans;

	sf::Sprite m_Shape;


	bool m_bCanSpawnBoss = false;
private:
	void AssignAppropiateParticleSystem();
	void TypeSpecificUpdate();
	void UpdateSlimes();
	void UpdateSnowmen(bool& m_PlayerHitByProjectile);
	void UpdateCactus(bool& m_PlayerHitByProjectile);

	void CheckForDestroyedSlimes();
	void CheckForDestroyedSnowmen();
	void CheckForDestroyedCactus();

	void TypeSpecificRender(sf::Shader* _tourchshader, bool _isInRangeOfLightSource);

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

	sf::Clock m_SpawnTimer;

	Slime* m_Slimeptr;
	CSnowman* m_Snowmanptr;

	sf::Clock m_DeathParticleTimer;

	CParticleSystem* m_DeathParticles;

	int m_iBossCount = 0;

	sf::Clock m_DropTimer;
};
#endif
