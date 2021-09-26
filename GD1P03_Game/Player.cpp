#include "Player.h"

CPlayer::CPlayer(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, CAudioManager* _audioManager, CTextureMaster* _textureMaster)
{
	// Init
	m_TestParticles = new CParticleSystem(200, sf::seconds(0.4f), sf::Color(70, 65, 60, 255));
	m_AudioManager = _audioManager;
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = &_world;
	m_TextureMaster = _textureMaster;
	m_Block = nullptr;
	m_Chest = nullptr;
	m_Door = nullptr;
	m_Pickaxe = nullptr;

	// Textures
	m_PlayerRightTex = new sf::Texture();
	m_PlayerLeftTex = new sf::Texture();
	if (m_PlayerRightTex->loadFromFile("Images/PlayerSpritesRight.png"))
	{
		std::cout << "Loaded Player Right Texture." << std::endl;
	}
	if (m_PlayerLeftTex->loadFromFile("Images/PlayerSpritesLeft.png"))
	{
		std::cout << "Loaded Player Left Texture." << std::endl;
	}
	m_Shape = sf::Sprite();
	m_Shape.setTexture(*m_PlayerRightTex, true);
	m_Shape.setTextureRect(sf::IntRect(0, 0, 100, 200));

	// Body
	CreateBody(100, -100, b2_dynamicBody);

	// Map Icon
	m_MapIconTex = new sf::Texture();
	m_MapIconTexRight = new sf::Texture();
	m_MapIconTex->loadFromFile("Images/PlayerIcon.png");
	m_MapIconTexRight->loadFromFile("Images/PlayerIconRight.png");
	m_MapIcon.setTexture(*m_MapIconTexRight, true);
	m_MapIcon.setOrigin(m_MapIcon.getGlobalBounds().width / 2, m_MapIcon.getGlobalBounds().height / 2);
	m_MapIcon.setScale(12, 12);
}

CPlayer::~CPlayer()
{
	DestroyBody();

	if (m_Pickaxe != nullptr)
	{
		delete m_Pickaxe;
		
	}
	m_Pickaxe = nullptr;

	if (m_Bow != nullptr)
	{
		delete m_Bow;
		
	}
	m_Bow = nullptr;

	m_InventoryMap.clear();
	m_Projectiles.clear();

	m_WorkBench = nullptr;
	delete m_TestParticles;
	m_TestParticles = nullptr;
	m_Furnace = nullptr;
	m_Chest = nullptr;
	m_Door = nullptr;
	delete m_MapIconTexRight;
	m_MapIconTexRight = nullptr;
	delete m_MapIconTex;
	m_MapIconTex = nullptr;
	delete m_PlayerRightTex;
	m_PlayerRightTex = nullptr;
	delete m_PlayerLeftTex;
	delete m_AnimationTimer;
	delete m_MineTimer;
	delete m_DamageTimer;
	delete m_DamageIndicatorTimer;
	m_DamageIndicatorTimer = nullptr;
	m_DamageTimer = nullptr;
	m_MineTimer = nullptr;
	m_AnimationTimer = nullptr;
	m_PlayerLeftTex = nullptr;
	m_Block = nullptr;
	m_Body = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
	m_AudioManager = nullptr;
	m_TextureMaster = nullptr;
}

void CPlayer::Start()
{
	for (int i = 0; i < 50; i++)
	{
		std::cout << i << std::endl;
		m_InventoryStackValues[i];
	}

	for (int i = 0; i < 50; i++)
	{
		std::cout << i << std::endl;
		m_InventoryMap[i];
	}

	m_AnimationTimer = new sf::Clock();
	m_MineTimer = new sf::Clock();
	m_DamageTimer = new sf::Clock();
	m_DamageIndicatorTimer = new sf::Clock();

	for (int i = 0; i < 50; i++)
	{
		if (m_InventoryStackValues[i] == 0)
		{
			m_Pickaxe = new CPickaxe();
			AddItemToInventory(m_Pickaxe, i);
			m_Pickaxe = nullptr;
			break;
		}
	}
	for (int i = 0; i < 50; i++)
	{
		if (m_InventoryStackValues[i] == 0)
		{
			for (int j = 0; j < 3; j++)
			{
				m_Block = new CBlock(m_TextureMaster->m_WorkBench, CBlock::BLOCKTYPE::WORKBENCH);
				AddItemToInventory(m_Block, i, true);
				m_Block = nullptr;
			}
			
			break;
		}
	}

	m_TestParticles->Start();
}

