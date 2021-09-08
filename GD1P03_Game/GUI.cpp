#include "GUI.h"


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
	m_InventoryItemStackCounters.clear();
	m_InventorySlotMap.clear();
	delete m_Planks;
	delete m_miniMap;
	delete m_HeartEmpty;
	delete m_HeartFull;
	delete m_Sky;
	delete m_MousePosTex;
	delete m_Dirt;
	delete m_Grass;
	delete m_ItemSpacer;
	delete m_CIITexture;
	delete m_Stone;
	delete m_Wood;
	delete m_Sand;
	delete m_MossyBrick;
	delete m_MousePointerTex;
	delete m_Chest;
	delete m_Leaves;
	m_Leaves = nullptr;
	m_Chest = nullptr;
	m_MousePointerTex = nullptr;
	m_MossyBrick = nullptr;
	m_Stone = nullptr;
	m_Wood = nullptr;
	m_Sand = nullptr;
	m_Planks = nullptr;
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

void GUI::HealthUI(sf::RenderWindow* _renderWindow, CPlayer* _player)
{
	std::string string = "Life: " + ToString(_player->GetCurrentHP()) + " / " + ToString(_player->GetMaxHP());
	m_HealthText.setString(string);
	m_HealthText.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 110, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 15);
	_renderWindow->draw(m_HealthText);

	if (_player->GetCurrentHP() > 0)
	{
		heart1.setTexture(*m_HeartFull, true);
		if (_player->GetCurrentHP() >= 20)
		{
			heart2.setTexture(*m_HeartFull, true);
			if (_player->GetCurrentHP() >= 40)
			{
				heart3.setTexture(*m_HeartFull, true);
				if (_player->GetCurrentHP() >= 60)
				{
					heart4.setTexture(*m_HeartFull, true);
					if (_player->GetCurrentHP() >= 80)
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

sf::Text GUI::InitHealthUI(CPlayer* _player)
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
	m_HealthString = "Life: " + ToString(_player->GetCurrentHP()) + " / " + ToString(_player->GetMaxHP());
	m_HealthText.setString(m_HealthString);
	m_HealthText.setFont(m_Font);
	m_HealthText.setFillColor(sf::Color::White);
	m_HealthText.setOutlineThickness(0.75f);
	m_HealthText.setOutlineColor(sf::Color::Black);
	m_HealthText.setOrigin(m_HealthText.getGlobalBounds().width / 2, m_HealthText.getGlobalBounds().height / 2);
	m_HealthText.setCharacterSize(18);
	return m_HealthText;
}

void GUI::MiniMapUI(sf::RenderWindow* _renderWindow, std::list<CBlock>& _chunk, std::list<sf::RectangleShape>& _skyChunk, CPlayer* _player)
{
	// Assigning Render Texture View and Zooming
	sf::View MiniMapView = sf::View(_player->GetShape().getPosition(), sf::Vector2f(200.0f, 200.0f));
	MiniMapView.zoom(57);
	m_miniMap->setView(MiniMapView);
	
	m_miniMap->clear();

	m_MiniMapWorldBackGround.setPosition(_renderWindow->getView().getCenter());
	m_miniMap->draw(m_MiniMapWorldBackGround);

	// Draw All Blocks In Radius 4.0f
	std::list<CBlock>::iterator it;
	// Blocks
	for (it = _chunk.begin(); it != _chunk.end(); it++)
	{
		float Mag1 = sqrt(((it->GetShape().getPosition().x - _player->GetShape().getPosition().x) * (it->GetShape().getPosition().x - _player->GetShape().getPosition().x)) + ((it->GetShape().getPosition().y - _player->GetShape().getPosition().y) * (it->GetShape().getPosition().y - _player->GetShape().getPosition().y)));
		if (Mag1 < 1920 * 4.0f)
		{
			m_miniMap->draw(it->GetShape());
		}
		if (Mag1 < _renderWindow->getSize().x * 1.0f)
		{
			it->GetShape().setColor(sf::Color::White);
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
	m_miniMap->draw(_player->m_MapIcon);
	
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

void GUI::InventoryUI(sf::RenderWindow* _renderWindow, CPlayer* _player, sf::View& _uiView, sf::View& _worldView, sf::Event& _event)
{
	_renderWindow->setView(_uiView);

	sf::Vector2f MousePos = _renderWindow->mapPixelToCoords((sf::Mouse::getPosition(*_renderWindow)), _uiView);
	m_MousePointer.setPosition(MousePos);

	// Row 1
	for (int i = 0; i < 10; i++)
	{
		_renderWindow->mapCoordsToPixel(m_InventorySlotMap[i].getPosition(), _uiView);
		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + (i * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 70);

		LetGoOfItemInInventory(_renderWindow, _uiView, _worldView, _event, _player, i);

		m_MousePointer.setPosition(MousePos);

		//ClickedItemInInventory(_event, _player, i);
		if (_player->m_bInventoryOpen && sf::Mouse::isButtonPressed(sf::Mouse::Left) && _player->m_InventoryMap[i].GetShape().getGlobalBounds().intersects(m_MousePointer.getGlobalBounds()) && _player->m_bInventoryOpen)
		{
			_player->m_InventoryMap[i].SetPosition(MousePos.x - 7, MousePos.y - 7);
		}
		else
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
			
		}
		//std::cout << _player->m_InventoryMap[0].GetPosition().x << _player->m_InventoryMap[0].GetPosition().y << std::endl;

		
		m_InventorySlotMap[i].setTexture(*m_ItemSpacer);

		m_InventoryItemStackCounters[i].setPosition(m_InventorySlotMap[i].getPosition().x - 8, m_InventorySlotMap[i].getPosition().y + 18);
		if (_player->m_InventoryStackValues[i] <= 1)
		{
			m_InventoryItemStackCounters[i].setString("");
		}
		else
		{
			m_InventoryItemStackCounters[i].setString(std::to_string(_player->m_InventoryStackValues[i]));
		}
	}
	// Row 2
	for (int i = 10; i < 20; i++)
	{
		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 10) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 135);
		_renderWindow->mapCoordsToPixel(m_InventorySlotMap[i].getPosition(), _uiView);

		LetGoOfItemInInventory(_renderWindow, _uiView, _worldView, _event, _player, i);

		m_MousePointer.setPosition(MousePos);

		//ClickedItemInInventory(_event, _player, i);
		if (_player->m_bInventoryOpen && sf::Mouse::isButtonPressed(sf::Mouse::Left) && _player->m_InventoryMap[i].GetShape().getGlobalBounds().intersects(m_MousePointer.getGlobalBounds()) && _player->m_bInventoryOpen)
		{
			_player->m_InventoryMap[i].SetPosition(MousePos.x - 7, MousePos.y - 7);
		}
		else
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
		}

		m_InventoryItemStackCounters[i].setPosition(m_InventorySlotMap[i].getPosition().x - 8, m_InventorySlotMap[i].getPosition().y + 18);
		if (_player->m_InventoryStackValues[i] <= 1)
		{
			m_InventoryItemStackCounters[i].setString("");
		}
		else
		{
			m_InventoryItemStackCounters[i].setString(std::to_string(_player->m_InventoryStackValues[i]));
		}
	}
	// Row 3
	for (int i = 20; i < 30; i++)
	{
		

		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 20) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 200);
		_renderWindow->mapCoordsToPixel(m_InventorySlotMap[i].getPosition(), _uiView);

		LetGoOfItemInInventory(_renderWindow, _uiView, _worldView, _event, _player, i);

		m_MousePointer.setPosition(MousePos);

		//ClickedItemInInventory(_event, _player, i);
		if (_player->m_bInventoryOpen && sf::Mouse::isButtonPressed(sf::Mouse::Left) && _player->m_InventoryMap[i].GetShape().getGlobalBounds().intersects(m_MousePointer.getGlobalBounds()) && _player->m_bInventoryOpen)
		{
			_player->m_InventoryMap[i].SetPosition(MousePos.x - 7, MousePos.y - 7);
		}
		else
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
		}

		m_InventoryItemStackCounters[i].setPosition(m_InventorySlotMap[i].getPosition().x - 8, m_InventorySlotMap[i].getPosition().y + 18);
		if (_player->m_InventoryStackValues[i] <= 1)
		{
			m_InventoryItemStackCounters[i].setString("");
		}
		else
		{
			m_InventoryItemStackCounters[i].setString(std::to_string(_player->m_InventoryStackValues[i]));
		}
	}
	// Row 4
	for (int i = 30; i < 40; i++)
	{
		

		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 30) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265);
		_renderWindow->mapCoordsToPixel(m_InventorySlotMap[i].getPosition(), _uiView);

		LetGoOfItemInInventory(_renderWindow, _uiView, _worldView, _event, _player, i);

		m_MousePointer.setPosition(MousePos);

		//ClickedItemInInventory(_event, _player, i);
		if (_player->m_bInventoryOpen && sf::Mouse::isButtonPressed(sf::Mouse::Left) && _player->m_InventoryMap[i].GetShape().getGlobalBounds().intersects(m_MousePointer.getGlobalBounds()) && _player->m_bInventoryOpen)
		{
			_player->m_InventoryMap[i].SetPosition(MousePos.x - 7, MousePos.y - 7);
		}
		else
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
		}

		m_InventoryItemStackCounters[i].setPosition(m_InventorySlotMap[i].getPosition().x - 8, m_InventorySlotMap[i].getPosition().y + 18);
		if (_player->m_InventoryStackValues[i] <= 1)
		{
			m_InventoryItemStackCounters[i].setString("");
		}
		else
		{
			m_InventoryItemStackCounters[i].setString(std::to_string(_player->m_InventoryStackValues[i]));
		}
	}
	// Row 5
	for (int i = 40; i < 50; i++)
	{
		
		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 40) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65);

		LetGoOfItemInInventory(_renderWindow,_uiView,_worldView,_event,_player,i);

		m_MousePointer.setPosition(MousePos);
		if (_player->m_bInventoryOpen && sf::Mouse::isButtonPressed(sf::Mouse::Left) && _player->m_InventoryMap[i].GetShape().getGlobalBounds().intersects(m_MousePointer.getGlobalBounds()) && _player->m_bInventoryOpen)
		{
			_player->m_InventoryMap[i].SetPosition(MousePos.x - 7, MousePos.y - 7);
		}
		else
		{
			_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
		}

		m_InventoryItemStackCounters[i].setPosition(m_InventorySlotMap[i].getPosition().x - 8, m_InventorySlotMap[i].getPosition().y + 18);
		if (_player->m_InventoryStackValues[i] <= 1)
		{
			m_InventoryItemStackCounters[i].setString("");
		}
		else
		{
			m_InventoryItemStackCounters[i].setString(std::to_string(_player->m_InventoryStackValues[i]));
		}
	}

	m_InventorySlotMap[_player->m_CurrentItemIndex].setTexture(*m_CIITexture);

}

