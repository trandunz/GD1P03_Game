#include "AudioManager.h"

CAudioManager::CAudioManager()
{
	srand(time(NULL));

	m_MineBuffer1.loadFromFile("Sounds/GroundMine.wav");
	m_MineBuffer2.loadFromFile("Sounds/GroundMine2.wav");
	m_MineBuffer3.loadFromFile("Sounds/GroundMine3.wav");
	m_MineBuffer4.loadFromFile("Sounds/GroundMine4.wav");

	m_PlayerHurt1.loadFromFile("Sounds/PlayerHit1.wav");
	m_PlayerHurt2.loadFromFile("Sounds/PlayerHit2.wav");
	m_PlayerHurt3.loadFromFile("Sounds/PlayerHit3.wav");

	m_SlimeHurt1.loadFromFile("Sounds/SlimeHit.wav");
	m_SlimeHurt2.loadFromFile("Sounds/SlimeHit2.wav");

	m_DeathBuffer.loadFromFile("Sounds/Death.wav");
	m_SlimeDeathBuffer.loadFromFile("Sounds/SlimeDeath.wav");

	m_BowBuffer.loadFromFile("Sounds/Bow.wav");

	m_PickupSoundBuffer.loadFromFile("Sounds/PickupItem.wav");

	m_KingSlimeSpawnBuffer.loadFromFile("Sounds/SlimeKingSpawn.wav");
	m_PotionDrinkBuffer.loadFromFile("Sounds/PotionDrink.wav");

	MineSoundBuffers[0] = m_MineBuffer1;
	MineSoundBuffers[1] = m_MineBuffer2;
	MineSoundBuffers[2] = m_MineBuffer3;
	MineSoundBuffers[3] = m_MineBuffer4;

	PlayerHurtBuffers[0] = m_PlayerHurt1;
	PlayerHurtBuffers[1] = m_PlayerHurt2;
	PlayerHurtBuffers[2] = m_PlayerHurt3;

	SlimeHurtBuffers[0] = m_SlimeHurt1;
	SlimeHurtBuffers[1] = m_SlimeHurt2;
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

void CAudioManager::PlayPlayerDamage()
{
	m_PlayerDamageSound.setBuffer(PlayerHurtBuffers[rand() % 3]);
	m_PlayerDamageSound.setVolume(25.0f);
	m_PlayerDamageSound.play();
}

void CAudioManager::PlaySlimeDamage(int _volume)
{
	m_SlimeDamageSound.setBuffer(SlimeHurtBuffers[1]);
	m_SlimeDamageSound.setVolume(_volume);
	m_SlimeDamageSound.play();
}

void CAudioManager::PlaySlimeDeath(int _volume)
{
	m_SlimeDeathSound.setBuffer(m_SlimeDeathBuffer);
	m_SlimeDeathSound.setVolume(_volume);
	m_SlimeDeathSound.play();
}

void CAudioManager::PlayPlayerDeath()
{
	m_PlayerDeathSound.setBuffer(m_DeathBuffer);
	m_PlayerDeathSound.setVolume(25.0f);
	m_PlayerDeathSound.play();
}

void CAudioManager::PlayPickupSound()
{
	m_PickupSound.setBuffer(m_PickupSoundBuffer);
	m_PickupSound.setVolume(10.0f);
	m_PickupSound.play();
}

void CAudioManager::PlayMusic()
{
	if (m_Music.getStatus() != m_Music.Playing)
	{
		m_Music.openFromFile("Music/TerrariaDay.wav");
		m_Music.setLoop(true);
		m_Music.play();
	}
	
	if (m_MusicLevel == 1)
	{
		m_Music.setVolume(0.f);
	}
	else if (m_MusicLevel == 2)
	{
		m_Music.setVolume(2.5f);
	}
	else if (m_MusicLevel == 3)
	{
		m_Music.setVolume(5.0f);
	}
	else if (m_MusicLevel == 4)
	{
		m_Music.setVolume(7.5f);
	}
	else
	{
		m_Music.setVolume(10.0f);
	}
}

void CAudioManager::PlayUnderGroundMusic()
{
	if (m_UnderGroundMusic.getStatus() != m_UnderGroundMusic.Playing)
	{
		m_UnderGroundMusic.openFromFile("Music/TerrariaUnderGround.wav");
		m_UnderGroundMusic.setLoop(true);
		m_UnderGroundMusic.play();
	}

	if (m_MusicLevel == 1)
	{
		m_UnderGroundMusic.setVolume(10.0f);
	}
	else if (m_MusicLevel == 2)
	{
		m_UnderGroundMusic.setVolume(7.5f);
	}
	else if (m_MusicLevel == 3)
	{
		m_UnderGroundMusic.setVolume(5.0f);
	}
	else if (m_MusicLevel == 4)
	{
		m_UnderGroundMusic.setVolume(2.5f);
	}
	else
	{
		m_UnderGroundMusic.setVolume(0.0f);
	}
}

void CAudioManager::PlayBowShot()
{
	m_BowShotSound.setBuffer(m_BowBuffer);
	m_BowShotSound.setVolume(30.0f);
	m_BowShotSound.play();
}

void CAudioManager::PlayKingSlimeSpawn()
{
	m_KingSlimeSpawnSound.setBuffer(m_KingSlimeSpawnBuffer);
	m_KingSlimeSpawnSound.setVolume(100.0f);
	m_KingSlimeSpawnSound.setPitch(0.5);
	m_KingSlimeSpawnSound.play();
}

void CAudioManager::CheckBackgroundMusic(sf::Vector2f _positionPlayer)
{
}

void CAudioManager::PlayPotionDrink()
{
	m_PotionDrinkSound.setBuffer(m_PotionDrinkBuffer);
	m_PotionDrinkSound.setVolume(30.0f);
	m_PotionDrinkSound.play();
}
