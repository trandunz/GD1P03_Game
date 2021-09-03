#include "AudioManager.h"

CAudioManager::CAudioManager()
{
}

CAudioManager::~CAudioManager()
{
}

void CAudioManager::Start()
{
	
	
}

void CAudioManager::Update()
{
}

void CAudioManager::PlayMusic()
{
	m_Music.openFromFile("Music/TerrariaDay.wav");
	m_Music.setLoop(true);
	m_Music.setVolume(10.0f);
	m_Music.play();
}
