#include "Player.h"

CPlayer::CPlayer(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, CAudioManager* _audioManager, CTextureMaster* _textureMaster, bool* _changeScenes, int* _sceneValue)
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
	m_Sword = nullptr;
	m_bChangeScenes = _changeScenes;
	m_SceneValue = _sceneValue;

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

	// Sprite
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
	m_InventoryMap.clear();
	m_InventoryStackValues.clear();
	m_Projectiles.clear();

	DestroyBody();

	if (m_Pickaxe != nullptr)
	{
		delete m_Pickaxe;
	}
	if (m_Bow != nullptr)
	{
		delete m_Bow;
	}
	if (m_Sword != nullptr)
	{
		delete m_Sword;
	}
	delete m_MapIconTexRight;
	delete m_MapIconTex;
	delete m_PlayerRightTex;
	delete m_TestParticles;
	delete m_PlayerLeftTex;
	delete m_AnimationTimer;
	delete m_MineTimer;
	delete m_DamageTimer;
	delete m_DamageIndicatorTimer;
	m_SceneValue = nullptr;
	m_bChangeScenes = nullptr;
	m_Sword = nullptr;
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
	m_Bow = nullptr;
	m_WorkBench = nullptr;
	m_Pickaxe = nullptr;
	m_TestParticles = nullptr;
	m_Furnace = nullptr;
	m_Chest = nullptr;
	m_Door = nullptr;
	m_MapIconTexRight = nullptr;
	m_MapIconTex = nullptr;
	m_PlayerRightTex = nullptr;
}

void CPlayer::Start()
{
	m_AnimationTimer = new sf::Clock();
	m_MineTimer = new sf::Clock();
	m_DamageTimer = new sf::Clock();
	m_DamageIndicatorTimer = new sf::Clock();

	InitInventory();
	InputInventoryToFile();

	if (!IsItemInInventory(CBlock::BLOCKTYPE::PICKAXE))
	{
		//Starting Items
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
	}
	

	// Particle System Start / Init
	m_TestParticles->Start();
}