void CPlayer::Update(sf::Vector2f _mousePos)
{
	// Player Is Red From Damage
	if (m_DamageIndicatorTimer->getElapsedTime().asSeconds() >= 0.2f && m_Shape.getColor() == sf::Color(100, 0, 0, 255))
	{
		m_Shape.setColor(sf::Color::White);
		m_DamageIndicatorTimer->restart();
	}

	m_MousePos = _mousePos;

	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);

	m_MapIcon.setPosition(m_Shape.getPosition());

	// Player Has Pickaxe ? Orient Sprite
	if (m_Pickaxe != nullptr)
	{
		m_Pickaxe->FlipSprite(m_Shape.getPosition(), m_Shape, m_PlayerLeftTex, m_PlayerRightTex);
	}
	// Player Has Bow ? Orient Sprite
	else if (m_Bow != nullptr)
	{
		m_Bow->FlipSprite(m_Shape.getPosition(), m_Shape, m_PlayerLeftTex, m_PlayerRightTex);
	}

	// Items
	for (std::map<int, CBlock>::iterator iit = m_InventoryMap.begin(); iit != m_InventoryMap.end(); iit++)
	{
		// Player Selects Basic Pickaxe
		if (iit->second.m_bIsItemAndSelected == true && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::BASIC)
		{
			std::cout << "New Pickaxe Created!" << std::endl;
			m_Pickaxe = new CPickaxe(m_RenderWindow, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, CBlock::PICKAXETYPE::BASIC);
			iit->second.m_bIsItemAndSelected = false;
		}
		// Player Selects Iron Pickaxe
		else if (iit->second.m_bIsItemAndSelected == true && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::IRON)
		{
			std::cout << "New Pickaxe Created!" << std::endl;
			m_Pickaxe = new CPickaxe(m_RenderWindow, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, CBlock::PICKAXETYPE::IRON);
			iit->second.m_bIsItemAndSelected = false;
		}
		// Player Selects Gold Pickaxe
		else if (iit->second.m_bIsItemAndSelected == true && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLD)
		{
			std::cout << "New GOLD Pickaxe Created!" << std::endl;
			m_Pickaxe = new CPickaxe(m_RenderWindow, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, CBlock::PICKAXETYPE::GOLD);
			iit->second.m_bIsItemAndSelected = false;
		}
		// Player Selects Diamond Pickaxe
		else if (iit->second.m_bIsItemAndSelected == true && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::DIAMOND)
		{
			std::cout << "New Pickaxe Created!" << std::endl;
			m_Pickaxe = new CPickaxe(m_RenderWindow, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, CBlock::PICKAXETYPE::DIAMOND);
			iit->second.m_bIsItemAndSelected = false;
		}
		// Player Selects pURPLE Pickaxe
		else if (iit->second.m_bIsItemAndSelected == true && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::PURPLE)
		{
			std::cout << "New Pickaxe Created!" << std::endl;
			m_Pickaxe = new CPickaxe(m_RenderWindow, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, CBlock::PICKAXETYPE::PURPLE);
			iit->second.m_bIsItemAndSelected = false;
		}
		// Player Selects GOLDEN Pickaxe
		else if (iit->second.m_bIsItemAndSelected == true && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLDEN)
		{
			std::cout << "New Pickaxe Created!" << std::endl;
			m_Pickaxe = new CPickaxe(m_RenderWindow, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, CBlock::PICKAXETYPE::GOLDEN);
			iit->second.m_bIsItemAndSelected = false;
		}
		// Player Selects Bow
		else if (iit->second.m_bIsItemAndSelected == true && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && iit->second.m_BowType == CBlock::BOWTYPE::DEFAULT)
		{
			std::cout << "New Bow Created!" << std::endl;
			m_Bow = new Bow(m_RenderWindow, *m_World, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, CBlock::BOWTYPE::DEFAULT);
			iit->second.m_bIsItemAndSelected = false;
		}
		// Player Selects Iron Bow
		else if (iit->second.m_bIsItemAndSelected == true && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && iit->second.m_BowType == CBlock::BOWTYPE::IRON)
		{
			std::cout << "New Iron Bow Created!" << std::endl;
			m_Bow = new Bow(m_RenderWindow, *m_World, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, CBlock::BOWTYPE::IRON);
			iit->second.m_bIsItemAndSelected = false;
		}
		// Player Unselects Basic PickAxe
		else if ((m_Pickaxe != nullptr && m_CurrentItemIndex != iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::BASIC && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLD && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::IRON && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::DIAMOND && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::PURPLE && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLDEN))
		{
		delete m_Pickaxe;
		m_Pickaxe = nullptr;
		}
		// Player Unselects Iron PickAxe
		else if ((m_Pickaxe != nullptr && m_CurrentItemIndex != iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::IRON && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLD && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::DIAMOND && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::BASIC && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::PURPLE && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLDEN))
		{
		delete m_Pickaxe;
		m_Pickaxe = nullptr;
		}
		// Player Unselects Gold PickAxe
		else if ((m_Pickaxe != nullptr && m_CurrentItemIndex != iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLD && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::DIAMOND && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::IRON && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::BASIC && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::PURPLE && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLDEN))
		{
			delete m_Pickaxe;
			m_Pickaxe = nullptr;
		}
		// Player Unselects Diamond PickAxe
		else if ((m_Pickaxe != nullptr && m_CurrentItemIndex != iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::DIAMOND && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLD && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::IRON && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::BASIC && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::PURPLE && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLDEN))
		{
			delete m_Pickaxe;
			m_Pickaxe = nullptr;
		}
		// Player Unselects Purple PickAxe
		else if ((m_Pickaxe != nullptr && m_CurrentItemIndex != iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::PURPLE && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLD && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::IRON && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::BASIC && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLDEN))
		{
			delete m_Pickaxe;
			m_Pickaxe = nullptr;
		}
		// Player Unselects Golden PickAxe
		else if ((m_Pickaxe != nullptr && m_CurrentItemIndex != iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLDEN && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLD && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::IRON && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::BASIC && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::PURPLE))
		{
			delete m_Pickaxe;
			m_Pickaxe = nullptr;
		}
		// Player Unselects Bow
		else if ((m_Bow != nullptr && m_CurrentItemIndex != iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && iit->second.m_BowType == CBlock::BOWTYPE::DEFAULT && iit->second.m_BowType != CBlock::BOWTYPE::IRON))
		{
			delete m_Bow;
			m_Bow = nullptr;
		}
		// Player Unselects Bow
		else if ((m_Bow != nullptr && m_CurrentItemIndex != iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && iit->second.m_BowType == CBlock::BOWTYPE::IRON && iit->second.m_BowType != CBlock::BOWTYPE::DEFAULT))
		{
			delete m_Bow;
			m_Bow = nullptr;
		}
		// Basic Pickaxe Moves Slots Wealst Out
		else if ((m_Pickaxe != nullptr && m_CurrentItemIndex == iit->first && iit->second.m_Type != CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::BASIC && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLD && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::IRON && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::DIAMOND && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::PURPLE && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLDEN))
		{
			delete m_Pickaxe;
			m_Pickaxe = nullptr;
		}
		// Iron Pickaxe Moves Slots Wealst Out
		else if ((m_Pickaxe != nullptr && m_CurrentItemIndex == iit->first && iit->second.m_Type != CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::IRON && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLD && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::BASIC && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::DIAMOND && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::PURPLE && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLDEN))
		{
			delete m_Pickaxe;
			m_Pickaxe = nullptr;
		}
		// Gold Pickaxe Moves Slots Wealst Out
		else if ((m_Pickaxe != nullptr && m_CurrentItemIndex == iit->first && iit->second.m_Type != CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLD && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::IRON && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::BASIC && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::DIAMOND && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::PURPLE && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLDEN))
		{
			delete m_Pickaxe;
			m_Pickaxe = nullptr;
		}
		// Diamond Pickaxe Moves Slots Wealst Out
		else if ((m_Pickaxe != nullptr && m_CurrentItemIndex == iit->first && iit->second.m_Type != CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::DIAMOND && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLD && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::BASIC && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::IRON && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::PURPLE && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLDEN))
		{
			delete m_Pickaxe;
			m_Pickaxe = nullptr;
		}
		// pURPLE Pickaxe Moves Slots Wealst Out
		else if ((m_Pickaxe != nullptr && m_CurrentItemIndex == iit->first && iit->second.m_Type != CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::PURPLE && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLD && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::BASIC && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::IRON && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLDEN))
		{
		delete m_Pickaxe;
		m_Pickaxe = nullptr;
		}
		// GOLDEN Pickaxe Moves Slots Wealst Out
		else if ((m_Pickaxe != nullptr && m_CurrentItemIndex == iit->first && iit->second.m_Type != CBlock::BLOCKTYPE::PICKAXE && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLDEN && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::GOLD && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::BASIC && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::IRON && m_Pickaxe->m_PickType != CBlock::PICKAXETYPE::PURPLE))
		{
		delete m_Pickaxe;
		m_Pickaxe = nullptr;
		}
		// Bow Moves Slots Wealst Out
		else if ((m_Bow != nullptr && m_CurrentItemIndex == iit->first && iit->second.m_Type != CBlock::BLOCKTYPE::BOW && iit->second.m_BowType == CBlock::BOWTYPE::DEFAULT && iit->second.m_BowType != CBlock::BOWTYPE::IRON))
		{
			delete m_Bow;
			m_Bow = nullptr;
		}
		// Bow Moves Slots Wealst Out
		else if ((m_Bow != nullptr && m_CurrentItemIndex == iit->first && iit->second.m_Type != CBlock::BLOCKTYPE::BOW && iit->second.m_BowType == CBlock::BOWTYPE::IRON && iit->second.m_BowType != CBlock::BOWTYPE::DEFAULT))
		{
		delete m_Bow;
		m_Bow = nullptr;
		}
	}
	
	b2Contact* contact;
	for (contact = m_World->GetContactList(); contact; contact = contact->GetNext())
	{
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();
		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);

		b2Vec2 vel1 = m_Body->GetLinearVelocityFromWorldPoint(m_Body->GetPosition());
		b2Vec2 vel2 = b->GetBody()->GetLinearVelocityFromWorldPoint(worldManifold.points[0]);
		b2Vec2 impactVelocity = vel1 - vel2;
		b2Vec2 worldposition = { m_Shape.getPosition().x, m_Shape.getPosition().y };
		
		if (a->GetBody() == m_Body || b->GetBody() == m_Body)
		{
			// Can Jump?
			if ((vel1.y <= 1.0f && vel1.y >= -1.0f))
			{
				m_bCanJump = true;
			}
			// Can Take Fall Damage?
			else if ((vel1.y > 0.0f))
			{
				m_bCanFallDamage = true;
			}

			// Fall Damage
			if (impactVelocity.y <= 110.7f && impactVelocity.y >= 70.7f)
			{
				//std::cout << impactVelocity.y << std::endl;
				TakeDamage(impactVelocity.y);
			}
			else if (impactVelocity.y > 110.7f)
			{
				//std::cout << impactVelocity.y << std::endl;
				TakeDamage(impactVelocity.y * 2);
			}

			

		}
		else if (vel1.y < -2.0f || vel1.y > 2.0f)
		{
			m_bCanJump = false;
		}
		
		a = nullptr;
		b = nullptr;
	}
	contact = nullptr;

	if (m_Bow != nullptr)
	{
		Attack();
	}

	for (CProjectile& projectile : m_Projectiles)
	{
		projectile.Update();
	}

	std::list<CProjectile>::iterator pit = m_Projectiles.begin();
	while (pit != m_Projectiles.end())
	{
		if (pit->m_bMARKASDESTROY)
		{
			pit = m_Projectiles.erase(pit);
		}
		pit++;
	}

	sf::Time elapsedTime = m_ParticleTimer.getElapsedTime();
	m_TestParticles->Update(elapsedTime);

	// HP Regen
	if (m_HealthRegenTimer.getElapsedTime().asSeconds() >= m_HeathRegenSpeed && m_DamageTimer->getElapsedTime().asSeconds() >= 3.0f)
	{
		Heal(m_HeathRegenAmount);
		m_HealthRegenTimer.restart();
	}
}

