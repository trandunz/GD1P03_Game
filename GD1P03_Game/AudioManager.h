#pragma once
#ifndef _AUDIOMANAGER_H__
#define _AUDIOMANAGER_H__

#include <SFML/Audio.hpp>



class CAudioManager
{
public:
	CAudioManager();
	~CAudioManager();

	void Start();
	void Update();

	void PlayMusic();

private:
	sf::SoundBuffer m_Buffer;
	sf::Sound m_ActiveSound;

	sf::Music m_Music;

};

#endif
