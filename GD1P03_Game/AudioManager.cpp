#include "AudioManager.h"

CAudioManager::CAudioManager()
{
	srand(time(NULL));

	m_MineBuffer1.loadFromFile("Sounds/GroundMine.wav");
	m_MineBuffer2.loadFromFile("Sounds/GroundMine2.wav");
	m_MineBuffer3.loadFromFile("Sounds/GroundMine3.wav");
	m_MineBuffer4.loadFromFile("Sounds/GroundMine4.wav");

	MineSoundBuffers[0] = m_MineBuffer1;
	MineSoundBuffers[1] = m_MineBuffer2;
	MineSoundBuffers[2] = m_MineBuffer3;
	MineSoundBuffers[3] = m_MineBuffer4;
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
	m_ActiveSound.setBuffer(MineSoundBuffers[1 + rand() % 2]);
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