void CPlayer::Update(sf::Vector2f _mousePos)
{
	// Player Is Red From Damage ? Reset Colour
	if (m_DamageIndicatorTimer->getElapsedTime().asSeconds() >= 0.2f && m_Shape.getColor() == sf::Color(100, 0, 0, 255))
	{
		m_Shape.setColor(sf::Color::White);
		m_DamageIndicatorTimer->restart();
	}

	// MousePos (sf::Vector2f)
	m_MousePos = _mousePos;

	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);

	// Set Map Icon Position
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
	// Player Has Sword ? Orient Sprite
	else if (m_Sword != nullptr)
	{
		m_Sword->FlipSprite(m_Shape.getPosition(), m_Shape, m_PlayerLeftTex, m_PlayerRightTex);
	}

	// Holdable Items (Pickaxe, Bow e.t.c)
	std::map<int, CBlock>::iterator cit;
	for (cit = m_InventoryMap.begin(); cit != m_InventoryMap.end(); cit++)
	{
		if (cit->second.m_bIsItemAndSelected == true)
		{
			if (m_Pickaxe == nullptr && cit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE)
			{
				std::cout << "New Pickaxe Created!" << std::endl;
				m_Pickaxe = new CPickaxe(m_RenderWindow, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, cit->second.m_PickType);
				cit->second.m_bIsItemAndSelected = false;
			}
			else if (m_Bow == nullptr && cit->second.m_Type == CBlock::BLOCKTYPE::BOW)
			{
				std::cout << "New Bow Created!" << std::endl;
				m_Bow = new Bow(m_RenderWindow, *m_World, m_Scale,m_Shape.getPosition().x, m_Shape.getPosition().y, cit->second.m_BowType);
				cit->second.m_bIsItemAndSelected = false;
			}
			else if (m_Bow == nullptr && cit->second.m_Type == CBlock::BLOCKTYPE::SWORD)
			{
				std::cout << "New Sword Created!" << std::endl;
				m_Sword = new CSword(m_RenderWindow, m_Scale, m_Shape.getPosition().x, m_Shape.getPosition().y, cit->second.m_SwordType);
				cit->second.m_bIsItemAndSelected = false;
			}

			// Outer For
			break;
		}
	}

	if (m_InventoryMap[m_CurrentItemIndex].m_Type != CBlock::BLOCKTYPE::PICKAXE && m_Pickaxe != nullptr)
	{
		delete m_Pickaxe;
		m_Pickaxe = nullptr;
	}
	else if (m_InventoryMap[m_CurrentItemIndex].m_Type != CBlock::BLOCKTYPE::BOW && m_Bow != nullptr)
	{
		delete m_Bow;
		m_Bow = nullptr;
	}
	else if (m_InventoryMap[m_CurrentItemIndex].m_Type != CBlock::BLOCKTYPE::SWORD && m_Sword != nullptr)
	{
		delete m_Sword;
		m_Sword = nullptr;
	}
	
	// Collision Constacts
	b2Contact* contact;
	for (contact = m_World->GetContactList(); contact; contact = contact->GetNext())
	{
		// b2WorldManifold (positions of collision e.t.c)
		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);

		// Grab Fixtures Of All Collisions
		b2Fixture* a = contact->GetFixtureA();
		b2Fixture* b = contact->GetFixtureB();

		// Get Lenear Velocity For All Bodies And Player
		b2Vec2 vel1 = m_Body->GetLinearVelocityFromWorldPoint(m_Body->GetPosition());
		b2Vec2 vel2 = b->GetBody()->GetLinearVelocityFromWorldPoint(worldManifold.points[0]);

		// Calculate Impact Velocity
		b2Vec2 impactVelocity = vel1 - vel2;
		
		// Cast sf::Vector2f to b2Vec2 For Sprite Position
		b2Vec2 worldposition = { m_Shape.getPosition().x, m_Shape.getPosition().y };
		
		if ((a->GetBody() == m_Body || b->GetBody() == m_Body) && (b->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 0x0006 || a->GetBody()->GetFixtureList()->GetFilterData().categoryBits == 0x0006))
		{
			// Velocity.y > 0 ? bCanJump = true
			if ((vel1.y <= 0.5f && vel1.y >= -0.5f))
			{
				m_bCanJump = true;
			}

			// Fall Damage
			if (impactVelocity.y <= 82.8f && impactVelocity.y >= 62.0f && m_bCanFallDamage)
			{
				//std::cout << impactVelocity.y << std::endl;
				TakeDamage(impactVelocity.y / 2);
			}
			else if (impactVelocity.y > 82.8f && m_bCanFallDamage)
			{
				//std::cout << impactVelocity.y << std::endl;
				TakeDamage(impactVelocity.y * 2);
			}

			// Velocity.y > 0 ? bCanJump = true
			if ((vel1.y > 2.0f || vel1.y <= -2.0f))
			{
				m_bCanFallDamage = true;
			}
			else
			{
				m_bCanFallDamage = false;
			}
		}
		// Velocity.y == 0 ? bCanJump = false
		else if (vel1.y < -0.1f || vel1.y > 0.1f)
		{
			m_bCanJump = false;
		}
		
		a = nullptr;
		b = nullptr;
	}
	contact = nullptr;

	// Player Has Bow Out ? Attack()
	if (m_Bow != nullptr)
	{
		Attack(m_Bow);
	}
	else if (m_Sword != nullptr)
	{
		Attack(m_Sword);
	}

	// Update All Projectiles
	for (CProjectile& projectile : m_Projectiles)
	{
		projectile.Update();
	}

	// Delete All Collided Projectiles
	std::list<CProjectile>::iterator pit = m_Projectiles.begin();
	while (pit != m_Projectiles.end())
	{
		if (pit->m_bMARKASDESTROY)
		{
			pit = m_Projectiles.erase(pit);
		}
		pit++;
	}

	// Update Particle System (Mine Particles)
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
	// Player Has PickAxe ? Render()
	if (m_Pickaxe != nullptr)
	{
		m_Pickaxe->Render();
	}
	// Player Has Bow ? Render()
	if (m_Bow != nullptr)
	{
		m_Bow->Render();
	}
	// Player Has Sword ? Render()
	if (m_Sword != nullptr)
	{
		m_Sword->Render();
	}

	// Draw All Projectiles
	for (CProjectile& projectile : m_Projectiles)
	{
		m_RenderWindow->draw(projectile.GetShape(), _defaultShader);
	}

	// Player Sprite
	m_RenderWindow->draw(m_Shape, _defaultShader);

	// Draw All Particles (Mining)
	m_RenderWindow->draw(*m_TestParticles);
}

void CPlayer::Movement()
{
	// Cast sf::Vector2f to b2Vec2 For Sprite Position
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

		// Set Velocity (b2Vec2)
		m_Velocity = b2Vec2(x, y);

		// Apply Velocity Through Impulse
		m_Body->ApplyLinearImpulseToCenter(m_MoveSpeed / 2 * m_Velocity, true);

		// Normalization (Using Opposing Impluse)
		float Mag = sqrt((m_Body->GetLinearVelocityFromWorldPoint(worldposition).x * m_Body->GetLinearVelocityFromWorldPoint(worldposition).x));
		if (Mag > m_MoveSpeed)
		{
			m_Body->ApplyLinearImpulseToCenter(-1 * m_MoveSpeed * b2Vec2(m_Velocity.x, 0.0f), true);
		}

		// Input Right ? Animation
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
		// Input Left ? Animation
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
		// Input  ? NA : Animation && Sound
		else if (m_Velocity.x == 0.0f)
		{
			// Input Right ? NA : Animation
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
			// Input Left ? NA : Animation
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
			// Input ? NA : Default Animation State
			else
			{
				m_Shape.setTextureRect(sf::IntRect(0, 0, 100, 200));
			}
		}

		// Input Right ? Audio
		if (m_Body->GetLinearVelocityFromWorldPoint(worldposition).x > 15.0f)
		{
			if (m_AudioManager != nullptr)
			{
				m_AudioManager->PlayRunning();
			}
		}
		// Input Left ? Audio
		else if (m_Body->GetLinearVelocityFromWorldPoint(worldposition).x < -15.0f)
		{
			if (m_AudioManager != nullptr)
			{
				m_AudioManager->PlayRunning();
			}
		}

		// Debug
		//std::cout << m_Shape.getPosition().x << std::endl;
		//std::cout << m_Shape.getPosition().y << std::endl;
	}
}

