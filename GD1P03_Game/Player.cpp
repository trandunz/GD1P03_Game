#include "Player.h"

CPlayer::CPlayer(sf::RenderWindow* _renderWindow, b2World& _world, const float& _scale, CAudioManager* _audioManager)
{
	m_AudioManager = _audioManager;
	m_RenderWindow = _renderWindow;
	m_Scale = _scale;
	m_World = &_world;
	m_Block = nullptr;

	m_PlayerFilter.categoryBits = 1;
	m_PlayerFilter.maskBits = 65535;
	m_PlayerFilter.groupIndex = 0;

	// Textures
	m_PlayerRightTex = new sf::Texture();
	m_PlayerLeftTex = new sf::Texture();
	if (m_PlayerRightTex->loadFromFile("Images/PlayerSpritesRight.png"))
	{
		std::cout << "Loaded Player" << std::endl;
	}
	if (m_PlayerLeftTex->loadFromFile("Images/PlayerSpritesLeft.png"))
	{
		std::cout << "Loaded Player" << std::endl;
	}
	m_Shape = sf::Sprite();
	m_Shape.setTexture(*m_PlayerRightTex, true);
	m_Shape.setTextureRect(sf::IntRect(0, 0, 100, 200));


	////falling object
	/*m_Shape.setSize(sf::Vector2f(100, 200));*/
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);

	//falling object physics
	m_BodyDef = b2BodyDef();
	m_BodyDef.position = b2Vec2(m_Shape.getPosition().x / _scale, (m_Shape.getPosition().y / _scale) - 5);
	m_BodyDef.type = b2_dynamicBody;
	m_BodyDef.linearDamping = 0.05f;
	m_BodyDef.angularDamping = 0.0f;
	m_BodyDef.fixedRotation = true;
	m_BodyDef.allowSleep = false;
	m_BodyDef.awake = true;
	m_BodyDef.gravityScale = 10.0f;
	m_Body = _world.CreateBody(&m_BodyDef);

	m_b2pShape.SetAsBox((100 / 4) / _scale, (190 / 2) / _scale);

	m_FixtureDef.density = 1.0f;
	m_FixtureDef.shape = &m_b2pShape;
	m_FixtureDef.friction = 2.0f;

	m_Body->CreateFixture(&m_FixtureDef);

	// Velocity
	m_Velocity = b2Vec2(0.0f, 0.0f);

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
	for (int i = 0; i < m_InventoryMap.size(); i++)
	{
		m_InventoryMap.erase(m_InventoryMap.begin());
		//std::cout << "Inventory Piece Destroyed" << std::endl;
	}
	for (int i = 0; i < m_DoorInventoryMap.size(); i++)
	{
		m_DoorInventoryMap.erase(m_DoorInventoryMap.begin());
		//std::cout << "Inventory Piece Destroyed" << std::endl;
	}
	m_DoorInventoryMap.clear();
	m_InventoryMap.clear();
	m_Door = nullptr;
	delete m_MapIconTexRight;
	m_MapIconTexRight = nullptr;
	delete m_MapIconTex;
	m_MapIconTex = nullptr;
	delete m_PlayerRightTex;
	m_PlayerRightTex = nullptr;
	delete m_PlayerLeftTex;
	delete m_AnimationTimer;
	m_AnimationTimer = nullptr;
	m_PlayerLeftTex = nullptr;
	m_Block = nullptr;
	m_Body = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
	m_AudioManager = nullptr;
}

void CPlayer::Start()
{
	m_AnimationTimer = new sf::Clock();
}

void CPlayer::Update(sf::Vector2f _mousePos, sf::Event& _event)
{
	m_MousePos = _mousePos;

	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);

	m_MapIcon.setPosition(m_Shape.getPosition());
}

void CPlayer::Render()
{
	m_RenderWindow->draw(m_Shape);
	
}

sf::Sprite CPlayer::GetShape()
{
	return m_Shape;
}

int CPlayer::GetCurrentHP()
{
	return m_Health;
}

int CPlayer::GetMaxHP()
{
	return m_MaxHP;
}

bool CPlayer::bMouseNotOverBlock(std::list<CBlock>& m_Chunk, sf::Sprite& _mousePositionSprite)
{
	for (CBlock& block : m_Chunk)
	{
		if (block.GetShape().getPosition() == _mousePositionSprite.getPosition())
		{
			return false;
		}
	}
	return true;
}

