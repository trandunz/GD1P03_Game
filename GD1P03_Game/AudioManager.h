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

	sf::SoundBuffer m_Buffer;
	sf::Sound m_ActiveSound;

	sf::Music m_Music;

private:
	
	sf::SoundBuffer m_MineBuffer1;
	sf::SoundBuffer m_MineBuffer2;
	sf::SoundBuffer m_MineBuffer3;
	sf::SoundBuffer m_MineBuffer4;

	sf::SoundBuffer m_PlayerHurt1;
	sf::SoundBuffer m_PlayerHurt2;
	sf::SoundBuffer m_PlayerHurt3;

	sf::SoundBuffer m_DeathBuffer;

	sf::SoundBuffer MineSoundBuffers[4]; 
	sf::SoundBuffer PlayerHurtBuffers[3];
};

#endif