void GUI::InitInventoryUI(CPlayer* _player, sf::RenderWindow* _renderWindow)
{
	
	// Row 1
	for (int i = 0; i < 10; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
		m_InventorySlotMap.emplace(i, test);
		sf::Color color = m_InventorySlotMap[i].getColor();
		color.a = 230.0f;
		m_InventorySlotMap[i].setColor(color);

		sf::Text stackcounter = sf::Text();
		stackcounter.setFont(m_Font);
		stackcounter.setCharacterSize(18);
		stackcounter.setFillColor(sf::Color::White);
		stackcounter.setOutlineThickness(0.75f);
		stackcounter.setOutlineColor(sf::Color::Black);
		stackcounter.setString("");
		_renderWindow->mapCoordsToPixel(stackcounter.getPosition());
		m_InventoryItemStackCounters.insert({ i, stackcounter });;
		m_InventoryItemStackCounters[i].setOrigin(m_InventoryItemStackCounters[i].getGlobalBounds().width / 2, m_InventoryItemStackCounters[i].getGlobalBounds().height / 2);
		
		_player->m_InventoryStackValues.emplace(i, 0);
	}


	// Row 2
	for (int i = 10; i < 20; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
		m_InventorySlotMap.emplace(i, test);
		sf::Color color = m_InventorySlotMap[i].getColor();
		color.a = 230.0f;
		m_InventorySlotMap[i].setColor(color);
		
		sf::Text stackcounter = sf::Text();
		stackcounter.setFont(m_Font);
		stackcounter.setCharacterSize(18);
		stackcounter.setFillColor(sf::Color::White);
		stackcounter.setOutlineThickness(0.75f);
		stackcounter.setOutlineColor(sf::Color::Black);
		stackcounter.setString("");
		m_InventoryItemStackCounters.insert({ i, stackcounter });
		m_InventoryItemStackCounters[i].setOrigin(m_InventoryItemStackCounters[i].getGlobalBounds().width / 2, m_InventoryItemStackCounters[i].getGlobalBounds().height / 2);
	
		_player->m_InventoryStackValues.emplace(i, 0);
	}
	// Row 3
	for (int i = 20; i < 30; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
		m_InventorySlotMap.emplace(i, test);
		sf::Color color = m_InventorySlotMap[i].getColor();
		color.a = 230.0f;
		m_InventorySlotMap[i].setColor(color);
		
		sf::Text stackcounter = sf::Text();
		stackcounter.setFont(m_Font);
		stackcounter.setCharacterSize(18);
		stackcounter.setFillColor(sf::Color::White);
		stackcounter.setOutlineThickness(0.75f);
		stackcounter.setOutlineColor(sf::Color::Black);
		stackcounter.setString("");
		m_InventoryItemStackCounters.insert({ i, stackcounter });
		m_InventoryItemStackCounters[i].setOrigin(m_InventoryItemStackCounters[i].getGlobalBounds().width / 2, m_InventoryItemStackCounters[i].getGlobalBounds().height / 2);
	
		_player->m_InventoryStackValues.emplace(i, 0);
	}
	// Row 4
	for (int i = 30; i < 40; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
		m_InventorySlotMap.emplace(i, test);
		sf::Color color = m_InventorySlotMap[i].getColor();
		color.a = 230.0f;
		m_InventorySlotMap[i].setColor(color);
		
		sf::Text stackcounter = sf::Text();
		stackcounter.setFont(m_Font);
		stackcounter.setCharacterSize(18);
		stackcounter.setFillColor(sf::Color::White);
		stackcounter.setOutlineThickness(0.75f);
		stackcounter.setOutlineColor(sf::Color::Black);
		stackcounter.setString("");
		m_InventoryItemStackCounters.insert({ i, stackcounter });
		m_InventoryItemStackCounters[i].setOrigin(m_InventoryItemStackCounters[i].getGlobalBounds().width / 2, m_InventoryItemStackCounters[i].getGlobalBounds().height / 2);
		
		_player->m_InventoryStackValues.emplace( i, 0 );
	}
	// Row 5
	for (int i = 40; i < 50; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);
		m_InventorySlotMap.emplace(i, test);
		sf::Color color = m_InventorySlotMap[i].getColor();
		color.a = 230.0f;
		m_InventorySlotMap[i].setColor(color);
		
		sf::Text stackcounter = sf::Text();
		stackcounter.setFont(m_Font);
		stackcounter.setCharacterSize(18);
		stackcounter.setFillColor(sf::Color::White);
		stackcounter.setOutlineThickness(0.75f);
		stackcounter.setOutlineColor(sf::Color::Black);
		stackcounter.setString("");
		m_InventoryItemStackCounters.insert({ i, stackcounter });
		m_InventoryItemStackCounters[i].setOrigin(m_InventoryItemStackCounters[i].getGlobalBounds().width / 2, m_InventoryItemStackCounters[i].getGlobalBounds().height / 2);
		
		_player->m_InventoryStackValues.emplace( i, 0 );
	}

	// Mouse Pointer
	m_MousePointer = sf::Sprite();
	m_MousePointer.setTexture(*m_MousePointerTex);
	m_MousePointer.setOrigin(m_MousePointer.getGlobalBounds().width / 2, m_MousePointer.getGlobalBounds().height / 2);
	
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
	m_Planks = new sf::Texture();
	m_Planks->loadFromFile("Images/Planks.png");
	m_Stone = new sf::Texture();
	m_Stone->loadFromFile("Images/Stone.png");
	m_Wood = new sf::Texture();
	m_Wood->loadFromFile("Images/Wood.png");
	m_Sand = new sf::Texture();
	m_Sand->loadFromFile("Images/Sand.png");
	m_MossyBrick = new sf::Texture();
	m_MossyBrick->loadFromFile("Images/MossyBrick.png");
	m_MousePointerTex = new sf::Texture();
	m_MousePointerTex->loadFromFile("Images/Mouse.png");
	m_Chest = new sf::Texture();
	m_Chest->loadFromFile("Images/Chest.png");
	m_Leaves = new sf::Texture();
	m_Leaves->loadFromFile("Images/Leaves.png");

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
	m_Planks->setSmooth(true);
	m_Stone->setSmooth(true);
	m_Sand->setSmooth(true);
	m_Wood->setSmooth(true);
	m_MossyBrick->setSmooth(true);
	m_MousePointerTex->setSmooth(true);
	m_Chest->setSmooth(true);
	m_Leaves->setSmooth(true);
}

