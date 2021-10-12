#include "AudioManager.h"

/// <summary>
/// CAudioManager Constructor
/// </summary>
CAudioManager::CAudioManager()
{
	std::cout << "AUDIOMANAGER" << std::endl;
	srand(time(NULL));

	LoadMineBuffers();

	LoadPlayerHurtBuffers();

	LoadSlimeHurtBuffers();

	LoadDeathBuffers();

	m_BowBuffer.loadFromFile("Sounds/Bow.wav");

	m_PickupSoundBuffer.loadFromFile("Sounds/PickupItem.wav");

	m_KingSlimeSpawnBuffer.loadFromFile("Sounds/SlimeKingSpawn.wav");

	m_PotionDrinkBuffer.loadFromFile("Sounds/PotionDrink.wav");

	m_GunShotBuffer.loadFromFile("Sounds/GunShot.wav");

	InitBufferArrays();
}

/// <summary>
/// Players the player running sound
/// </summary>
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

/// <summary>
/// Plays Block Placing Sound
/// </summary>
void CAudioManager::PlayBlockPlace()
{
	m_Buffer.loadFromFile("Sounds/PlaceBlock.wav");
	m_ActiveSound.setBuffer(m_Buffer);
	m_ActiveSound.setVolume(25.0f);
	m_ActiveSound.play();
}

/// <summary>
/// Plays Ground Mine Sound
/// </summary>
void CAudioManager::PlayGroundMine()
{
	m_ActiveSound.setBuffer(MineSoundBuffers[1 + rand() % 2]);
	m_ActiveSound.setVolume(25.0f);
	m_ActiveSound.play();
}

/// <summary>
/// Plays Player Damage Sound
/// </summary>
void CAudioManager::PlayPlayerDamage()
{
	m_PlayerDamageSound.setBuffer(PlayerHurtBuffers[rand() % 3]);
	m_PlayerDamageSound.setVolume(25.0f);
	m_PlayerDamageSound.play();
}

/// <summary>
/// Plays Slime Damage Sound
/// </summary>
/// <param name="_volume"></param>
void CAudioManager::PlaySlimeDamage(int _volume)
{
	m_SlimeDamageSound.setBuffer(SlimeHurtBuffers[1]);
	m_SlimeDamageSound.setVolume(_volume);
	m_SlimeDamageSound.play();
}

/// <summary>
/// Plays Slime Death Sound
/// </summary>
/// <param name="_volume"></param>
void CAudioManager::PlaySlimeDeath(int _volume)
{
	m_SlimeDeathSound.setBuffer(m_SlimeDeathBuffer);
	m_SlimeDeathSound.setVolume(_volume);
	m_SlimeDeathSound.play();
}

/// <summary>
/// Plays Player Death Sound
/// </summary>
void CAudioManager::PlayPlayerDeath()
{
	m_PlayerDeathSound.setBuffer(m_DeathBuffer);
	m_PlayerDeathSound.setVolume(25.0f);
	m_PlayerDeathSound.play();
}

/// <summary>
/// Plays Pickup Item Sound
/// </summary>
void CAudioManager::PlayPickupSound()
{
	m_PickupSound.setBuffer(m_PickupSoundBuffer);
	m_PickupSound.setVolume(10.0f);
	m_PickupSound.play();
}

