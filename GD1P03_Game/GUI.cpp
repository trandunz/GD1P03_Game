#include "GUI.h"

void GUI::HealthUI(sf::RenderWindow* _renderWindow)
{
	std::string string = "Life: " + ToString(m_Player->GetCurrentHP()) + " / " + ToString(m_Player->GetMaxHP());
	m_HealthText.setString(string);
	m_HealthText.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 110, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 15);
	_renderWindow->draw(m_HealthText);

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

	// Position And Draw
	heart1.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 200, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 30);
	_renderWindow->draw(heart1);
	heart2.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 175, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 30);
	_renderWindow->draw(heart2);
	heart3.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 150, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 30);
	_renderWindow->draw(heart3);
	heart4.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 125, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 30);
	_renderWindow->draw(heart4);
	heart5.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 100, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 30);
	_renderWindow->draw(heart5);
}

sf::Text GUI::InitHealthUI()
{
	// Heart Containers
	heart1 = sf::Sprite();
	heart2 = sf::Sprite();
	heart3 = sf::Sprite();
	heart4 = sf::Sprite();
	heart5 = sf::Sprite();

	heart1.setOrigin(heart1.getGlobalBounds().width / 2, heart1.getGlobalBounds().height / 2);
	heart2.setOrigin(heart1.getGlobalBounds().width / 2, heart1.getGlobalBounds().height / 2);
	heart3.setOrigin(heart1.getGlobalBounds().width / 2, heart1.getGlobalBounds().height / 2);
	heart4.setOrigin(heart1.getGlobalBounds().width / 2, heart1.getGlobalBounds().height / 2);
	heart5.setOrigin(heart1.getGlobalBounds().width / 2, heart1.getGlobalBounds().height / 2);

	heart1.setScale(sf::Vector2f(0.4, 0.4));
	heart2.setScale(sf::Vector2f(0.4, 0.4));
	heart3.setScale(sf::Vector2f(0.4, 0.4));
	heart4.setScale(sf::Vector2f(0.4, 0.4));
	heart5.setScale(sf::Vector2f(0.4, 0.4));

	// Health Text
	m_HealthText = sf::Text();
	m_HealthString = "Life: " + ToString(m_Player->GetCurrentHP()) + " / " + ToString(m_Player->GetMaxHP());
	m_HealthText.setString(m_HealthString);
	m_HealthText.setFont(m_Font);
	m_HealthText.setFillColor(sf::Color::White);
	m_HealthText.setOutlineThickness(0.75f);
	m_HealthText.setOutlineColor(sf::Color::Black);
	m_HealthText.setOrigin(m_HealthText.getGlobalBounds().width / 2, m_HealthText.getGlobalBounds().height / 2);
	m_HealthText.setCharacterSize(18);
	return m_HealthText;
}

void GUI::MiniMapUI(sf::RenderWindow* _renderWindow, std::list<CBlock>& _chunk, std::list<sf::RectangleShape>& _skyChunk)
{
	// Assigning Render Texture View and Zooming
	sf::View MiniMapView = sf::View(m_Player->GetShape().getPosition(), sf::Vector2f(200.0f, 200.0f));
	MiniMapView.zoom(57);
	m_miniMap->setView(MiniMapView);
	
	m_miniMap->clear();

	m_MiniMapWorldBackGround.setPosition(_renderWindow->getView().getCenter());
	m_miniMap->draw(m_MiniMapWorldBackGround);

	// Draw All Blocks In Radius 1.8f
	std::list<CBlock>::iterator it;

	// Blocks
	for (it = _chunk.begin(); it != _chunk.end(); it++)
	{
		float Mag1 = sqrt(((it->GetShape().getPosition().x - m_Player->GetShape().getPosition().x) * (it->GetShape().getPosition().x - m_Player->GetShape().getPosition().x)) + ((it->GetShape().getPosition().y - m_Player->GetShape().getPosition().y) * (it->GetShape().getPosition().y - m_Player->GetShape().getPosition().y)));
		if (Mag1 < _renderWindow->getSize().x * 4.0f)
		{
			m_miniMap->draw(it->GetShape());
		}
	}

	// Border
	m_MiniMapBG1.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 140, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 160);
	m_MiniMapBG2.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 140, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 160);
	m_MiniMapBG3.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 140, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 160);
	m_MiniMapShape.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 140, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 160);
	_renderWindow->draw(m_MiniMapBG1);
	_renderWindow->draw(m_MiniMapBG2);
	_renderWindow->draw(m_MiniMapBG3);

	// Draw Player To RendTexture
	m_miniMap->draw(m_Player->m_MapIcon);
	
	// Update RendTexture
	m_miniMap->display();
	/*m_MiniMapShape.getTexture()->copyToImage().saveToFile("MINIMAP.png");*/
	
	// Draw Shape With Applied RenderText To Main Window
	_renderWindow->draw(m_MiniMapShape);
}