void CPlayer::Render(sf::Shader* _defaultShader)
{
	// Player Has PickAxe?
	if (m_Pickaxe != nullptr)
	{
		m_Pickaxe->Render();
	}
	else if (m_Bow != nullptr)
	{
		m_Bow->Render();
	}

	for (CProjectile& projectile : m_Projectiles)
	{
		m_RenderWindow->draw(projectile.m_Shape);
	}

	m_RenderWindow->draw(m_Shape, _defaultShader);

	m_RenderWindow->draw(*m_TestParticles);
}

void CPlayer::Movement()
{
	b2Vec2 worldposition = { m_Shape.getPosition().x, m_Shape.getPosition().y };

	// Input
	if (m_bCanMove)
	{
		int x = 0;
		int y = 0;

		// Down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			y++;
		}
		// Left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			x = -1;
		}
		// Right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			x = 1;
		}

		m_Velocity = b2Vec2(x, y);

		m_Body->ApplyLinearImpulseToCenter(m_MoveSpeed / 2 * m_Velocity, true);

		float Mag = sqrt((m_Body->GetLinearVelocityFromWorldPoint(worldposition).x * m_Body->GetLinearVelocityFromWorldPoint(worldposition).x));
		if (Mag > m_MoveSpeed)
		{
			m_Body->ApplyLinearImpulseToCenter(-1 * m_MoveSpeed * b2Vec2(m_Velocity.x, 0.0f), true);
		}

		// Animation & Input Right
		if (m_Velocity.x > 0.1f)
		{
			if (m_Shape.getTexture() != m_PlayerRightTex)
			{
				m_Shape.setTexture(*m_PlayerRightTex);
				m_MapIcon.setTexture(*m_MapIconTexRight);
			}
			if (m_AnimationTimer->getElapsedTime() >= sf::Time(sf::seconds(0.03f)))
			{
				if (m_Shape.getTextureRect().left < 1500)
				{
					m_Shape.setTextureRect(sf::IntRect(m_Shape.getTextureRect().left + 100, m_Shape.getTextureRect().top, m_Shape.getTextureRect().width, m_Shape.getTextureRect().height));
				}
				else if (m_Shape.getTextureRect() == sf::IntRect(1500, 0, 100, 200))
				{
					m_Shape.setTextureRect(sf::IntRect(100, 0, 100, 200));
				}
				m_AnimationTimer->restart();
			}
		}
		// Animation & Input Left
		else if (m_Velocity.x < -0.1f)
		{
			if (m_Shape.getTexture() != m_PlayerLeftTex)
			{
				m_Shape.setTexture(*m_PlayerLeftTex);
				m_MapIcon.setTexture(*m_MapIconTex);
			}
			if (m_AnimationTimer->getElapsedTime() >= sf::Time(sf::seconds(0.03f)))
			{
				if (m_Shape.getTextureRect().left < 1500)
				{
					m_Shape.setTextureRect(sf::IntRect(m_Shape.getTextureRect().left + 100, m_Shape.getTextureRect().top, m_Shape.getTextureRect().width, m_Shape.getTextureRect().height));
				}

				else if (m_Shape.getTextureRect() == sf::IntRect(1500, 0, 100, 200))
				{
					m_Shape.setTextureRect(sf::IntRect(100, 0, 100, 200));
				}
				m_AnimationTimer->restart();
			}
		}
		// No Input Animation And Audio
		else if (m_Velocity.x == 0.0f)
		{
			// Animation & No Input & Movement Right
			if (m_Body->GetLinearVelocityFromWorldPoint(worldposition).x > 15.0f)
			{
				if (m_AudioManager != nullptr)
				{
					m_AudioManager->PlayRunning();
				}

				if (m_Shape.getTexture() != m_PlayerRightTex)
				{
					m_Shape.setTexture(*m_PlayerRightTex);
					m_MapIcon.setTexture(*m_MapIconTexRight);


				}
				if (m_AnimationTimer->getElapsedTime() >= sf::Time(sf::seconds(0.03f)))
				{
					if (m_Shape.getTextureRect().left < 1500)
					{
						m_Shape.setTextureRect(sf::IntRect(m_Shape.getTextureRect().left + 100, m_Shape.getTextureRect().top, m_Shape.getTextureRect().width, m_Shape.getTextureRect().height));
					}
					else if (m_Shape.getTextureRect() == sf::IntRect(1500, 0, 100, 200))
					{
						m_Shape.setTextureRect(sf::IntRect(100, 0, 100, 200));
					}
					m_AnimationTimer->restart();
				}
			}
			// Animation & No Input & Movement Left
			else if (m_Body->GetLinearVelocityFromWorldPoint(worldposition).x < -15.0f)
			{
				if (m_AudioManager != nullptr)
				{
					m_AudioManager->PlayRunning();
				}

				if (m_Shape.getTexture() != m_PlayerLeftTex)
				{
					m_Shape.setTexture(*m_PlayerLeftTex);
					m_MapIcon.setTexture(*m_MapIconTex);
				}
				if (m_AnimationTimer->getElapsedTime() >= sf::Time(sf::seconds(0.03f)))
				{
					if (m_Shape.getTextureRect().left < 1500)
					{
						m_Shape.setTextureRect(sf::IntRect(m_Shape.getTextureRect().left + 100, m_Shape.getTextureRect().top, m_Shape.getTextureRect().width, m_Shape.getTextureRect().height));
					}

					else if (m_Shape.getTextureRect() == sf::IntRect(1500, 0, 100, 200))
					{
						m_Shape.setTextureRect(sf::IntRect(100, 0, 100, 200));
					}
					m_AnimationTimer->restart();
				}
			}
			// No Input & No Movement (Sprite State)
			else
			{
				m_Shape.setTextureRect(sf::IntRect(0, 0, 100, 200));
			}
		}

		// Input Audio Right
		if (m_Body->GetLinearVelocityFromWorldPoint(worldposition).x > 15.0f)
		{
			if (m_AudioManager != nullptr)
			{
				m_AudioManager->PlayRunning();
			}
		}
		// Input Audio Left
		else if (m_Body->GetLinearVelocityFromWorldPoint(worldposition).x < -15.0f)
		{
			if (m_AudioManager != nullptr)
			{
				m_AudioManager->PlayRunning();
			}
		}

		//std::cout << m_Shape.getPosition().x << std::endl;
		//std::cout << m_Shape.getPosition().y << std::endl;
	}
}