/// <summary>
/// Plays Plains Music
/// </summary>
void CAudioManager::PlayMusic()
{
	if (m_Music.getStatus() != m_Music.Playing)
	{
		m_MusicSand.pause();
		m_MusicIce.pause();
		m_MusicHell.pause();
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

/// <summary>
/// Plays Sand Music
/// </summary>
void CAudioManager::PlayMusicSand()
{
	if (m_MusicSand.getStatus() != m_MusicSand.Playing)
	{
		m_Music.pause();
		m_MusicIce.pause();
		m_MusicHell.pause();
		m_MusicSand.openFromFile("Music/TerrariaDaySand.wav");
		m_MusicSand.setLoop(true);
		m_MusicSand.play();
	}

	if (m_MusicLevel == 1)
	{
		m_MusicSand.setVolume(0.f);
	}
	else if (m_MusicLevel == 2)
	{
		m_MusicSand.setVolume(2.5f);
	}
	else if (m_MusicLevel == 3)
	{
		m_MusicSand.setVolume(5.0f);
	}
	else if (m_MusicLevel == 4)
	{
		m_MusicSand.setVolume(7.5f);
	}
	else
	{
		m_MusicSand.setVolume(10.0f);
	}
}

/// <summary>
/// Plays Ice Music
/// </summary>
void CAudioManager::PlayMusicIce()
{
	if (m_MusicIce.getStatus() != m_MusicIce.Playing)
	{
		m_Music.pause();
		m_MusicHell.pause();
		m_MusicSand.pause();
		m_MusicIce.openFromFile("Music/TerrariaDayIce.wav");
		m_MusicIce.setLoop(true);
		m_MusicIce.play();
	}

	if (m_MusicLevel == 1)
	{
		m_MusicIce.setVolume(0.f);
	}
	else if (m_MusicLevel == 2)
	{
		m_MusicIce.setVolume(2.5f);
	}
	else if (m_MusicLevel == 3)
	{
		m_MusicIce.setVolume(5.0f);
	}
	else if (m_MusicLevel == 4)
	{
		m_MusicIce.setVolume(7.5f);
	}
	else
	{
		m_MusicIce.setVolume(10.0f);
	}
}

/// <summary>
/// Plays Hell Music
/// </summary>
void CAudioManager::PlayMusicHell()
{
	if (m_MusicHell.getStatus() != m_MusicHell.Playing)
	{
		m_Music.pause();
		m_MusicSand.pause();
		m_MusicIce.pause();
		m_MusicHell.openFromFile("Music/TerrariaDayHell.wav");
		m_MusicHell.setLoop(true);
		m_MusicHell.play();
	}

	if (m_MusicLevel == 1)
	{
		m_MusicHell.setVolume(0.f);
	}
	else if (m_MusicLevel == 2)
	{
		m_MusicHell.setVolume(2.5f);
	}
	else if (m_MusicLevel == 3)
	{
		m_MusicHell.setVolume(5.0f);
	}
	else if (m_MusicLevel == 4)
	{
		m_MusicHell.setVolume(7.5f);
	}
	else
	{
		m_MusicHell.setVolume(10.0f);
	}
}

/// <summary>
/// Plays Plains Underground Music
/// </summary>
void CAudioManager::PlayUnderGroundMusic()
{
	if (m_UnderGroundMusic.getStatus() != m_UnderGroundMusic.Playing)
	{
		m_UnderGroundMusicSand.pause();
		m_UnderGroundMusicIce.pause();
		m_UnderGroundMusicHell.pause();
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

/// <summary>
/// Play Sand Undergroung Music
/// </summary>
void CAudioManager::PlayUnderGroundMusicSand()
{
	if (m_UnderGroundMusicSand.getStatus() != m_UnderGroundMusicSand.Playing)
	{
		m_UnderGroundMusic.pause();
		m_UnderGroundMusicIce.pause();
		m_UnderGroundMusicHell.pause();
		m_UnderGroundMusicSand.openFromFile("Music/TerrariaUnderGroundSand.wav");
		m_UnderGroundMusicSand.setLoop(true);
		m_UnderGroundMusicSand.play();
	}

	if (m_MusicLevel == 1)
	{
		m_UnderGroundMusicSand.setVolume(10.0f);
	}
	else if (m_MusicLevel == 2)
	{
		m_UnderGroundMusicSand.setVolume(7.5f);
	}
	else if (m_MusicLevel == 3)
	{
		m_UnderGroundMusicSand.setVolume(5.0f);
	}
	else if (m_MusicLevel == 4)
	{
		m_UnderGroundMusicSand.setVolume(2.5f);
	}
	else
	{
		m_UnderGroundMusicSand.setVolume(0.0f);
	}
}

/// <summary>
/// Play Ice Underground Music
/// </summary>
void CAudioManager::PlayUnderGroundMusicIce()
{
	if (m_UnderGroundMusicIce.getStatus() != m_UnderGroundMusicIce.Playing)
	{
		m_UnderGroundMusicSand.pause();
		m_UnderGroundMusic.pause();
		m_UnderGroundMusicHell.pause();
		m_UnderGroundMusicIce.openFromFile("Music/TerrariaUnderGroundIce.wav");
		m_UnderGroundMusicIce.setLoop(true);
		m_UnderGroundMusicIce.play();
	}

	if (m_MusicLevel == 1)
	{
		m_UnderGroundMusicIce.setVolume(10.0f);
	}
	else if (m_MusicLevel == 2)
	{
		m_UnderGroundMusicIce.setVolume(7.5f);
	}
	else if (m_MusicLevel == 3)
	{
		m_UnderGroundMusicIce.setVolume(5.0f);
	}
	else if (m_MusicLevel == 4)
	{
		m_UnderGroundMusicIce.setVolume(2.5f);
	}
	else
	{
		m_UnderGroundMusicIce.setVolume(0.0f);
	}
}

/// <summary>
/// Play Hell Underground Music
/// </summary>
void CAudioManager::PlayUnderGroundMusicHell()
{
	if (m_UnderGroundMusicHell.getStatus() != m_UnderGroundMusicHell.Playing)
	{
		m_UnderGroundMusicSand.pause();
		m_UnderGroundMusic.pause();
		m_UnderGroundMusicIce.pause();
		m_UnderGroundMusicHell.openFromFile("Music/TerrariaUnderGroundHell.wav");
		m_UnderGroundMusicHell.setLoop(true);
		m_UnderGroundMusicHell.play();
	}

	if (m_MusicLevel == 1)
	{
		m_UnderGroundMusicHell.setVolume(10.0f);
	}
	else if (m_MusicLevel == 2)
	{
		m_UnderGroundMusicHell.setVolume(7.5f);
	}
	else if (m_MusicLevel == 3)
	{
		m_UnderGroundMusicHell.setVolume(5.0f);
	}
	else if (m_MusicLevel == 4)
	{
		m_UnderGroundMusicHell.setVolume(2.5f);
	}
	else
	{
		m_UnderGroundMusicHell.setVolume(0.0f);
	}
}

/// <summary>
/// Play Bow Shot Sound
/// </summary>
void CAudioManager::PlayBowShot()
{
	m_BowShotSound.setBuffer(m_BowBuffer);
	m_BowShotSound.setVolume(30.0f);
	m_BowShotSound.play();
}

/// <summary>
/// Play King slime spawn sound
/// </summary>
void CAudioManager::PlayKingSlimeSpawn()
{
	m_KingSlimeSpawnSound.setBuffer(m_KingSlimeSpawnBuffer);
	m_KingSlimeSpawnSound.setVolume(100.0f);
	m_KingSlimeSpawnSound.setPitch(0.5);
	m_KingSlimeSpawnSound.play();
}

/// <summary>
/// Play Potion Drink Sound
/// </summary>
void CAudioManager::PlayPotionDrink()
{
	m_PotionDrinkSound.setBuffer(m_PotionDrinkBuffer);
	m_PotionDrinkSound.setVolume(30.0f);
	m_PotionDrinkSound.play();
}

// Play Gunshot Sound
void CAudioManager::PlayGunShot()
{
	m_GunShotSound.setBuffer(m_GunShotBuffer);
	m_GunShotSound.setVolume(30.0f);
	m_GunShotSound.play();
}

// Play Mine buffers
void CAudioManager::LoadMineBuffers()
{
	m_MineBuffer1.loadFromFile("Sounds/GroundMine.wav");
	m_MineBuffer2.loadFromFile("Sounds/GroundMine2.wav");
	m_MineBuffer3.loadFromFile("Sounds/GroundMine3.wav");
	m_MineBuffer4.loadFromFile("Sounds/GroundMine4.wav");
}

/// <summary>
/// Loads player hurt buffers
/// </summary>
void CAudioManager::LoadPlayerHurtBuffers()
{
	m_PlayerHurt1.loadFromFile("Sounds/PlayerHit1.wav");
	m_PlayerHurt2.loadFromFile("Sounds/PlayerHit2.wav");
	m_PlayerHurt3.loadFromFile("Sounds/PlayerHit3.wav");
}

/// <summary>
/// Loads Slime hurt buffers
/// </summary>
void CAudioManager::LoadSlimeHurtBuffers()
{
	m_SlimeHurt1.loadFromFile("Sounds/SlimeHit.wav");
	m_SlimeHurt2.loadFromFile("Sounds/SlimeHit2.wav");
}

/// <summary>
/// Load death buffers
/// </summary>
void CAudioManager::LoadDeathBuffers()
{
	m_DeathBuffer.loadFromFile("Sounds/Death.wav");
	m_SlimeDeathBuffer.loadFromFile("Sounds/SlimeDeath.wav");
}

/// <summary>
/// Initializes all buffer arrays with appropriate buffers
/// </summary>
void CAudioManager::InitBufferArrays()
{
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
