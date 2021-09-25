#include "GUI.h"


GUI::GUI()
{
	m_Font = sf::Font();
	m_Font.loadFromFile("Fonts/ANDYB.TTF");
	m_TempBlock = nullptr;

	m_CraftTimer = new sf::Clock();
	m_FirstEmpySlotTimer = new sf::Clock();
}

GUI::~GUI()
{
	m_InventoryItemStackCounters.clear();
	m_InventorySlotMap.clear();
	m_CraftingSlots.clear();
	m_CraftList.clear();
	m_ChestSlots.clear();
	m_ChestItemStackCounters.clear();
	
	m_TempBlock = nullptr;
	delete m_miniMap;
	m_miniMap = nullptr;
	delete m_CraftTimer;
	m_CraftTimer = nullptr;
	delete m_FirstEmpySlotTimer;
	m_FirstEmpySlotTimer = nullptr;
}

void GUI::HealthUI(sf::RenderWindow* _renderWindow, CPlayer* _player, CTextureMaster* _textureMaster)
{
	std::string string = "Life: " + ToString(_player->GetCurrentHP()) + " / " + ToString(_player->GetMaxHP());
	m_HealthText.setString(string);
	m_HealthText.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 110, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 15);
	_renderWindow->draw(m_HealthText);

	if (_player->GetCurrentHP() > 0)
	{
		heart1.setTexture(*_textureMaster->m_HeartFull, true);
		if (_player->GetCurrentHP() >= 20)
		{
			heart2.setTexture(*_textureMaster->m_HeartFull, true);
			if (_player->GetCurrentHP() >= 40)
			{
				heart3.setTexture(*_textureMaster->m_HeartFull, true);
				if (_player->GetCurrentHP() >= 60)
				{
					heart4.setTexture(*_textureMaster->m_HeartFull, true);
					if (_player->GetCurrentHP() >= 80)
					{
						heart5.setTexture(*_textureMaster->m_HeartFull, true);
					}
					else
					{
						heart5.setTexture(*_textureMaster->m_HeartEmpty, true);
					}
					
				}
				else
				{
					heart4.setTexture(*_textureMaster->m_HeartEmpty, true);
					heart5.setTexture(*_textureMaster->m_HeartEmpty, true);
				}
				
			}
			else
			{
				heart3.setTexture(*_textureMaster->m_HeartEmpty, true);
				heart4.setTexture(*_textureMaster->m_HeartEmpty, true);
				heart5.setTexture(*_textureMaster->m_HeartEmpty, true);
			}
			
		}
		else
		{
			heart2.setTexture(*_textureMaster->m_HeartEmpty, true);
			heart3.setTexture(*_textureMaster->m_HeartEmpty, true);
			heart4.setTexture(*_textureMaster->m_HeartEmpty, true);
			heart5.setTexture(*_textureMaster->m_HeartEmpty, true);
		}
	}
	else
	{
		heart1.setTexture(*_textureMaster->m_HeartEmpty, true);
		heart2.setTexture(*_textureMaster->m_HeartEmpty, true);
		heart3.setTexture(*_textureMaster->m_HeartEmpty, true);
		heart4.setTexture(*_textureMaster->m_HeartEmpty, true);
		heart5.setTexture(*_textureMaster->m_HeartEmpty, true);
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

void GUI::MiniMapUI(sf::RenderWindow* _renderWindow, std::list<CBlock>& _chunk, std::list<sf::RectangleShape>& _skyChunk, CPlayer* _player, sf::Shader* _shader, sf::Shader* _shaderUI)
{

	// Assigning Render Texture View and Zooming
	sf::View MiniMapView = sf::View(_player->GetShape().getPosition(), sf::Vector2f(200.0f, 200.0f));
	MiniMapView.zoom(25);
	m_miniMap->setView(MiniMapView);

	m_MiniMapWorldBackGround.setPosition(_renderWindow->getView().getCenter());
	m_miniMap->draw(m_MiniMapWorldBackGround, _shaderUI);

	// Draw All Blocks In Radius
	std::list<CBlock>::iterator it;
	float Mag1 = 0;
	// Blocks
	for (it = _chunk.begin(); it != _chunk.end(); it++)
	{
		Mag1 = sqrt(((it->GetShape().getPosition().x - _player->GetShape().getPosition().x) * (it->GetShape().getPosition().x - _player->GetShape().getPosition().x)) + ((it->GetShape().getPosition().y - _player->GetShape().getPosition().y) * (it->GetShape().getPosition().y - _player->GetShape().getPosition().y)));
		if (Mag1 < 1920 * 1.8f)
		{
			m_miniMap->draw(it->GetShape(), _shaderUI);
		}
		else
		{
			continue;
		}
	}

	// Border
	m_MiniMapBG1.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 140, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 160);
	m_MiniMapBG2.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 140, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 160);
	m_MiniMapBG3.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 140, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 160);
	m_MiniMapShape.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 140, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 160);
	_renderWindow->draw(m_MiniMapBG1, _shaderUI);
	_renderWindow->draw(m_MiniMapBG2, _shaderUI);
	_renderWindow->draw(m_MiniMapBG3, _shaderUI);

	// Draw Player To RendTexture
	m_miniMap->draw(_player->m_MapIcon, _shaderUI);
	
	// Update RendTexture
	m_miniMap->display();
	/*m_MiniMapShape.getTexture()->copyToImage().saveToFile("MINIMAP.png");*/
	
	// Draw Shape With Applied RenderText To Main Window
	_renderWindow->draw(m_MiniMapShape, _shaderUI);
}

void GUI::InitMiniMap(sf::RenderWindow* _renderWindow, CTextureMaster* _textureMaster)
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
	m_MiniMapWorldBackGround.setTexture(*_textureMaster->m_Sky, true);
	m_MiniMapWorldBackGround.setOrigin(m_MiniMapWorldBackGround.getGlobalBounds().width / 2, m_MiniMapWorldBackGround.getGlobalBounds().height / 2);
	m_MiniMapWorldBackGround.setScale(12, 12);

	m_DrawTimer.restart();
}

