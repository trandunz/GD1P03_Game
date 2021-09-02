#include "GUI.h"

void GUI::HealthUI(sf::RenderWindow* _renderWindow)
{
	// Health Text
	sf::Text testText = sf::Text();
	std::string string = "Life: " +  ToString(m_Player->GetCurrentHP()) + " / " + ToString(m_Player->GetMaxHP());
	testText.setFont(m_Font);
	testText.setString(string);
	testText.setFillColor(sf::Color::White);
	testText.setOutlineThickness(0.75f);
	testText.setOutlineColor(sf::Color::Black);
	testText.setOrigin(testText.getGlobalBounds().width / 2, testText.getGlobalBounds().height / 2);
	testText.setCharacterSize(18);
	testText.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 120, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 15);
	_renderWindow->draw(testText);

	// Heart Containers
	sf::Sprite heart1 = sf::Sprite();
	sf::Sprite heart2 = sf::Sprite();
	sf::Sprite heart3 = sf::Sprite();
	sf::Sprite heart4 = sf::Sprite();
	sf::Sprite heart5 = sf::Sprite();
	if (m_Player->GetCurrentHP() > 0)
	{
		heart1.setTexture(*m_HeartFull, true);
		if (m_Player->GetCurrentHP() >= 20)
		{
			heart2.setTexture(*m_HeartFull, true);
			if (m_Player->GetCurrentHP() >= 40)
			{
				heart3.setTexture(*m_HeartFull, true);
				if (m_Player->GetCurrentHP() >= 60)
				{
					heart4.setTexture(*m_HeartFull, true);
					if (m_Player->GetCurrentHP() >= 80)
					{
						heart5.setTexture(*m_HeartFull, true);
					}
					else
					{
						heart5.setTexture(*m_HeartEmpty, true);
					}
					
				}
				else
				{
					heart4.setTexture(*m_HeartEmpty, true);
					heart5.setTexture(*m_HeartEmpty, true);
				}
				
			}
			else
			{
				heart3.setTexture(*m_HeartEmpty, true);
				heart4.setTexture(*m_HeartEmpty, true);
				heart5.setTexture(*m_HeartEmpty, true);
			}
			
		}
		else
		{
			heart2.setTexture(*m_HeartEmpty, true);
			heart3.setTexture(*m_HeartEmpty, true);
			heart4.setTexture(*m_HeartEmpty, true);
			heart5.setTexture(*m_HeartEmpty, true);
		}
	}
	else
	{
		heart1.setTexture(*m_HeartEmpty, true);
		heart2.setTexture(*m_HeartEmpty, true);
		heart3.setTexture(*m_HeartEmpty, true);
		heart4.setTexture(*m_HeartEmpty, true);
		heart5.setTexture(*m_HeartEmpty, true);
	}
	
	heart1.setScale(sf::Vector2f(0.4, 0.4));
	heart2.setScale(sf::Vector2f(0.4, 0.4));
	heart3.setScale(sf::Vector2f(0.4, 0.4));
	heart4.setScale(sf::Vector2f(0.4, 0.4));
	heart5.setScale(sf::Vector2f(0.4, 0.4));

	heart1.setOrigin(heart1.getGlobalBounds().width / 2, heart1.getGlobalBounds().height / 2);
	heart2.setOrigin(heart1.getGlobalBounds().width / 2, heart1.getGlobalBounds().height / 2);
	heart3.setOrigin(heart1.getGlobalBounds().width / 2, heart1.getGlobalBounds().height / 2);
	heart4.setOrigin(heart1.getGlobalBounds().width / 2, heart1.getGlobalBounds().height / 2);
	heart5.setOrigin(heart1.getGlobalBounds().width / 2, heart1.getGlobalBounds().height / 2);

	heart1.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 210, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 35);
	_renderWindow->draw(heart1);
	heart2.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 185, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 35);
	_renderWindow->draw(heart2);
	heart3.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 160, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 35);
	_renderWindow->draw(heart3);
	heart4.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 135, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 35);
	_renderWindow->draw(heart4);
	heart5.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 110, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 35);
	_renderWindow->draw(heart5);
}