bool CPlayer::bMouseNotOverDoor(std::list<CDoor>& m_Doors, sf::Sprite& _mousePositionSprite)
{
	for (CDoor& door : m_Doors)
	{
		if (door.GetShape().getPosition() == _mousePositionSprite.getPosition())
		{
			return false;
		}
	}
	return true;
}

void CPlayer::Movement(sf::Event& _event)
{
	b2Vec2 worldposition = { m_Shape.getPosition().x, m_Shape.getPosition().y };

	if (m_bCanMove)
	{
		int x = 0;
		int y = 0;

		// Fly
		if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Key::Space)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -250.0f), true);
		}

		// Jump
		if (_event.type == sf::Event::KeyPressed && _event.key.code == sf::Keyboard::Key::W)
		{
			m_Body->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -250.0f), true);
		}

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

		m_Body->ApplyLinearImpulseToCenter(15 * m_Velocity, true);

		float Mag = sqrt((m_Body->GetLinearVelocityFromWorldPoint(worldposition).x * m_Body->GetLinearVelocityFromWorldPoint(worldposition).x));
		if (Mag > m_MoveSpeed)
		{
			m_Body->ApplyLinearImpulseToCenter(-1 * m_MoveSpeed * b2Vec2(m_Velocity.x, 0.0f), true);
		}
	}
	
	if (m_Velocity.x > 0.1f )
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
	else if (m_Velocity.x == 0.0f)
	{
		m_Shape.setTextureRect(sf::IntRect(0, 0, 100, 200));
	}

	if (m_Body->GetLinearVelocityFromWorldPoint(worldposition).x > 15.0f)
	{
		if (m_AudioManager != nullptr)
		{
			m_AudioManager->PlayRunning();
		}
	}
	else if (m_Body->GetLinearVelocityFromWorldPoint(worldposition).x < -15.0f)
	{
		if (m_AudioManager != nullptr)
		{
			m_AudioManager->PlayRunning();
		}
	}
	
	// Move Sprite And Body
	/*m_Body->SetLinearVelocity(m_MoveSpeed * m_Velocity);*/
	
	/*m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);*/
	//std::cout << m_Shape.getPosition().x << std::endl;
	//std::cout << m_Shape.getPosition().y << std::endl;
	
}

void CPlayer::PlaceBlocks(std::list<CDoor>& m_Doors, std::list<CBlock>& m_Chunk, sf::Event& _event, sf::Sprite& _mousePositionSprite)
{
	float Mag = sqrt(((_mousePositionSprite.getPosition().x - GetShape().getPosition().x) * (_mousePositionSprite.getPosition().x - GetShape().getPosition().x)) + ((_mousePositionSprite.getPosition().y - GetShape().getPosition().y) * (_mousePositionSprite.getPosition().y - GetShape().getPosition().y)));
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && Mag < m_InteractionRange * 100 && Mag > 90.0f)
	{
		if (bMouseNotOverBlock(m_Chunk, _mousePositionSprite) && bMouseNotOverDoor(m_Doors, _mousePositionSprite) && !SelectedItemIsEmpty())
		{
			m_InventoryMap[m_CurrentItemIndex];
			if (m_InventoryMap[m_CurrentItemIndex].m_Type == m_InventoryMap[m_CurrentItemIndex].BLOCKTYPE::DOOR)
			{
				m_Door = new CDoor(m_RenderWindow, *m_World, m_Scale, _mousePositionSprite.getPosition().x, _mousePositionSprite.getPosition().y);
				m_Door->SetSize(100, 200);

				m_Doors.push_back(*m_Door);
				m_Door->m_ArrayIndex = (m_Shape.getPosition().x);

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
			}
			else
			{
				m_Block = new CBlock(m_RenderWindow, *m_World, m_InventoryMap[m_CurrentItemIndex].GetShape().getTexture(), m_Scale, _mousePositionSprite.getPosition().x, _mousePositionSprite.getPosition().y);
				//m_Block->SetSize(100, 100);
				m_Chunk.push_back(*m_Block);
				m_Block = nullptr;
				m_Door = nullptr;

				if (m_InventoryStackValues[m_CurrentItemIndex] <= 0)
				{
					RemoveItemFromInventory(m_CurrentItemIndex);
				}
				else
				{
					m_InventoryStackValues[m_CurrentItemIndex]--;
				}
			}

			m_AudioManager->PlayBlockPlace();
		}
		
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && Mag < m_InteractionRange * 100)
	{
		std::cout << "Mouse Right" << std::endl;

		std::list<CBlock>::iterator it;
		for (it = m_Chunk.begin(); it != m_Chunk.end(); it++)
		{
			if (it->GetShape().getPosition() == _mousePositionSprite.getPosition())
			{
				if (!bMouseNotOverBlock(m_Chunk, _mousePositionSprite))
				{
					Lst_MoveToFront(m_Chunk, it);
					m_Chunk.pop_front();

					m_AudioManager->PlayGroundMine();

					return;
				}
			}
		}

		for (CDoor& doors : m_Doors)
		{
			if (doors.GetShape().getPosition() == _mousePositionSprite.getPosition())
			{
				if (bMouseNotOverBlock(m_Chunk, _mousePositionSprite) && !bMouseNotOverDoor(m_Doors, _mousePositionSprite))
				{
					std::cout << "Open Door" << std::endl;
					doors.OCDoor(m_Shape.getPosition());
					return;
				}
			}
		}
	}
}

