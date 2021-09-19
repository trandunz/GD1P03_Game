#include "Spawner.h"

Spawner::Spawner(CAudioManager* _audioManager, sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CPlayer* _player, CEnemy::ENEMYTYPE _type, sf::Shader* _shader)
{
	m_World = &_world;
	m_RenderWindow = _renderWindow;
	m_TextureMaster = _textureMaster;
	m_Player = _player;
	m_Slimeptr = nullptr;
	m_Zombieptr = nullptr;
	m_Scale = _scale;
	m_Type = _type;
	m_Shader = _shader;
	m_AudioManager = _audioManager;

	m_Texture = new sf::Texture();
	m_Texture->loadFromFile("Images/SlimeSpawner.png");
	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(_posX, _posY);

	Start();
}

void Spawner::Start()
{
	m_SpawnTimer = new sf::Clock();
}

void Spawner::Update()
{
	switch (m_Type)
	{
	case CEnemy::ENEMYTYPE::DEFAULT:
		break;
	case CEnemy::ENEMYTYPE::ZOMBIE:
		break;
	case CEnemy::ENEMYTYPE::SLIME:
	{
		if (m_bSpawn && m_Slimes.size() < m_SpawnCount)
		{
			if (m_SpawnTimer->getElapsedTime().asSeconds() >= 5)
			{
				m_Slimeptr = new Slime(m_RenderWindow, *m_World, m_TextureMaster, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y);
				m_Slimes.push_front(*m_Slimeptr);
				m_Slimes.front().SetPlayer(m_Player);
				m_Slimes.front().Start();
				m_Slimeptr = nullptr;
				std::cout << "Slime Spawned!" << "(" << m_Slimes.size() << ")" << std::endl;
				m_SpawnTimer->restart();
			}
		}

		
		break;
	}
		
	default:
	{
		break;
	}
		
	}

	for (Slime& slime : m_Slimes)
	{
		// Slime Dies
		if (slime.GetHealth() <= 0.0f && m_Player != nullptr)
		{
			slime.m_MARKASDESTORY = true;

			slime.LoosePlayer();

			//m_bClose = true;
		}
		// Slime Spawns When Player Dead
		else if (!slime.bHasPlayer() && m_Player != nullptr)
		{
			slime.SetPlayer(m_Player);
		}

		// Update
		slime.Update();

		std::list<Slime>::iterator sit;
		for (sit = m_Slimes.begin(); sit != m_Slimes.end(); sit++)
		{
			if (sit->m_MARKASDESTORY)
			{
				// Distance Based Sound
				float Mag1 = sqrt(((sit->GetShape().getPosition().x - m_Player->GetShape().getPosition().x) * (sit->GetShape().getPosition().x - m_Player->GetShape().getPosition().x)) + ((sit->GetShape().getPosition().y - m_Player->GetShape().getPosition().y) * (sit->GetShape().getPosition().y - m_Player->GetShape().getPosition().y)));
				if (Mag1 <= 1920 * 1.8f && Mag1 <= 520 * 1.8f)
				{
					m_AudioManager->PlaySlimeDeath(30);
				}
				else if (Mag1 <= 1920 * 1.8f && Mag1 <= 720 * 1.8f)
				{
					m_AudioManager->PlaySlimeDeath(25);
				}
				else if (Mag1 <= 1920 * 1.8f && Mag1 <= 920 * 1.8f)
				{
					m_AudioManager->PlaySlimeDeath(20);
				}
				else if (Mag1 <= 1920 * 1.8f && Mag1 <= 1120 * 1.8f)
				{
					m_AudioManager->PlaySlimeDeath(15);
				}
				else if (Mag1 <= 1920 * 1.8f && Mag1 <= 1320 * 1.8f)
				{
					m_AudioManager->PlaySlimeDeath(10);
				}
				else if (Mag1 <= 1920 * 1.8f && Mag1 <= 1520 * 1.8f)
				{
					m_AudioManager->PlaySlimeDeath(5);
				}
				else if (Mag1 <= 1920 * 1.8f && Mag1 <= 1720 * 1.8f)
				{
					m_AudioManager->PlaySlimeDeath(3);
				}
				else if (Mag1 <= 1920 * 1.8f)
				{
					m_AudioManager->PlaySlimeDeath(1);
				}

				// Delete
				sit->m_MARKASDESTORY = false;
				m_Slimes.erase(sit);
			}
		}
	}
}

void Spawner::Render()
{
	// Sprite
	m_RenderWindow->draw(m_Shape, m_Shader);

	switch (m_Type)
	{
	case CEnemy::ENEMYTYPE::DEFAULT:
		break;
	case CEnemy::ENEMYTYPE::ZOMBIE:
		break;
	case CEnemy::ENEMYTYPE::SLIME:
	{
		for (Slime& slime : m_Slimes)
		{
			slime.Render(m_Shader);
		}
		break;
	}
	default:
		break;
	}
	
}

void Spawner::LoosePlayer()
{
	m_Player = nullptr;

	// Enemies Loose The Player Pointer
	for (Slime& slime : m_Slimes)
	{
		slime.SetPlayer(nullptr);
	}
}

void Spawner::SetPlayer(CPlayer* _player)
{
	m_Player = _player;

	for (Slime& slime : m_Slimes)
	{
		slime.SetPlayer(m_Player);
	}
}

void Spawner::SetSpawnCount(int _amount)
{
	m_SpawnCount = _amount;
}

int Spawner::GetSpawnCount()
{
	return m_SpawnCount;
}

void Spawner::ToggleSpawning()
{
	m_bSpawn = !m_bSpawn;
}

Spawner::~Spawner()
{
	m_Zombies.clear();
	m_Slimes.clear();

	delete m_SpawnTimer;
	m_SpawnTimer = nullptr;
	m_AudioManager = nullptr;
	delete m_Texture;
	m_Shader = nullptr;
	m_Texture = nullptr;
	m_Player = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
	m_Slimeptr = nullptr;
	m_Zombieptr = nullptr;
}
