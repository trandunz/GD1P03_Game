#pragma once
#ifndef _AUDIOMANAGER_H__
#define _AUDIOMANAGER_H__

#include <SFML/Audio.hpp>
#include <iostream>


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

	 sf::SoundBuffer m_Buffer;
	 sf::Sound m_ActiveSound;

	 sf::Music m_Music;

private:
	

};

#endif