void CPlayer::Movement(sf::Event& _event)
{
	b2Vec2 worldposition = { m_Shape.getPosition().x, m_Shape.getPosition().y };

	// Input
	if (m_bCanMove)
	{
		int x = 0;
		int y = 0;

		// Fly
		if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Key::Space && m_bCanJump)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -420.0f), true);
			m_bCanJump = false;
		}

		// Jump
		if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Key::W && m_bCanJump)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -420.0f), true);
			m_bCanJump = false;
		}
	}
}

void CPlayer::Interact(std::list<CFurnace>& m_Furnaces, std::list<CChest>& m_Chests, std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite, std::list<CWorkBench>& m_WorkBenches )
{
	float Mag = sqrt(((_mousePositionSprite.getPosition().x - m_Shape.getPosition().x) * (_mousePositionSprite.getPosition().x - m_Shape.getPosition().x)) + ((_mousePositionSprite.getPosition().y - m_Shape.getPosition().y) * (_mousePositionSprite.getPosition().y - m_Shape.getPosition().y)));
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && Mag < m_InteractionRange * 100 && Mag > 80.0f && m_bCanPlace)
	{
		// Item
		if (m_Pickaxe != nullptr)
		{
			// Break Block
			if (!bMouseNotOver(m_Chunk, _mousePositionSprite))
			{
				Mine(m_Chunk, _mousePositionSprite);
			}
			else if (!bMouseNotOver(m_Doors, _mousePositionSprite))
			{
				Mine(m_Doors, _mousePositionSprite);
			}
			else if (!bMouseNotOver(m_Chests, _mousePositionSprite))
			{
				for (CChest& chest : m_Chests)
				{
					if (_mousePositionSprite.getPosition() == chest.GetPosition())
					{
						if (chest.GetInventorySize() <= 0)
						{
							Mine(m_Chests, _mousePositionSprite);
							break;
						}
					}
				}
			}
			else if (!bMouseNotOver(m_Furnaces, _mousePositionSprite))
			{
				Mine(m_Furnaces, _mousePositionSprite);
			}
			else if (!bMouseNotOver(m_WorkBenches, _mousePositionSprite))
			{
				Mine(m_WorkBenches, _mousePositionSprite);
			}
		}
		else if (m_Bow != nullptr)
		{
		}
		// Left Mouse Clicked And In Empty Space
		else if (bMouseNotOver(m_Chunk, _mousePositionSprite) && bMouseNotOver(m_Doors, _mousePositionSprite) && !SelectedItemIsEmpty() && bMouseNotOver(m_Chests, _mousePositionSprite) && bMouseNotOver(m_Furnaces, _mousePositionSprite) && bMouseNotOver(m_WorkBenches, _mousePositionSprite) && _mousePositionSprite.getGlobalBounds().contains(m_MousePos))
		{
			// Place Door
			if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::DOOR)
			{
				PlaceDoor(m_Doors, _mousePositionSprite);
			}
			// Swing Pickaxe
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::PICKAXE)
			{
			}
			// Bow
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::BOW)
			{
			}
			// Iron Ingot
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::IRONINGOT)
			{
			}
			// Gold Ingot
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::GOLDINGOT)
			{
			}
			// Diamond Ingot
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::DIAMOND)
			{
			}
			// Purple Ingot
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::PURPLEINGOT)
			{
			}
			// Golden Ingot
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::GOLDENINGOT)
			{
			}
			//Chest
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::CHEST)
			{
				PlaceChest(m_Chests, _mousePositionSprite);
			}
			//Furnace
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::FURNACE)
			{
				PlaceFurnace(m_Furnaces, _mousePositionSprite);
			}
			//Furnace
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::WORKBENCH)
			{
				PlaceWorkBench(m_WorkBenches, _mousePositionSprite);
			}
			// Place Block
			else
			{
				PlaceBlock(m_Chunk, _mousePositionSprite);
			}
		}

	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && Mag < m_InteractionRange * 100 && m_bCanPlace)
	{
		// Item
		if (m_Pickaxe != nullptr)
		{
			// Break Block
			if (!bMouseNotOver(m_Chunk, _mousePositionSprite))
			{
				Mine(m_Chunk, _mousePositionSprite);
			}
			else if (!bMouseNotOver(m_Doors, _mousePositionSprite))
			{
				Mine(m_Doors, _mousePositionSprite);
			}
			else if (!bMouseNotOver(m_Chests, _mousePositionSprite))
			{
				Mine(m_Chests, _mousePositionSprite);
			}
			else if (!bMouseNotOver(m_Furnaces, _mousePositionSprite))
			{
				Mine(m_Furnaces, _mousePositionSprite);
			}
			else if (!bMouseNotOver(m_WorkBenches, _mousePositionSprite))
			{
				Mine(m_WorkBenches, _mousePositionSprite);
			}
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && Mag < m_InteractionRange * 100 && m_bCanPlace)
	{
		//std::cout << "Mouse Right" << std::endl;

		// Door Opening And Closing
		for (CDoor& doors : m_Doors)
		{
			if (doors.GetShape().getPosition() == _mousePositionSprite.getPosition())
			{
				if (bMouseNotOver(m_Chunk, _mousePositionSprite) && !bMouseNotOver(m_Doors, _mousePositionSprite))
				{
					if (m_MineTimer->getElapsedTime() >= sf::Time(sf::seconds(0.2f)))
					{
						std::cout << "Open Door" << std::endl;
						doors.OCDoor(m_Shape.getPosition());
						m_MineTimer->restart();
						break;
					}
				}
			}
		}
		// Chest Opening

		std::list<CChest>::iterator chit = m_Chests.begin();
		float MagToPlayer = 1000;
		while (chit != m_Chests.end())
		{
			MagToPlayer = sqrt(((chit->GetShape().getPosition().x - m_Shape.getPosition().x)* (chit->GetShape().getPosition().x - m_Shape.getPosition().x)) + ((chit->GetShape().getPosition().y - m_Shape.getPosition().y) * (chit->GetShape().getPosition().y - m_Shape.getPosition().y)));
			if (MagToPlayer < 200)
			{
				if (m_MineTimer->getElapsedTime() >= sf::Time(sf::seconds(0.2f)))
				{
					std::cout << "Open Chest" << std::endl;
					m_bInventoryOpen = true;
					m_bPlayerIsInChest = true;
					chit->m_bIsOpen = true;
					m_bCanPlace = !m_bInventoryOpen;
					m_bCanMove = !m_bInventoryOpen;
					m_MineTimer->restart();
				}
			}
			else
			{
				chit->m_bIsOpen = false;
			}
			chit++;
		}

		// Furnace Opening
		for (CFurnace& furnace : m_Furnaces)
		{
			if (furnace.GetShape().getPosition() == _mousePositionSprite.getPosition())
			{
				if (!bMouseNotOver(m_Furnaces, _mousePositionSprite))
				{
					if (m_MineTimer->getElapsedTime() >= sf::Time(sf::seconds(0.2f)))
					{
						std::cout << "Open Furnace" << std::endl;
						m_bInventoryOpen = true;
						m_bCanPlace = !m_bInventoryOpen;
						m_bCanMove = !m_bInventoryOpen;
						m_MineTimer->restart();
						return;
					}
				}
			}
		}

		// WorkBench Opening
		for (CWorkBench& workbench : m_WorkBenches)
		{
			if (workbench.GetShape().getPosition() == _mousePositionSprite.getPosition())
			{
				if (!bMouseNotOver(m_WorkBenches, _mousePositionSprite))
				{
					if (m_MineTimer->getElapsedTime() >= sf::Time(sf::seconds(0.2f)))
					{
						std::cout << "Open WorkBench" << std::endl;
						m_bInventoryOpen = true;
						m_bCanPlace = !m_bInventoryOpen;
						m_bCanMove = !m_bInventoryOpen;
						m_MineTimer->restart();
						return;
					}
				}
			}
		}
	}
}