void GUI::MiniMapUI(sf::RenderWindow* _renderWindow, std::list<CBlock>& _chunk, std::list<sf::Sprite>& _skyChunk)
{
	// Assigning Render Texture Stuff
	sf::View MiniMapView = sf::View(m_Player->GetShape().getPosition(), sf::Vector2f(200.0f, 200.0f));
	MiniMapView.zoom(60);
	m_miniMap = new sf::RenderTexture();
	m_miniMap->create(200, 200);
	m_miniMap->setView(MiniMapView);
	for (sf::Sprite& sky : _skyChunk)
	{
		m_miniMap->draw(sky);
	}
	for (CBlock& block : _chunk)
	{
		m_miniMap->draw(block.GetShape());
	}
	m_miniMap->draw(m_Player->GetShape());
	m_miniMap->display();

	// Background
	sf::RectangleShape test1 = sf::RectangleShape();
	test1.setSize(sf::Vector2f(210, 210));
	test1.setFillColor(sf::Color::Black);
	test1.setOrigin(test1.getGlobalBounds().width / 2, test1.getGlobalBounds().height / 2);
	test1.setPosition(200, 200);
	_renderWindow->mapCoordsToPixel(test1.getPosition());
	test1.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 140, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 160);
	_renderWindow->draw(test1);

	// Inner Background
	sf::RectangleShape test2 = sf::RectangleShape();
	test2.setSize(sf::Vector2f(205, 205));
	test2.setFillColor(sf::Color(147, 214, 2));
	test2.setOrigin(test2.getGlobalBounds().width / 2, test2.getGlobalBounds().height / 2);
	test2.setPosition(200, 200);
	_renderWindow->mapCoordsToPixel(test2.getPosition());
	test2.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 140, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 160);
	_renderWindow->draw(test2);

	// Inner Background
	sf::RectangleShape test3 = sf::RectangleShape();
	test3.setSize(sf::Vector2f(200, 200));
	test3.setFillColor(sf::Color::Black);
	test3.setOrigin(test3.getGlobalBounds().width / 2, test3.getGlobalBounds().height / 2);
	test3.setPosition(200, 200);
	_renderWindow->mapCoordsToPixel(test3.getPosition());
	test3.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 140, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 160);
	_renderWindow->draw(test3);

	// Assigning Sprite Texture And Drawing (MiniMap)
	sf::RectangleShape test = sf::RectangleShape();
	test.setSize(sf::Vector2f(200, 200));
	test.setTexture(&m_miniMap->getTexture());
	//test.setFillColor(sf::Color::White);
	test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
	test.setPosition(200, 200);
	_renderWindow->mapCoordsToPixel(test.getPosition());
	test.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 140, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 160);
	_renderWindow->draw(test);

	delete m_miniMap;
	m_miniMap = nullptr;
}

