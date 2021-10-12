#include "Spawner.h"

/// <summary>
/// Spawner Constructor
/// </summary>
/// <param name="_audioManager"></param>
/// <param name="_renderWindow"></param>
/// <param name="_world"></param>
/// <param name="_textureMaster"></param>
/// <param name="_scale"></param>
/// <param name="_posX"></param>
/// <param name="_posY"></param>
/// <param name="_player"></param>
/// <param name="_type"></param>
/// <param name="_shader"></param>
/// <param name="_tourchShader"></param>
/// <param name="_sprite"></param>
Spawner::Spawner(CAudioManager* _audioManager, sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CPlayer* _player, CEnemy::ENEMYTYPE _type, sf::Shader* _shader, sf::Shader* _tourchShader, bool _sprite)
{
	m_World = &_world;
	m_RenderWindow = _renderWindow;
	m_TextureMaster = _textureMaster;
	m_Player = _player;
	m_Slimeptr = nullptr;
	m_Scale = _scale;
	m_Type = _type;
	m_Shader = _shader;
	m_AudioManager = _audioManager;
	m_TourchShader = _tourchShader;
	m_Snowmanptr = nullptr;

	AssignAppropiateParticleSystem();

	if (_sprite)
	{
		m_Texture = new sf::Texture();
		m_Texture->loadFromFile("Images/SlimeSpawner.png");
		m_Shape.setTexture(*m_Texture, true);
	}
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(_posX, _posY);

	Start();
}

/// <summary>
/// Spawner Start
/// </summary>
void Spawner::Start()
{
	m_DeathParticles->Start();
}

/// <summary>
/// Spawner Update
/// </summary>
void Spawner::Update()
{
	TypeSpecificUpdate();

	UpdateSlimes();

	UpdateSnowmen();

	sf::Time elapsedTime = m_DeathParticleTimer.getElapsedTime();
	m_DeathParticles->Update(elapsedTime);

	CheckForDestroyedSlimes();

	CheckForDestroyedSnowmen();
}

/// <summary>
/// Spawner Render
/// </summary>
/// <param name="_tourchshader"></param>
/// <param name="_isInRangeOfLightSource"></param>
void Spawner::Render(sf::Shader* _tourchshader, bool _isInRangeOfLightSource)
{
	// Sprite
	m_RenderWindow->draw(m_Shape);

	TypeSpecificRender(_tourchshader, _isInRangeOfLightSource);

	m_RenderWindow->draw(*m_DeathParticles);
}

/// <summary>
/// Sets all enemy's m_Player pointers to nullptr
/// </summary>
void Spawner::LoosePlayer()
{
	m_Player = nullptr;

	// Enemies Loose The Player Pointer
	for (Slime& slime : m_Slimes)
	{
		slime.SetPlayer(nullptr);
	}

	for (CSnowman& snowman : m_Snowmans)
	{
		snowman.SetPlayer(nullptr);
	}
}

/// <summary>
/// Sets all enemy's m_Player pointers to _player
/// </summary>
/// <param name="_player"></param>
void Spawner::SetPlayer(CPlayer* _player)
{
	m_Player = _player;

	for (Slime& slime : m_Slimes)
	{
		slime.SetPlayer(m_Player);
	}

	for (CSnowman& snowman : m_Snowmans)
	{
		snowman.SetPlayer(m_Player);
	}
}

/// <summary>
/// Sets the maximum number of enemies a given spawner can have spawned at one time
/// </summary>
/// <param name="_amount"></param>
void Spawner::SetSpawnCount(int _amount)
{
	m_SpawnCount = _amount;
}

/// <summary>
/// returns an int refering to m_SpawnCount
/// </summary>
/// <returns></returns>
int Spawner::GetSpawnCount()
{
	return m_SpawnCount;
}

/// <summary>
/// Toggles spawning on and off
/// </summary>
void Spawner::ToggleSpawning()
{
	m_bSpawn = !m_bSpawn;
}

/// <summary>
/// Clears all enemy lists
/// </summary>
void Spawner::KillAllChilderan()
{
	m_Slimes.clear();
	m_Snowmans.clear();
}