void GUI::InventoryUI(sf::RenderWindow* _renderWindow, CPlayer* _player, sf::View& _uiView, sf::View& _worldView, sf::Event& _event, CTextureMaster* _textureMaster, std::list<CChest>& _chests)
{
	_renderWindow->setView(_uiView);

	sf::Vector2f MousePos = _renderWindow->mapPixelToCoords((sf::Mouse::getPosition(*_renderWindow)), _uiView);

	if (m_FirstEmpyChestSlotTimer.getElapsedTime().asSeconds() >= 0.1f)
	{
		FindFirstEmptyChestSlot(_chests);
		m_FirstEmpyChestSlotTimer.restart();
	}

	// Row 1
	for (int i = 0; i < 10; i++)
	{
		_renderWindow->mapCoordsToPixel(m_InventorySlotMap[i].getPosition(), _uiView);
		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + (i * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 70);

		m_MousePointer.setPosition(MousePos);

		//ClickedItemInInventory(_event, _player, i);
		if (bPlayerIsMovingAnItem(_player))
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			HoldItemInInventory(_player, _chests);
		}
		else
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
			m_InventoryItemStackCounters[i].setPosition(m_InventorySlotMap[i].getPosition().x - 8, m_InventorySlotMap[i].getPosition().y + 18);
		}
		//std::cout << _player->m_InventoryMap[0].GetPosition().x << _player->m_InventoryMap[0].GetPosition().y << std::endl;

		
		m_InventorySlotMap[i].setTexture(*_textureMaster->m_ItemSpacer);

		
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

		m_MousePointer.setPosition(MousePos);

		//ClickedItemInInventory(_event, _player, i);
		if (bPlayerIsMovingAnItem(_player))
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			HoldItemInInventory(_player, _chests);
		}
		else
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
			m_InventoryItemStackCounters[i].setPosition(m_InventorySlotMap[i].getPosition().x - 8, m_InventorySlotMap[i].getPosition().y + 18);
		}

		
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

		m_MousePointer.setPosition(MousePos);

		//ClickedItemInInventory(_event, _player, i);
		if (bPlayerIsMovingAnItem(_player))
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			HoldItemInInventory(_player, _chests);
		}
		else
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
			m_InventoryItemStackCounters[i].setPosition(m_InventorySlotMap[i].getPosition().x - 8, m_InventorySlotMap[i].getPosition().y + 18);
		}

		
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

		m_MousePointer.setPosition(MousePos);

		//ClickedItemInInventory(_event, _player, i);
		if (bPlayerIsMovingAnItem(_player))
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			HoldItemInInventory(_player, _chests);
		}
		else
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
			m_InventoryItemStackCounters[i].setPosition(m_InventorySlotMap[i].getPosition().x - 8, m_InventorySlotMap[i].getPosition().y + 18);
		}

		
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

		m_MousePointer.setPosition(MousePos);
		//ClickedItemInInventory(_event, _player, i);
		if (bPlayerIsMovingAnItem(_player))
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			HoldItemInInventory(_player, _chests);
		}
		else
		{
			_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);

			m_InventoryItemStackCounters[i].setPosition(m_InventorySlotMap[i].getPosition().x - 8, m_InventorySlotMap[i].getPosition().y + 18);
		}

		if (_player->m_InventoryStackValues[i] <= 1)
		{
			m_InventoryItemStackCounters[i].setString("");
		}
		else
		{
			m_InventoryItemStackCounters[i].setString(std::to_string(_player->m_InventoryStackValues[i]));
		}
	}

	m_InventorySlotMap[_player->m_CurrentItemIndex].setTexture(*_textureMaster->m_CIITexture);


}

void GUI::InitInventoryUI(CPlayer* _player, sf::RenderWindow* _renderWindow, CTextureMaster* _textureMaster)
{
	// Row 1
	for (int i = 0; i < 10; i++)
	{
		sf::Sprite test = sf::Sprite();
		test.setTexture(*_textureMaster->m_ItemSpacer, true);
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
		test.setTexture(*_textureMaster->m_ItemSpacer, true);
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
		test.setTexture(*_textureMaster->m_ItemSpacer, true);
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
		test.setTexture(*_textureMaster->m_ItemSpacer, true);
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
		test.setTexture(*_textureMaster->m_ItemSpacer, true);
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

	for (int i = 0; i < 50; i++)
	{
		std::cout << i << std::endl;
		m_InventoryItemStackCounters[i];
	}

	// Mouse Pointer
	m_MousePointer = sf::Sprite();
	m_MousePointer.setTexture(*_textureMaster->m_MousePointerTex);
	m_MousePointer.setOrigin(m_MousePointer.getGlobalBounds().width / 2, m_MousePointer.getGlobalBounds().height / 2);
	
}

void GUI::InitMousePosSprite(CTextureMaster* _textureMaster)
{
	m_MousePos.setTexture(*_textureMaster->m_MousePosTex, true);
	m_MousePos.setOrigin(m_MousePos.getGlobalBounds().width / 2, m_MousePos.getGlobalBounds().height / 2);
}

void GUI::HotBarScrolling(sf::Event& _event, CPlayer* _player)
{
	// Canvas Zooming
	if (_event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
	{
		if (_event.mouseWheelScroll.delta >= 1)
		{
			if (_player->bInventoryOpen())
			{
				// Cycle through crafting items
			}

			if (_player->m_CurrentItemIndex < 10 && _player->m_CurrentItemIndex >= 0 && !_player->bInventoryOpen())
			{
				_player->m_CurrentItemIndex--;
			}
			if (_player->m_CurrentItemIndex < 0 && !_player->bInventoryOpen())
			{
				_player->m_CurrentItemIndex = 9;
			}
			std::cout << "zoom In" << std::endl;

			// Items
			for (std::map<int, CBlock>::iterator iit = _player->m_InventoryMap.begin(); iit != _player->m_InventoryMap.end(); iit++)
			{
				if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::BASIC)
				{
					std::cout << "Pickaxe Selected!" << std::endl;
					iit->second.m_bIsItemAndSelected = true;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::IRON)
				{
					std::cout << "Iron Pickaxe Selected!" << std::endl;
					iit->second.m_bIsItemAndSelected = true;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLD)
				{
					std::cout << "Gold Pickaxe Selected!" << std::endl;
					iit->second.m_bIsItemAndSelected = true;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::DIAMOND)
				{
					std::cout << "Diamond Pickaxe Selected!" << std::endl;
					iit->second.m_bIsItemAndSelected = true;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen())
				{
					std::cout << "Bow Selected!" << std::endl;
					iit->second.m_bIsItemAndSelected = true;
				}
			}
		}
		else if (_event.mouseWheelScroll.delta <= -1)
		{
			if (_player->bInventoryOpen())
			{
				// Cycle through crafting items
			}

			if (_player->m_CurrentItemIndex < 10 && !_player->bInventoryOpen())
			{
				_player->m_CurrentItemIndex++;
			}
			if (_player->m_CurrentItemIndex > 9 && !_player->bInventoryOpen())
			{
				_player->m_CurrentItemIndex = 0;
			}
			std::cout << "zoom out" << std::endl;

			// Items
			for (std::map<int, CBlock>::iterator iit = _player->m_InventoryMap.begin(); iit != _player->m_InventoryMap.end(); iit++)
			{
				if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::BASIC)
				{
					std::cout << "Pickaxe Selected!" << std::endl;
					iit->second.m_bIsItemAndSelected = true;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::IRON)
				{
					std::cout << "Iron Pickaxe Selected!" << std::endl;
					iit->second.m_bIsItemAndSelected = true;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLD)
				{
					std::cout << "Gold Pickaxe Selected!" << std::endl;
					iit->second.m_bIsItemAndSelected = true;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::DIAMOND)
				{
					std::cout << "Diamond Pickaxe Selected!" << std::endl;
					iit->second.m_bIsItemAndSelected = true;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen())
				{
					std::cout << "Bow Selected!" << std::endl;
					iit->second.m_bIsItemAndSelected = true;
				}
			}
		}
	}
}

