#pragma once
#ifndef _AUDIOMANAGER_H__
#define _AUDIOMANAGER_H__

#include <SFML/Audio.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>


 class CAudioManager
{
public:
	CAudioManager();
	~CAudioManager();

	static void Start();
	static void Update();

	void PlayRunning();
	void PlayBlockPlace();
	void PlayGroundMine();
	void PlayMusic();

	void PlayPlayerDamage();
	void PlayPlayerDeath();

	void PlaySlimeDamage(int _volume = 10);
	void PlaySlimeDeath(int _volume = 10);

	void PlayBowShot();

	void PlayKingSlimeSpawn();

	void CheckBackgroundMusic(sf::Vector2f _positionPlayer);



private:
	sf::SoundBuffer m_Buffer;
	sf::Sound m_ActiveSound;

	sf::Sound m_PlayerDeathSound;
	sf::Sound m_PlayerDamageSound;

	sf::Sound m_SlimeDeathSound;
	sf::Sound m_SlimeDamageSound;

	sf::Sound m_KingSlimeSpawnSound;

	sf::Sound m_BowShotSound;

	sf::Music m_Music;

	sf::SoundBuffer m_MineBuffer1;
	sf::SoundBuffer m_MineBuffer2;
	sf::SoundBuffer m_MineBuffer3;
	sf::SoundBuffer m_MineBuffer4;

	sf::SoundBuffer m_PlayerHurt1;
	sf::SoundBuffer m_PlayerHurt2;
	sf::SoundBuffer m_PlayerHurt3;

	sf::SoundBuffer m_SlimeHurt1;
	sf::SoundBuffer m_SlimeHurt2;

	sf::SoundBuffer m_DeathBuffer;
	sf::SoundBuffer m_SlimeDeathBuffer;

	sf::SoundBuffer m_KingSlimeSpawnBuffer;

	sf::SoundBuffer m_BowBuffer;

	sf::SoundBuffer MineSoundBuffers[4]; 
	sf::SoundBuffer PlayerHurtBuffers[3];
	sf::SoundBuffer SlimeHurtBuffers[2];
};

#endif

