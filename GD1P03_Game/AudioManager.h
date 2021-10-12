#pragma once
#ifndef _AUDIOMANAGER_H__
#define _AUDIOMANAGER_H__

// Non-Local Includes
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

 class CAudioManager
{
public:
	CAudioManager();

	void PlayRunning();
	void PlayBlockPlace();
	void PlayGroundMine();
	void PlayMusic();
	void PlayMusicSand();
	void PlayMusicIce();
	void PlayMusicHell();
	void PlayUnderGroundMusic();
	void PlayUnderGroundMusicSand();
	void PlayUnderGroundMusicIce();
	void PlayUnderGroundMusicHell();

	void PlayPlayerDamage();
	void PlayPlayerDeath();

	void PlayPickupSound();

	void PlaySlimeDamage(int _volume = 10);
	void PlaySlimeDeath(int _volume = 10);

	void PlayBowShot();

	void PlayKingSlimeSpawn();

	void PlayPotionDrink();

	void PlayGunShot();

	int m_MusicLevel = 1;

private:
	void LoadMineBuffers();
	void LoadPlayerHurtBuffers();
	void LoadSlimeHurtBuffers();
	void LoadDeathBuffers();

	void InitBufferArrays();

	sf::SoundBuffer m_Buffer;
	sf::Sound m_ActiveSound;

	sf::Sound m_PlayerDeathSound;
	sf::Sound m_PlayerDamageSound;

	sf::Sound m_SlimeDeathSound;
	sf::Sound m_SlimeDamageSound;

	sf::Sound m_KingSlimeSpawnSound;

	sf::Sound m_BowShotSound;

	sf::Music m_Music;
	sf::Music m_UnderGroundMusic;

	sf::Music m_MusicSand;
	sf::Music m_UnderGroundMusicSand;

	sf::Music m_MusicIce;
	sf::Music m_UnderGroundMusicIce;

	sf::Music m_MusicHell;
	sf::Music m_UnderGroundMusicHell;

	sf::Sound m_PickupSound;
	sf::SoundBuffer m_PickupSoundBuffer;

	sf::Sound m_PotionDrinkSound;
	sf::SoundBuffer m_PotionDrinkBuffer;

	sf::Sound m_GunShotSound;
	sf::SoundBuffer m_GunShotBuffer;

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

