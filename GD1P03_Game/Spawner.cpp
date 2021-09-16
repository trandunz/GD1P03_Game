#include "Spawner.h"

Spawner::Spawner(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CPlayer* _player, CEnemy::ENEMYTYPE _type)
{
	m_World = &_world;
	m_RenderWindow = _renderWindow;
	m_TextureMaster = _textureMaster;
	m_Player = _player;
	m_Slimeptr = nullptr;
	m_Zombieptr = nullptr;
	m_Scale = _scale;
	m_Type = _type;

	m_Texture = new sf::Texture();
	m_Texture->loadFromFile("Images/SlimeSpawner.png");
	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(_posX, _posY);
}

void Spawner::Start()
{
	m_SpawnTimer = sf::Clock();
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
			if (m_SpawnTimer.getElapsedTime().asSeconds() >= 5)
			{
				m_Slimeptr = new Slime(m_RenderWindow, *m_World, m_TextureMaster, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y);
				m_Slimeptr->SetPlayer(m_Player);
				m_Slimes.push_back(*m_Slimeptr);
				m_Slimeptr = nullptr;
				std::cout << "Slime Spawned!" << "(" << m_Slimes.size() << ")" << std::endl;
				m_SpawnTimer.restart();
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

			//
			// Cleanup
			std::list<Slime>::iterator sit;
			for (sit = m_Slimes.begin(); sit != m_Slimes.end(); sit++)
			{
				if (sit->m_MARKASDESTORY)
				{
					m_Slimes.erase(sit);
				}
			}
		}
		break;
	}
		
	default:
	{
		break;
	}
		
	}
}

void Spawner::Render()
{
	// Sprite
	m_RenderWindow->draw(m_Shape);

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
			slime.Render();
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

	delete m_Texture;
	m_Texture = nullptr;
	m_Player = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
	m_Slimeptr = nullptr;
	m_Zombieptr = nullptr;
}