void GUI::InitMiniMap(sf::RenderWindow* _renderWindow)
{
	m_miniMap = new sf::RenderTexture();
	m_miniMap->create(200, 200);

	// Background
	m_MiniMapBG1 = sf::RectangleShape();
	m_MiniMapBG1.setSize(sf::Vector2f(210, 210));
	m_MiniMapBG1.setFillColor(sf::Color::Black);
	m_MiniMapBG1.setOrigin(m_MiniMapBG1.getGlobalBounds().width / 2, m_MiniMapBG1.getGlobalBounds().height / 2);
	_renderWindow->mapCoordsToPixel(m_MiniMapBG1.getPosition());
	// Inner Background
	m_MiniMapBG2 = sf::RectangleShape();
	m_MiniMapBG2.setSize(sf::Vector2f(205, 205));
	m_MiniMapBG2.setFillColor(sf::Color(147, 214, 2));
	m_MiniMapBG2.setOrigin(m_MiniMapBG2.getGlobalBounds().width / 2, m_MiniMapBG2.getGlobalBounds().height / 2);
	_renderWindow->mapCoordsToPixel(m_MiniMapBG2.getPosition());
	// Inner Background
	m_MiniMapBG3 = sf::RectangleShape();
	m_MiniMapBG3.setSize(sf::Vector2f(200, 200));
	m_MiniMapBG3.setFillColor(sf::Color::Black);
	m_MiniMapBG3.setOrigin(m_MiniMapBG3.getGlobalBounds().width / 2, m_MiniMapBG3.getGlobalBounds().height / 2);
	_renderWindow->mapCoordsToPixel(m_MiniMapBG3.getPosition());
	// Assigning Sprite Texture And Drawing (MiniMap)
	m_MiniMapShape = sf::RectangleShape();
	m_MiniMapShape.setTexture(&m_miniMap->getTexture());
	m_MiniMapShape.setSize(sf::Vector2f(200, 200));
	//test.setFillColor(sf::Color::White);
	m_MiniMapShape.setOrigin(m_MiniMapShape.getGlobalBounds().width / 2, m_MiniMapShape.getGlobalBounds().height / 2);
	_renderWindow->mapCoordsToPixel(m_MiniMapShape.getPosition());
	
	// World BackGround
	m_MiniMapWorldBackGround = sf::Sprite();
	m_MiniMapWorldBackGround.setTexture(*m_Sky, true);
	m_MiniMapWorldBackGround.setOrigin(m_MiniMapWorldBackGround.getGlobalBounds().width / 2, m_MiniMapWorldBackGround.getGlobalBounds().height / 2);
	m_MiniMapWorldBackGround.setScale(12, 12);
}

void GUI::InventoryUI(sf::RenderWindow* _renderWindow, CPlayer* _player, sf::View& _uiView, sf::View& _worldView)
{

	// Row 1
	for (int i = 0; i < 10; i++)
	{
		
		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + (i * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 70);
		_renderWindow->mapCoordsToPixel(m_InventorySlotMap[i].getPosition());
		_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
		_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition());
	}

	// Row 2
	for (int i = 10; i < 20; i++)
	{
		

		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 10) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 135);
		_renderWindow->mapCoordsToPixel(m_InventorySlotMap[i].getPosition());
		_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);

		_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition());

		
	}
	// Row 3
	for (int i = 20; i < 30; i++)
	{
		

		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 20) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 200);
		_renderWindow->mapCoordsToPixel(m_InventorySlotMap[i].getPosition());
		

		_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
		_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition());
	}
	// Row 4
	for (int i = 30; i < 40; i++)
	{
		

		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 30) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265);
		_renderWindow->mapCoordsToPixel(m_InventorySlotMap[i].getPosition());
		

		_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
		_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition());
	}
	// Row 5
	for (int i = 40; i < 50; i++)
	{
		

		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 40) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65);
		_renderWindow->mapCoordsToPixel(m_InventorySlotMap[i].getPosition());
		

		_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
		_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition());
	}
}