/// <summary>
/// Spawner Destructor
/// </summary>
Spawner::~Spawner()
{
	std::cout << "Spawner destoryed" << std::endl;

	for (std::list<Slime>::iterator it = m_Slimes.begin(); it != m_Slimes.end(); it++)
	{
		it = m_Slimes.erase(it);
	}
	m_Slimes.clear();
	for (std::list<CSnowman>::iterator it = m_Snowmans.begin(); it != m_Snowmans.end(); it++)
	{
		it = m_Snowmans.erase(it);
	}
	m_Snowmans.clear();

	delete m_DeathParticles;
	m_DeathParticles = nullptr;
	m_AudioManager = nullptr;
	delete m_Texture;
	m_Snowmanptr = nullptr;
	m_TourchShader = nullptr;
	m_Shader = nullptr;
	m_Texture = nullptr;
	m_Player = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
	m_Slimeptr = nullptr;
}

/// <summary>
/// Creates and assignes the appropiate colour to particle system
/// </summary>
void Spawner::AssignAppropiateParticleSystem()
{
	switch (m_Type)
	{
	case CEnemy::ENEMYTYPE::DEFAULT:
		break;
	case CEnemy::ENEMYTYPE::ZOMBIE:
		break;
	case CEnemy::ENEMYTYPE::SLIME:
	{
		m_DeathParticles = new CParticleSystem(600, sf::seconds(0.6f), sf::Color(0, 99, 28, 225));
		break;
	}
	case CEnemy::ENEMYTYPE::SNOWMAN:
	{
		m_DeathParticles = new CParticleSystem(600, sf::seconds(0.6f), sf::Color(240, 240, 240, 225));
		break;
	}
	default:
		m_DeathParticles = new CParticleSystem(600, sf::seconds(0.6f), sf::Color(79, 0, 24, 225));
		break;
	}
}