void CPlayer::Attack()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_AttackTimer.getElapsedTime().asSeconds() >= m_AttackSpeed)
	{
		m_AudioManager->PlayBowShot();
		m_Projectile = new CProjectile(*m_World, m_Shape.getPosition().x, m_Shape.getPosition().y - 50, m_MousePos);
		
		m_Projectiles.push_back(*m_Projectile);
		m_Projectile = nullptr;
		m_AttackTimer.restart();
	}
}

b2Body* CPlayer::GetBody()
{
	return m_Body;
}

int CPlayer::GetCurrentHP()
{
	return m_Health;
}

int CPlayer::GetMaxHP()
{
	return m_MaxHP;
}

void CPlayer::TakeDamage(float _damage, bool _fallDamage)
{
	if (m_DamageTimer->getElapsedTime().asSeconds() >= 0.4f && !_fallDamage)
	{

		std::cout << "Player Took Damage!" << std::endl;
		m_Health -= _damage;
		m_DamageTimer->restart();
		m_DamageIndicatorTimer->restart();
		m_AudioManager->PlayPlayerDamage();

		// Set Red
		m_Shape.setColor(sf::Color(100, 0, 0, 255));
	}
	else if (m_DamageTimer->getElapsedTime().asSeconds() >= 0.1f && _fallDamage)
	{
		std::cout << "Player Took Damage!" << std::endl;
		m_Health -= m_Health;
		m_DamageTimer->restart();
		m_DamageIndicatorTimer->restart();
		m_AudioManager->PlayPlayerDamage();

		// Set Red
		m_Shape.setColor(sf::Color(100, 0, 0, 255));
		
	}
}

sf::Sprite& CPlayer::GetShape()
{
	return m_Shape;
}

void CPlayer::SetCurrentHP(int _amount)
{
	m_Health = _amount;
}

void CPlayer::SetMaxHP(int _amount)
{
	m_MaxHP = _amount;
}

void CPlayer::SetMousePos(sf::Vector2f _position)
{
	m_MousePos = _position;
}

bool CPlayer::bInventoryOpen()
{
	return m_bInventoryOpen;
}

void CPlayer::bInventoryOpen(bool _bInventoryOpen)
{
	m_bInventoryOpen = _bInventoryOpen;
}

bool CPlayer::bCanMove()
{
	return m_bCanMove;
}