void CPlayer::Movement(sf::Event& _event)
{
	// Cast sf::Vector2f to b2Vec2 For Sprite Position
	b2Vec2 worldposition = { m_Shape.getPosition().x, m_Shape.getPosition().y };

	// Input
	if (m_bCanMove)
	{
		int x = 0;
		int y = 0;

		// Jump (Space)
		if (_event.type == sf::Event::KeyPressed && (_event.key.code == sf::Keyboard::Key::Space || _event.key.code == sf::Keyboard::Key::W ) && m_bCanJump)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -420.0f), true);
			m_bCanJump = false;
		}
	}
}

void CPlayer::Interact(std::list<CFurnace>& m_Furnaces, std::list<CChest>& m_Chests, std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite, std::list<CWorkBench>& m_WorkBenches, std::list<CBlock>& m_Tourches )
{
	// Mag From Mouse To Player
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
			else if (!bMouseNotOver(m_Tourches, _mousePositionSprite))
			{
				Mine(m_Tourches, _mousePositionSprite);
			}
		}
		else if (m_Bow != nullptr)
		{
		}
		else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::POTION)
		{
			switch (m_InventoryMap[m_CurrentItemIndex].m_PotionType)
			{
			case CBlock::POTIONTYPE::HPSMALL:
			{
				if (m_HPPotionTimer.getElapsedTime().asSeconds() >= 10.0f)
				{
					Heal(50.0f);

					// Decrement Stack Counter / Remove Item From Inventory
					if (m_InventoryStackValues[m_CurrentItemIndex] <= 1)
					{
						RemoveItemFromInventory(m_CurrentItemIndex);
					}
					else
					{
						m_InventoryStackValues[m_CurrentItemIndex]--;
					}

					m_AudioManager->PlayPotionDrink();

					m_HPPotionTimer.restart();
				}

				break;
			}
			case CBlock::POTIONTYPE::HPLARGE:
			{
				if (m_HPPotionTimer.getElapsedTime().asSeconds() >= 10.0f)
				{
					Heal(100.0f);

					// Decrement Stack Counter / Remove Item From Inventory
					if (m_InventoryStackValues[m_CurrentItemIndex] <= 1)
					{
						RemoveItemFromInventory(m_CurrentItemIndex);
					}
					else
					{
						m_InventoryStackValues[m_CurrentItemIndex]--;
					}

					m_AudioManager->PlayPotionDrink();

					m_HPPotionTimer.restart();
				}

				break;
			}
			default:
				break;
			}
		}
		// Left Mouse Clicked And In Empty Space
		else if (bMouseNotOver(m_Chunk, _mousePositionSprite) && bMouseNotOver(m_Doors, _mousePositionSprite) && !SelectedItemIsEmpty() && bMouseNotOver(m_Chests, _mousePositionSprite) && bMouseNotOver(m_Furnaces, _mousePositionSprite) && bMouseNotOver(m_WorkBenches, _mousePositionSprite) && bMouseNotOver(m_Tourches, _mousePositionSprite) && _mousePositionSprite.getGlobalBounds().contains(m_MousePos))
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
			// Potions
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::POTION)
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
			// ARROWS
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::PROJECTILE)
			{
			}
			// Red Slime
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::REDSLIME)
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
			// SWORDS
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::SWORD)
			{
			}
			// BROKEN SWORD
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::BROKENSWORD)
			{
			}
			// EMPTY BEAKER
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::EMPTYBEAKER)
			{
			}
			// EMPTY BEAKER
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::TOURCH)
			{
				PlaceTourch(m_Tourches, _mousePositionSprite);
			}
			// PORTAL KEY GREEN
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::PORTALKEYGREEN)
			{
				*m_bChangeScenes = true;
				*m_SceneValue = _JUNGLE_;
			}
			// PORTAL KEY RED
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::PORTALKEYYELLOW)
			{
				*m_bChangeScenes = true;
				*m_SceneValue = _SAND_;
			}
			// PORTAL KEY YELLOW
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::PORTALKEYRED)
			{
				*m_bChangeScenes = true;
				*m_SceneValue = _HELL_;
			}
			// PORTAL KEY PLAINS
			else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::PORTALKEYPLAINS)
			{
				*m_bChangeScenes = true;
				*m_SceneValue = _PLAINS_;
			}
			// Place Block
			else
			{
				PlaceBlock(m_Chunk, _mousePositionSprite);
			}
		}
		// PORTAL KEY GREEN
		else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::PORTALKEYGREEN)
		{
		*m_bChangeScenes = true;
		*m_SceneValue = _JUNGLE_;
		}
		// PORTAL KEY RED
		else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::PORTALKEYYELLOW)
		{
		*m_bChangeScenes = true;
		*m_SceneValue = _SAND_;
		}
		// PORTAL KEY YELLOW
		else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::PORTALKEYRED)
		{
		*m_bChangeScenes = true;
		*m_SceneValue = _HELL_;
		}
		// PORTAL KEY PLAINS
		else if (m_InventoryMap[m_CurrentItemIndex].m_Type == CBlock::BLOCKTYPE::PORTALKEYPLAINS)
		{
		*m_bChangeScenes = true;
		*m_SceneValue = _PLAINS_;
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

void CPlayer::Attack(CBlock* _item)
{
	if (_item->m_BowType == CBlock::BOWTYPE::IRONGUN)
	{
		m_AttackSpeed = 0.7f;
	}
	else if (_item->m_BowType == CBlock::BOWTYPE::GOLDGUN)
	{
		m_AttackSpeed = 0.6f;
	}
	else if (_item->m_BowType == CBlock::BOWTYPE::PURPLEGUN)
	{
		m_AttackSpeed = 0.5f;
	}
	else if (_item->m_BowType == CBlock::BOWTYPE::GOLDENGUN)
	{
		m_AttackSpeed = 0.4f;
	}
	else if (_item->m_BowType == CBlock::BOWTYPE::GREENGUN)
	{
		m_AttackSpeed = 0.3f;
	}
	else
	{
		m_AttackSpeed = 0.8f;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_AttackTimer.getElapsedTime().asSeconds() >= m_AttackSpeed && !m_bInventoryOpen)
	{
		if (_item->m_Type == CBlock::BLOCKTYPE::BOW)
		{
			for (int i = 0; i < m_InventoryMap.size(); i++)
			{
				if (m_InventoryMap[i].m_Type == CBlock::BLOCKTYPE::PROJECTILE && m_InventoryStackValues[i] > 0 && _item->m_BowType <= CBlock::BOWTYPE::GREEN && m_InventoryMap[i].m_ProjectileType != CBlock::PROJECTILETYPE::GOLDENBULLET && m_InventoryMap[i].m_ProjectileType != CBlock::PROJECTILETYPE::PURPLEBULLET && m_InventoryMap[i].m_ProjectileType != CBlock::PROJECTILETYPE::GOLDBULLET && m_InventoryMap[i].m_ProjectileType != CBlock::PROJECTILETYPE::IRONBULLET)
				{
					switch (m_InventoryMap[i].m_ProjectileType)
					{
					case CBlock::PROJECTILETYPE::ARROW:
					{
						m_AudioManager->PlayBowShot();
						m_Projectile = new CProjectile(*m_World, m_Bow->m_Sprite->getPosition().x, m_Bow->m_Sprite->getPosition().y, m_MousePos, CBlock::PROJECTILETYPE::ARROW, m_Bow);
						break;
					}
					case CBlock::PROJECTILETYPE::FIREARROW:
					{
						m_AudioManager->PlayBowShot();
						m_Projectile = new CProjectile(*m_World, m_Bow->m_Sprite->getPosition().x, m_Bow->m_Sprite->getPosition().y, m_MousePos, CBlock::PROJECTILETYPE::FIREARROW, m_Bow);
						break;
					}
					case CBlock::PROJECTILETYPE::CURSEDARROW:
					{
						m_AudioManager->PlayBowShot();
						m_Projectile = new CProjectile(*m_World, m_Bow->m_Sprite->getPosition().x, m_Bow->m_Sprite->getPosition().y, m_MousePos, CBlock::PROJECTILETYPE::CURSEDARROW, m_Bow);
						break;
					}
					case CBlock::PROJECTILETYPE::POISONARROW:
					{
						m_AudioManager->PlayBowShot();
						m_Projectile = new CProjectile(*m_World, m_Shape.getPosition().x, m_Shape.getPosition().y - 50, m_MousePos, CBlock::PROJECTILETYPE::POISONARROW, m_Bow);
						break;
					}
					default:
						break;
					}

					if (m_Projectile != nullptr)
					{
						m_Projectiles.push_back(*m_Projectile);

						if (m_InventoryStackValues[i] <= 1)
						{
							RemoveItemFromInventory(i);
						}
						else
						{
							m_InventoryStackValues[i]--;
						}
					}
					
					break;
				}
				else if (m_InventoryMap[i].m_Type == CBlock::BLOCKTYPE::PROJECTILE && m_InventoryStackValues[i] > 0 && _item->m_BowType > CBlock::BOWTYPE::GREEN && m_InventoryMap[i].m_ProjectileType != CBlock::PROJECTILETYPE::POISONARROW && m_InventoryMap[i].m_ProjectileType != CBlock::PROJECTILETYPE::CURSEDARROW && m_InventoryMap[i].m_ProjectileType != CBlock::PROJECTILETYPE::FIREARROW && m_InventoryMap[i].m_ProjectileType != CBlock::PROJECTILETYPE::ARROW)
				{
					switch (m_InventoryMap[i].m_ProjectileType)
					{
					case CBlock::PROJECTILETYPE::IRONBULLET:
					{
						m_AudioManager->PlayGunShot();
						m_Projectile = new CProjectile(*m_World, m_Bow->m_Sprite->getPosition().x, m_Bow->m_Sprite->getPosition().y, m_MousePos, CBlock::PROJECTILETYPE::IRONBULLET);
						break;
					}
					case CBlock::PROJECTILETYPE::GOLDBULLET:
					{
						m_AudioManager->PlayGunShot();
						m_Projectile = new CProjectile(*m_World, m_Bow->m_Sprite->getPosition().x, m_Bow->m_Sprite->getPosition().y, m_MousePos, CBlock::PROJECTILETYPE::GOLDBULLET);
						break;
					}
					case CBlock::PROJECTILETYPE::PURPLEBULLET:
					{
						m_AudioManager->PlayGunShot();
						m_Projectile = new CProjectile(*m_World, m_Bow->m_Sprite->getPosition().x, m_Bow->m_Sprite->getPosition().y, m_MousePos, CBlock::PROJECTILETYPE::PURPLEBULLET);
						break;
					}
					case CBlock::PROJECTILETYPE::GOLDENBULLET:
					{
						m_AudioManager->PlayGunShot();
						m_Projectile = new CProjectile(*m_World, m_Bow->m_Sprite->getPosition().x, m_Bow->m_Sprite->getPosition().y, m_MousePos, CBlock::PROJECTILETYPE::GOLDENBULLET);
						break;
					}
					default:
					{
						break;
					}
					}
					
					if (m_Projectile != nullptr)
					{
						m_Projectiles.push_back(*m_Projectile);

						if (m_InventoryStackValues[i] <= 1)
						{
							RemoveItemFromInventory(i);
						}
						else
						{
							m_InventoryStackValues[i]--;
						}
					}
					break;
				}

			}
			m_Projectile = nullptr;
			m_AttackTimer.restart();
		}
	}
}

void CPlayer::InitInventory()
{
	// Init Inventory
	for (int i = 0; i < 50; i++)
	{
		m_InventoryStackValues[i];
	}
	for (int i = 0; i < 50; i++)
	{
		m_InventoryMap[i];
	}
}

b2Body* CPlayer::GetBody()
{
	return m_Body;
}

CPickaxe* CPlayer::GetPickaxe()
{
	return m_Pickaxe;
}

void CPlayer::SetPickaxe(CPickaxe* _pickaxe)
{
	m_Pickaxe = _pickaxe;
}

Bow* CPlayer::GetBow()
{
	return m_Bow;
}

void CPlayer::SetBow(Bow* _bow)
{
	m_Bow = _bow;
}

CSword* CPlayer::GetSword()
{
	return m_Sword;
}

void CPlayer::SetSword(CSword* _sword)
{
	m_Sword = _sword;
}

int CPlayer::GetCurrentHP()
{
	return m_Health;
}

int CPlayer::GetMaxHP()
{
	return m_MaxHP;
}

void CPlayer::TakeDamage(float _damage)
{
	if (m_DamageTimer->getElapsedTime().asSeconds() >= 0.3f)
	{

		std::cout << "Player Took Damage!" << std::endl;
		m_Health -= _damage;
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
		if (it->second.m_Type == _block->m_Type && it->second.m_PickType == _block->m_PickType && it->second.m_ProjectileType == _block->m_ProjectileType && it->second.m_PotionType == _block->m_PotionType && it->second.m_SwordType == _block->m_SwordType)
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
			if (m_InventorySize < 49)
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
			if (m_InventorySize < 49)
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
	}
	else
	{
		if (m_InventorySize < 49)
		{
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
			m_InventoryStackValues[_position]++;
			_block->m_PositionInInventory = _position;
			m_InventoryMap.insert_or_assign(_position, *_block);
			//std::cout << "Added Item To Inventory - ";
			std::cout << _position << std::endl;

			m_AudioManager->PlayPickupSound();
		}
	}
	
}

bool CPlayer::IsItemInInventory(CBlock::BLOCKTYPE _type)
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

int CPlayer::IsItemInInventory(CBlock::BLOCKTYPE _type, bool _bReturnAmount)
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
				if (m_Pickaxe != nullptr)
				{
					delete m_Pickaxe;
				}
				m_Pickaxe = nullptr;
			}
			if (it->second.m_Type == CBlock::BLOCKTYPE::BOW)
			{
				if (m_Bow != nullptr)
				{
					delete m_Bow;
				}
				m_Bow = nullptr;
			}
			if (it->second.m_Type == CBlock::BLOCKTYPE::SWORD)
			{
				if (m_Sword != nullptr)
				{
					delete m_Sword;
				}
				m_Sword = nullptr;
			}

			while (m_InventoryStackValues[_position] > 0)
			{
				m_InventoryStackValues[_position]--;
			}
			m_InventorySize--;
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
void CPlayer::Mine(std::list<T>& m_Chunk, sf::Sprite& _mousePositionSprite)
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