void GUI::HotBarScrolling(sf::Event& _event, CPlayer* _player)
{
	// Canvas Zooming
	if (_event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
	{
		if (_event.mouseWheelScroll.delta >= 1)
		{
			if (_player->m_CurrentItemIndex < 10 && _player->m_CurrentItemIndex >= 0)
			{
				_player->m_CurrentItemIndex--;
			}
			if (_player->m_CurrentItemIndex < 0)
			{
				_player->m_CurrentItemIndex = 9;
			}
			std::cout << "zoom In" << std::endl;

			// Items
			for (std::map<int, CBlock>::iterator iit = _player->m_InventoryMap.begin(); iit != _player->m_InventoryMap.end(); iit++)
			{
				if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == iit->second.PICKAXE)
				{
					std::cout << "Pickaxe Selected!" << std::endl;
					iit->second.m_bIsItemAndSelected = true;
				}
			}
		}
		else if (_event.mouseWheelScroll.delta <= -1)
		{
			if (_player->m_CurrentItemIndex < 10)
			{
				_player->m_CurrentItemIndex++;
			}
			if (_player->m_CurrentItemIndex > 9)
			{
				_player->m_CurrentItemIndex = 0;
			}
			std::cout << "zoom out" << std::endl;

			// Items
			for (std::map<int, CBlock>::iterator iit = _player->m_InventoryMap.begin(); iit != _player->m_InventoryMap.end(); iit++)
			{
				if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == iit->second.PICKAXE)
				{
					std::cout << "Pickaxe Selected!" << std::endl;
					iit->second.m_bIsItemAndSelected = true;
				}
			}
		}
	}
}