/// <summary>
/// Type Specific Update
/// </summary>
void Spawner::TypeSpecificUpdate()
{
	int x = 0;
	int y = 0;
	int Mag = 0;

	switch (m_Type)
	{
	case CEnemy::ENEMYTYPE::DEFAULT:
		break;
	case CEnemy::ENEMYTYPE::ZOMBIE:
		break;
	case CEnemy::ENEMYTYPE::SLIME:
	{
		if (m_bSpawn && m_Slimes.size() < m_SpawnCount && m_Player != nullptr)
		{
			if (m_SpawnTimer.getElapsedTime().asSeconds() >= m_SpawnFrequency)
			{
				if (m_bCanSpawnBoss)
				{
					m_Slimeptr = new Slime(m_RenderWindow, *m_World, m_TextureMaster, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, *m_AudioManager, true);
				}
				else
				{
					m_Slimeptr = new Slime(m_RenderWindow, *m_World, m_TextureMaster, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, *m_AudioManager);
				}

				std::cout << "slime spawned" << std::endl;
				m_Slimes.push_front(*m_Slimeptr);
				m_Slimeptr = nullptr;
				m_Slimes.front().SetPlayer(m_Player);
				m_Slimes.front().Start();

				if (m_Slimes.front().m_bIsBoss)
				{
					x = m_Slimes.front().GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
					y = m_Slimes.front().GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
					Mag = sqrt((x * x) + (y * y));

					m_iBossCount++;

					if (m_Player == nullptr)
					{
						if (m_Shape.getPosition().y < 5000 || m_iBossCount > 1)
						{
							m_Slimes.pop_front();
							m_iBossCount--;
						}
						else
						{
							if (Mag < 1920 * 4.8)
							{
								std::cout << "Boss Slime Spawned!" << "(" << m_Slimes.size() << ")" << std::endl;
								m_AudioManager->PlayKingSlimeSpawn();
							}
						}
					}
					else
					{
						if (m_Shape.getPosition().y < 5000 || m_iBossCount > 1 || m_Player->GetShape().getPosition().y < 5500)
						{
							m_Slimes.pop_front();
							m_iBossCount--;
						}
						else
						{
							if (Mag < 1920 * 4.8)
							{
								std::cout << "Boss Slime Spawned!" << "(" << m_Slimes.size() << ")" << std::endl;
								m_AudioManager->PlayKingSlimeSpawn();
							}
						}
					}

				}

				m_Slimeptr = nullptr;
				//std::cout << "Slime Spawned!" << "(" << m_Slimes.size() << ")" << std::endl;
				m_SpawnTimer.restart();
			}
		}
		break;
	}
	case CEnemy::ENEMYTYPE::SNOWMAN:
	{
		if (m_bSpawn && m_Snowmans.size() < m_SpawnCount && m_Player != nullptr)
		{
			if (m_SpawnTimer.getElapsedTime().asSeconds() >= m_SpawnFrequency)
			{
				if (m_bCanSpawnBoss)
				{
					m_Snowmanptr = new CSnowman(m_RenderWindow, *m_World, m_TextureMaster, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, *m_AudioManager, true);
				}
				else
				{
					m_Snowmanptr = new CSnowman(m_RenderWindow, *m_World, m_TextureMaster, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, *m_AudioManager);
				}

				std::cout << "Snowman spawned" << std::endl;
				m_Snowmans.push_front(*m_Snowmanptr);
				m_Snowmanptr = nullptr;
				m_Snowmans.front().SetPlayer(m_Player);
				m_Snowmans.front().Start();

				if (m_Snowmans.front().m_bIsBoss)
				{
					x = m_Snowmans.front().GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
					y = m_Snowmans.front().GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
					Mag = sqrt((x * x) + (y * y));

					m_iBossCount++;

					if (m_Player == nullptr)
					{
						if (m_Shape.getPosition().y < 5000 || m_iBossCount > 1)
						{
							m_Snowmans.pop_front();
							m_iBossCount--;
						}
						else
						{
							if (Mag < 1920 * 4.8)
							{
								std::cout << "Boss Snowman Spawned!" << "(" << m_Snowmans.size() << ")" << std::endl;
								m_AudioManager->PlayKingSlimeSpawn();
							}
						}
					}
					else
					{
						if (m_Shape.getPosition().y < 5000 || m_iBossCount > 1 || m_Player->GetShape().getPosition().y < 5500)
						{
							m_Snowmans.pop_front();
							m_iBossCount--;
						}
						else
						{
							if (Mag < 1920 * 4.8)
							{
								std::cout << "Boss Snowman Spawned!" << "(" << m_Snowmans.size() << ")" << std::endl;
								m_AudioManager->PlayKingSlimeSpawn();
							}
						}
					}

				}

				m_Snowmanptr = nullptr;
				//std::cout << "Slime Spawned!" << "(" << m_Slimes.size() << ")" << std::endl;
				m_SpawnTimer.restart();
			}
		}
		break;
	}
	case CEnemy::ENEMYTYPE::NPC:
	{
		break;
	}
	default:
	{
		break;
	}
	}
}

/// <summary>
/// Updates all Slimes
/// </summary>
void Spawner::UpdateSlimes()
{
	int x = 0;
	int y = 0;
	int Mag = 0;

	for (Slime& slime : m_Slimes)
	{
		x = slime.GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
		y = slime.GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
		Mag = sqrt((x * x) + (y * y));

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
		else if (Mag > 6000)
		{
			slime.m_MARKASDESTORY = true;
			slime.LoosePlayer();
		}

		// Update
		slime.Update();
	}
}

/// <summary>
/// Updates all Snowmen
/// </summary>
void Spawner::UpdateSnowmen()
{
	int x = 0;
	int y = 0;
	int Mag = 0;

	for (CSnowman& snowman : m_Snowmans)
	{
		x = snowman.GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
		y = snowman.GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
		Mag = sqrt((x * x) + (y * y));

		// Slime Dies
		if (snowman.GetHealth() <= 0.0f && m_Player != nullptr)
		{
			snowman.m_MARKASDESTORY = true;

			snowman.LoosePlayer();

			//m_bClose = true;
		}
		// Slime Spawns When Player Dead
		else if (!snowman.bHasPlayer() && m_Player != nullptr)
		{
			snowman.SetPlayer(m_Player);
		}
		else if (Mag > 6000)
		{
			snowman.m_MARKASDESTORY = true;
			snowman.LoosePlayer();
		}

		// Update
		snowman.Update();
	}
}