void CPlayer::PlaceTourch(std::list<CBlock>& m_Tourches, sf::Sprite& _mousePositionSprite)
{
	if (m_MineTimer->getElapsedTime() >= sf::Time(sf::seconds(0.2f)))
	{
		// Chest
		m_Block = new CBlock(m_RenderWindow, *m_World, m_TextureMaster->m_Tourch, m_Scale, _mousePositionSprite.getPosition().x, _mousePositionSprite.getPosition().y, true, CBlock::BLOCKTYPE::TOURCH);
		m_Block->m_ArrayIndex = (m_Shape.getPosition().x);
		m_Tourches.push_back(*m_Block);
		m_Block = nullptr;

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
	m_FixtureDef.filter.categoryBits = _PLAYER_FILTER_;
	m_FixtureDef.filter.groupIndex = -_PLAYER_GROUPINDEX_;
	m_FixtureDef.filter.maskBits = _ENEMY_FILTER_;
	m_FixtureDef.filter.maskBits = _WORLD_FILTER_;
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

void CPlayer::OutPutInventoryToFile()
{
	// OFstream
	std::ofstream out_file;

	//
	// output_inventory_types
	out_file.open("Output/output_inventory_types.txt");
	out_file.clear();
	for (int i = 0; i < m_InventoryMap.size(); i++)
	{
		out_file << (int) m_InventoryMap[i].m_Type << std::endl;
	}
	out_file.close();

	//
	// output_inventory_types
	out_file.open("Output/output_inventory_pickaxetypes.txt");
	out_file.clear();
	for (int i = 0; i < m_InventoryMap.size(); i++)
	{
		out_file << (int)m_InventoryMap[i].m_PickType << std::endl;
	}
	out_file.close();

	//
	// output_inventory_stackvalues
	out_file.open("Output/output_inventory_stackvalues.txt");
	for (int i = 0; i < m_InventoryMap.size(); i++)
	{
		out_file << m_InventoryStackValues[i] << std::endl;
	}
	out_file.close();

	//
	// output_inventory_types
	out_file.open("Output/output_inventory_bowtypes.txt");
	out_file.clear();
	for (int i = 0; i < m_InventoryMap.size(); i++)
	{
		out_file << (int)m_InventoryMap[i].m_BowType << std::endl;
	}

	out_file.close();

	//
	// output_inventory_types
	out_file.open("Output/output_inventory_swordtypes.txt");
	out_file.clear();
	for (int i = 0; i < m_InventoryMap.size(); i++)
	{
		out_file << (int)m_InventoryMap[i].m_SwordType << std::endl;
	}

	out_file.close();

	//
	// output_inventory_types
	out_file.open("Output/output_inventory_projtypes.txt");
	out_file.clear();
	for (int i = 0; i < m_InventoryMap.size(); i++)
	{
		out_file << (int)m_InventoryMap[i].m_ProjectileType << std::endl;
	}

	out_file.close();

	//
	// output_inventory_types
	out_file.open("Output/output_inventory_potiontypes.txt");
	out_file.clear();
	for (int i = 0; i < m_InventoryMap.size(); i++)
	{
		out_file << (int)m_InventoryMap[i].m_PotionType << std::endl;
	}

	out_file.close();
}

void CPlayer::InputInventoryToFile()
{
	//
	// Reading From File??
	//
	int types[50] = {};
	int stackvalues[50] = {};
	int bowtypes[50] = {};
	int swordtypes[50] = {};
	int picktypes[50] = {};
	int projtypes[50] = {};
	int potiontypes[50] = {};

	// Main Types
	std::ifstream xoutputs("Output/output_inventory_types.txt");
	if (xoutputs.is_open())
	{
		for (int i = 0; i < 50; i++)
		{
			xoutputs >> types[i];
		}
		xoutputs.close();
	}

	// Stack Values
	xoutputs.open("Output/output_inventory_stackvalues.txt");
	if (xoutputs.is_open())
	{
		for (int i = 0; i < 50; i++)
		{
			xoutputs >> stackvalues[i];
		}
		xoutputs.close();
	}

	// Bow Types
	xoutputs.open("Output/output_inventory_bowtypes.txt");
	if (xoutputs.is_open())
	{
		for (int i = 0; i < 50; i++)
		{
			xoutputs >> bowtypes[i];
		}
		xoutputs.close();
	}

	// Sword Types
	xoutputs.open("Output/output_inventory_swordtypes.txt");
	if (xoutputs.is_open())
	{
		for (int i = 0; i < 50; i++)
		{
			xoutputs >> swordtypes[i];
		}
		xoutputs.close();
	}

	// Pick Types
	xoutputs.open("Output/output_inventory_pickaxetypes.txt");
	if (xoutputs.is_open())
	{
		for (int i = 0; i < 50; i++)
		{
			xoutputs >> picktypes[i];
		}
		xoutputs.close();
	}

	// Projectile Types
	xoutputs.open("Output/output_inventory_projtypes.txt");
	if (xoutputs.is_open())
	{
		for (int i = 0; i < 50; i++)
		{
			xoutputs >> projtypes[i];
		}
		xoutputs.close();
	}

	// PotionTypes
	xoutputs.open("Output/output_inventory_potiontypes.txt");
	if (xoutputs.is_open())
	{
		for (int i = 0; i < 50; i++)
		{
			xoutputs >> potiontypes[i];
		}
		xoutputs.close();
	}

	for (int i = 0; i < 50; i++)
	{
		if (stackvalues[i] != 0)
		{
			switch (types[i])
			{
			case 0:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					switch (picktypes[i])
					{
					case 0:
					{
						m_Pickaxe = new CPickaxe();
						AddItemToInventory(m_Pickaxe, i, false);
						m_Pickaxe = nullptr;
						break;
					}
					case 1:
					{
						m_Pickaxe = new CPickaxe(CBlock::PICKAXETYPE::IRON);
						AddItemToInventory(m_Pickaxe, i, false);
						m_Pickaxe = nullptr;
						break;
					}
					case 2:
					{
						m_Pickaxe = new CPickaxe(CBlock::PICKAXETYPE::GOLD);
						AddItemToInventory(m_Pickaxe, i, false);
						m_Pickaxe = nullptr;
						break;
					}
					case 3:
					{
						m_Pickaxe = new CPickaxe(CBlock::PICKAXETYPE::DIAMOND);
						AddItemToInventory(m_Pickaxe, i, false);
						m_Pickaxe = nullptr;
						break;
					}
					case 4:
					{
						m_Pickaxe = new CPickaxe(CBlock::PICKAXETYPE::OBSIDIAN);
						AddItemToInventory(m_Pickaxe, i, false);
						m_Pickaxe = nullptr;
						break;
					}
					case 5:
					{
						m_Pickaxe = new CPickaxe(CBlock::PICKAXETYPE::PURPLE);
						AddItemToInventory(m_Pickaxe, i, false);
						m_Pickaxe = nullptr;
						break;
					}
					case 6:
					{
						m_Pickaxe = new CPickaxe(CBlock::PICKAXETYPE::GOLDEN);
						AddItemToInventory(m_Pickaxe, i, false);
						m_Pickaxe = nullptr;
						break;
					}
					default:
						break;
					}
				}

				break;
			}
			case 1:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Door = new CDoor();
					AddItemToInventory(m_Door, i, false);
					m_Door = nullptr;
				}
				
				break;
			}
			case 2:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Chest = new CChest();
					AddItemToInventory(m_Chest, i, true);
					m_Chest = nullptr;
				}

				break;
			}
			case 3:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Dirt, CBlock::BLOCKTYPE::DIRT);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 4:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Stone, CBlock::BLOCKTYPE::STONE);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 5:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Wood, CBlock::BLOCKTYPE::WOOD);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 6:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Planks, CBlock::BLOCKTYPE::PLANKS);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 7:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Sand, CBlock::BLOCKTYPE::SAND);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 8:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_MossyBrick, CBlock::BLOCKTYPE::MOSSYBRICK);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 9:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Grass, CBlock::BLOCKTYPE::GRASS);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 11:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Leaves, CBlock::BLOCKTYPE::LEAVES);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 12:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Furnace, CBlock::BLOCKTYPE::FURNACE);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 13:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_IronOre, CBlock::BLOCKTYPE::IRONORE);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 14:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_GoldOre, CBlock::BLOCKTYPE::GOLDORE);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 15:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_DiamondOre, CBlock::BLOCKTYPE::DIAMONDORE);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 16:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Coal, CBlock::BLOCKTYPE::COALORE);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 17:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Obsidian, CBlock::BLOCKTYPE::OBSIDIAN);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 18:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					// Bow
					switch (bowtypes[i])
					{
					case 0:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::BASIC);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					case 1:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::OBSIDIAN);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					case 2:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::ICE);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					case 3:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::BLOOD);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					case 4:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::IRON);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					case 5:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::GOLDEN);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					case 6:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::PURPLE);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					case 7:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::GREEN);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					case 8:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::IRONGUN);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					case 9:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::GOLDGUN);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					case 10:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::PURPLEGUN);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					case 11:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::GOLDENGUN);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					case 12:
					{
						m_Bow = new Bow(CBlock::BOWTYPE::GREENGUN);
						AddItemToInventory(m_Bow, i, false);
						m_Bow = nullptr;
						break;
					}
					default:
						break;
					}
				}
				

				break;
			}
			case 19:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 20:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 21:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 22:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Cloud, CBlock::BLOCKTYPE::CLOUD);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 23:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_WorkBench, CBlock::BLOCKTYPE::WORKBENCH);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 24:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_PurpleIngot, CBlock::BLOCKTYPE::PURPLEINGOT);
					AddItemToInventory(m_Block, i, true);
				}

				m_Block = nullptr;
				break;
			}
			case 25:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_GoldenIngot, CBlock::BLOCKTYPE::GOLDENINGOT);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 26:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Anvil, CBlock::BLOCKTYPE::ANVIL);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 27:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Tourch, CBlock::BLOCKTYPE::TOURCH);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 28:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_PurpleOre, CBlock::BLOCKTYPE::PURPLEORE);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 29:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_GoldenIngot, CBlock::BLOCKTYPE::GOLDENORE);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 30:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					// Projectile
					switch (projtypes[i])
					{
					case 0:
					{
						m_Projectile = new CProjectile();
						AddItemToInventory(m_Projectile, i, true);
						m_Projectile = nullptr;
						break;
					}
					case 1:
					{
						m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::FIREARROW);
						AddItemToInventory(m_Projectile, i, true);
						m_Projectile = nullptr;
						break;
					}
					case 2:
					{
						m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::CURSEDARROW);
						AddItemToInventory(m_Projectile, i, true);
						m_Projectile = nullptr;
						break;
					}
					case 3:
					{
						m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::POISONARROW);
						AddItemToInventory(m_Projectile, i, true);
						m_Projectile = nullptr;
						break;
					}
					case 4:
					{
						m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::IRONBULLET);
						AddItemToInventory(m_Projectile, i, true);
						m_Projectile = nullptr;
						break;
					}
					case 5:
					{
						m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::GOLDBULLET);
						AddItemToInventory(m_Projectile, i, true);
						m_Projectile = nullptr;
						break;
					}
					case 6:
					{
						m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::PURPLEBULLET);
						AddItemToInventory(m_Projectile, i, true);
						m_Projectile = nullptr;
						break;
					}
					case 7:
					{
						m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::GOLDENBULLET);
						AddItemToInventory(m_Projectile, i, true);
						m_Projectile = nullptr;
						break;
					}
					default:
						break;
					}
				}
				
				break;
			}
			case 31:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					// Projectile
					switch (potiontypes[i])
					{
					case 0:
					{
						CPotion* potion= new CPotion(CBlock::POTIONTYPE::HPSMALL);
						AddItemToInventory(potion, i, true);
						potion = nullptr;
						break;
					}
					case 1:
					{
						CPotion* potion = new CPotion(CBlock::POTIONTYPE::HPLARGE);
						AddItemToInventory(potion, i, true);
						potion = nullptr;
						break;
					}
					default:
						break;
					}
				}

				break;
			}
			case 32:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_GlassBeaker, CBlock::BLOCKTYPE::EMPTYBEAKER);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 33:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_RedSlime, CBlock::BLOCKTYPE::REDSLIME);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 34:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					switch (swordtypes[i])
					{
					case 0:
					{
						m_Sword = new CSword(CBlock::SWORDTYPE::WOOD);
						AddItemToInventory(m_Sword, i, false);
						m_Sword = nullptr;
						break;
					}
					case 1:
					{
						m_Sword = new CSword(CBlock::SWORDTYPE::ANCIENT);
						AddItemToInventory(m_Sword, i, false);
						m_Sword = nullptr;
						break;
					}
					case 2:
					{
						m_Sword = new CSword(CBlock::SWORDTYPE::FLAME);
						AddItemToInventory(m_Sword, i, false);
						m_Sword = nullptr;
						break;
					}
					case 3:
					{
						m_Sword = new CSword(CBlock::SWORDTYPE::GOD);
						AddItemToInventory(m_Sword, i, false);
						m_Sword = nullptr;
						break;
					}
					case 4:
					{
						m_Sword = new CSword(CBlock::SWORDTYPE::GOLDEN);
						AddItemToInventory(m_Sword, i, false);
						m_Sword = nullptr;
						break;
					}
					case 5:
					{
						m_Sword = new CSword(CBlock::SWORDTYPE::GREEN);
						AddItemToInventory(m_Sword, i, false);
						m_Sword = nullptr;
						break;
					}
					case 6:
					{
						m_Sword = new CSword(CBlock::SWORDTYPE::PURPLE);
						AddItemToInventory(m_Sword, i, false);
						m_Sword = nullptr;
						break;
					}
					case 7:
					{
						m_Sword = new CSword(CBlock::SWORDTYPE::SLIME);
						AddItemToInventory(m_Sword, i, false);
						m_Sword = nullptr;
						break;
					}
					default:
						break;
					}
				}

				break;
			}
			case 35:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_RedSlime, CBlock::BLOCKTYPE::BROKENSWORD);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 36:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_PortalKeyGreen, CBlock::BLOCKTYPE::PORTALKEYGREEN);
					AddItemToInventory(m_Block, i, false);
					m_Block = nullptr;
				}

				break;
			}
			case 37:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_PortalKeyRed, CBlock::BLOCKTYPE::PORTALKEYRED);
					AddItemToInventory(m_Block, i, false);
					m_Block = nullptr;
				}

				break;
			}
			case 38:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_PortalKeyYellow, CBlock::BLOCKTYPE::PORTALKEYYELLOW);
					AddItemToInventory(m_Block, i, false);
					m_Block = nullptr;
				}

				break;
			}
			case 39:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_SandStone, CBlock::BLOCKTYPE::SANDSTONE);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 40:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_HardSandStone, CBlock::BLOCKTYPE::HARDSANDSTONE);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 41:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_Cactas, CBlock::BLOCKTYPE::CACTUS);
					AddItemToInventory(m_Block, i, true);
					m_Block = nullptr;
				}

				break;
			}
			case 42:
			{
				for (int J = 0; J < stackvalues[i]; J++)
				{
					m_Block = new CBlock(m_TextureMaster->m_PortalKeyPlains, CBlock::BLOCKTYPE::PORTALKEYPLAINS);
					AddItemToInventory(m_Block, i, false);
					m_Block = nullptr;
				}

				break;
			}
			default:
				break;
			}
		}
	}

}

void CPlayer::ToggleGodMode()
{
	m_bGodMode = !m_bGodMode;
}

bool CPlayer::GetGodMode()
{
	return m_bGodMode;
}

b2World* CPlayer::GetWorld()
{
	return m_World;
}