void GUI::InventoryUI(sf::RenderWindow* _renderWindow)
{
	// Row 1
	for (int i = 0; i < 10; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
		_renderWindow->mapCoordsToPixel(test.getPosition());
		m_InventorySlotMap.emplace(i, test);
		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + (i * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 70);
		
		sf::Sprite ITEM = sf::Sprite();
		ITEM.setTexture(*m_Dirt, true);
		ITEM.setScale(sf::Vector2f(0.6, 0.6));
		ITEM.setOrigin(ITEM.getGlobalBounds().width / 2, ITEM.getGlobalBounds().height / 2);
		_renderWindow->mapCoordsToPixel(ITEM.getPosition());
		ITEM.setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + (i * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 70);
		m_InventoryMap.emplace(i, ITEM);
	}

	// Row 2
	for (int i = 10; i < 20; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
		_renderWindow->mapCoordsToPixel(test.getPosition());
		m_InventorySlotMap.emplace(i, test);
		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 10) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 135);
		
		sf::Sprite ITEM = sf::Sprite();
		ITEM.setTexture(*m_Dirt, true);
		ITEM.setScale(sf::Vector2f(0.6, 0.6));
		ITEM.setOrigin(ITEM.getGlobalBounds().width / 2, ITEM.getGlobalBounds().height / 2);
		_renderWindow->mapCoordsToPixel(ITEM.getPosition());
		ITEM.setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 10) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 135);
		m_InventoryMap.emplace(i, ITEM);
	}
	if (m_bInventoryOpen)
	{
		
		
		// Row 3
		for (int i = 20; i < 30; i++)
		{
			sf::Sprite test = sf::Sprite();
			test.setTexture(*m_ItemSpacer, true);
			test.setScale(sf::Vector2f(0.6, 0.6));
			test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
			_renderWindow->mapCoordsToPixel(test.getPosition());
			m_InventorySlotMap.emplace(i, test);
			m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 20) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 200);
			
			sf::Sprite ITEM = sf::Sprite();
			ITEM.setTexture(*m_Dirt, true);
			ITEM.setScale(sf::Vector2f(0.6, 0.6));
			ITEM.setOrigin(ITEM.getGlobalBounds().width / 2, ITEM.getGlobalBounds().height / 2);
			_renderWindow->mapCoordsToPixel(ITEM.getPosition());
			ITEM.setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 20) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 200);
			m_InventoryMap.emplace(i, ITEM);
		}
		// Row 4
		for (int i = 30; i < 40; i++)
		{
			sf::Sprite test = sf::Sprite();
			test.setTexture(*m_ItemSpacer, true);
			test.setScale(sf::Vector2f(0.6, 0.6));
			test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
			_renderWindow->mapCoordsToPixel(test.getPosition());
			m_InventorySlotMap.emplace(i, test);
			m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 30) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265);
			
			sf::Sprite ITEM = sf::Sprite();
			ITEM.setTexture(*m_Dirt, true);
			ITEM.setScale(sf::Vector2f(0.6, 0.6));
			ITEM.setOrigin(ITEM.getGlobalBounds().width / 2, ITEM.getGlobalBounds().height / 2);
			_renderWindow->mapCoordsToPixel(ITEM.getPosition());
			ITEM.setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 30) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265);
			m_InventoryMap.emplace(i, ITEM);
		}
		// Row 5
		for (int i = 40; i < 50; i++)
		{
			sf::Sprite test = sf::Sprite();
			test.setTexture(*m_ItemSpacer, true);
			test.setScale(sf::Vector2f(0.6, 0.6));
			test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
			_renderWindow->mapCoordsToPixel(test.getPosition());
			m_InventorySlotMap.emplace(i, test);
			m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 40) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65);
			
			sf::Sprite ITEM = sf::Sprite();
			ITEM.setTexture(*m_Dirt, true);
			ITEM.setScale(sf::Vector2f(0.6, 0.6));
			ITEM.setOrigin(ITEM.getGlobalBounds().width / 2, ITEM.getGlobalBounds().height / 2);
			_renderWindow->mapCoordsToPixel(ITEM.getPosition());
			ITEM.setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 40) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65);
			m_InventoryMap.emplace(i, ITEM);
		}

		//
		// Actual Items
		
		// 
		
	}
}