b2Body* CPlayer::GetBody()
{
	return m_Body;
}

void CPlayer::ResetSpritePos()
{
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
}

/// <summary>
/// 
/// </summary>
/// <param name="list"></param>
/// <param name="element"></param>
void CPlayer::Lst_MoveToFront(std::list<CBlock>& list, std::list<CBlock>::iterator element)
{
	if (element != list.begin())
	{
		list.splice(list.begin(), list, element, std::next(element));
	}
}

void CPlayer::AddItemToInventory(CBlock* _block)
{
	if (IsBlockInInventory(_block))
	{
		
	}
	else
	{
		_block->GetShape().setScale(0.4f, 0.4f);
		_block->GetShape().setOrigin(_block->GetShape().getGlobalBounds().width / 2, _block->GetShape().getGlobalBounds().height / 2);
		m_RenderWindow->mapCoordsToPixel(_block->GetShape().getPosition());
		_block->m_PositionInInventory = m_InventoryMap.size();
		m_InventoryStackValues[m_InventoryMap.size()]++;
		m_InventoryMap.insert({ m_InventoryMap.size(), *_block });
		
	}
	
	
	_block = nullptr;
}

void CPlayer::AddItemToInventory(CDoor* _door)
{
	if (IsDoorInInventory(_door))
	{
	}
	else
	{
		_door->GetShape().setScale(0.4f, 0.4f);
		_door->GetShape().setOrigin(_door->GetShape().getGlobalBounds().width / 2, _door->GetShape().getGlobalBounds().height / 2);
		m_RenderWindow->mapCoordsToPixel(_door->GetShape().getPosition());
		_door->m_PositionInInventory = m_InventoryMap.size();

		m_InventoryStackValues[m_InventoryMap.size()]++;
		m_InventoryMap.insert({ m_InventoryMap.size(), *_door });

		_door = nullptr;
	}
	
}

void CPlayer::RemoveItemFromInventory(int _position)
{
	std::map<int, CBlock>::iterator it = m_InventoryMap.begin();

	while (it != m_InventoryMap.end())
	{
		if (it->first == _position)
		{
			m_InventoryStackValues[_position]--;

			it->second.m_PositionInInventory = -1;
			m_InventoryMap.erase(it);

			return;
		}
		it++;
	}
}

void CPlayer::ToggleInventoryUI()
{
	m_bInventoryOpen = !m_bInventoryOpen;
}

bool CPlayer::SelectedItemIsEmpty()
{
	std::map<int, CBlock>::iterator it = m_InventoryMap.begin();

	while (it != m_InventoryMap.end())
	{
		if (m_CurrentItemIndex == it->second.m_PositionInInventory)
		{
			std::cout << " I Have A Block!" << std::endl;
			return false;
		}
		it++;
	}
	return true;
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
			delete _block;
			_block = nullptr;
			return true;
		}
	}
	return false;
}

bool CPlayer::IsDoorInInventory(CDoor* _door)
{
	std::map<int, CBlock>::iterator it;
	for (it = m_InventoryMap.begin(); it != m_InventoryMap.end(); it++)
	{
		if (it->second.m_Type == _door->m_Type)
		{
			// increase number of that type
			m_InventoryStackValues[it->first]++;
			delete _door;
			_door = nullptr;
			return true;
		}
	}
	return false;
}