void GUI::LetGoOfItemInInventory(sf::RenderWindow* _renderWindow, sf::View& _uiView, sf::View& _worldView, sf::Event& _event, CPlayer* _player, int _iterator)
{

	_renderWindow->setView(_uiView);
	if (m_MousePointer.getGlobalBounds().intersects(_player->m_InventoryMap[_iterator].GetShape().getGlobalBounds()) && _player->m_bInventoryOpen)
	{
		for (std::map<int, sf::Sprite>::iterator sit = m_InventorySlotMap.begin(); sit != m_InventorySlotMap.end(); sit++)
		{
			if (sit->second.getGlobalBounds().contains(m_MousePointer.getPosition()) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				_player->m_InventoryMap[_iterator].m_bIsMovingItemInInv = false;
				_player->m_InventoryMap[sit->first];
				_player->m_InventoryStackValues[sit->first];
				std::map<int, CBlock>::iterator cit = _player->m_InventoryMap.find(_iterator);
				std::map<int, int>::iterator vit = _player->m_InventoryStackValues.find(_iterator);

				//std::cout << cit->first << "th ITEM" << std::endl;
				//std::cout << sit->first << "th SPACE" << std::endl;

				if (sit->first != cit->first)
				{
					std::cout << "Mouse Released!" << std::endl;
					_player->m_InventoryMap[_iterator].m_PositionInInventory = sit->first;
					std::swap(_player->m_InventoryStackValues[sit->first], vit->second);
					std::swap(_player->m_InventoryMap[sit->first], cit->second);
					_player->m_InventoryMap.erase(cit);
					_player->m_InventoryStackValues.erase(vit);

					vit = _player->m_InventoryStackValues.end();
					sit = m_InventorySlotMap.end();
				}
				else if (sit->first == cit->first)
				{
					
				}
			}
		}
	}

}