void GUI::InitInventoryUI()
{
	// Current Item
	m_CurrentItemIndicator.setTexture(*m_CIITexture, true);
	m_CurrentItemIndicator.setOrigin(m_CurrentItemIndicator.getGlobalBounds().width / 2, m_CurrentItemIndicator.getGlobalBounds().height / 2);


	// Row 1
	for (int i = 0; i < 10; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
		m_InventorySlotMap.emplace(i, test);
		
		//
		//// Actual Items
		//sf::Sprite ITEM = sf::Sprite();
		//ITEM.setTexture(*m_Dirt, true);
		//ITEM.setScale(sf::Vector2f(0.4, 0.4));
		//ITEM.setOrigin(ITEM.getGlobalBounds().width / 2, ITEM.getGlobalBounds().height / 2);
		//m_InventoryMap.emplace(i, ITEM);
	}


	// Row 2
	for (int i = 10; i < 20; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
		m_InventorySlotMap.emplace(i, test);

		//
		//// Actual Items
		//sf::Sprite ITEM = sf::Sprite();
		//ITEM.setTexture(*m_Dirt, true);
		//ITEM.setScale(sf::Vector2f(0.4, 0.4));
		//ITEM.setOrigin(ITEM.getGlobalBounds().width / 2, ITEM.getGlobalBounds().height / 2);
		//m_InventoryMap.emplace(i, ITEM);
		
	}
	// Row 3
	for (int i = 20; i < 30; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
		m_InventorySlotMap.emplace(i, test);

		////
		//// Actual Items
		//sf::Sprite ITEM = sf::Sprite();
		//ITEM.setTexture(*m_Dirt, true);
		//ITEM.setScale(sf::Vector2f(0.4, 0.4));
		//ITEM.setOrigin(ITEM.getGlobalBounds().width / 2, ITEM.getGlobalBounds().height / 2);
		//m_InventoryMap.emplace(i, ITEM);
	}
	// Row 4
	for (int i = 30; i < 40; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
		m_InventorySlotMap.emplace(i, test);

		//
		//// Actual Items
		//sf::Sprite ITEM = sf::Sprite();
		//ITEM.setTexture(*m_Dirt, true);
		//ITEM.setScale(sf::Vector2f(0.4, 0.4));
		//ITEM.setOrigin(ITEM.getGlobalBounds().width / 2, ITEM.getGlobalBounds().height / 2);
		//m_InventoryMap.emplace(i, ITEM);
	}
	// Row 5
	for (int i = 40; i < 50; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
		m_InventorySlotMap.emplace(i, test);

		//
		//// Actual Items
		//sf::Sprite ITEM = sf::Sprite();
		//ITEM.setTexture(*m_Dirt, true);
		//ITEM.setScale(sf::Vector2f(0.4, 0.4));
		//ITEM.setOrigin(ITEM.getGlobalBounds().width / 2, ITEM.getGlobalBounds().height / 2);
		//m_InventoryMap.emplace(i, ITEM);
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
	m_Sky->loadFromFile("Images/TerrariaBG.jpg"); // Sky Blue
	m_Sky->setSmooth(true);
	m_ItemSpacer = new sf::Texture();
	m_ItemSpacer->loadFromFile("Images/ItemSpacer.png");
	m_HeartFull = new sf::Texture();
	m_HeartFull->loadFromFile("Images/HeartFull.png");
	m_HeartEmpty = new sf::Texture();
	m_HeartEmpty->loadFromFile("Images/HeartEmpty.png");
	m_CIITexture = new sf::Texture();
	m_CIITexture->loadFromFile("Images/CurrentItem.png");

	m_MousePos.setTexture(*m_MousePosTex, true);
	m_MousePos.setOrigin(m_MousePos.getGlobalBounds().width / 2, m_MousePos.getGlobalBounds().height / 2);

	// Set Smooth
	m_Dirt->setSmooth(true);
	m_Grass->setSmooth(true);
	m_Sky->setSmooth(true);
	m_ItemSpacer->setSmooth(true);
	m_HeartFull->setSmooth(true);
	m_HeartEmpty->setSmooth(true);
	m_CIITexture->setSmooth(true);
}



void GUI::CraftingUI(sf::RenderWindow* _renderWindow, CPlayer* _player)
{
	if (_player->m_bInventoryOpen)
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

void GUI::Render(sf::RenderWindow* _renderWindow, CPlayer* _player)
{
	if (_player->m_bInventoryOpen)
	{
		// Render
		for (int i = 0; i < m_InventorySlotMap.size(); i++)
		{
			_renderWindow->draw(m_InventorySlotMap[i]);
			_renderWindow->draw(_player->m_InventoryMap[i].GetShape());
		}
	}
	else
	{
		// Render
		for (int i = 0; i < 10; i++)
		{
			_renderWindow->draw(m_InventorySlotMap[i]);
			_renderWindow->draw(_player->m_InventoryMap[i].GetShape());
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
	
	m_InventorySlotMap.clear();
	delete m_miniMap;
	delete m_HeartEmpty;
	delete m_HeartFull;
	delete m_Sky;
	delete m_MousePosTex;
	delete m_Dirt;
	delete m_Grass;
	delete m_ItemSpacer;
	delete m_CIITexture;
	m_CIITexture = nullptr;
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
