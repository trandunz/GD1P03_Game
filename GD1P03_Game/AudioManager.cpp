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

void CAudioManager::PlayRunning()
{
	if (m_ActiveSound.getStatus() == sf::Sound::Status::Stopped)
	{
		if (m_Buffer.loadFromFile("Sounds/Run.wav"))
		{
			//std::cout << "SOund Loaded!" << std::endl;
		}
		else
		{
			//std::cout << "SOund Loaded!" << std::endl;
		}
		m_ActiveSound.setBuffer(m_Buffer);
		m_ActiveSound.setVolume(25.0f);
		m_ActiveSound.play();
	}
	
}

void CAudioManager::PlayBlockPlace()
{
	m_Buffer.loadFromFile("Sounds/PlaceBlock.wav");
	m_ActiveSound.setBuffer(m_Buffer);
	m_ActiveSound.setVolume(25.0f);
	m_ActiveSound.play();
}

void CAudioManager::PlayGroundMine()
{
	m_Buffer.loadFromFile("Sounds/GroundMine.wav");
	m_ActiveSound.setBuffer(m_Buffer);
	m_ActiveSound.setVolume(25.0f);
	m_ActiveSound.play();
}

void CAudioManager::PlayMusic()
{
	m_Music.openFromFile("Music/TerrariaDay.wav");
	m_Music.setLoop(true);
	m_Music.setVolume(10.0f);
	m_Music.play();
}