void GUI::ClickedItemInInventory(sf::Event& _event, CPlayer* _player, int _iterator)
{
	if (_player->m_bInventoryOpen && _event.type == sf::Event::MouseButtonPressed && _player->m_InventoryMap[_iterator].GetShape().getGlobalBounds().intersects(m_MousePointer.getGlobalBounds()) && _player->m_bInventoryOpen)
	{
		if (!_player->m_InventoryMap[_iterator].m_bIsMovingItemInInv)
		{
			_player->m_InventoryMap[_iterator].m_bIsMovingItemInInv = true;
		}
	}
}

void GUI::DropCurrentlyHeldItem(CPlayer* _player, sf::Event& _event)
{
	for (int i = 0; i < _player->m_InventoryMap.size(); i++)
	{
		if (m_InventorySlotMap[i].getGlobalBounds().contains(m_MousePointer.getPosition()))
		{
			std::cout << "Droped Item" << std::endl;
			if (_player->m_InventoryStackValues[i] == 0)
			{
				break;
			}
			else if (_player->m_InventoryStackValues[i] <= 1)
			{
				_player->RemoveItemFromInventory(i);
				break;
			}
			else
			{
				_player->m_InventoryStackValues[i]--;
				break;
			}
			break;
		}

	}
}

bool GUI::MousePointerOverSlot()
{
	for (int i = 0; i < m_InventorySlotMap.size(); i++)
	{
		if (m_InventorySlotMap[i].getGlobalBounds().contains(m_MousePointer.getPosition()))
		{
			return true;
		}
	}
	return false;
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

void GUI::Render(sf::RenderWindow* _renderWindow, CPlayer* _player, sf::View& _worldView, sf::View& _uiView)
{
	if (_player->m_bInventoryOpen)
	{
		// Render
		for (int i = 0; i < m_InventorySlotMap.size(); i++)
		{
			_renderWindow->draw(m_InventorySlotMap[i]);
			_renderWindow->draw(_player->m_InventoryMap[i].GetShape());
			_renderWindow->draw(m_InventoryItemStackCounters[i]);
		}
	}
	else
	{
		// Render
		for (int i = 0; i < 10; i++)
		{
			_renderWindow->draw(m_InventorySlotMap[i]);
			_renderWindow->draw(_player->m_InventoryMap[i].GetShape());
			_renderWindow->draw(m_InventoryItemStackCounters[i]);
		}
	}

	if (_player->m_bInventoryOpen)
	{
		//std::cout << _player->m_InventoryMap[0].GetShape().getPosition().x << "-" << _player->m_InventoryMap[0].GetShape().getPosition().y << " Item" << std::endl;
		//std::cout << m_MousePointer.getPosition().x << "-" << m_MousePointer.getPosition().y << " Mouse" << std::endl;
		_renderWindow->draw(m_MousePointer);
	}
	else
	{
		_renderWindow->setView(_worldView);
		_renderWindow->draw(m_MousePos);
		_renderWindow->setView(_uiView);
	}
}

std::string GUI::ToString(int32 integer)
{
	char numstr[10]; // enough to hold all numbers up to 32-bits
	sprintf_s(numstr, "%i", integer);
	return numstr;
}

void GUI::SetPlayer(CPlayer* _player)
{
	//m_Player = _player;
}

CPlayer* GUI::GetPlayer()
{
	return nullptr;
}

void GUI::SetFont(sf::Font& _font)
{
	m_Font = _font;
}