void GUI::InitHotBarScrolling(CPlayer* _player)
{
	// Items
	for (std::map<int, CBlock>::iterator iit = _player->m_InventoryMap.begin(); iit != _player->m_InventoryMap.end(); iit++)
	{
		if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE)
		{
			std::cout << "Pickaxe Selected!" << std::endl;
			iit->second.m_bIsItemAndSelected = true;
		}

		else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW)
		{
			std::cout << "Bow Selected!" << std::endl;
			iit->second.m_bIsItemAndSelected = true;
		}
	}
}

void GUI::LetGoOfItemInInventory(sf::RenderWindow* _renderWindow, sf::View& _uiView, sf::View& _worldView, sf::Event& _event, CPlayer* _player)
{
	// Set View As Function Called Is In Polled Update
	_renderWindow->setView(_uiView);

	if (!_player->m_bPlayerIsInChest)
	{
		for (int j = 0; j < m_InventorySlotMap.size(); j++)
		{
			if (bPlayerIsMovingAnItem(_player, j) && _player->bInventoryOpen())
			{
				for (std::map<int, sf::Sprite>::iterator sit = m_InventorySlotMap.begin(); sit != m_InventorySlotMap.end(); ++sit)
				{
					if (sit->second.getGlobalBounds().contains(m_MousePointer.getPosition()) && _event.type == sf::Event::MouseButtonReleased)
					{
						_player->m_InventoryMap[sit->first];
						_player->m_InventoryStackValues[sit->first];
						std::map<int, CBlock>::iterator cit = _player->m_InventoryMap.find(j);
						std::map<int, int>::iterator vit = _player->m_InventoryStackValues.find(j);

						if (sit->first != cit->first)
						{
							_player->m_InventoryMap[j].m_PositionInInventory = sit->first;
							_player->m_InventoryMap[sit->first].m_PositionInInventory = j;
							std::swap(_player->m_InventoryStackValues[sit->first], vit->second);
							std::swap(_player->m_InventoryMap[sit->first], cit->second);

							// Moved Item Into Currently Selected Slot?
							for (std::map<int, CBlock>::iterator iit = _player->m_InventoryMap.begin(); iit != _player->m_InventoryMap.end(); ++iit)
							{
								if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_bIsItemAndSelected == false)
								{
									std::cout << "Pickaxe Selected!" << std::endl;
									iit->second.m_bIsItemAndSelected = true;
									iit->second.m_bIsMovingItemInInv = false;
									break;
								}
								else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && iit->second.m_bIsItemAndSelected == false)
								{
									std::cout << "Bow Selected!" << std::endl;
									iit->second.m_bIsItemAndSelected = true;
									iit->second.m_bIsMovingItemInInv = false;
									break;
								}
							}

							// Debug
							std::cout << "Mouse Released On Selected Item!" << std::endl;

							// Saves My Sanity
							for (int i = 0; i < 50; i++)
							{
								_player->m_InventoryMap[i].m_bIsMovingItemInInv = false;
							}
							break;
						}
						else if (sit->first == cit->first)
						{

						}

						// Debug
						std::cout << "Mouse Released!" << std::endl;

						// Saves My Sanity
						for (int i = 0; i < 50; i++)
						{
							_player->m_InventoryMap[i].m_bIsMovingItemInInv = false;
						}
					}
					else if (_event.type == sf::Event::MouseButtonReleased)
					{
						// Saves My Sanity
						for (int i = 0; i < 50; i++)
						{
							_player->m_InventoryMap[i].m_bIsMovingItemInInv = false;
						}
					}
				}
			}
		}
	}
	
	_renderWindow->setView(_worldView);
	// Set View As Function Is Called In Polled Update
}

void GUI::ClickedItemInInventory(sf::Event& _event, CPlayer* _player, std::list<CChest>& _chests)
{
	if (_player != nullptr)
	{
		if (_player->m_bInventoryOpen)
		{
			for (int i = 0; i < _player->m_InventoryMap.size(); i++)
			{
				if (_player->m_bInventoryOpen && _player->m_InventoryMap[i].GetShape().getGlobalBounds().contains(m_MousePointer.getPosition()) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					for (CChest& chest : _chests)
					{
						if (!bPlayerIsMovingAnItem(_player) && chest.m_bIsOpen && chest.GetInventorySize() < 50 && _player->m_InventoryMap[i].GetShape().getGlobalBounds().contains(m_MousePointer.getPosition()))
						{
							for (int b = 0; b < 50; b++)
							{
								if (chest.m_InventoryStackValues[b] == 0)
								{
									std::cout << "Added Item To Chest!" << std::endl;

									for (int x = 0; x < _player->m_InventoryStackValues[i]; x++)
									{
										chest.AddItemToInventory(&_player->m_InventoryMap[i], true);
										std::cout << chest.m_InventoryStackValues[i] << std::endl;
									}

									_player->RemoveItemFromInventory(i);
									break;
								}
							}
							break;
						}
					}

					if (!_player->m_bPlayerIsInChest)
					{
						std::cout << "Moving Item In Inventory!" << std::endl;
						_player->m_InventoryMap[i].m_bIsMovingItemInInv = true;
					}

					break;
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (_player->m_bPlayerIsInChest)
					{
						for (CChest& chest : _chests)
						{
							for (int j = 0; j < chest.m_Inventory.size(); j++)
							{
								if (!bPlayerIsMovingAnItem(_player) && chest.m_bIsOpen && chest.m_Inventory[j].GetShape().getGlobalBounds().contains(m_MousePointer.getPosition()))
								{
									for (int c = 0; c < 50; c++)
									{
										if (_player->m_InventoryStackValues[c] == 0)
										{
											switch (chest.m_Inventory[j].m_Type)
											{
											case CBlock::BLOCKTYPE::DOOR:
											{
												chest.m_Inventory[j].m_PositionInInventory = c;

												for (int x = 0; x < chest.m_InventoryStackValues[j]; x++)
												{
													_player->AddItemToInventory(&chest.m_Inventory[j], c, false);
												}

												break;
											}
											case CBlock::BLOCKTYPE::PICKAXE:
											{
												chest.m_Inventory[j].m_PositionInInventory = c;
												for (int x = 0; x < chest.m_InventoryStackValues[j]; x++)
												{
													_player->AddItemToInventory(&chest.m_Inventory[j], c, false);
												}
												break;
											}
											case CBlock::BLOCKTYPE::BOW:
											{
												chest.m_Inventory[j].m_PositionInInventory = c;
												for (int x = 0; x < chest.m_InventoryStackValues[j]; x++)
												{
													_player->AddItemToInventory(&chest.m_Inventory[j], c, false);
												}
												break;
											}
											default:
											{
												chest.m_Inventory[j].m_PositionInInventory = c;
												for (int x = 0; x < chest.m_InventoryStackValues[j]; x++)
												{
													_player->AddItemToInventory(&chest.m_Inventory[j], c);
												}
												break;
											}
											}

											chest.RemoveItemFromInventory(j);
											break;
										}
									}
									return;

								}
							}
						}

					}
				}
			}
		}
	}
	
}