void GUI::InitTextureMaster()
{
	//Textures
	m_Dirt = new sf::Texture();
	m_Dirt->loadFromFile("Images/Dirt.png"); // Dirt
	m_Grass = new sf::Texture();
	m_Grass->loadFromFile("Images/Grass.png"); // Grass
	m_MousePosTex = new sf::Texture();
	m_MousePosTex->loadFromFile("Images/MousePos.png"); // Mouse Pos Indicator
	m_Sky = new sf::Texture();
	m_Sky->loadFromFile("Images/Sky.png"); // Sky Blue
	m_ItemSpacer = new sf::Texture();
	m_ItemSpacer->loadFromFile("Images/ItemSpacer.png");
	m_HeartFull = new sf::Texture();
	m_HeartFull->loadFromFile("Images/HeartFull.png");
	m_HeartEmpty = new sf::Texture();
	m_HeartEmpty->loadFromFile("Images/HeartEmpty.png");

	m_MousePos.setTexture(*m_MousePosTex, true);
	m_MousePos.setOrigin(m_MousePos.getGlobalBounds().width / 2, m_MousePos.getGlobalBounds().height / 2);
}

void GUI::ToggleInventoryUI()
{
	m_bInventoryOpen = !m_bInventoryOpen;
}

void GUI::CraftingUI(sf::RenderWindow* _renderWindow)
{
	if (m_bInventoryOpen)
	{
		sf::Color color = sf::Color();
		
		// Row 1
		for (int i = 50; i < 55; i++)
		{
			sf::Sprite test = sf::Sprite();
			test.setTexture(*m_ItemSpacer, true);
			test.setScale(sf::Vector2f(0.6, 0.6));
			test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
			_renderWindow->mapCoordsToPixel(test.getPosition());
			m_InventorySlotMap.emplace(i, test);
			m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65 + 65 + ((i-50) * 65));
			color = m_InventorySlotMap[i].getColor();
			color.a = 55.0f * (i - 50 );
			m_InventorySlotMap[i].setColor(color);
		}
	}
}

void GUI::Render(sf::RenderWindow* _renderWindow)
{
	if (m_bInventoryOpen)
	{
		// Render
		for (int i = 0; i < m_InventorySlotMap.size(); i++)
		{
			_renderWindow->draw(m_InventorySlotMap[i]);
			_renderWindow->draw(m_InventoryMap[i]);
		}
	}
	else
	{
		// Render
		for (int i = 0; i < 10; i++)
		{
			_renderWindow->draw(m_InventorySlotMap[i]);
			_renderWindow->draw(m_InventoryMap[i]);
		}
	}
	
}

std::string GUI::ToString(int32 integer)
{
	char numstr[10]; // enough to hold all numbers up to 32-bits
	sprintf_s(numstr, "%i", integer);
	return numstr;
}

GUI::GUI()
{
	m_Font = sf::Font();
	m_Font.loadFromFile("Fonts/ANDYB.TTF");
}

GUI::~GUI()
{
	for (int i = 0; i < m_InventorySlotMap.size(); i++)
	{
		m_InventorySlotMap.erase(m_InventorySlotMap.begin());
		//std::cout << "Inventory Piece Destroyed" << std::endl;
	}
	for (int i = 0; i < m_InventoryMap.size(); i++)
	{
		m_InventoryMap.erase(m_InventoryMap.begin());
		//std::cout << "Inventory Piece Destroyed" << std::endl;
	}
	m_InventorySlotMap.clear();
	m_InventoryMap.clear();
	delete m_miniMap;
	delete m_HeartEmpty;
	delete m_HeartFull;
	delete m_Sky;
	delete m_MousePosTex;
	delete m_Dirt;
	delete m_Grass;
	delete m_ItemSpacer;
	m_miniMap = nullptr;
	m_HeartEmpty = nullptr;
	m_HeartFull = nullptr;
	m_ItemSpacer = nullptr;
	m_Sky = nullptr;
	m_MousePosTex = nullptr;
	m_Dirt = nullptr;
	m_Grass = nullptr;
}

void GUI::SetPlayer(CPlayer* _player)
{
	m_Player = _player;
}

CPlayer* GUI::GetPlayer()
{
	return m_Player;
}

void GUI::SetFont(sf::Font& _font)
{
	m_Font = _font;
}