void CPlayer::Heal(float _amount)
{
	for (int i = 0; i < _amount; i++)
	{
		if (m_Health > 0 && m_Health < m_MaxHP)
		{
			m_Health++;
		}
		else
		{
			break;
		}
	}
}

void CPlayer::bCanMove(bool _bcanMove)
{
	m_bCanMove = _bcanMove;
}

bool CPlayer::bCanPlace()
{
	return m_bCanPlace;
}

void CPlayer::bCanPlace(bool _bcanPlace)
{
	m_bCanPlace = _bcanPlace;
}

void CPlayer::ResetSpritePos()
{
	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

template <class T>
bool CPlayer::bMouseNotOver(std::list<T>& m_Chunk, sf::Sprite& _mousePositionSprite)
{
	for (T& block : m_Chunk)
	{
		if (block.GetShape().getPosition() == _mousePositionSprite.getPosition())
		{
			return false;
		}
	}
	return true;
}

bool CPlayer::bMouseOverIventoryItem(std::map<int, CBlock>& m_Inventory, sf::Sprite& _mousePositionSprite)
{
	for (std::map<int, CBlock>::iterator iit = m_Inventory.begin(); iit != m_Inventory.end(); iit++)
	{
		if (iit->second.GetShape().getGlobalBounds().intersects(_mousePositionSprite.getGlobalBounds()))
		{
			return true;
		}
	}
	return false;
}

bool CPlayer::IsBlockInInventory(CBlock* _block)
{
	std::map<int, CBlock>::iterator it;
	for (it = m_InventoryMap.begin(); it != m_InventoryMap.end(); it++)
	{
		if (it->second.m_Type == _block->m_Type)
		{
			// increase number of that type
			m_InventoryStackValues[it->first]++;
			return true;
		}
	}
	return false;
}

void CPlayer::AddItemToInventory(CBlock* _block, bool _canStack)
{
	if (_canStack == true)
	{
		if (IsBlockInInventory(_block))
		{
			m_AudioManager->PlayPickupSound();
		}
		else
		{
			//
			// Reading From File??
			//
			std::string line;
			std::ifstream myfile("Output/FirstEmptyInventorySlot.txt");
			myfile.is_open();
			int firstEmpty = 0;
			myfile >> firstEmpty;

			_block->GetShape().setScale(0.4f, 0.4f);
			_block->GetShape().setOrigin(_block->GetShape().getGlobalBounds().width / 2, _block->GetShape().getGlobalBounds().height / 2);
			m_RenderWindow->mapCoordsToPixel(_block->GetShape().getPosition());
			m_InventorySize++;
			// increase number of that type
			m_InventoryStackValues[firstEmpty]++;
			_block->m_PositionInInventory = firstEmpty;
			m_InventoryMap.insert_or_assign(firstEmpty, *_block);
			//std::cout << "Added Item To Inventory - ";
			std::cout << firstEmpty << std::endl;

			myfile.close();

			m_AudioManager->PlayPickupSound();
		}
	}
	else
	{
		//
		// Reading From File??
		//
		std::string line;
		std::ifstream myfile("Output/FirstEmptyInventorySlot.txt");
		myfile.is_open();
		int firstEmpty;
		myfile >> firstEmpty;

		if (_block->m_Type == CBlock::BLOCKTYPE::DOOR)
		{
			_block->GetShape().setScale(0.23f, 0.23f);
			_block->GetShape().setOrigin(0.0f, m_Shape.getGlobalBounds().height / 2 - m_Shape.getGlobalBounds().height / 3.75f);
		}
		else
		{
			_block->GetShape().setScale(0.4f, 0.4f);
			_block->GetShape().setOrigin(_block->GetShape().getGlobalBounds().width / 2, _block->GetShape().getGlobalBounds().height / 2);
		}
		
		m_RenderWindow->mapCoordsToPixel(_block->GetShape().getPosition());
		m_InventorySize++;
		// increase number of that type
		m_InventoryStackValues[firstEmpty]++;
		_block->m_PositionInInventory = firstEmpty;
		m_InventoryMap.insert_or_assign(firstEmpty, *_block);
		//std::cout << "Added Item To Inventory - ";
		std::cout << firstEmpty << std::endl;

		myfile.close();

		m_AudioManager->PlayPickupSound();
	}
}

void CPlayer::AddItemToInventory(CBlock* _block, int _position, bool _canStack)
{
	if (_canStack)
	{
		if (IsBlockInInventory(_block))
		{
			m_AudioManager->PlayPickupSound();
		}
		else
		{
			_block->GetShape().setScale(0.4f, 0.4f);
			_block->GetShape().setOrigin(_block->GetShape().getGlobalBounds().width / 2, _block->GetShape().getGlobalBounds().height / 2);
			m_RenderWindow->mapCoordsToPixel(_block->GetShape().getPosition());
			m_InventorySize++;
			// increase number of that type
			m_InventoryStackValues[_position]++;
			_block->m_PositionInInventory = _position;
			m_InventoryMap.insert_or_assign(_position, *_block);
			//std::cout << "Added Item To Inventory - ";
			//std::cout << _position << std::endl;

			m_AudioManager->PlayPickupSound();
		}
	}
	else
	{
		_block->GetShape().setScale(0.4f, 0.4f);
		_block->GetShape().setOrigin(_block->GetShape().getGlobalBounds().width / 2, _block->GetShape().getGlobalBounds().height / 2);
		m_RenderWindow->mapCoordsToPixel(_block->GetShape().getPosition());
		m_InventorySize++;
		// increase number of that type
		m_InventoryStackValues[_position]++;
		_block->m_PositionInInventory = _position;
		m_InventoryMap.insert_or_assign(_position, *_block);
		//std::cout << "Added Item To Inventory - ";
		std::cout << _position << std::endl;

		m_AudioManager->PlayPickupSound();
	}
	
}

bool CPlayer::IsItemInventory(CBlock::BLOCKTYPE _type)
{
	for (int i = 0; i < m_InventoryMap.size(); i++)
	{
		if (m_InventoryMap[i].m_Type == _type)
		{
			return true;
		}
	}
	return false;
}

int CPlayer::GetPositionInInventory(CBlock::BLOCKTYPE _type)
{
	for (int i = 0; i < m_InventoryMap.size(); i++)
	{
		if (m_InventoryMap[i].m_Type == _type)
		{
			return m_InventoryMap[i].m_PositionInInventory;
		}
	}
	return NULL;
}

int CPlayer::IsItemInventory(CBlock::BLOCKTYPE _type, bool _bReturnAmount)
{
	int tempCount = 0;
	for (int i = 0; i < m_InventoryStackValues.size(); i++)
	{
		if (m_InventoryMap[i].m_Type == _type)
		{
			tempCount = m_InventoryStackValues[i];
			return tempCount;
		}
	}
	return tempCount;
}

void CPlayer::RemoveItemFromInventory(int _position)
{
	std::map<int, CBlock>::iterator it = m_InventoryMap.begin();

	while (it != m_InventoryMap.end())
	{
		if (it->first == _position)
		{
			if (it->second.m_Type == CBlock::BLOCKTYPE::PICKAXE)
			{
				m_Pickaxe = nullptr;
			}

			while (m_InventoryStackValues[_position] > 0)
			{
				m_InventoryStackValues[_position]--;
			}
			it = m_InventoryMap.erase(it);

			return;
		}
		it++;
	}
}

void CPlayer::ToggleInventoryUI(std::list<CChest>& _chests)
{
	m_bInventoryOpen = !m_bInventoryOpen;
	m_bCanMove = !m_bInventoryOpen;
	m_bCanPlace = !m_bInventoryOpen;

	if (m_bInventoryOpen == false)
	{
		for (CChest& chest : _chests)
		{
			chest.m_bIsOpen = false;
		}
		m_bPlayerIsInChest = false;
	}
}

bool CPlayer::SelectedItemIsEmpty()
{
	std::map<int, CBlock>::iterator it = m_InventoryMap.begin();

	while (it != m_InventoryMap.end())
	{
		if (m_CurrentItemIndex == it->second.m_PositionInInventory)
		{
			//std::cout << " I Have A Block!" << std::endl;
			return false;
		}
		it++;
	}
	return true;
}

template <typename T>
int CPlayer::Mine(std::list<T>& m_Chunk, sf::Sprite& _mousePositionSprite)
{
	typename std::list<T>::iterator it;
	for (it = m_Chunk.begin(); it != m_Chunk.end(); it++)
	{
		if (it->GetShape().getPosition() == _mousePositionSprite.getPosition() && _mousePositionSprite.getGlobalBounds().contains(m_MousePos))
		{
			if (!bMouseNotOver(m_Chunk, _mousePositionSprite))
			{
				if (m_MineTimer->getElapsedTime() >= sf::Time(sf::seconds(0.3f)))
				{
					if (it->m_BlockStrength <= m_Pickaxe->m_PickaxePower * 5)
					{
						it->m_BlockStrength -= 1 * m_Pickaxe->m_PickaxePower;

						m_ParticleTimer.restart();
						m_TestParticles->SetEmitter(_mousePositionSprite.getPosition());
					}

					if (it->m_BlockStrength > 0)
					{
						
					}
					else if (it->m_BlockStrength <= 0)
					{
						if (it->m_Type == CBlock::BLOCKTYPE::DOOR)
						{
							m_Door = new CDoor();
							AddItemToInventory(m_Door, false);
							it = m_Chunk.erase(it);
						}
						else if (it->m_Type == CBlock::BLOCKTYPE::CHEST)
						{
							m_Block = new CBlock(m_TextureMaster->m_Chest, CBlock::BLOCKTYPE::CHEST);
							AddItemToInventory(m_Block);
							it = m_Chunk.erase(it);
						}
						else if (it->m_Type == CBlock::BLOCKTYPE::FURNACE)
						{
							m_Block = new CBlock(m_TextureMaster->m_Furnace, CBlock::BLOCKTYPE::FURNACE);
							AddItemToInventory(m_Block);
							it = m_Chunk.erase(it);
						}
						else if (it->m_Type == CBlock::BLOCKTYPE::WORKBENCH)
						{
							m_Block = new CBlock(m_TextureMaster->m_WorkBench, CBlock::BLOCKTYPE::WORKBENCH);
							AddItemToInventory(m_Block);
							it = m_Chunk.erase(it);
						}
						else
						{
							m_Block = new CBlock(it->m_Texture, it->m_Type);
							AddItemToInventory(m_Block);
							it = m_Chunk.erase(it);
						}
						m_Block = nullptr;
						m_Door = nullptr;
					}
					

					m_MineTimer->restart();

					m_AudioManager->PlayGroundMine();
					break;
				}
			}
		}
	}
	
}

void CPlayer::PlaceBlock(std::list<CBlock>& m_Chunk, sf::Sprite& _mousePositionSprite)
{
	if (m_MineTimer->getElapsedTime() >= sf::Time(sf::seconds(0.2f)))
	{
		// Block
		m_Block = new CBlock(m_RenderWindow, *m_World, m_InventoryMap[m_CurrentItemIndex].GetShape().getTexture(), m_Scale, _mousePositionSprite.getPosition().x, _mousePositionSprite.getPosition().y,false, m_InventoryMap[m_CurrentItemIndex].m_Type);
		if (m_Block->m_Type == CBlock::BLOCKTYPE::SAND)
		{
			m_World->DestroyBody(m_Block->m_Body);
			m_Block->m_BodyDef.type = b2_dynamicBody;
			m_Block->m_BodyDef.position = b2Vec2(_mousePositionSprite.getPosition().x / m_Scale, (_mousePositionSprite.getPosition().y / m_Scale));
			m_Block->m_BodyDef.fixedRotation = true;
			m_Block->m_Body = m_World->CreateBody(&m_Block->m_BodyDef);

			m_Block->m_b2pShape.SetAsBox((100 / 2) / m_Scale, (100 / 2) / m_Scale);

			m_Block->m_FixtureDef.density = 2.0f;
			m_Block->m_FixtureDef.shape = &m_Block->m_b2pShape;
			m_Block->m_FixtureDef.friction = 2.0f;
			m_Block->m_Body->CreateFixture(&m_Block->m_FixtureDef);
		}
		m_Chunk.push_back(*m_Block);
		m_Block = nullptr;
		m_Door = nullptr;

		if (m_InventoryStackValues[m_CurrentItemIndex] <= 1)
		{
			RemoveItemFromInventory(m_CurrentItemIndex);
		}
		else
		{
			m_InventoryStackValues[m_CurrentItemIndex]--;
		}

		// Audio
		m_AudioManager->PlayBlockPlace();
		m_MineTimer->restart();
	}
}

void CPlayer::PlaceDoor(std::list<CDoor>& m_Doors, sf::Sprite& _mousePositionSprite)
{
	if (m_MineTimer->getElapsedTime() >= sf::Time(sf::seconds(0.2f)))
	{
		// Door
		m_Door = new CDoor(m_RenderWindow, *m_World, m_Scale, _mousePositionSprite.getPosition().x, _mousePositionSprite.getPosition().y);
		m_Door->SetSizeAndPos(_mousePositionSprite.getPosition().x, _mousePositionSprite.getPosition().y, 100, 200);
		m_Doors.push_back(*m_Door);
		m_Door->m_ArrayIndex = (m_Shape.getPosition().x);
		m_Block = nullptr;
		m_Door = nullptr;

		// Decrement Stack Counter / Remove Item From Inventory
		if (m_InventoryStackValues[m_CurrentItemIndex] <= 1)
		{
			RemoveItemFromInventory(m_CurrentItemIndex);
		}
		else
		{
			m_InventoryStackValues[m_CurrentItemIndex]--;
		}

		// Audio
		m_AudioManager->PlayBlockPlace();
		m_MineTimer->restart();
	}
}

void CPlayer::PlaceChest(std::list<CChest>& m_Chests, sf::Sprite& _mousePositionSprite)
{
	if (m_MineTimer->getElapsedTime() >= sf::Time(sf::seconds(0.2f)))
	{
		// Chest
		m_Chest = new CChest(m_RenderWindow, *m_World, m_Scale, _mousePositionSprite.getPosition().x, _mousePositionSprite.getPosition().y);
		m_Chest->SetSizeAndPos(_mousePositionSprite.getPosition().x, _mousePositionSprite.getPosition().y, 100, 100);
		m_Chests.push_back(*m_Chest);
		m_Chest->m_ArrayIndex = (m_Shape.getPosition().x);
		m_Chest = nullptr;

		// Decrement Stack Counter / Remove Item From Inventory
		if (m_InventoryStackValues[m_CurrentItemIndex] <= 1)
		{
			RemoveItemFromInventory(m_CurrentItemIndex);
		}
		else
		{
			m_InventoryStackValues[m_CurrentItemIndex]--;
		}

		// Audio
		m_AudioManager->PlayBlockPlace();
		m_MineTimer->restart();
	}
}

void CPlayer::PlaceFurnace(std::list<CFurnace>& m_Furnaces, sf::Sprite& _mousePositionSprite)
{
	if (m_MineTimer->getElapsedTime() >= sf::Time(sf::seconds(0.2f)))
	{
		// Chest
		m_Furnace = new CFurnace(m_RenderWindow, *m_World, m_Scale, _mousePositionSprite.getPosition().x, _mousePositionSprite.getPosition().y);
		m_Furnace->SetSizeAndPos(_mousePositionSprite.getPosition().x, _mousePositionSprite.getPosition().y, 100, 100);
		m_Furnace->m_ArrayIndex = (m_Shape.getPosition().x);
		m_Furnaces.push_back(*m_Furnace);
		m_Furnace = nullptr;

		// Decrement Stack Counter / Remove Item From Inventory
		if (m_InventoryStackValues[m_CurrentItemIndex] <= 1)
		{
			RemoveItemFromInventory(m_CurrentItemIndex);
		}
		else
		{
			m_InventoryStackValues[m_CurrentItemIndex]--;
		}

		// Audio
		m_AudioManager->PlayBlockPlace();
		m_MineTimer->restart();
	}
}

void CPlayer::PlaceWorkBench(std::list<CWorkBench>& m_WorkBenches, sf::Sprite& _mousePositionSprite)
{
	if (m_MineTimer->getElapsedTime() >= sf::Time(sf::seconds(0.2f)))
	{
		// Chest
		m_WorkBench = new CWorkBench(m_RenderWindow, *m_World, m_Scale, _mousePositionSprite.getPosition().x, _mousePositionSprite.getPosition().y);
		m_WorkBench->SetSizeAndPos(_mousePositionSprite.getPosition().x, _mousePositionSprite.getPosition().y, 100, 100);
		m_WorkBench->m_ArrayIndex = (m_Shape.getPosition().x);
		m_WorkBenches.push_back(*m_WorkBench);
		m_WorkBench = nullptr;

		// Decrement Stack Counter / Remove Item From Inventory
		if (m_InventoryStackValues[m_CurrentItemIndex] <= 1)
		{
			RemoveItemFromInventory(m_CurrentItemIndex);
		}
		else
		{
			m_InventoryStackValues[m_CurrentItemIndex]--;
		}

		// Audio
		m_AudioManager->PlayBlockPlace();
		m_MineTimer->restart();
	}
}

void CPlayer::CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor)
{
	//falling object physics
	m_BodyDef = b2BodyDef();
	m_BodyDef.position = b2Vec2(_posX / m_Scale, (_posY / m_Scale));
	m_BodyDef.type = _type;
	m_BodyDef.enabled = true;
	m_BodyDef.linearDamping = 2.0f;
	m_BodyDef.angularDamping = 0.0f;
	m_BodyDef.fixedRotation = true;
	m_BodyDef.allowSleep = false;
	m_BodyDef.gravityScale = 10.0f;
	m_BodyDef.bullet = false;
	m_Body = m_World->CreateBody(&m_BodyDef);

	b2Vec2 vertices[8];
	vertices[0].Set(-45 / m_Scale, -85 / m_Scale);
	vertices[1].Set(-40 / m_Scale, -90 / m_Scale);
	vertices[2].Set(45 / m_Scale, -85 / m_Scale);
	vertices[3].Set(40 / m_Scale, -90 / m_Scale);
	vertices[4].Set(45 / m_Scale, 85 / m_Scale);
	vertices[5].Set(40 / m_Scale, 90 / m_Scale);
	vertices[6].Set(-45 / m_Scale, 85 / m_Scale);
	vertices[7].Set(-40 / m_Scale, 90 / m_Scale);

	m_b2pShape.Set(vertices, 8);

	m_FixtureDef.density = 0.8f;
	m_FixtureDef.shape = &m_b2pShape;
	m_FixtureDef.friction = 0.5f;
	m_FixtureDef.restitution = 0.01f;
	m_FixtureDef.filter.categoryBits = 0x0002;
	m_FixtureDef.filter.groupIndex = -3;
	m_FixtureDef.filter.maskBits = 0x0006;
	m_FixtureDef.filter.maskBits = 0x0004;
	m_Body->CreateFixture(&m_FixtureDef);

	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

void CPlayer::DestroyBody()
{
	if (m_World != nullptr && m_Body != nullptr)
	{
		m_World->DestroyBody(m_Body);
		m_Body = nullptr;
	}
}

void CPlayer::Lst_MoveToFront(std::list<CBlock>& list, std::list<CBlock>::iterator element)
{
	if (element != list.begin())
	{
		list.splice(list.begin(), list, element, std::next(element));
	}
}

void CPlayer::Lst_MoveToFront(std::list<CDoor>& list, std::list<CDoor>::iterator element)
{
	if (element != list.begin())
	{
		list.splice(list.begin(), list, element, std::next(element));
	}
}

void CPlayer::Lst_MoveToFront(std::list<CChest>& list, std::list<CChest>::iterator element)
{
	if (element != list.begin())
	{
		list.splice(list.begin(), list, element, std::next(element));
	}
}

b2World* CPlayer::GetWorld()
{
	return m_World;
}