void GUI::HoldItemInInventory(CPlayer* _player, std::list<CChest>& _chests)
{
	for (int i = 0; i < m_InventorySlotMap.size(); i++)
	{
		if (_player->bInventoryOpen() && sf::Mouse::isButtonPressed(sf::Mouse::Left) && bPlayerIsMovingAnItem(_player, i) && !_player->m_bPlayerIsInChest)
		{
			_player->m_InventoryMap[i].GetShape().setPosition(m_MousePointer.getPosition());
			m_InventoryItemStackCounters[i].setPosition(_player->m_InventoryMap[i].GetShape().getPosition().x - 8, _player->m_InventoryMap[i].GetShape().getPosition().y + 18);
		}
		else if (_player->bInventoryOpen() && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !bPlayerIsMovingAnItem(_player, i))
		{
			_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
			m_InventoryItemStackCounters[i].setPosition(m_InventorySlotMap[i].getPosition().x - 8, m_InventorySlotMap[i].getPosition().y + 18);
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
			}
			else if (_player->m_InventoryStackValues[i] <= 1)
			{
				_player->RemoveItemFromInventory(i);
			}
			else
			{
				_player->m_InventoryStackValues[i]--;
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

void GUI::CraftingUI(sf::RenderWindow* _renderWindow, CPlayer* _player, CTextureMaster* _textureMaster, sf::View& _uiView)
{
	if (m_FirstEmpySlotTimer->getElapsedTime().asSeconds() >= 0.01f)
	{
		FindFirstEmptyInventorySlot(_player);
		m_FirstEmpySlotTimer->restart();
	}
	// Positions
	for (int i = 0; i < 11; i++)
	{
		_renderWindow->mapCoordsToPixel(m_CraftingSlots[i].getPosition(), _uiView);
		m_CraftingSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + (i * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65 + 65 + ((1) * 65));
	}

	// Color / Alpha
	for (CBlock& item : m_CraftList)
	{
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::DOOR)
		{
			item.SetPosition(m_CraftingSlots[0].getPosition().x + 4.5f, m_CraftingSlots[0].getPosition().y - 3.5f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::DOOR)
		{
			item.SetPosition(m_CraftingSlots[0].getPosition().x + 4.5f, m_CraftingSlots[0].getPosition().y - 3.5f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PLANKS)
		{
			item.SetPosition(m_CraftingSlots[1].getPosition().x, m_CraftingSlots[1].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PLANKS)
		{
			item.SetPosition(m_CraftingSlots[1].getPosition().x, m_CraftingSlots[1].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHEST)
		{
			item.SetPosition(m_CraftingSlots[2].getPosition().x, m_CraftingSlots[2].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHEST)
		{
			item.SetPosition(m_CraftingSlots[2].getPosition().x, m_CraftingSlots[2].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::FURNACE)
		{
			item.SetPosition(m_CraftingSlots[3].getPosition().x, m_CraftingSlots[3].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::FURNACE)
		{
			item.SetPosition(m_CraftingSlots[3].getPosition().x, m_CraftingSlots[3].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW)
		{
			item.SetPosition(m_CraftingSlots[4].getPosition().x, m_CraftingSlots[4].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW)
		{
			item.SetPosition(m_CraftingSlots[4].getPosition().x, m_CraftingSlots[4].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::IRONINGOT)
		{
			item.SetPosition(m_CraftingSlots[5].getPosition().x, m_CraftingSlots[5].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::IRONINGOT)
		{
			item.SetPosition(m_CraftingSlots[5].getPosition().x, m_CraftingSlots[5].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::GOLDINGOT)
		{
			item.SetPosition(m_CraftingSlots[6].getPosition().x, m_CraftingSlots[6].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::GOLDINGOT)
		{
			item.SetPosition(m_CraftingSlots[6].getPosition().x, m_CraftingSlots[6].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[7].getPosition().x, m_CraftingSlots[7].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[7].getPosition().x, m_CraftingSlots[7].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::IRON)
		{
			item.SetPosition(m_CraftingSlots[8].getPosition().x, m_CraftingSlots[8].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::IRON)
		{
			item.SetPosition(m_CraftingSlots[8].getPosition().x, m_CraftingSlots[8].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::GOLD)
		{
			item.SetPosition(m_CraftingSlots[9].getPosition().x, m_CraftingSlots[9].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::GOLD)
		{
			item.SetPosition(m_CraftingSlots[9].getPosition().x, m_CraftingSlots[9].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[10].getPosition().x, m_CraftingSlots[10].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[10].getPosition().x, m_CraftingSlots[10].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

	}

	// Setting Can Craft
	for (CBlock& item : m_CraftList)
	{
		if (_player->IsItemInventory(CBlock::BLOCKTYPE::WOOD, true) > 0 && item.m_Type == CBlock::BLOCKTYPE::PLANKS)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (_player->IsItemInventory(CBlock::BLOCKTYPE::WOOD, true) <= 0 && item.m_Type == CBlock::BLOCKTYPE::PLANKS)
		{
			item.m_bCanCraft = false;
		}
		if (_player->IsItemInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 6 && item.m_Type == CBlock::BLOCKTYPE::DOOR)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (_player->IsItemInventory(CBlock::BLOCKTYPE::PLANKS, true) < 6 && item.m_Type == CBlock::BLOCKTYPE::DOOR)
		{
			item.m_bCanCraft = false;
		}
		if (_player->IsItemInventory(CBlock::BLOCKTYPE::STONE, true) >= 9 && item.m_Type == CBlock::BLOCKTYPE::FURNACE)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (_player->IsItemInventory(CBlock::BLOCKTYPE::STONE, true) < 9 && item.m_Type == CBlock::BLOCKTYPE::FURNACE)
		{
			item.m_bCanCraft = false;
		}
		if (_player->IsItemInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 9 && item.m_Type == CBlock::BLOCKTYPE::CHEST)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (_player->IsItemInventory(CBlock::BLOCKTYPE::PLANKS, true) < 9 && item.m_Type == CBlock::BLOCKTYPE::CHEST)
		{
			item.m_bCanCraft = false;
		}
		if (_player->IsItemInventory(CBlock::BLOCKTYPE::IRONORE, true) >= 3 && item.m_Type == CBlock::BLOCKTYPE::IRONINGOT && m_bCanSmelt)
		{
			if (m_bCanSmelt)
			{
				item.m_bCanCraft = true;
			}
		}
		else if ((_player->IsItemInventory(CBlock::BLOCKTYPE::IRONORE, true) < 3 && item.m_Type == CBlock::BLOCKTYPE::IRONINGOT) || (item.m_Type == CBlock::BLOCKTYPE::IRONINGOT && !m_bCanSmelt))
		{
			item.m_bCanCraft = false;
		}
		if (_player->IsItemInventory(CBlock::BLOCKTYPE::GOLDORE, true) >= 3 && item.m_Type == CBlock::BLOCKTYPE::GOLDINGOT && m_bCanSmelt)
		{
			if (m_bCanSmelt)
			{
				item.m_bCanCraft = true;
			}
		}
		else if ((_player->IsItemInventory(CBlock::BLOCKTYPE::GOLDORE, true) < 3 && item.m_Type == CBlock::BLOCKTYPE::GOLDINGOT) || (item.m_Type == CBlock::BLOCKTYPE::GOLDINGOT && !m_bCanSmelt))
		{
			item.m_bCanCraft = false;
		}
		if (_player->IsItemInventory(CBlock::BLOCKTYPE::DIAMONDORE, true) >= 2 && item.m_Type == CBlock::BLOCKTYPE::DIAMOND)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if ((_player->IsItemInventory(CBlock::BLOCKTYPE::DIAMONDORE, true) < 2 && item.m_Type == CBlock::BLOCKTYPE::DIAMOND))
		{
			item.m_bCanCraft = false;
		}
		if (_player->IsItemInventory(CBlock::BLOCKTYPE::IRONINGOT, true) >= 3 && _player->IsItemInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 2 && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::IRON && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInventory(CBlock::BLOCKTYPE::IRONINGOT, true) < 3 || _player->IsItemInventory(CBlock::BLOCKTYPE::PLANKS, true) < 2) && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::IRON) || (item.m_Type == CBlock::BLOCKTYPE::PICKAXE && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}
		if (_player->IsItemInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) >= 3 && _player->IsItemInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 2 && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::GOLD && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) < 3 || _player->IsItemInventory(CBlock::BLOCKTYPE::PLANKS, true) < 2) && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::GOLD) || (item.m_Type == CBlock::BLOCKTYPE::PICKAXE && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}
		if (_player->IsItemInventory(CBlock::BLOCKTYPE::DIAMOND, true) >= 3 && _player->IsItemInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 2 && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::DIAMOND && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInventory(CBlock::BLOCKTYPE::DIAMOND, true) < 3 || _player->IsItemInventory(CBlock::BLOCKTYPE::PLANKS, true) < 2) && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::DIAMOND) || (item.m_Type == CBlock::BLOCKTYPE::PICKAXE && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}
	}

	// Removing And Adding Items
	for (CBlock& item : m_CraftList)
	{
		if (item.GetShape().getGlobalBounds().contains(m_MousePointer.getPosition()) && item.m_bCanCraft && sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_CraftTimer->getElapsedTime().asSeconds() >= 0.2f)
		{
			switch (item.m_Type)
			{
			case CBlock::BLOCKTYPE::DOOR:
			{
				m_TempBlock = new CBlock(_textureMaster->m_DoorLeft, CBlock::BLOCKTYPE::DOOR);
				_player->AddItemToInventory(m_TempBlock, false);

				// Remove 6 Planks
				for (int i = 0; i < 6; i++)
				{
					if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS)] <= 1)
					{
						_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS));
					}
					else
					{
						_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS)]--;
					}
				}
				break;
			}
			case CBlock::BLOCKTYPE::FURNACE:
			{
				m_TempBlock = new CBlock(item.m_Texture, item.m_Type);
				_player->AddItemToInventory(m_TempBlock);

				// Remove 9 Stone
				for (int i = 0; i < 9; i++)
				{
					if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::STONE)] <= 1)
					{
						_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::STONE));
					}
					else
					{
						_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::STONE)]--;
					}
				}
				break;
			}
			case CBlock::BLOCKTYPE::PLANKS:
			{
				// Add 2 Planks
				for (int i = 0; i < 2; i++)
				{
					m_TempBlock = new CBlock(item.m_Texture, item.m_Type);
					_player->AddItemToInventory(m_TempBlock);
					m_TempBlock = nullptr;
				}
				// Remove 1 Wood
				if (true)
				{
					if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::WOOD)] <= 1)
					{
						_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::WOOD));
					}
					else
					{
						_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::WOOD)]--;
					}
				}
				break;
			}
			case CBlock::BLOCKTYPE::CHEST:
			{
				m_TempBlock = new CBlock(item.m_Texture, item.m_Type);
				_player->AddItemToInventory(m_TempBlock);

				// Remove 9 Plank
				for (int i = 0; i < 9; i++)
				{
					if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS)] <= 1)
					{
						_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS));
					}
					else
					{
						_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS)]--;
					}
				}
				break;
			}
			case CBlock::BLOCKTYPE::IRONINGOT:
			{
				m_TempBlock = new CBlock(_textureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
				_player->AddItemToInventory(m_TempBlock);

				// Remove 3 Iron Ore
				for (int i = 0; i < 3; i++)
				{
					if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::IRONORE)] <= 1)
					{
						_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::IRONORE));
					}
					else
					{
						_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::IRONORE)]--;
					}
				}
				break;
			}
			case CBlock::BLOCKTYPE::GOLDINGOT:
			{
				m_TempBlock = new CBlock(_textureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
				_player->AddItemToInventory(m_TempBlock);

				// Remove 3 Gold Ore
				for (int i = 0; i < 3; i++)
				{
					if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDORE)] <= 1)
					{
						_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDORE));
					}
					else
					{
						_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDORE)]--;
					}
				}
				break;
			}
			case CBlock::BLOCKTYPE::DIAMOND:
			{
				m_TempBlock = new CBlock(_textureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
				_player->AddItemToInventory(m_TempBlock);

				// Remove 1 Diamond Ore
				for (int i = 0; i < 2; i++)
				{
					if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::DIAMONDORE)] <= 1)
					{
						_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::DIAMONDORE));
					}
					else
					{
						_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::DIAMONDORE)]--;
					}
				}
				break;
			}
			case CBlock::BLOCKTYPE::PICKAXE:
			{
				switch (item.m_PickType)
				{
				case CBlock::PICKAXETYPE::IRON:
				{
					// Remove Current Pickaxe
					//int tempPos = _player->GetPositionInInventory(CBlock::BLOCKTYPE::PICKAXE);
					//_player->RemoveItemFromInventory(tempPos);

					CPickaxe* temp = new CPickaxe(CBlock::PICKAXETYPE::IRON);
					_player->AddItemToInventory(temp, false);

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 3 Iron Ingots
					for (int i = 0; i < 3; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::IRONINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::IRONINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::IRONINGOT)]--;
						}
					}

					// Remove 2 Planks
					for (int i = 0; i < 2; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS)]--;
						}
					}

					temp = nullptr;
					break;
				}
				case CBlock::PICKAXETYPE::GOLD:
				{
					// Remove Current Pickaxe
					//int tempPos = _player->GetPositionInInventory(CBlock::BLOCKTYPE::PICKAXE);
					//_player->RemoveItemFromInventory(tempPos);

					CPickaxe* temp = new CPickaxe(CBlock::PICKAXETYPE::GOLD);
					_player->AddItemToInventory(temp, false);

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 3 Gold Ingots
					for (int i = 0; i < 3; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDINGOT)]--;
						}
					}

					// Remove 2 Planks
					for (int i = 0; i < 2; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS)]--;
						}
					}


					temp = nullptr;
					break;
				}
				case CBlock::PICKAXETYPE::DIAMOND:
				{
					// Remove Current Pickaxe
					//int tempPos = _player->GetPositionInInventory(CBlock::BLOCKTYPE::PICKAXE);
					//_player->RemoveItemFromInventory(tempPos);

					CPickaxe* temp = new CPickaxe(CBlock::PICKAXETYPE::DIAMOND);
					_player->AddItemToInventory(temp, false);

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 3 Diamond
					for (int i = 0; i < 3; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::DIAMOND)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::DIAMOND));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::DIAMOND)]--;
						}
					}

					// Remove 2 Planks
					for (int i = 0; i < 2; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PLANKS)]--;
						}
					}


					temp = nullptr;
					break;
				}
				default:
				{
					break;
				}
				}

				// Exit Pickaxe Case
				break;
			}
			default:
			{
				break;
			}
			}

			m_TempBlock = nullptr;
			m_CraftTimer->restart();
		}
	}
}