/// <summary>
/// Check if any slimes have m_MARKASDESTORY, if true : erase slime
/// </summary>
void Spawner::CheckForDestroyedSlimes()
{
	std::list<Slime>::iterator sit;
	for (sit = m_Slimes.begin(); sit != m_Slimes.end(); sit++)
	{
		if (sit->m_MARKASDESTORY)
		{
			// Distance Based Sound
			float Mag1 = sqrt(((sit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x) * (sit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x)) + ((sit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y) * (sit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y)));
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

			float x = 0;
			float y = 0;

			switch (sit->m_SlimeType)
			{
			case Slime::SLIMETYPE::GREEN:
			{
				m_DeathParticles->SetColor(sf::Color(0, 148, 0, 225));

				if (m_Player != nullptr)
				{
					x = sit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
					y = sit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
					Mag1 = sqrt((x * x) + (y * y));

					if (Mag1 < 1920 * 1.5)
					{
						// Add 2 RedSlime To Inventory As Reward
						for (int i = 0; i < 2; i++)
						{
							CBlock* temp = new CBlock(m_TextureMaster->m_RedSlime, CBlock::BLOCKTYPE::REDSLIME);
							m_Player->AddItemToInventory(temp, true);
							temp = nullptr;
						}
					}

				}

				break;
			}
			case Slime::SLIMETYPE::RED:
			{
				m_DeathParticles->SetColor(sf::Color(148, 0, 0, 225));

				if (m_Player != nullptr)
				{
					x = sit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
					y = sit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
					Mag1 = sqrt((x * x) + (y * y));

					if (Mag1 < 1920 * 1.5)
					{
						// Add 2 RedSlime To Inventory As Reward
						for (int i = 0; i < 2; i++)
						{
							CBlock* temp = new CBlock(m_TextureMaster->m_RedSlime, CBlock::BLOCKTYPE::REDSLIME);
							m_Player->AddItemToInventory(temp, true);
							temp = nullptr;
						}
					}
				}

				break;
			}
			case Slime::SLIMETYPE::BOSSYELLOW:
			{
				m_DeathParticles->SetColor(sf::Color(148, 140, 0, 225));

				m_iBossCount--;
				if (m_Player != nullptr)
				{
					x = sit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
					y = sit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
					Mag1 = sqrt((x * x) + (y * y));

					if (Mag1 < 1920 * 1.5 && m_DropTimer.getElapsedTime().asSeconds() >= 1.0f)
					{
						// Add 2 RedSlime To Inventory As Reward
						for (int i = 0; i < 2; i++)
						{
							CBlock* temp = new CBlock(m_TextureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
							m_Player->AddItemToInventory(temp, true);
							temp = nullptr;

						}

						// Drop Yellow PortalKey
						CBlock* temp = new CBlock(m_TextureMaster->m_PortalKeyYellow, CBlock::BLOCKTYPE::PORTALKEYYELLOW);
						m_Player->AddItemToInventory(temp, false);
						temp = nullptr;

						m_SpawnFrequency = 120.0f;

						m_DropTimer.restart();
					}
				}

				break;
			}
			case Slime::SLIMETYPE::BOSSPURPLE:
			{
				m_DeathParticles->SetColor(sf::Color(48, 0, 148, 225));

				m_iBossCount--;
				if (m_Player != nullptr)
				{
					x = sit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
					y = sit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
					Mag1 = sqrt((x * x) + (y * y));

					if (Mag1 < 1920 * 1.5 && m_DropTimer.getElapsedTime().asSeconds() >= 1.0f)
					{
						// Add 2 RedSlime To Inventory As Reward
						for (int i = 0; i < 2; i++)
						{
							CBlock* temp = new CBlock(m_TextureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
							m_Player->AddItemToInventory(temp, true);
							temp = nullptr;
						}

						// Drop Yellow PortalKey
						CBlock* temp = new CBlock(m_TextureMaster->m_PortalKeyBlue, CBlock::BLOCKTYPE::PORTALKEYBLUE);
						m_Player->AddItemToInventory(temp, false);
						temp = nullptr;

						m_SpawnFrequency = 120.0f;

						m_DropTimer.restart();
					}
				}

				break;
			}
			case Slime::SLIMETYPE::BOSSRED:
			{
				m_DeathParticles->SetColor(sf::Color(148, 0, 0, 225));

				m_iBossCount--;
				if (m_Player != nullptr)
				{
					x = sit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
					y = sit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
					Mag1 = sqrt((x * x) + (y * y));

					if (Mag1 < 1920 * 1.5 && m_DropTimer.getElapsedTime().asSeconds() >= 1.0f)
					{
						// Add 2 RedSlime To Inventory As Reward
						for (int i = 0; i < 2; i++)
						{
							CBlock* temp = new CBlock(m_TextureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
							m_Player->AddItemToInventory(temp, true);
							temp = nullptr;


						}

						// Drop Yellow PortalKey
						CBlock* temp = new CBlock(m_TextureMaster->m_PortalKeyRed, CBlock::BLOCKTYPE::PORTALKEYRED);
						m_Player->AddItemToInventory(temp, false);
						temp = nullptr;
						m_SpawnFrequency = 120.0f;

						m_DropTimer.restart();
					}
				}

				break;
			}
			case Slime::SLIMETYPE::BOSSBLUE:
			{
				m_DeathParticles->SetColor(sf::Color(0, 70, 148, 225));

				m_iBossCount--;
				if (m_Player != nullptr)
				{
					x = sit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
					y = sit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
					Mag1 = sqrt((x * x) + (y * y));

					if (Mag1 < 1920 * 1.5 && m_DropTimer.getElapsedTime().asSeconds() >= 1.0f)
					{
						// Add 2 Diamond To Inventory As Reward
						for (int i = 0; i < 2; i++)
						{
							CBlock* temp = new CBlock(m_TextureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
							m_Player->AddItemToInventory(temp, true);
							temp = nullptr;
						}

						// Drop Yellow PortalKey
						CBlock* temp = new CBlock(m_TextureMaster->m_PortalKeyBlue, CBlock::BLOCKTYPE::PORTALKEYBLUE);
						m_Player->AddItemToInventory(temp, false);
						temp = nullptr;
						m_SpawnFrequency = 120.0f;

						m_DropTimer.restart();
					}
				}

				break;
			}
			case Slime::SLIMETYPE::BOSSGREEN:
			{
				m_DeathParticles->SetColor(sf::Color(0, 148, 0, 225));

				m_iBossCount--;
				if (m_Player != nullptr)
				{
					x = sit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
					y = sit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
					Mag1 = sqrt((x * x) + (y * y));

					if (Mag1 < 1920 * 1.5 && m_DropTimer.getElapsedTime().asSeconds() >= 1.0f)
					{
						// Add 2 RedSlime To Inventory As Reward
						for (int i = 0; i < 2; i++)
						{
							CBlock* temp = new CBlock(m_TextureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
							m_Player->AddItemToInventory(temp, true);
							temp = nullptr;
						}

						// Drop Yellow PortalKey
						CBlock* temp = new CBlock(m_TextureMaster->m_PortalKeyPlains, CBlock::BLOCKTYPE::PORTALKEYPLAINS);
						m_Player->AddItemToInventory(temp, false);
						temp = nullptr;
						m_SpawnFrequency = 120.0f;

						m_DropTimer.restart();
					}
				}

				break;
			}
			case Slime::SLIMETYPE::PURPLE:
			{
				m_DeathParticles->SetColor(sf::Color(148, 0, 148, 225));

				if (m_Player != nullptr)
				{
					x = sit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
					y = sit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
					Mag1 = sqrt((x * x) + (y * y));

					if (Mag1 < 1920 * 1.5)
					{
						// Add 2 RedSlime To Inventory As Reward
						for (int i = 0; i < 2; i++)
						{
							CBlock* temp = new CBlock(m_TextureMaster->m_RedSlime, CBlock::BLOCKTYPE::REDSLIME);
							m_Player->AddItemToInventory(temp, true);
							temp = nullptr;
						}
					}
				}

				break;
			}
			case Slime::SLIMETYPE::BLUE:
			{
				m_DeathParticles->SetColor(sf::Color(0, 70, 148, 225));

				if (m_Player != nullptr)
				{
					x = sit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
					y = sit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
					Mag1 = sqrt((x * x) + (y * y));

					if (Mag1 < 1920 * 1.5)
					{
						// Add 2 RedSlime To Inventory As Reward
						for (int i = 0; i < 2; i++)
						{
							CBlock* temp = new CBlock(m_TextureMaster->m_RedSlime, CBlock::BLOCKTYPE::REDSLIME);
							m_Player->AddItemToInventory(temp, true);
							temp = nullptr;
						}
					}
				}

				break;
			}
			case Slime::SLIMETYPE::YELLOW:
			{
				m_DeathParticles->SetColor(sf::Color(148, 140, 0, 225));

				if (m_Player != nullptr)
				{
					x = sit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
					y = sit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
					Mag1 = sqrt((x * x) + (y * y));

					if (Mag1 < 1920 * 1.5)
					{
						// Add 2 RedSlime To Inventory As Reward
						for (int i = 0; i < 2; i++)
						{
							CBlock* temp = new CBlock(m_TextureMaster->m_RedSlime, CBlock::BLOCKTYPE::REDSLIME);
							m_Player->AddItemToInventory(temp, true);
							temp = nullptr;
						}
					}
				}

				break;
			}
			default:
			{
				m_DeathParticles->SetColor(sf::Color(0, 148, 0, 225));
				break;
			}
			}
			m_DeathParticleTimer.restart();
			m_DeathParticles->SetEmitter(sit->GetShape().getPosition());
			m_SpawnTimer.restart();

			sit = m_Slimes.erase(sit);
		}
	}
}

/// <summary>
/// Check if any Slimen have m_MARKASDESTORY, if true : erase Slimen
/// </summary>
void Spawner::CheckForDestroyedSnowmen()
{
	std::list<CSnowman>::iterator snit;
	for (snit = m_Snowmans.begin(); snit != m_Snowmans.end(); snit++)
	{
		if (snit->m_MARKASDESTORY)
		{
			// Distance Based Sound
			float Mag1 = sqrt(((snit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x) * (snit->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x)) + ((snit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y) * (snit->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y)));
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
			snit->m_MARKASDESTORY = false;

			float x = 0;
			float y = 0;

			m_DeathParticles->SetColor(sf::Color(240, 240, 240, 225));
			m_DeathParticleTimer.restart();
			m_DeathParticles->SetEmitter(snit->GetShape().getPosition());
			m_SpawnTimer.restart();

			snit = m_Snowmans.erase(snit);
		}
	}
}

/// <summary>
/// Type Specific Render
/// </summary>
/// <param name="_tourchshader"></param>
/// <param name="_isInRangeOfLightSource"></param>
void Spawner::TypeSpecificRender(sf::Shader* _tourchshader, bool _isInRangeOfLightSource)
{
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
			if (_isInRangeOfLightSource)
			{
				slime.Render(_tourchshader);
			}
			else
			{
				slime.Render(m_Shader);
			}
		}
		break;
	}
	case CEnemy::ENEMYTYPE::SNOWMAN:
	{
		for (CSnowman& snowman : m_Snowmans)
		{
			if (_isInRangeOfLightSource)
			{
				snowman.Render(_tourchshader);
			}
			else
			{
				snowman.Render(m_Shader);
			}
		}
		break;
	}
	case CEnemy::ENEMYTYPE::NPC:
	{
		break;
	}
	default:
		break;
	}
}