void GUI::InitCraftingUI(CTextureMaster* _textureMaster)
{
	sf::Color color = sf::Color();

	// Row 1
	for (int i = 0; i < 11; i++)
	{
		std::cout << "Create Crafting Space" << std::endl;

		sf::Sprite test = sf::Sprite();
		test.setTexture(*_textureMaster->m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);

		m_CraftingSlots.emplace(i, test);

		color = m_CraftingSlots[i].getColor();
		color.a = 210.0f;
		m_CraftingSlots[i].setColor(color);
	}

	std::cout << "Size of slots: " << m_CraftingSlots.size() << std::endl;

	// Init Craft List
	CBlock* Recipe = new CBlock(_textureMaster->m_Bow, CBlock::BLOCKTYPE::BOW);
	m_CraftList.push_back(*Recipe);
	CBlock* Recipe2 = new CBlock(_textureMaster->m_DoorLeft, CBlock::BLOCKTYPE::DOOR);
	Recipe2->GetShape().setScale(0.23f, 0.23f);
	m_CraftList.push_back(*Recipe2);
	CBlock* Recipe3 = new CBlock(_textureMaster->m_Planks, CBlock::BLOCKTYPE::PLANKS);
	m_CraftList.push_back(*Recipe3);
	CBlock* Recipe4 = new CBlock(_textureMaster->m_Chest, CBlock::BLOCKTYPE::CHEST);
	m_CraftList.push_back(*Recipe4);
	CBlock* Recipe5 = new CBlock(_textureMaster->m_Furnace, CBlock::BLOCKTYPE::FURNACE);
	m_CraftList.push_back(*Recipe5);
	CBlock* Recipe6 = new CBlock(_textureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
	m_CraftList.push_back(*Recipe6);
	CBlock* Recipe7 = new CBlock(_textureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
	m_CraftList.push_back(*Recipe7);
	CBlock* Recipe8 = new CBlock(_textureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
	m_CraftList.push_back(*Recipe8);
	CPickaxe* Recipe9 = new CPickaxe(CPickaxe::PICKAXETYPE::IRON);
	m_CraftList.push_back(*Recipe9);
	CPickaxe* Recipe10 = new CPickaxe(CPickaxe::PICKAXETYPE::GOLD);
	m_CraftList.push_back(*Recipe10);
	CPickaxe* Recipe11 = new CPickaxe(CPickaxe::PICKAXETYPE::DIAMOND);
	m_CraftList.push_back(*Recipe11);
	Recipe = nullptr;
	Recipe2 = nullptr;
	Recipe3 = nullptr;
	Recipe4 = nullptr;
	Recipe5 = nullptr;
	Recipe6 = nullptr;
	Recipe7 = nullptr;
	Recipe8 = nullptr;
	Recipe9 = nullptr;
	Recipe10 = nullptr;
	Recipe11 = nullptr;
}

bool GUI::bIsCraftingSpaceEmpty(int _position)
{
	for (CBlock& item : m_CraftList)
	{
		if (item.m_PositionInInventory == item.m_PositionInInventory)
		{
			return false;
		}
	}
	
	return true;
}

void GUI::ChestUI(sf::RenderWindow* _renderWindow, CPlayer* _player, sf::View& _uiView, sf::View& _worldView, sf::Event& _event, CTextureMaster* _textureMaster, std::list<CChest>& _chests)
{
	_renderWindow->setView(_uiView);

	sf::Vector2f MousePos = _renderWindow->mapPixelToCoords((sf::Mouse::getPosition(*_renderWindow)), _uiView);

	// Row 1
	for (int i = 0; i < 10; i++)
	{
		_renderWindow->mapCoordsToPixel(m_ChestSlots[i].getPosition(), _uiView);
		m_ChestSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 900 + (i * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 70);

		for (CChest& chest : _chests)
		{
			_renderWindow->mapCoordsToPixel(chest.m_Inventory[i].GetPosition(), _uiView);
			chest.m_Inventory[i].SetPosition(m_ChestSlots[i].getPosition().x, m_ChestSlots[i].getPosition().y);

			if (chest.m_bIsOpen)
			{
				m_ChestItemStackCounters[i].setPosition(m_ChestSlots[i].getPosition().x - 8, m_ChestSlots[i].getPosition().y + 18);
				if (chest.m_InventoryStackValues[i] <= 1)
				{
					m_ChestItemStackCounters[i].setString("");
				}
				else
				{
					m_ChestItemStackCounters[i].setString(std::to_string(chest.m_InventoryStackValues[i]));
				}
			}
		}

		m_ChestSlots[i].setTexture(*_textureMaster->m_ItemSpacer);

		
	}
	// Row 2
	for (int i = 10; i < 20; i++)
	{
		_renderWindow->mapCoordsToPixel(m_ChestSlots[i].getPosition(), _uiView);
		m_ChestSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 900 + ((i-10) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 135);

		for (CChest& chest : _chests)
		{
			_renderWindow->mapCoordsToPixel(chest.m_Inventory[i].GetPosition(), _uiView);
			chest.m_Inventory[i].SetPosition(m_ChestSlots[i].getPosition().x, m_ChestSlots[i].getPosition().y);

			m_ChestItemStackCounters[i].setPosition(m_ChestSlots[i].getPosition().x - 8, m_ChestSlots[i].getPosition().y + 18);
			if (chest.m_bIsOpen)
			{
				m_ChestItemStackCounters[i].setPosition(m_ChestSlots[i].getPosition().x - 8, m_ChestSlots[i].getPosition().y + 18);
				if (chest.m_InventoryStackValues[i] <= 1)
				{
					m_ChestItemStackCounters[i].setString("");
				}
				else
				{
					m_ChestItemStackCounters[i].setString(std::to_string(chest.m_InventoryStackValues[i]));
				}
			}
		}

		m_ChestSlots[i].setTexture(*_textureMaster->m_ItemSpacer);

		
	}
	// Row 3
	for (int i = 20; i < 30; i++)
	{
		_renderWindow->mapCoordsToPixel(m_ChestSlots[i].getPosition(), _uiView);
		m_ChestSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 900 + ((i - 20) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 200);

		for (CChest& chest : _chests)
		{
			_renderWindow->mapCoordsToPixel(chest.m_Inventory[i].GetPosition(), _uiView);
			chest.m_Inventory[i].SetPosition(m_ChestSlots[i].getPosition().x, m_ChestSlots[i].getPosition().y);

			m_ChestItemStackCounters[i].setPosition(m_ChestSlots[i].getPosition().x - 8, m_ChestSlots[i].getPosition().y + 18);
			if (chest.m_bIsOpen)
			{
				m_ChestItemStackCounters[i].setPosition(m_ChestSlots[i].getPosition().x - 8, m_ChestSlots[i].getPosition().y + 18);
				if (chest.m_InventoryStackValues[i] <= 1)
				{
					m_ChestItemStackCounters[i].setString("");
				}
				else
				{
					m_ChestItemStackCounters[i].setString(std::to_string(chest.m_InventoryStackValues[i]));
				}
			}
		}

		m_ChestSlots[i].setTexture(*_textureMaster->m_ItemSpacer);
	}
	// Row 4
	for (int i = 30; i < 40; i++)
	{
		_renderWindow->mapCoordsToPixel(m_ChestSlots[i].getPosition(), _uiView);
		m_ChestSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 900 + ((i - 30) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265);

		for (CChest& chest : _chests)
		{
			_renderWindow->mapCoordsToPixel(chest.m_Inventory[i].GetPosition(), _uiView);
			chest.m_Inventory[i].SetPosition(m_ChestSlots[i].getPosition().x, m_ChestSlots[i].getPosition().y);

			m_ChestItemStackCounters[i].setPosition(m_ChestSlots[i].getPosition().x - 8, m_ChestSlots[i].getPosition().y + 18);
			if (chest.m_bIsOpen)
			{
				m_ChestItemStackCounters[i].setPosition(m_ChestSlots[i].getPosition().x - 8, m_ChestSlots[i].getPosition().y + 18);
				if (chest.m_InventoryStackValues[i] <= 1)
				{
					m_ChestItemStackCounters[i].setString("");
				}
				else
				{
					m_ChestItemStackCounters[i].setString(std::to_string(chest.m_InventoryStackValues[i]));
				}
			}
		}

		m_ChestSlots[i].setTexture(*_textureMaster->m_ItemSpacer);
	}
	// Row 5
	for (int i = 40; i < 50; i++)
	{
		_renderWindow->mapCoordsToPixel(m_ChestSlots[i].getPosition(), _uiView);
		m_ChestSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 900 + ((i - 40) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65);

		for (CChest& chest : _chests)
		{
			_renderWindow->mapCoordsToPixel(chest.m_Inventory[i].GetPosition(), _uiView);
			chest.m_Inventory[i].SetPosition(m_ChestSlots[i].getPosition().x, m_ChestSlots[i].getPosition().y);

			m_ChestItemStackCounters[i].setPosition(m_ChestSlots[i].getPosition().x - 8, m_ChestSlots[i].getPosition().y + 18);
			if (chest.m_bIsOpen)
			{
				m_ChestItemStackCounters[i].setPosition(m_ChestSlots[i].getPosition().x - 8, m_ChestSlots[i].getPosition().y + 18);
				if (chest.m_InventoryStackValues[i] <= 1)
				{
					m_ChestItemStackCounters[i].setString("");
				}
				else
				{
					m_ChestItemStackCounters[i].setString(std::to_string(chest.m_InventoryStackValues[i]));
				}
			}
		}

		m_ChestSlots[i].setTexture(*_textureMaster->m_ItemSpacer);
	}

}

void GUI::InitChestUI(sf::RenderWindow* _renderWindow, CTextureMaster* _textureMaster)
{
	sf::Color color = sf::Color();

	// Row 1
	for (int i = 0; i < 10; i++)
	{
		std::cout << "Create Chest Space" << std::endl;

		sf::Sprite test = sf::Sprite();
		test.setTexture(*_textureMaster->m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);

		m_ChestSlots.emplace(i, test);

		color = m_CraftingSlots[i].getColor();
		color.a = 210.0f;
		m_CraftingSlots[i].setColor(color);

		sf::Text stackcounter = sf::Text();
		stackcounter.setFont(m_Font);
		stackcounter.setCharacterSize(18);
		stackcounter.setFillColor(sf::Color::White);
		stackcounter.setOutlineThickness(0.75f);
		stackcounter.setOutlineColor(sf::Color::Black);
		stackcounter.setString("");
		_renderWindow->mapCoordsToPixel(stackcounter.getPosition());
		m_ChestItemStackCounters.insert_or_assign(i, stackcounter);
		m_ChestItemStackCounters[i].setOrigin(m_ChestItemStackCounters[i].getGlobalBounds().width / 2, m_ChestItemStackCounters[i].getGlobalBounds().height / 2);
	}

	// Row 2
	for (int i = 10; i < 20; i++)
	{
		std::cout << "Create Chest Space" << std::endl;

		sf::Sprite test = sf::Sprite();
		test.setTexture(*_textureMaster->m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);

		m_ChestSlots.emplace(i, test);

		color = m_CraftingSlots[i].getColor();
		color.a = 210.0f;
		m_CraftingSlots[i].setColor(color);

		sf::Text stackcounter = sf::Text();
		stackcounter.setFont(m_Font);
		stackcounter.setCharacterSize(18);
		stackcounter.setFillColor(sf::Color::White);
		stackcounter.setOutlineThickness(0.75f);
		stackcounter.setOutlineColor(sf::Color::Black);
		stackcounter.setString("");
		_renderWindow->mapCoordsToPixel(stackcounter.getPosition());
		m_ChestItemStackCounters.insert_or_assign(i, stackcounter);
		m_ChestItemStackCounters[i].setOrigin(m_ChestItemStackCounters[i].getGlobalBounds().width / 2, m_ChestItemStackCounters[i].getGlobalBounds().height / 2);
	}

	// Row 3
	for (int i = 20; i < 30; i++)
	{
		std::cout << "Create Chest Space" << std::endl;

		sf::Sprite test = sf::Sprite();
		test.setTexture(*_textureMaster->m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);

		m_ChestSlots.emplace(i, test);

		color = m_CraftingSlots[i].getColor();
		color.a = 210.0f;
		m_CraftingSlots[i].setColor(color);

		sf::Text stackcounter = sf::Text();
		stackcounter.setFont(m_Font);
		stackcounter.setCharacterSize(18);
		stackcounter.setFillColor(sf::Color::White);
		stackcounter.setOutlineThickness(0.75f);
		stackcounter.setOutlineColor(sf::Color::Black);
		stackcounter.setString("");
		_renderWindow->mapCoordsToPixel(stackcounter.getPosition());
		m_ChestItemStackCounters.insert_or_assign(i, stackcounter);
		m_ChestItemStackCounters[i].setOrigin(m_ChestItemStackCounters[i].getGlobalBounds().width / 2, m_ChestItemStackCounters[i].getGlobalBounds().height / 2);
	}

	// Row 4
	for (int i = 30; i < 40; i++)
	{
		std::cout << "Create Chest Space" << std::endl;

		sf::Sprite test = sf::Sprite();
		test.setTexture(*_textureMaster->m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);

		m_ChestSlots.emplace(i, test);

		color = m_CraftingSlots[i].getColor();
		color.a = 210.0f;
		m_CraftingSlots[i].setColor(color);

		sf::Text stackcounter = sf::Text();
		stackcounter.setFont(m_Font);
		stackcounter.setCharacterSize(18);
		stackcounter.setFillColor(sf::Color::White);
		stackcounter.setOutlineThickness(0.75f);
		stackcounter.setOutlineColor(sf::Color::Black);
		stackcounter.setString("");
		_renderWindow->mapCoordsToPixel(stackcounter.getPosition());
		m_ChestItemStackCounters.insert_or_assign(i, stackcounter);
		m_ChestItemStackCounters[i].setOrigin(m_ChestItemStackCounters[i].getGlobalBounds().width / 2, m_ChestItemStackCounters[i].getGlobalBounds().height / 2);
	}

	// Row 5
	for (int i = 40; i < 50; i++)
	{
		std::cout << "Create Chest Space" << std::endl;

		sf::Sprite test = sf::Sprite();
		test.setTexture(*_textureMaster->m_ItemSpacer, true);
		test.setScale(sf::Vector2f(0.6, 0.6));
		test.setOrigin(test.getGlobalBounds().width / 2, test.getGlobalBounds().height / 2);

		m_ChestSlots.emplace(i, test);

		color = m_CraftingSlots[i].getColor();
		color.a = 210.0f;
		m_CraftingSlots[i].setColor(color);

		sf::Text stackcounter = sf::Text();
		stackcounter.setFont(m_Font);
		stackcounter.setCharacterSize(18);
		stackcounter.setFillColor(sf::Color::White);
		stackcounter.setOutlineThickness(0.75f);
		stackcounter.setOutlineColor(sf::Color::Black);
		stackcounter.setString("");
		_renderWindow->mapCoordsToPixel(stackcounter.getPosition());
		m_ChestItemStackCounters.insert_or_assign(i, stackcounter);
		m_ChestItemStackCounters[i].setOrigin(m_ChestItemStackCounters[i].getGlobalBounds().width / 2, m_ChestItemStackCounters[i].getGlobalBounds().height / 2);
	}


	std::cout << "Size of slots: " << m_CraftingSlots.size() << std::endl;
}

void GUI::Render(sf::RenderWindow* _renderWindow, CPlayer* _player, sf::View& _worldView, sf::View& _uiView, std::list<CChest>& _chests, sf::Shader* _defaultShader)
{
	if (_player->bInventoryOpen())
	{
		for (int i = 0; i < m_InventorySlotMap.size(); i++)
		{
			_renderWindow->draw(m_InventorySlotMap[i], _defaultShader);
			_renderWindow->draw(_player->m_InventoryMap[i].GetShape(), _defaultShader);
			_renderWindow->draw(m_InventoryItemStackCounters[i], _defaultShader);
		}

		for (int i = 0; i < m_CraftingSlots.size(); i++)
		{
			_renderWindow->draw(m_CraftingSlots[i], _defaultShader);
			
		}

		if (_player->m_bPlayerIsInChest)
		{
			for (int i = 0; i < m_ChestSlots.size(); i++)
			{
				//std::cout << "Drawing Chest Slots" << std::endl;
				_renderWindow->draw(m_ChestSlots[i], _defaultShader);
			}

			for (CChest& chest : _chests)
			{
				if (chest.m_bIsOpen)
				{
					for (int i = 0; i < m_ChestSlots.size(); i++)
					{
						//std::cout << "Drawing Chest Items" << std::endl;
						_renderWindow->draw(chest.m_Inventory[i].GetShape(), _defaultShader);
						_renderWindow->draw(m_ChestItemStackCounters[i], _defaultShader);
					}
				}
			}
			
		}

		for (int i = 0; i < m_CraftList.size(); i++)
		{
			_renderWindow->draw(m_CraftList[i].GetShape(), _defaultShader);
		}

		// Render Moving Item On Top Always
		if (bPlayerIsMovingAnItem(_player))
		{
			_renderWindow->draw(_player->m_InventoryMap[bGetPositionOfMovingItem(_player)].GetShape(), _defaultShader);
			_renderWindow->draw(m_InventoryItemStackCounters[bGetPositionOfMovingItem(_player)], _defaultShader);
		}
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			_renderWindow->draw(m_InventorySlotMap[i], _defaultShader);
			_renderWindow->draw(_player->m_InventoryMap[i].GetShape(), _defaultShader);
			_renderWindow->draw(m_InventoryItemStackCounters[i], _defaultShader);
		}
	}

	if (_player->bInventoryOpen())
	{
		_renderWindow->draw(m_MousePointer, _defaultShader);
	}
	else
	{
		_renderWindow->setView(_worldView);

		_renderWindow->draw(m_MousePos, _defaultShader);

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

int GUI::FindFirstEmptyInventorySlot(CPlayer* _player)
{
	int i = 0;
	//
	// ofstream
	// x Pos
	std::ofstream out_file;

	out_file.open("Output/FirstEmptyInventorySlot.txt");
	if (out_file.is_open())
	{
		for (i = 0; i < m_InventorySlotMap.size(); i++)
		{
			if (_player->m_InventoryStackValues[i] == 0)
			{
				out_file << i << std::endl;
				break;
			}

		}
		out_file.close();
	}
	else
	{
		std::cout << "OutPut File Not Open!" << std::endl;
	}

	//
	return i;
}

int GUI::FindLastEmptyInventorySlot(CPlayer* _player)
{
	int i = 0;
	for (i = m_InventorySlotMap.size(); i > 0; i--)
	{
		if (_player->m_InventoryStackValues[i] == 0)
		{
			return i - 1;
			break;
		}
	}
	
}

int GUI::FindFirstEmptyChestSlot(std::list<CChest>& _chests)
{
	int i = 0;
	//
	// ofstream
	std::ofstream out_file;

	out_file.open("Output/FirstEmptyChestSlot.txt");
	if (out_file.is_open())
	{
		for (i = 0; i < m_ChestSlots.size(); i++)
		{
			for (CChest& chest : _chests)
			{
				if (chest.m_bIsOpen)
				{
					if (chest.m_InventoryStackValues[i] == 0)
					{
						out_file << i << std::endl;
						break;
					}
				}
			}
		}
		out_file.close();
	}
	else
	{
		std::cout << "OutPut File Not Open!" << std::endl;
	}

	//
	return i;
}

bool GUI::bPlayerIsMovingAnItem(CPlayer* _player)
{
	int i = 0;
	for (i = 0; i < _player->m_InventoryMap.size(); i++)
	{
		if (_player->m_InventoryMap[i].m_bIsMovingItemInInv)
		{
			return true;
		}
	}
	return false;
}

bool GUI::bPlayerIsMovingAnItem(CPlayer* _player, int _iterator)
{
	if (_player->m_InventoryMap[_iterator].m_bIsMovingItemInInv)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

int GUI::bGetPositionOfMovingItem(CPlayer* _player)
{
	if (bPlayerIsMovingAnItem(_player))
	{
		int i = 0;
		for (i = 0; i < _player->m_InventoryMap.size(); i++)
		{
			if (_player->m_InventoryMap[i].m_bIsMovingItemInInv)
			{
				return i;
			}
		}
	}
	return -1;
}

