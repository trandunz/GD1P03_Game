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

void GUI::FPSUI(sf::RenderWindow* _renderWindow, float _fps)
{
	m_Fps = ToString(_fps);
	m_FpsText.setString(m_Fps);
	m_FpsText.setOrigin(m_FpsText.getGlobalBounds().width / 2, m_FpsText.getGlobalBounds().height / 2);
	m_FpsText.setPosition(_renderWindow->getView().getCenter().x, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 15);
	_renderWindow->draw(m_FpsText);
}

void GUI::InitFpsUI(float _fps)
{

	// Health Text
	m_FpsText = sf::Text();
	m_Fps = ToString(_fps);
	m_FpsText.setString(m_Fps);
	m_FpsText.setFont(m_Font);
	m_FpsText.setFillColor(sf::Color::White);
	m_FpsText.setOutlineThickness(0.75f);
	m_FpsText.setOutlineColor(sf::Color::Black);
	m_FpsText.setOrigin(m_FpsText.getGlobalBounds().width / 2, m_FpsText.getGlobalBounds().height / 2);
	m_FpsText.setCharacterSize(22);
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

	if (_player->GetGodMode())
	{
		heart1.setTexture(*_textureMaster->m_GoldenHeart, true);
		heart2.setTexture(*_textureMaster->m_GoldenHeart, true);
		heart3.setTexture(*_textureMaster->m_GoldenHeart, true);
		heart4.setTexture(*_textureMaster->m_GoldenHeart, true);
		heart5.setTexture(*_textureMaster->m_GoldenHeart, true);
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
	if (_player->GetShape().getPosition().x < -34200 || _player->GetShape().getPosition().x > 34200 - 14000)
	{
		if (_player->GetShape().getPosition().y < -4500 || _player->GetShape().getPosition().y > 10000)
		{
			sf::View MiniMapView = sf::View(sf::Vector2f(_player->GetShape().getPosition().x, m_miniMap->getView().getCenter().y), sf::Vector2f(200.0f, 200.0f));
			MiniMapView.zoom(13);
			m_miniMap->setView(MiniMapView);
		}
		else
		{
			sf::View MiniMapView = sf::View(sf::Vector2f(m_miniMap->getView().getCenter().x, _player->GetShape().getPosition().y), sf::Vector2f(200.0f, 200.0f));
			MiniMapView.zoom(13);
			m_miniMap->setView(MiniMapView);
		}
	}
	else
	{
		if (_player->GetShape().getPosition().y < -4500 || _player->GetShape().getPosition().y > 10000)
		{
			sf::View MiniMapView = sf::View(sf::Vector2f(_player->GetShape().getPosition().x, m_miniMap->getView().getCenter().y), sf::Vector2f(200.0f, 200.0f));
			MiniMapView.zoom(13);
			m_miniMap->setView(MiniMapView);
		}
		else
		{

			sf::View MiniMapView = sf::View(_player->GetShape().getPosition(), sf::Vector2f(200.0f, 200.0f));
			MiniMapView.zoom(13);
			m_miniMap->setView(MiniMapView);
		}
	}


	m_MiniMapWorldBackGround.setPosition(m_miniMap->getView().getCenter());
	m_miniMap->draw(m_MiniMapWorldBackGround, _shaderUI);

	// Draw All Blocks In Radius
	std::list<CBlock>::iterator it;
	float Mag1 = 0;
	float x = 0;
	float y = 0;
	// Blocks
	for (it = _chunk.begin(); it != _chunk.end(); it++)
	{
		x = it->GetShape().getPosition().x - m_miniMap->getView().getCenter().x;
		y = it->GetShape().getPosition().y - m_miniMap->getView().getCenter().y;
		Mag1 = sqrt((x * x) + (y * y));
		if (Mag1 < 1920 * 1.1f)
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
				if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CPickaxe::PICKAXETYPE::BASIC)
				{
					std::cout << "Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CPickaxe::PICKAXETYPE::IRON)
				{
					std::cout << "Iron Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLD)
				{
					std::cout << "Gold Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::DIAMOND)
				{
					std::cout << "Diamond Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::PURPLE)
				{
					std::cout << "purple Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLDEN)
				{
					std::cout << "gOLDEN Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::OBSIDIAN)
				{
					std::cout << "oBSIDIAN Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::BASIC)
				{
					std::cout << "Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::IRON)
				{
					std::cout << "Iron Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::PURPLE)
				{
					std::cout << "Purple Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GOLDEN)
				{
					std::cout << "Golden Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GREEN)
				{
					std::cout << "Green Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::ICE)
				{
					std::cout << "Ice Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::BLOOD)
				{
					std::cout << "Blood Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::OBSIDIAN)
				{
					std::cout << "Obsidian Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::IRONGUN)
				{
					std::cout << "Iron Gun Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GOLDGUN)
				{
					std::cout << "Gold Gun Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::PURPLEGUN)
				{
					std::cout << "Purple Gun Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GOLDENGUN)
				{
					std::cout << "Golden Gun Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GREENGUN)
				{
					std::cout << "Green Gun Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
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
				if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CPickaxe::PICKAXETYPE::BASIC)
				{
					std::cout << "Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CPickaxe::PICKAXETYPE::IRON)
				{
					std::cout << "Iron Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLD)
				{
					std::cout << "Gold Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::DIAMOND)
				{
					std::cout << "Diamond Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::PURPLE)
				{
					std::cout << "purple Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLDEN)
				{
					std::cout << "gOLDEN Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::OBSIDIAN)
				{
					std::cout << "oBSIDIAN Pickaxe Selected!" << "( gui)" << std::endl;
					if (_player->GetPickaxe() != nullptr)
					{
						delete _player->GetPickaxe();
						_player->SetPickaxe(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::BASIC)
				{
					std::cout << "Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::IRON)
				{
					std::cout << "Iron Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::PURPLE)
				{
					std::cout << "Purple Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GOLDEN)
				{
					std::cout << "Golden Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GREEN)
				{
					std::cout << "Green Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::ICE)
				{
					std::cout << "Ice Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::BLOOD)
				{
					std::cout << "Blood Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::OBSIDIAN)
				{
					std::cout << "Obsidian Bow Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::IRONGUN)
				{
					std::cout << "Iron Gun Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GOLDGUN)
				{
					std::cout << "Gold Gun Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::PURPLEGUN)
				{
					std::cout << "Purple Gun Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GOLDENGUN)
				{
					std::cout << "Golden Gun Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
				}
				else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GREENGUN)
				{
					std::cout << "Green Gun Selected!" << "( gui)" << std::endl;
					if (_player->GetBow() != nullptr)
					{
						delete _player->GetBow();
						_player->SetBow(nullptr);
					}
					iit->second.m_bIsItemAndSelected = true;
					break;
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
		// Items
		for (std::map<int, CBlock>::iterator iit = _player->m_InventoryMap.begin(); iit != _player->m_InventoryMap.end(); iit++)
		{
			if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CPickaxe::PICKAXETYPE::BASIC)
			{
				std::cout << "Pickaxe Selected!" << "( gui)" << std::endl;
				if (_player->GetPickaxe() != nullptr)
				{
					delete _player->GetPickaxe();
					_player->SetPickaxe(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CPickaxe::PICKAXETYPE::IRON)
			{
				std::cout << "Iron Pickaxe Selected!" << "( gui)" << std::endl;
				if (_player->GetPickaxe() != nullptr)
				{
					delete _player->GetPickaxe();
					_player->SetPickaxe(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLD)
			{
				std::cout << "Gold Pickaxe Selected!" << "( gui)" << std::endl;
				if (_player->GetPickaxe() != nullptr)
				{
					delete _player->GetPickaxe();
					_player->SetPickaxe(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::DIAMOND)
			{
				std::cout << "Diamond Pickaxe Selected!" << "( gui)" << std::endl;
				if (_player->GetPickaxe() != nullptr)
				{
					delete _player->GetPickaxe();
					_player->SetPickaxe(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::PURPLE)
			{
				std::cout << "purple Pickaxe Selected!" << "( gui)" << std::endl;
				if (_player->GetPickaxe() != nullptr)
				{
					delete _player->GetPickaxe();
					_player->SetPickaxe(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::GOLDEN)
			{
				std::cout << "gOLDEN Pickaxe Selected!" << "( gui)" << std::endl;
				if (_player->GetPickaxe() != nullptr)
				{
					delete _player->GetPickaxe();
					_player->SetPickaxe(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && !_player->bInventoryOpen() && iit->second.m_PickType == CBlock::PICKAXETYPE::OBSIDIAN)
			{
				std::cout << "oBSIDIAN Pickaxe Selected!" << "( gui)" << std::endl;
				if (_player->GetPickaxe() != nullptr)
				{
					delete _player->GetPickaxe();
					_player->SetPickaxe(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::BASIC)
			{
				std::cout << "Bow Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::IRON)
			{
				std::cout << "Iron Bow Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::PURPLE)
			{
				std::cout << "Purple Bow Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GOLDEN)
			{
				std::cout << "Golden Bow Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GREEN)
			{
				std::cout << "Green Bow Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::ICE)
			{
				std::cout << "Ice Bow Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::BLOOD)
			{
				std::cout << "Blood Bow Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::OBSIDIAN)
			{
				std::cout << "Obsidian Bow Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::IRONGUN)
			{
				std::cout << "Iron Gun Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GOLDGUN)
			{
				std::cout << "Gold Gun Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::PURPLEGUN)
			{
				std::cout << "Purple Gun Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GOLDENGUN)
			{
				std::cout << "Golden Gun Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
			else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && !_player->bInventoryOpen() && iit->second.m_BowType == CBlock::BOWTYPE::GREENGUN)
			{
				std::cout << "Green Gun Selected!" << "( gui)" << std::endl;
				if (_player->GetBow() != nullptr)
				{
					delete _player->GetBow();
					_player->SetBow(nullptr);
				}
				iit->second.m_bIsItemAndSelected = true;
				break;
			}
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

						if (sit->first >= 50)
						{
							if (sit->first == 50 && sit->first != cit->first && _player->m_InventoryMap[cit->first].m_Type == CBlock::BLOCKTYPE::HELMET)
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
										if (_player->GetPickaxe() != nullptr)
										{
											delete _player->GetPickaxe();
											_player->SetPickaxe(nullptr);
										}

										std::cout << "Pickaxe Selected!" << std::endl;
										iit->second.m_bIsItemAndSelected = true;
										iit->second.m_bIsMovingItemInInv = false;
										break;
									}
									else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && iit->second.m_bIsItemAndSelected == false)
									{
										if (_player->GetBow() != nullptr)
										{
											delete _player->GetBow();
											_player->SetBow(nullptr);
										}

										std::cout << "Bow Selected!" << std::endl;
										iit->second.m_bIsItemAndSelected = true;
										iit->second.m_bIsMovingItemInInv = false;
										break;
									}
								}

								// Debug
								std::cout << "Mouse Released On Selected Item!" << std::endl;

								// Saves My Sanity
								for (int i = 0; i < _player->m_InventoryMap.size(); i++)
								{
									_player->m_InventoryMap[i].m_bIsMovingItemInInv = false;
								}

								_player->SetArmour(_player->GetArmour() + _player->m_InventoryMap[sit->first].m_ArmourValue);
								std::cout << "Armour : " << _player->GetArmour() << std::endl;
								_player->m_HelmetShape.setTexture(_player->m_InventoryMap[50].m_ArmourHeadLeft, true);
							}
							else if (sit->first == 50 && sit->first == cit->first && _player->m_InventoryMap[cit->first].m_Type == CBlock::BLOCKTYPE::HELMET)
							{
							}
							if (sit->first == 51 && sit->first != cit->first && _player->m_InventoryMap[cit->first].m_Type == CBlock::BLOCKTYPE::CHESTPLATE)
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
										if (_player->GetPickaxe() != nullptr)
										{
											delete _player->GetPickaxe();
											_player->SetPickaxe(nullptr);
										}

										std::cout << "Pickaxe Selected!" << std::endl;
										iit->second.m_bIsItemAndSelected = true;
										iit->second.m_bIsMovingItemInInv = false;
										break;
									}
									else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && iit->second.m_bIsItemAndSelected == false)
									{
										if (_player->GetBow() != nullptr)
										{
											delete _player->GetBow();
											_player->SetBow(nullptr);
										}

										std::cout << "Bow Selected!" << std::endl;
										iit->second.m_bIsItemAndSelected = true;
										iit->second.m_bIsMovingItemInInv = false;
										break;
									}
								}

								// Debug
								std::cout << "Mouse Released On Selected Item!" << std::endl;

								// Saves My Sanity
								for (int i = 0; i < _player->m_InventoryMap.size(); i++)
								{
									_player->m_InventoryMap[i].m_bIsMovingItemInInv = false;
								}

								_player->SetArmour(_player->GetArmour() + _player->m_InventoryMap[sit->first].m_ArmourValue);
								std::cout << "Armour : " << _player->GetArmour() << std::endl;
								_player->m_ChesPlateShape.setTexture(_player->m_InventoryMap[51].m_ArmourChestLeft, true);

								break;
							}
							else if (sit->first == 51 && sit->first == cit->first && _player->m_InventoryMap[cit->first].m_Type == CBlock::BLOCKTYPE::CHESTPLATE)
							{
							}
							if (sit->first == 52 && sit->first != cit->first && _player->m_InventoryMap[cit->first].m_Type == CBlock::BLOCKTYPE::LEGGINGS)
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
									if (_player->GetPickaxe() != nullptr)
									{
										delete _player->GetPickaxe();
										_player->SetPickaxe(nullptr);
									}

									std::cout << "Pickaxe Selected!" << std::endl;
									iit->second.m_bIsItemAndSelected = true;
									iit->second.m_bIsMovingItemInInv = false;
									break;
								}
								else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && iit->second.m_bIsItemAndSelected == false)
								{
									if (_player->GetBow() != nullptr)
									{
										delete _player->GetBow();
										_player->SetBow(nullptr);
									}

									std::cout << "Bow Selected!" << std::endl;
									iit->second.m_bIsItemAndSelected = true;
									iit->second.m_bIsMovingItemInInv = false;
									break;
								}
							}

							// Debug
							std::cout << "Mouse Released On Selected Item!" << std::endl;

							// Saves My Sanity
							for (int i = 0; i < _player->m_InventoryMap.size(); i++)
							{
								_player->m_InventoryMap[i].m_bIsMovingItemInInv = false;
							}

							_player->SetArmour(_player->GetArmour() + _player->m_InventoryMap[sit->first].m_ArmourValue);
							std::cout << "Armour : " << _player->GetArmour() << std::endl;
							_player->m_LegsShape.setTexture(_player->m_InventoryMap[52].m_ArmourLegsLeft, true);

							break;
							}
							else if (sit->first == 52 && sit->first == cit->first && _player->m_InventoryMap[cit->first].m_Type == CBlock::BLOCKTYPE::LEGGINGS)
							{
							}
							if (sit->first == 57 && sit->first != cit->first && _player->m_InventoryMap[cit->first].m_Type == CBlock::BLOCKTYPE::TOURCH)
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
										if (_player->GetPickaxe() != nullptr)
										{
											delete _player->GetPickaxe();
											_player->SetPickaxe(nullptr);
										}

										std::cout << "Pickaxe Selected!" << std::endl;
										iit->second.m_bIsItemAndSelected = true;
										iit->second.m_bIsMovingItemInInv = false;
										break;
									}
									else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && iit->second.m_bIsItemAndSelected == false)
									{
										if (_player->GetBow() != nullptr)
										{
											delete _player->GetBow();
											_player->SetBow(nullptr);
										}

										std::cout << "Bow Selected!" << std::endl;
										iit->second.m_bIsItemAndSelected = true;
										iit->second.m_bIsMovingItemInInv = false;
										break;
									}
								}

								// Debug
								std::cout << "Mouse Released On Selected Item!" << std::endl;

								// Saves My Sanity
								for (int i = 0; i < _player->m_InventoryMap.size(); i++)
								{
									_player->m_InventoryMap[i].m_bIsMovingItemInInv = false;
								}
								break;
							}
							else if (sit->first == 57 && sit->first == cit->first && _player->m_InventoryMap[cit->first].m_Type == CBlock::BLOCKTYPE::TOURCH)
							{
							}
							if (sit->first == 58 && sit->first != cit->first && _player->m_InventoryMap[cit->first].m_Type == CBlock::BLOCKTYPE::PROJECTILE)
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
										if (_player->GetPickaxe() != nullptr)
										{
											delete _player->GetPickaxe();
											_player->SetPickaxe(nullptr);
										}

										std::cout << "Pickaxe Selected!" << std::endl;
										iit->second.m_bIsItemAndSelected = true;
										iit->second.m_bIsMovingItemInInv = false;
										break;
									}
									else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && iit->second.m_bIsItemAndSelected == false)
									{
										if (_player->GetBow() != nullptr)
										{
											delete _player->GetBow();
											_player->SetBow(nullptr);
										}

										std::cout << "Bow Selected!" << std::endl;
										iit->second.m_bIsItemAndSelected = true;
										iit->second.m_bIsMovingItemInInv = false;
										break;
									}
								}

								// Debug
								std::cout << "Mouse Released On Selected Item!" << std::endl;

								// Saves My Sanity
								for (int i = 0; i < _player->m_InventoryMap.size(); i++)
								{
									_player->m_InventoryMap[i].m_bIsMovingItemInInv = false;
								}
								break;
							}
							else if (sit->first == 58 && sit->first == cit->first && _player->m_InventoryMap[cit->first].m_Type == CBlock::BLOCKTYPE::PROJECTILE)
							{
							}
							if (sit->first == 59 && sit->first != cit->first && _player->m_InventoryMap[cit->first].m_Type == CBlock::BLOCKTYPE::PROJECTILE)
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
										if (_player->GetPickaxe() != nullptr)
										{
											delete _player->GetPickaxe();
											_player->SetPickaxe(nullptr);
										}

										std::cout << "Pickaxe Selected!" << std::endl;
										iit->second.m_bIsItemAndSelected = true;
										iit->second.m_bIsMovingItemInInv = false;
										break;
									}
									else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && iit->second.m_bIsItemAndSelected == false)
									{
										if (_player->GetBow() != nullptr)
										{
											delete _player->GetBow();
											_player->SetBow(nullptr);
										}

										std::cout << "Bow Selected!" << std::endl;
										iit->second.m_bIsItemAndSelected = true;
										iit->second.m_bIsMovingItemInInv = false;
										break;
									}
								}

								// Debug
								std::cout << "Mouse Released On Selected Item!" << std::endl;

								// Saves My Sanity
								for (int i = 0; i < _player->m_InventoryMap.size(); i++)
								{
									_player->m_InventoryMap[i].m_bIsMovingItemInInv = false;
								}
								break;
							}
							else if (sit->first == 59 && sit->first == cit->first && _player->m_InventoryMap[cit->first].m_Type == CBlock::BLOCKTYPE::PROJECTILE)
							{
							}
						
						}
						else
						{
							if (sit->first != cit->first)
							{
								if (_player->m_InventoryMap[j].m_ArmourValue > 0)
								{
									_player->SetArmour(_player->GetArmour() - _player->m_InventoryMap[j].m_ArmourValue);
								}

								_player->m_InventoryMap[j].m_PositionInInventory = sit->first;
								_player->m_InventoryMap[sit->first].m_PositionInInventory = j;
								std::swap(_player->m_InventoryStackValues[sit->first], vit->second);
								std::swap(_player->m_InventoryMap[sit->first], cit->second);

								// Moved Item Into Currently Selected Slot?
								for (std::map<int, CBlock>::iterator iit = _player->m_InventoryMap.begin(); iit != _player->m_InventoryMap.end(); ++iit)
								{
									if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::PICKAXE && iit->second.m_bIsItemAndSelected == false)
									{
										if (_player->GetPickaxe() != nullptr)
										{
											delete _player->GetPickaxe();
											_player->SetPickaxe(nullptr);
										}

										std::cout << "Pickaxe Selected!" << std::endl;
										iit->second.m_bIsItemAndSelected = true;
										iit->second.m_bIsMovingItemInInv = false;
										break;
									}
									else if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == CBlock::BLOCKTYPE::BOW && iit->second.m_bIsItemAndSelected == false)
									{
										if (_player->GetBow() != nullptr)
										{
											delete _player->GetBow();
											_player->SetBow(nullptr);
										}

										std::cout << "Bow Selected!" << std::endl;
										iit->second.m_bIsItemAndSelected = true;
										iit->second.m_bIsMovingItemInInv = false;
										break;
									}
								}

								// Debug
								std::cout << "Mouse Released On Selected Item!" << std::endl;

								// Saves My Sanity
								for (int i = 0; i < _player->m_InventoryMap.size(); i++)
								{
									_player->m_InventoryMap[i].m_bIsMovingItemInInv = false;
								}
								break;
							}
							else if (sit->first == cit->first)
							{
								
							}
						}
						
						_player->m_HelmetShape.setOrigin(_player->m_HelmetShape.getGlobalBounds().width / 2, _player->m_HelmetShape.getGlobalBounds().height / 2);
						_player->m_ChesPlateShape.setOrigin(_player->m_HelmetShape.getGlobalBounds().width / 2, _player->m_HelmetShape.getGlobalBounds().height / 2);
						_player->m_LegsShape.setOrigin(_player->m_HelmetShape.getGlobalBounds().width / 2, _player->m_HelmetShape.getGlobalBounds().height / 2);

						// Debug
						std::cout << "Mouse Released!" << std::endl;

						// Saves My Sanity
						for (int i = 0; i < _player->m_InventoryMap.size(); i++)
						{
							_player->m_InventoryMap[i].m_bIsMovingItemInInv = false;
						}
					}
					else if (_event.type == sf::Event::MouseButtonReleased)
					{
						// Saves My Sanity
						for (int i = 0; i < _player->m_InventoryMap.size(); i++)
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
				if (_player->m_bInventoryOpen && _player->m_InventoryMap[i].GetShape().getGlobalBounds().contains(m_MousePointer.getPosition()))
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

					return;
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _event.type == sf::Event::MouseButtonPressed)
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
	for (int i = 0; i < 10; i++)
	{
		_renderWindow->mapCoordsToPixel(m_CraftingSlots[i].getPosition(), _uiView);
		m_CraftingSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + (i * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65 + 65 + ((1) * 65));
	}
	// Positions
	for (int i = 10; i < 20; i++)
	{
		_renderWindow->mapCoordsToPixel(m_CraftingSlots[i].getPosition(), _uiView);
		m_CraftingSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 10) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65 + 65 + ((2) * 65));
	}
	// Positions
	for (int i = 20; i < 30; i++)
	{
		_renderWindow->mapCoordsToPixel(m_CraftingSlots[i].getPosition(), _uiView);
		m_CraftingSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 20) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65 + 65 + ((3) * 65));
	}
	// Positions
	for (int i = 30; i < 40; i++)
	{
		_renderWindow->mapCoordsToPixel(m_CraftingSlots[i].getPosition(), _uiView);
		m_CraftingSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 30) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65 + 65 + ((4) * 65));
	}
	// Positions
	for (int i = 40; i < 50; i++)
	{
		_renderWindow->mapCoordsToPixel(m_CraftingSlots[i].getPosition(), _uiView);
		m_CraftingSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 40) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65 + 65 + ((5) * 65));
	}
	// Positions
	for (int i = 50; i < 60; i++)
	{
		_renderWindow->mapCoordsToPixel(m_CraftingSlots[i].getPosition(), _uiView);
		m_CraftingSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + ((i - 50) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65 + 65 + ((6) * 65));
	}

	// Color / Alpha
	for (CBlock& item : m_CraftList)
	{
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PLANKS)
		{
			item.SetPosition(m_CraftingSlots[0].getPosition().x, m_CraftingSlots[0].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PLANKS)
		{
			item.SetPosition(m_CraftingSlots[0].getPosition().x, m_CraftingSlots[0].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHEST)
		{
			item.SetPosition(m_CraftingSlots[1].getPosition().x, m_CraftingSlots[1].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHEST)
		{
			item.SetPosition(m_CraftingSlots[1].getPosition().x, m_CraftingSlots[1].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::DOOR)
		{
			item.SetPosition(m_CraftingSlots[2].getPosition().x + 4.5f, m_CraftingSlots[2].getPosition().y - 3.5f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::DOOR)
		{
			item.SetPosition(m_CraftingSlots[2].getPosition().x + 4.5f, m_CraftingSlots[2].getPosition().y - 3.5f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::BASIC)
		{
			item.SetPosition(m_CraftingSlots[3].getPosition().x, m_CraftingSlots[3].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::BASIC)
		{
			item.SetPosition(m_CraftingSlots[3].getPosition().x, m_CraftingSlots[3].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::IRON)
		{
			item.SetPosition(m_CraftingSlots[4].getPosition().x, m_CraftingSlots[4].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::IRON)
		{
			item.SetPosition(m_CraftingSlots[4].getPosition().x, m_CraftingSlots[4].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::PURPLE)
		{
			item.SetPosition(m_CraftingSlots[5].getPosition().x, m_CraftingSlots[5].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::PURPLE)
		{
			item.SetPosition(m_CraftingSlots[5].getPosition().x, m_CraftingSlots[5].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::GOLDEN)
		{
			item.SetPosition(m_CraftingSlots[6].getPosition().x, m_CraftingSlots[6].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::GOLDEN)
		{
			item.SetPosition(m_CraftingSlots[6].getPosition().x, m_CraftingSlots[6].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::TOURCH)
		{
			item.SetPosition(m_CraftingSlots[7].getPosition().x, m_CraftingSlots[7].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::TOURCH)
		{
			item.SetPosition(m_CraftingSlots[7].getPosition().x, m_CraftingSlots[7].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::FURNACE)
		{
			item.SetPosition(m_CraftingSlots[8].getPosition().x, m_CraftingSlots[8].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::FURNACE)
		{
			item.SetPosition(m_CraftingSlots[8].getPosition().x, m_CraftingSlots[8].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::IRONINGOT)
		{
			item.SetPosition(m_CraftingSlots[9].getPosition().x, m_CraftingSlots[9].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::IRONINGOT)
		{
			item.SetPosition(m_CraftingSlots[9].getPosition().x, m_CraftingSlots[9].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::GOLDINGOT)
		{
			item.SetPosition(m_CraftingSlots[10].getPosition().x, m_CraftingSlots[10].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::GOLDINGOT)
		{
			item.SetPosition(m_CraftingSlots[10].getPosition().x, m_CraftingSlots[10].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[11].getPosition().x, m_CraftingSlots[11].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[11].getPosition().x, m_CraftingSlots[11].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PURPLEINGOT)
		{
			item.SetPosition(m_CraftingSlots[12].getPosition().x, m_CraftingSlots[12].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PURPLEINGOT)
		{
			item.SetPosition(m_CraftingSlots[12].getPosition().x, m_CraftingSlots[12].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::GOLDENINGOT)
		{
			item.SetPosition(m_CraftingSlots[13].getPosition().x, m_CraftingSlots[13].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::GOLDENINGOT)
		{
			item.SetPosition(m_CraftingSlots[13].getPosition().x, m_CraftingSlots[13].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::ANVIL)
		{
			item.SetPosition(m_CraftingSlots[14].getPosition().x, m_CraftingSlots[14].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::ANVIL)
		{
			item.SetPosition(m_CraftingSlots[14].getPosition().x, m_CraftingSlots[14].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::WORKBENCH)
		{
			item.SetPosition(m_CraftingSlots[15].getPosition().x, m_CraftingSlots[15].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::WORKBENCH)
		{
			item.SetPosition(m_CraftingSlots[15].getPosition().x, m_CraftingSlots[15].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::IRON)
		{
			item.SetPosition(m_CraftingSlots[16].getPosition().x, m_CraftingSlots[16].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::IRON)
		{
			item.SetPosition(m_CraftingSlots[16].getPosition().x, m_CraftingSlots[16].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::GOLD)
		{
			item.SetPosition(m_CraftingSlots[17].getPosition().x, m_CraftingSlots[17].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::GOLD)
		{
			item.SetPosition(m_CraftingSlots[17].getPosition().x, m_CraftingSlots[17].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[18].getPosition().x, m_CraftingSlots[18].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[18].getPosition().x, m_CraftingSlots[18].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::PURPLE)
		{
			item.SetPosition(m_CraftingSlots[19].getPosition().x, m_CraftingSlots[19].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::PURPLE)
		{
			item.SetPosition(m_CraftingSlots[19].getPosition().x, m_CraftingSlots[19].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::GOLDEN)
		{
			item.SetPosition(m_CraftingSlots[20].getPosition().x, m_CraftingSlots[20].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PICKAXE && item.m_PickType == CBlock::PICKAXETYPE::GOLDEN)
		{
			item.SetPosition(m_CraftingSlots[20].getPosition().x, m_CraftingSlots[20].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::ARROW)
		{
			item.SetPosition(m_CraftingSlots[21].getPosition().x, m_CraftingSlots[21].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::ARROW)
		{
			item.SetPosition(m_CraftingSlots[21].getPosition().x, m_CraftingSlots[21].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::FIREARROW)
		{
			item.SetPosition(m_CraftingSlots[22].getPosition().x, m_CraftingSlots[22].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::FIREARROW)
		{
			item.SetPosition(m_CraftingSlots[22].getPosition().x, m_CraftingSlots[22].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::POISONARROW)
		{
			item.SetPosition(m_CraftingSlots[23].getPosition().x, m_CraftingSlots[23].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::POISONARROW)
		{
			item.SetPosition(m_CraftingSlots[23].getPosition().x, m_CraftingSlots[23].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::CURSEDARROW)
		{
			item.SetPosition(m_CraftingSlots[24].getPosition().x, m_CraftingSlots[24].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::CURSEDARROW)
		{
			item.SetPosition(m_CraftingSlots[24].getPosition().x, m_CraftingSlots[24].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::POTION && item.m_PotionType == CBlock::POTIONTYPE::HPSMALL)
		{
			item.SetPosition(m_CraftingSlots[25].getPosition().x, m_CraftingSlots[25].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::POTION && item.m_PotionType == CBlock::POTIONTYPE::HPSMALL)
		{
			item.SetPosition(m_CraftingSlots[25].getPosition().x, m_CraftingSlots[25].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::POTION && item.m_PotionType == CBlock::POTIONTYPE::HPLARGE)
		{
			item.SetPosition(m_CraftingSlots[26].getPosition().x, m_CraftingSlots[26].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::POTION && item.m_PotionType == CBlock::POTIONTYPE::HPLARGE)
		{
			item.SetPosition(m_CraftingSlots[26].getPosition().x, m_CraftingSlots[26].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

		//
		// Guns
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::IRONGUN)
		{
			item.SetPosition(m_CraftingSlots[27].getPosition().x, m_CraftingSlots[27].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::IRONGUN)
		{
			item.SetPosition(m_CraftingSlots[27].getPosition().x, m_CraftingSlots[27].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::GOLDGUN)
		{
			item.SetPosition(m_CraftingSlots[28].getPosition().x, m_CraftingSlots[28].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::GOLDGUN)
		{
			item.SetPosition(m_CraftingSlots[28].getPosition().x, m_CraftingSlots[28].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::PURPLEGUN)
		{
			item.SetPosition(m_CraftingSlots[29].getPosition().x, m_CraftingSlots[29].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::PURPLEGUN)
		{
			item.SetPosition(m_CraftingSlots[29].getPosition().x, m_CraftingSlots[29].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
	
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::GOLDENGUN)
		{
			item.SetPosition(m_CraftingSlots[30].getPosition().x, m_CraftingSlots[30].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::BOW && item.m_BowType == CBlock::BOWTYPE::GOLDENGUN)
		{
			item.SetPosition(m_CraftingSlots[30].getPosition().x, m_CraftingSlots[30].getPosition().y);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}


		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::IRONBULLET)
		{
			item.SetPosition(m_CraftingSlots[31].getPosition().x, m_CraftingSlots[31].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::IRONBULLET)
		{
			item.SetPosition(m_CraftingSlots[31].getPosition().x, m_CraftingSlots[31].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
	
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::GOLDBULLET)
		{
			item.SetPosition(m_CraftingSlots[32].getPosition().x, m_CraftingSlots[32].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::GOLDBULLET)
		{
			item.SetPosition(m_CraftingSlots[32].getPosition().x, m_CraftingSlots[32].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::PURPLEBULLET)
		{
			item.SetPosition(m_CraftingSlots[33].getPosition().x, m_CraftingSlots[33].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::PURPLEBULLET)
		{
			item.SetPosition(m_CraftingSlots[33].getPosition().x, m_CraftingSlots[33].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::GOLDENBULLET)
		{
			item.SetPosition(m_CraftingSlots[34].getPosition().x, m_CraftingSlots[34].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::PROJECTILE && item.m_ProjectileType == CBlock::PROJECTILETYPE::GOLDENBULLET)
		{
			item.SetPosition(m_CraftingSlots[34].getPosition().x, m_CraftingSlots[34].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

		//
		// Helmets
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::HELMET && item.m_ArmourType == CBlock::ARMOURTYPE::CACTUS)
		{
			item.SetPosition(m_CraftingSlots[35].getPosition().x, m_CraftingSlots[35].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::HELMET && item.m_ArmourType == CBlock::ARMOURTYPE::CACTUS)
		{
			item.SetPosition(m_CraftingSlots[35].getPosition().x, m_CraftingSlots[35].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::HELMET && item.m_ArmourType == CBlock::ARMOURTYPE::IRON)
		{
			item.SetPosition(m_CraftingSlots[38].getPosition().x, m_CraftingSlots[38].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::HELMET && item.m_ArmourType == CBlock::ARMOURTYPE::IRON)
		{
			item.SetPosition(m_CraftingSlots[38].getPosition().x, m_CraftingSlots[38].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::HELMET && item.m_ArmourType == CBlock::ARMOURTYPE::GOLD)
		{
			item.SetPosition(m_CraftingSlots[41].getPosition().x, m_CraftingSlots[41].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::HELMET && item.m_ArmourType == CBlock::ARMOURTYPE::GOLD)
		{
			item.SetPosition(m_CraftingSlots[41].getPosition().x, m_CraftingSlots[41].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::HELMET && item.m_ArmourType == CBlock::ARMOURTYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[44].getPosition().x, m_CraftingSlots[44].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::HELMET && item.m_ArmourType == CBlock::ARMOURTYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[44].getPosition().x, m_CraftingSlots[44].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::HELMET && item.m_ArmourType == CBlock::ARMOURTYPE::PURPLE)
		{
			item.SetPosition(m_CraftingSlots[47].getPosition().x, m_CraftingSlots[47].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::HELMET && item.m_ArmourType == CBlock::ARMOURTYPE::PURPLE)
		{
			item.SetPosition(m_CraftingSlots[47].getPosition().x, m_CraftingSlots[47].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::HELMET && item.m_ArmourType == CBlock::ARMOURTYPE::GOLDEN)
		{
			item.SetPosition(m_CraftingSlots[50].getPosition().x, m_CraftingSlots[50].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::HELMET && item.m_ArmourType == CBlock::ARMOURTYPE::GOLDEN)
		{
			item.SetPosition(m_CraftingSlots[50].getPosition().x, m_CraftingSlots[50].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

		//
		// ChestPlates
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && item.m_ArmourType == CBlock::ARMOURTYPE::CACTUS)
		{
			item.SetPosition(m_CraftingSlots[36].getPosition().x, m_CraftingSlots[36].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && item.m_ArmourType == CBlock::ARMOURTYPE::CACTUS)
		{
			item.SetPosition(m_CraftingSlots[36].getPosition().x, m_CraftingSlots[36].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && item.m_ArmourType == CBlock::ARMOURTYPE::IRON)
		{
			item.SetPosition(m_CraftingSlots[39].getPosition().x, m_CraftingSlots[39].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && item.m_ArmourType == CBlock::ARMOURTYPE::IRON)
		{
			item.SetPosition(m_CraftingSlots[39].getPosition().x, m_CraftingSlots[39].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && item.m_ArmourType == CBlock::ARMOURTYPE::GOLD)
		{
			item.SetPosition(m_CraftingSlots[42].getPosition().x, m_CraftingSlots[42].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && item.m_ArmourType == CBlock::ARMOURTYPE::GOLD)
		{
			item.SetPosition(m_CraftingSlots[42].getPosition().x, m_CraftingSlots[42].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && item.m_ArmourType == CBlock::ARMOURTYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[45].getPosition().x, m_CraftingSlots[45].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && item.m_ArmourType == CBlock::ARMOURTYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[45].getPosition().x, m_CraftingSlots[45].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && item.m_ArmourType == CBlock::ARMOURTYPE::PURPLE)
		{
			item.SetPosition(m_CraftingSlots[48].getPosition().x, m_CraftingSlots[48].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && item.m_ArmourType == CBlock::ARMOURTYPE::PURPLE)
		{
			item.SetPosition(m_CraftingSlots[48].getPosition().x, m_CraftingSlots[48].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && item.m_ArmourType == CBlock::ARMOURTYPE::GOLDEN)
		{
			item.SetPosition(m_CraftingSlots[51].getPosition().x, m_CraftingSlots[51].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && item.m_ArmourType == CBlock::ARMOURTYPE::GOLDEN)
		{
			item.SetPosition(m_CraftingSlots[51].getPosition().x, m_CraftingSlots[51].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

		//
		// Leggings
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && item.m_ArmourType == CBlock::ARMOURTYPE::CACTUS)
		{
			item.SetPosition(m_CraftingSlots[37].getPosition().x, m_CraftingSlots[37].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && item.m_ArmourType == CBlock::ARMOURTYPE::CACTUS)
		{
			item.SetPosition(m_CraftingSlots[37].getPosition().x, m_CraftingSlots[37].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && item.m_ArmourType == CBlock::ARMOURTYPE::IRON)
		{
			item.SetPosition(m_CraftingSlots[40].getPosition().x, m_CraftingSlots[40].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && item.m_ArmourType == CBlock::ARMOURTYPE::IRON)
		{
			item.SetPosition(m_CraftingSlots[40].getPosition().x, m_CraftingSlots[40].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && item.m_ArmourType == CBlock::ARMOURTYPE::GOLD)
		{
			item.SetPosition(m_CraftingSlots[43].getPosition().x, m_CraftingSlots[43].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && item.m_ArmourType == CBlock::ARMOURTYPE::GOLD)
		{
			item.SetPosition(m_CraftingSlots[43].getPosition().x, m_CraftingSlots[43].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && item.m_ArmourType == CBlock::ARMOURTYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[46].getPosition().x, m_CraftingSlots[46].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && item.m_ArmourType == CBlock::ARMOURTYPE::DIAMOND)
		{
			item.SetPosition(m_CraftingSlots[46].getPosition().x, m_CraftingSlots[46].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && item.m_ArmourType == CBlock::ARMOURTYPE::PURPLE)
		{
			item.SetPosition(m_CraftingSlots[49].getPosition().x, m_CraftingSlots[49].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && item.m_ArmourType == CBlock::ARMOURTYPE::PURPLE)
		{
			item.SetPosition(m_CraftingSlots[49].getPosition().x, m_CraftingSlots[49].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}
		if (item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && item.m_ArmourType == CBlock::ARMOURTYPE::GOLDEN)
		{
			item.SetPosition(m_CraftingSlots[52].getPosition().x, m_CraftingSlots[52].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 255;
			item.GetShape().setColor(tempcolor);
		}
		else if (!item.m_bCanCraft && item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && item.m_ArmourType == CBlock::ARMOURTYPE::GOLDEN)
		{
			item.SetPosition(m_CraftingSlots[52].getPosition().x, m_CraftingSlots[52].getPosition().y + 6.0f);
			sf::Color tempcolor;
			tempcolor = item.GetShape().getColor();
			tempcolor.a = 70;
			item.GetShape().setColor(tempcolor);
		}

	}

	// Setting Can Craft
	for (CBlock& item : m_CraftList)
	{
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::WOOD, true) > 0 &&
			 item.m_Type == CBlock::BLOCKTYPE::PLANKS)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (_player->IsItemInInventory(CBlock::BLOCKTYPE::WOOD, true) <= 0 && 
			item.m_Type == CBlock::BLOCKTYPE::PLANKS)
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 6 && 
			item.m_Type == CBlock::BLOCKTYPE::DOOR)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) < 6 && 
			item.m_Type == CBlock::BLOCKTYPE::DOOR)
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::STONE, true) >= 9 && 
			item.m_Type == CBlock::BLOCKTYPE::FURNACE)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (_player->IsItemInInventory(CBlock::BLOCKTYPE::STONE, true) < 9 && 
			item.m_Type == CBlock::BLOCKTYPE::FURNACE)
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 9 && 
			item.m_Type == CBlock::BLOCKTYPE::CHEST)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) < 9 && 
			item.m_Type == CBlock::BLOCKTYPE::CHEST)
		{
			item.m_bCanCraft = false;
		}

		//
		// Iron,Gold,Diamond
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONORE, true) >= 3 && 
			item.m_Type == CBlock::BLOCKTYPE::IRONINGOT && m_bCanSmelt)
		{
			if (m_bCanSmelt)
			{
				item.m_bCanCraft = true;
			}
		}
		else if ((_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONORE, true) < 3 && 
			item.m_Type == CBlock::BLOCKTYPE::IRONINGOT) || 
			(item.m_Type == CBlock::BLOCKTYPE::IRONINGOT && !m_bCanSmelt))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDORE, true) >= 3 && 
			item.m_Type == CBlock::BLOCKTYPE::GOLDINGOT && m_bCanSmelt)
		{
			if (m_bCanSmelt)
			{
				item.m_bCanCraft = true;
			}
		}
		else if ((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDORE, true) < 3 && 
			item.m_Type == CBlock::BLOCKTYPE::GOLDINGOT) || 
			(item.m_Type == CBlock::BLOCKTYPE::GOLDINGOT && !m_bCanSmelt))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::DIAMONDORE, true) >= 2 && 
			item.m_Type == CBlock::BLOCKTYPE::DIAMOND)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if ((_player->IsItemInInventory(CBlock::BLOCKTYPE::DIAMONDORE, true) < 2 && 
			item.m_Type == CBlock::BLOCKTYPE::DIAMOND))
		{
			item.m_bCanCraft = false;
		}

		//
		// Pickaxes
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) >= 3 && 
			_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 2 && 
			item.m_Type == CBlock::BLOCKTYPE::PICKAXE && 
			item.m_PickType == CBlock::PICKAXETYPE::IRON && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) < 3 || 
			_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) < 2) && 
			item.m_Type == CBlock::BLOCKTYPE::PICKAXE && 
			item.m_PickType == CBlock::PICKAXETYPE::IRON) || 
			(item.m_Type == CBlock::BLOCKTYPE::PICKAXE && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}
		
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) >= 3 && 
			_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 2 && 
			item.m_Type == CBlock::BLOCKTYPE::PICKAXE && 
			item.m_PickType == CBlock::PICKAXETYPE::GOLD && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) < 3 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) < 2) && 
			item.m_Type == CBlock::BLOCKTYPE::PICKAXE && 
			item.m_PickType == CBlock::PICKAXETYPE::GOLD) || 
			(item.m_Type == CBlock::BLOCKTYPE::PICKAXE && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}
		
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::DIAMOND, true) >= 3 && 
			_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 2 && 
			item.m_Type == CBlock::BLOCKTYPE::PICKAXE && 
			item.m_PickType == CBlock::PICKAXETYPE::DIAMOND && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::DIAMOND, true) < 3 || 
			_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) < 2) && 
			item.m_Type == CBlock::BLOCKTYPE::PICKAXE && 
			item.m_PickType == CBlock::PICKAXETYPE::DIAMOND) || 
			(item.m_Type == CBlock::BLOCKTYPE::PICKAXE && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}
		
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) >= 3 && 
			_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 2 && 
			item.m_Type == CBlock::BLOCKTYPE::PICKAXE && 
			item.m_PickType == CBlock::PICKAXETYPE::PURPLE && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) < 3 || 
			_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) < 2) && 
			item.m_Type == CBlock::BLOCKTYPE::PICKAXE && 
			item.m_PickType == CBlock::PICKAXETYPE::PURPLE) || 
			(item.m_Type == CBlock::BLOCKTYPE::PICKAXE && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}
		
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) >= 3 && 
			_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 2 && 
			item.m_Type == CBlock::BLOCKTYPE::PICKAXE && 
			item.m_PickType == CBlock::PICKAXETYPE::GOLDEN && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) < 3 || 
			_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) < 2) && 
			item.m_Type == CBlock::BLOCKTYPE::PICKAXE && 
			item.m_PickType == CBlock::PICKAXETYPE::GOLDEN) || 
			(item.m_Type == CBlock::BLOCKTYPE::PICKAXE && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}
		
		//
		// Purple,Golden
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEORE, true) >= 3 && 
			item.m_Type == CBlock::BLOCKTYPE::PURPLEINGOT && m_bCanSmelt)
		{
			if (m_bCanSmelt)
			{
				item.m_bCanCraft = true;
			}
		}
		else if ((_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEORE, true) < 3 && 
			item.m_Type == CBlock::BLOCKTYPE::PURPLEINGOT) || 
			(item.m_Type == CBlock::BLOCKTYPE::PURPLEINGOT && !m_bCanSmelt))
		{
			item.m_bCanCraft = false;
		}
		
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENORE, true) >= 3 && 
			item.m_Type == CBlock::BLOCKTYPE::GOLDENINGOT && m_bCanSmelt)
		{
			if (m_bCanSmelt)
			{
				item.m_bCanCraft = true;
			}
		}
		else if ((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENORE, true) < 3 && 
			item.m_Type == CBlock::BLOCKTYPE::GOLDENINGOT) || 
			(item.m_Type == CBlock::BLOCKTYPE::GOLDENINGOT && !m_bCanSmelt))
		{
			item.m_bCanCraft = false;
		}
		
		//
		// Anvil
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) >= 6 &&
			item.m_Type == CBlock::BLOCKTYPE::ANVIL && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) < 6) &&
			item.m_Type == CBlock::BLOCKTYPE::ANVIL) ||
			(item.m_Type == CBlock::BLOCKTYPE::ANVIL && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}

		//
		// WorkBench
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 6 &&
			item.m_Type == CBlock::BLOCKTYPE::WORKBENCH)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) < 6) &&
			item.m_Type == CBlock::BLOCKTYPE::WORKBENCH) ||
			(item.m_Type == CBlock::BLOCKTYPE::WORKBENCH))
		{
			item.m_bCanCraft = false;
		}
		
		//
		// Coal
		if ((_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) > 0 && 
			_player->IsItemInInventory(CBlock::BLOCKTYPE::COALORE, true) > 0) && 
			item.m_Type == CBlock::BLOCKTYPE::TOURCH)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if ((_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) <= 0 || 
			_player->IsItemInInventory(CBlock::BLOCKTYPE::COALORE, true) <= 0) && 
			item.m_Type == CBlock::BLOCKTYPE::TOURCH)
		{
			item.m_bCanCraft = false;
		}
		
		//
		// Bows
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) >= 3 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::BASIC && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) < 3 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::BASIC) ||
			(item.m_Type == CBlock::BLOCKTYPE::BOW && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}
		
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) >= 3 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::IRON && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) < 3 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::IRON) ||
			(item.m_Type == CBlock::BLOCKTYPE::BOW && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) >= 3 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::PURPLE && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) < 3 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::PURPLE) ||
			(item.m_Type == CBlock::BLOCKTYPE::BOW && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) >= 3 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::GOLDEN && m_bCanWorkBench)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) < 3 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::GOLDEN) ||
			(item.m_Type == CBlock::BLOCKTYPE::BOW && !m_bCanWorkBench))
		{
			item.m_bCanCraft = false;
		}

		//
		// Guns
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::IRONGUN && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::IRONGUN) ||
			(item.m_Type == CBlock::BLOCKTYPE::BOW &&
				item.m_BowType == CBlock::BOWTYPE::IRONGUN && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::GOLDGUN && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::GOLDGUN) ||
			(item.m_Type == CBlock::BLOCKTYPE::BOW &&
				item.m_BowType == CBlock::BOWTYPE::GOLDGUN && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::PURPLEGUN && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::PURPLEGUN) ||
			(item.m_Type == CBlock::BLOCKTYPE::BOW &&
				item.m_BowType == CBlock::BOWTYPE::PURPLEGUN && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::GOLDENGUN && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::BOW &&
			item.m_BowType == CBlock::BOWTYPE::GOLDENGUN) ||
			(item.m_Type == CBlock::BLOCKTYPE::BOW &&
				item.m_BowType == CBlock::BOWTYPE::GOLDENGUN && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		//
		// Arrows
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) > 0 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::STONE, true) > 0 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) > 0 &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::ARROW)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) <= 0 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::STONE, true) <= 0 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) <= 0) &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::ARROW))
		{
			item.m_bCanCraft = false;
		}
		
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) > 0 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::STONE, true) > 0 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) > 0 &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::FIREARROW)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) <= 0 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::STONE, true) <= 0 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) <= 0) &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::FIREARROW))
		{
			item.m_bCanCraft = false;
		}
		
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) > 0 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::STONE, true) > 0 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) > 0 &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::CURSEDARROW)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) <= 0 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::STONE, true) <= 0 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) <= 0) &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::CURSEDARROW))
		{
			item.m_bCanCraft = false;
		}
		
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) > 0 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::STONE, true) > 0 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) > 0 &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::POISONARROW)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::PLANKS, true) <= 0 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::STONE, true) <= 0 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::LEAVES, true) <= 0) &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::POISONARROW))
		{
			item.m_bCanCraft = false;
		}

		//
		// Potions
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::EMPTYBEAKER, true) >= 1 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::REDSLIME, true) >= 1 &&
			item.m_Type == CBlock::BLOCKTYPE::POTION &&
			item.m_PotionType == CBlock::POTIONTYPE::HPSMALL)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::EMPTYBEAKER, true) < 1 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::REDSLIME, true) < 1) &&
			item.m_Type == CBlock::BLOCKTYPE::POTION &&
			item.m_PotionType == CBlock::POTIONTYPE::HPSMALL))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::EMPTYBEAKER, true) >= 1 &&
			_player->IsItemInInventory(CBlock::BLOCKTYPE::REDSLIME, true) >= 2 &&
			item.m_Type == CBlock::BLOCKTYPE::POTION &&
			item.m_PotionType == CBlock::POTIONTYPE::HPLARGE)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::EMPTYBEAKER, true) < 1 ||
			_player->IsItemInInventory(CBlock::BLOCKTYPE::REDSLIME, true) < 2) &&
			item.m_Type == CBlock::BLOCKTYPE::POTION &&
			item.m_PotionType == CBlock::POTIONTYPE::HPLARGE))
		{
			item.m_bCanCraft = false;
		}

		//
		// Bullets
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) >= 1 &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::IRONBULLET && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) < 1) &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::IRONBULLET) ||
			(item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
				item.m_ProjectileType == CBlock::PROJECTILETYPE::IRONBULLET && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) >= 1 &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::GOLDBULLET && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) < 1) &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::GOLDBULLET) ||
			(item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
				item.m_ProjectileType == CBlock::PROJECTILETYPE::GOLDBULLET && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) >= 1 &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::PURPLEBULLET && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) < 1) &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::PURPLEBULLET) ||
			(item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
				item.m_ProjectileType == CBlock::PROJECTILETYPE::PURPLEBULLET && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) >= 1 &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::GOLDENBULLET && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) < 1) &&
			item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
			item.m_ProjectileType == CBlock::PROJECTILETYPE::GOLDENBULLET) ||
			(item.m_Type == CBlock::BLOCKTYPE::PROJECTILE &&
				item.m_ProjectileType == CBlock::PROJECTILETYPE::GOLDENBULLET && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		//
		// Helmets
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::CACTUS, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::HELMET &&
			item.m_ArmourType == CBlock::ARMOURTYPE::CACTUS && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::CACTUS, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::HELMET &&
			item.m_ArmourType == CBlock::ARMOURTYPE::CACTUS) ||
			(item.m_Type == CBlock::BLOCKTYPE::HELMET && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::HELMET &&
			item.m_ArmourType == CBlock::ARMOURTYPE::IRON && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::HELMET &&
			item.m_ArmourType == CBlock::ARMOURTYPE::IRON) ||
			(item.m_Type == CBlock::BLOCKTYPE::HELMET && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::HELMET &&
			item.m_ArmourType == CBlock::ARMOURTYPE::GOLD && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::HELMET &&
			item.m_ArmourType == CBlock::ARMOURTYPE::GOLD) ||
			(item.m_Type == CBlock::BLOCKTYPE::HELMET && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::DIAMOND, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::HELMET &&
			item.m_ArmourType == CBlock::ARMOURTYPE::DIAMOND && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::DIAMOND, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::HELMET &&
			item.m_ArmourType == CBlock::ARMOURTYPE::DIAMOND) ||
			(item.m_Type == CBlock::BLOCKTYPE::HELMET && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::HELMET &&
			item.m_ArmourType == CBlock::ARMOURTYPE::PURPLE && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::HELMET &&
			item.m_ArmourType == CBlock::ARMOURTYPE::PURPLE) ||
			(item.m_Type == CBlock::BLOCKTYPE::HELMET && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::HELMET &&
			item.m_ArmourType == CBlock::ARMOURTYPE::GOLDEN && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::HELMET &&
			item.m_ArmourType == CBlock::ARMOURTYPE::GOLDEN) ||
			(item.m_Type == CBlock::BLOCKTYPE::HELMET && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		//
		// ChestPlates
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::CACTUS, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE &&
			item.m_ArmourType == CBlock::ARMOURTYPE::CACTUS && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::CACTUS, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE &&
			item.m_ArmourType == CBlock::ARMOURTYPE::CACTUS) ||
			(item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE &&
			item.m_ArmourType == CBlock::ARMOURTYPE::IRON && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE &&
			item.m_ArmourType == CBlock::ARMOURTYPE::IRON) ||
			(item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE &&
			item.m_ArmourType == CBlock::ARMOURTYPE::GOLD && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE &&
			item.m_ArmourType == CBlock::ARMOURTYPE::GOLD) ||
			(item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::DIAMOND, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE &&
			item.m_ArmourType == CBlock::ARMOURTYPE::DIAMOND && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::DIAMOND, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE &&
			item.m_ArmourType == CBlock::ARMOURTYPE::DIAMOND) ||
			(item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE &&
			item.m_ArmourType == CBlock::ARMOURTYPE::PURPLE && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE &&
			item.m_ArmourType == CBlock::ARMOURTYPE::PURPLE) ||
			(item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE &&
			item.m_ArmourType == CBlock::ARMOURTYPE::GOLDEN && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE &&
			item.m_ArmourType == CBlock::ARMOURTYPE::GOLDEN) ||
			(item.m_Type == CBlock::BLOCKTYPE::CHESTPLATE && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		//
		// Leggings
		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::CACTUS, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::LEGGINGS &&
			item.m_ArmourType == CBlock::ARMOURTYPE::CACTUS && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::CACTUS, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::LEGGINGS &&
			item.m_ArmourType == CBlock::ARMOURTYPE::CACTUS) ||
			(item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::LEGGINGS &&
			item.m_ArmourType == CBlock::ARMOURTYPE::IRON && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::IRONINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::LEGGINGS &&
			item.m_ArmourType == CBlock::ARMOURTYPE::IRON) ||
			(item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::LEGGINGS &&
			item.m_ArmourType == CBlock::ARMOURTYPE::GOLD && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::LEGGINGS &&
			item.m_ArmourType == CBlock::ARMOURTYPE::GOLD) ||
			(item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::DIAMOND, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::LEGGINGS &&
			item.m_ArmourType == CBlock::ARMOURTYPE::DIAMOND && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::DIAMOND, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::LEGGINGS &&
			item.m_ArmourType == CBlock::ARMOURTYPE::DIAMOND) ||
			(item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::LEGGINGS &&
			item.m_ArmourType == CBlock::ARMOURTYPE::PURPLE && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::PURPLEINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::LEGGINGS &&
			item.m_ArmourType == CBlock::ARMOURTYPE::PURPLE) ||
			(item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}

		if (_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) >= 5 &&
			item.m_Type == CBlock::BLOCKTYPE::LEGGINGS &&
			item.m_ArmourType == CBlock::ARMOURTYPE::GOLDEN && m_bCanAnvil)
		{
			if (!item.m_bCanCraft)
			{
				item.m_bCanCraft = true;
			}
		}
		else if (((_player->IsItemInInventory(CBlock::BLOCKTYPE::GOLDENINGOT, true) < 5) &&
			item.m_Type == CBlock::BLOCKTYPE::LEGGINGS &&
			item.m_ArmourType == CBlock::ARMOURTYPE::GOLDEN) ||
			(item.m_Type == CBlock::BLOCKTYPE::LEGGINGS && !m_bCanAnvil))
		{
			item.m_bCanCraft = false;
		}
	}

	// Removing And Adding Items
	for (CBlock& item : m_CraftList)
	{
		if (item.GetShape().getGlobalBounds().contains(m_MousePointer.getPosition()) && item.m_bCanCraft && sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_CraftTimer->getElapsedTime().asSeconds() >= 0.2f && _player->m_InventorySize < 49)
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
			case CBlock::BLOCKTYPE::PURPLEINGOT:
			{
				m_TempBlock = new CBlock(_textureMaster->m_PurpleIngot, CBlock::BLOCKTYPE::PURPLEINGOT);
				_player->AddItemToInventory(m_TempBlock);

				// Remove 3 Purple Ore
				for (int i = 0; i < 3; i++)
				{
					if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEORE)] <= 1)
					{
						_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEORE));
					}
					else
					{
						_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEORE)]--;
					}
				}
				break;
			}
			case CBlock::BLOCKTYPE::GOLDENINGOT:
			{
				m_TempBlock = new CBlock(_textureMaster->m_GoldenIngot, CBlock::BLOCKTYPE::GOLDENINGOT);
				_player->AddItemToInventory(m_TempBlock);

				// Remove 3 Purple Ore
				for (int i = 0; i < 3; i++)
				{
					if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENORE)] <= 1)
					{
						_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENORE));
					}
					else
					{
						_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENORE)]--;
					}
				}
				break;
			}
			case CBlock::BLOCKTYPE::ANVIL:
			{
				CWorkBench* workbench = new CWorkBench(CBlock::WORKBENCHTYPE::ANVIL);
				_player->AddItemToInventory(workbench, false);
				workbench = nullptr;
				

				// Remove 6 IRON INGOT
				for (int i = 0; i < 6; i++)
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
				break;
			}
			case CBlock::BLOCKTYPE::WORKBENCH:
			{
				m_TempBlock = new CBlock(_textureMaster->m_WorkBench, CBlock::BLOCKTYPE::WORKBENCH);
				_player->AddItemToInventory(m_TempBlock, false);

				// Remove 6 PLANKS
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
			case CBlock::BLOCKTYPE::TOURCH:
			{
				m_TempBlock = new CBlock(_textureMaster->m_Tourch, CBlock::BLOCKTYPE::TOURCH);
				_player->AddItemToInventory(m_TempBlock);

				// Remove 1 COAL
				for (int i = 0; i < 1; i++)
				{
					if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::COALORE)] <= 1)
					{
						_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::COALORE));
					}
					else
					{
						_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::COALORE)]--;
					}
				}
				// Remove 1 PLANKS
				for (int i = 0; i < 1; i++)
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
				case CBlock::PICKAXETYPE::PURPLE:
				{
					// Remove Current Pickaxe
					//int tempPos = _player->GetPositionInInventory(CBlock::BLOCKTYPE::PICKAXE);
					//_player->RemoveItemFromInventory(tempPos);

					CPickaxe* temp = new CPickaxe(CBlock::PICKAXETYPE::PURPLE);
					_player->AddItemToInventory(temp, false);

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 3 Diamond
					for (int i = 0; i < 3; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)]--;
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
				case CBlock::PICKAXETYPE::GOLDEN:
				{
					// Remove Current Pickaxe
					//int tempPos = _player->GetPositionInInventory(CBlock::BLOCKTYPE::PICKAXE);
					//_player->RemoveItemFromInventory(tempPos);

					CPickaxe* temp = new CPickaxe(CBlock::PICKAXETYPE::GOLDEN);
					_player->AddItemToInventory(temp, false);

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 3 Diamond
					for (int i = 0; i < 3; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)]--;
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

			case CBlock::BLOCKTYPE::BOW:
			{
				switch (item.m_BowType)
				{
				case CBlock::BOWTYPE::IRON:
				{
					Bow* tempbow = new Bow(CBlock::BOWTYPE::IRON);
					_player->AddItemToInventory(tempbow, false);
					tempbow = nullptr;

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 3 Iron
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
					// Remove 5 LEAVES
					for (int i = 0; i < 5; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)]--;
						}
					}
					break;
				}
				case CBlock::BOWTYPE::PURPLE:
				{
					Bow* tempbow = new Bow(CBlock::BOWTYPE::PURPLE);
					_player->AddItemToInventory(tempbow, false);
					tempbow = nullptr;

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 3 PURPLE
					for (int i = 0; i < 3; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)]--;
						}
					}
					// Remove 5 LEAVES
					for (int i = 0; i < 5; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)]--;
						}
					}
					break;
				}
				case CBlock::BOWTYPE::GOLDEN:
				{
					Bow* tempbow = new Bow(CBlock::BOWTYPE::GOLDEN);
					_player->AddItemToInventory(tempbow, false);
					tempbow = nullptr;

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 3 gOLDEN
					for (int i = 0; i < 3; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)]--;
						}
					}
					// Remove 5 LEAVES
					for (int i = 0; i < 5; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)]--;
						}
					}
					break;
				}
				case CBlock::BOWTYPE::IRONGUN:
				{
					Bow* tempbow = new Bow(CBlock::BOWTYPE::IRONGUN);
					_player->AddItemToInventory(tempbow, false);
					tempbow = nullptr;

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 5 iRON
					for (int i = 0; i < 5; i++)
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
					break;
				}
				case CBlock::BOWTYPE::GOLDGUN:
				{
					Bow* tempbow = new Bow(CBlock::BOWTYPE::GOLDGUN);
					_player->AddItemToInventory(tempbow, false);
					tempbow = nullptr;

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 5 gold
					for (int i = 0; i < 5; i++)
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
					break;
				}
				case CBlock::BOWTYPE::PURPLEGUN:
				{
					Bow* tempbow = new Bow(CBlock::BOWTYPE::PURPLEGUN);
					_player->AddItemToInventory(tempbow, false);
					tempbow = nullptr;

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 5 gold
					for (int i = 0; i < 5; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)]--;
						}
					}
					break;
				}
				case CBlock::BOWTYPE::GOLDENGUN:
				{
					Bow* tempbow = new Bow(CBlock::BOWTYPE::GOLDENGUN);
					_player->AddItemToInventory(tempbow, false);
					tempbow = nullptr;

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 5 gold
					for (int i = 0; i < 5; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)]--;
						}
					}
					break;
				}
				default:
				{
					Bow* tempbow = new Bow(CBlock::BOWTYPE::BASIC);
					_player->AddItemToInventory(tempbow, false);
					tempbow = nullptr;

					// Player Is Holding Pixkaxe During Swap
					InitHotBarScrolling(_player);

					// Remove 3 Planks
					for (int i = 0; i < 3; i++)
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
					// Remove 5 LEAVES
					for (int i = 0; i < 5; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)]--;
						}
					}
					break;
				}
				}
				break;
			}

			case CBlock::BLOCKTYPE::SWORD:
			{
				break;
			}

			case CBlock::BLOCKTYPE::PROJECTILE:
			{
				switch (item.m_ProjectileType)
				{
				case CBlock::PROJECTILETYPE::ARROW:
				{
					CProjectile* temparrow = new CProjectile(CBlock::PROJECTILETYPE::ARROW);
					for (int i = 0; i < 3; i++)
					{
						_player->AddItemToInventory(temparrow, true);
					}
					temparrow = nullptr;

					// Remove 1 Planks
					for (int i = 0; i < 1; i++)
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
					// Remove 1 Leaves
					for (int i = 0; i < 1; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)]--;
						}
					}
					// Remove 1 Stone
					for (int i = 0; i < 1; i++)
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
				case CBlock::PROJECTILETYPE::FIREARROW:
				{
					CProjectile* temparrow = new CProjectile(CBlock::PROJECTILETYPE::FIREARROW);
					for (int i = 0; i < 3; i++)
					{
						_player->AddItemToInventory(temparrow, true);
					}
					temparrow = nullptr;

					// Remove 1 Planks
					for (int i = 0; i < 1; i++)
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
					// Remove 1 Leaves
					for (int i = 0; i < 1; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)]--;
						}
					}
					// Remove 1 Stone
					for (int i = 0; i < 1; i++)
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
				case CBlock::PROJECTILETYPE::CURSEDARROW:
				{
					CProjectile* temparrow = new CProjectile(CBlock::PROJECTILETYPE::CURSEDARROW);
					for (int i = 0; i < 3; i++)
					{
						_player->AddItemToInventory(temparrow, true);
					}
					temparrow = nullptr;

					// Remove 1 Planks
					for (int i = 0; i < 1; i++)
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
					// Remove 1 Leaves
					for (int i = 0; i < 1; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)]--;
						}
					}
					// Remove 1 Stone
					for (int i = 0; i < 1; i++)
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
				case CBlock::PROJECTILETYPE::POISONARROW:
				{
					CProjectile* temparrow = new CProjectile(CBlock::PROJECTILETYPE::POISONARROW);
					for (int i = 0; i < 3; i++)
					{
						_player->AddItemToInventory(temparrow, true);
					}
					temparrow = nullptr;

					// Remove 1 Planks
					for (int i = 0; i < 1; i++)
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
					// Remove 1 Leaves
					for (int i = 0; i < 1; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)]--;
						}
					}
					// Remove 1 Stone
					for (int i = 0; i < 1; i++)
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
				case CBlock::PROJECTILETYPE::IRONBULLET:
				{
					CProjectile* temparrow = new CProjectile(CBlock::PROJECTILETYPE::IRONBULLET);
					for (int i = 0; i < 5; i++)
					{
						_player->AddItemToInventory(temparrow, true);
					}
					temparrow = nullptr;

					// Remove 1 Iron
					for (int i = 0; i < 1; i++)
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
					break;
				}
				case CBlock::PROJECTILETYPE::GOLDBULLET:
				{
					CProjectile* temparrow = new CProjectile(CBlock::PROJECTILETYPE::GOLDBULLET);
					for (int i = 0; i < 5; i++)
					{
						_player->AddItemToInventory(temparrow, true);
					}
					temparrow = nullptr;

					// Remove 1 Gold
					for (int i = 0; i < 1; i++)
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
					break;
				}
				case CBlock::PROJECTILETYPE::PURPLEBULLET:
				{
					CProjectile* temparrow = new CProjectile(CBlock::PROJECTILETYPE::PURPLEBULLET);
					for (int i = 0; i < 5; i++)
					{
						_player->AddItemToInventory(temparrow, true);
					}
					temparrow = nullptr;

					// Remove 1 Gold
					for (int i = 0; i < 1; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)]--;
						}
					}
					break;
				}
				case CBlock::PROJECTILETYPE::GOLDENBULLET:
				{
					CProjectile* temparrow = new CProjectile(CBlock::PROJECTILETYPE::GOLDENBULLET);
					for (int i = 0; i < 5; i++)
					{
						_player->AddItemToInventory(temparrow, true);
					}
					temparrow = nullptr;

					// Remove 1 Gold
					for (int i = 0; i < 1; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)]--;
						}
					}
					break;
				}
				default:
				{
					CProjectile* temparrow = new CProjectile(CBlock::PROJECTILETYPE::ARROW);
					for (int i = 0; i < 3; i++)
					{
						_player->AddItemToInventory(temparrow, true);
					}
					temparrow = nullptr;

					// Remove 1 Planks
					for (int i = 0; i < 1; i++)
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
					// Remove 1 Leaves
					for (int i = 0; i < 1; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::LEAVES)]--;
						}
					}
					// Remove 1 Stone
					for (int i = 0; i < 1; i++)
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
				break;
				}
			default:
			{
				break;
			}
			}

			case CBlock::BLOCKTYPE::POTION:
			{
				// potion stuff
				switch (item.m_PotionType)
				{
				case CBlock::POTIONTYPE::HPSMALL:
				{
					CPotion* tempPotion = new CPotion(CBlock::POTIONTYPE::HPSMALL);
					for (int i = 0; i < 3; i++)
					{
						_player->AddItemToInventory(tempPotion, true);
					}
					tempPotion = nullptr;

					// Remove 1 Empty Beaker
					for (int i = 0; i < 1; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::EMPTYBEAKER)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::EMPTYBEAKER));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::EMPTYBEAKER)]--;
						}
					}
					// Remove 1 Red Slime
					for (int i = 0; i < 1; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::REDSLIME)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::REDSLIME));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::REDSLIME)]--;
						}
					}

					break;
				}
				case CBlock::POTIONTYPE::HPLARGE:
				{
					CPotion* tempPotion = new CPotion(CBlock::POTIONTYPE::HPLARGE);
					for (int i = 0; i < 3; i++)
					{
						_player->AddItemToInventory(tempPotion, true);
					}
					tempPotion = nullptr;

					// Remove 1 Empty Beaker
					for (int i = 0; i < 1; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::EMPTYBEAKER)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::EMPTYBEAKER));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::EMPTYBEAKER)]--;
						}
					}
					// Remove 2 Red Slime
					for (int i = 0; i < 1; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::REDSLIME)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::REDSLIME));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::REDSLIME)]--;
						}
					}

					break;
				}
				default:
					break;
				}
				break;
			}

			case CBlock::BLOCKTYPE::HELMET:
			{
				// potion stuff
				switch (item.m_ArmourType)
				{
				case CBlock::ARMOURTYPE::CACTUS:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::HELMET, CBlock::ARMOURTYPE::CACTUS);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 5 Cactus
					for (int i = 0; i < 5; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::CACTUS)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::CACTUS));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::CACTUS)]--;
						}
					}
					break;
				}
				case CBlock::ARMOURTYPE::IRON:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::HELMET, CBlock::ARMOURTYPE::IRON);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 5 IRONINGOT
					for (int i = 0; i < 5; i++)
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
					break;
				}
				case CBlock::ARMOURTYPE::GOLD:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::HELMET, CBlock::ARMOURTYPE::GOLD);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 5 GOLDINGOT
					for (int i = 0; i < 5; i++)
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
					break;
				}
				case CBlock::ARMOURTYPE::DIAMOND:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::HELMET, CBlock::ARMOURTYPE::DIAMOND);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 5 DIAMOND
					for (int i = 0; i < 5; i++)
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
					break;
				}
				case CBlock::ARMOURTYPE::PURPLE:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::HELMET, CBlock::ARMOURTYPE::PURPLE);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 5 PURPLE
					for (int i = 0; i < 5; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)]--;
						}
					}
					break;
				}
				case CBlock::ARMOURTYPE::GOLDEN:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::HELMET, CBlock::ARMOURTYPE::GOLDEN);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 5 GOLDENINGOT
					for (int i = 0; i < 5; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)]--;
						}
					}
					break;
				}
				default:
					break;
				}

				break;
			}
			case CBlock::BLOCKTYPE::CHESTPLATE:
			{
				// potion stuff
				switch (item.m_ArmourType)
				{
				case CBlock::ARMOURTYPE::CACTUS:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::CHESTPLATE, CBlock::ARMOURTYPE::CACTUS);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 8 Cactus
					for (int i = 0; i < 8; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::CACTUS)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::CACTUS));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::CACTUS)]--;
						}
					}
					break;
				}
				case CBlock::ARMOURTYPE::IRON:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::CHESTPLATE, CBlock::ARMOURTYPE::IRON);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 8 IRONINGOT
					for (int i = 0; i < 8; i++)
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
					break;
				}
				case CBlock::ARMOURTYPE::GOLD:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::CHESTPLATE, CBlock::ARMOURTYPE::GOLD);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 8 GOLDINGOT
					for (int i = 0; i < 8; i++)
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
					break;
				}
				case CBlock::ARMOURTYPE::DIAMOND:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::CHESTPLATE, CBlock::ARMOURTYPE::DIAMOND);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 5 DIAMOND
					for (int i = 0; i < 8; i++)
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
					break;
				}
				case CBlock::ARMOURTYPE::PURPLE:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::CHESTPLATE, CBlock::ARMOURTYPE::PURPLE);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 8 PURPLE
					for (int i = 0; i < 8; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)]--;
						}
					}
					break;
				}
				case CBlock::ARMOURTYPE::GOLDEN:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::CHESTPLATE, CBlock::ARMOURTYPE::GOLDEN);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 8 GOLDENINGOT
					for (int i = 0; i < 8; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)]--;
						}
					}
					break;
				}
				default:
					break;
				}

				break;
			}
			case CBlock::BLOCKTYPE::LEGGINGS:
			{
				// potion stuff
				switch (item.m_ArmourType)
				{
				case CBlock::ARMOURTYPE::CACTUS:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::LEGGINGS, CBlock::ARMOURTYPE::CACTUS);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 7 Cactus
					for (int i = 0; i < 7; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::CACTUS)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::CACTUS));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::CACTUS)]--;
						}
					}
					break;
				}
				case CBlock::ARMOURTYPE::IRON:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::LEGGINGS, CBlock::ARMOURTYPE::IRON);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 7 IRONINGOT
					for (int i = 0; i < 7; i++)
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
					break;
				}
				case CBlock::ARMOURTYPE::GOLD:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::LEGGINGS, CBlock::ARMOURTYPE::GOLD);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 7 GOLDINGOT
					for (int i = 0; i < 7; i++)
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
					break;
				}
				case CBlock::ARMOURTYPE::DIAMOND:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::LEGGINGS, CBlock::ARMOURTYPE::DIAMOND);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 7 DIAMOND
					for (int i = 0; i < 7; i++)
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
					break;
				}
				case CBlock::ARMOURTYPE::PURPLE:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::LEGGINGS, CBlock::ARMOURTYPE::PURPLE);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 7 PURPLE
					for (int i = 0; i < 7; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::PURPLEINGOT)]--;
						}
					}
					break;
				}
				case CBlock::ARMOURTYPE::GOLDEN:
				{
					CArmour* armour = new CArmour(CBlock::BLOCKTYPE::LEGGINGS, CBlock::ARMOURTYPE::GOLDEN);
					for (int i = 0; i < 1; i++)
					{
						_player->AddItemToInventory(armour, false);
					}
					armour = nullptr;

					// Remove 7 GOLDENINGOT
					for (int i = 0; i < 7; i++)
					{
						if (_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)] <= 1)
						{
							_player->RemoveItemFromInventory(_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT));
						}
						else
						{
							_player->m_InventoryStackValues[_player->GetPositionInInventory(CBlock::BLOCKTYPE::GOLDENINGOT)]--;
						}
					}
					break;
				}
				default:
					break;
				}

				break;
			}
			}

			m_TempBlock = nullptr;
			m_CraftTimer->restart();
			// Player Is Holding Pixkaxe During Swap
			InitHotBarScrolling(_player);
			return;
		}
	}
}

void GUI::InitCraftingUI(CTextureMaster* _textureMaster)
{
	sf::Color color = sf::Color();

	// Row 1
	for (int i = 0; i < 10; i++)
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
	// Row 2
	for (int i = 10; i < 20; i++)
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
	// Row 2
	for (int i = 20; i < 30; i++)
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
	// Row 4
	for (int i = 30; i < 40; i++)
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
	// Row 4
	for (int i = 40; i < 50; i++)
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
	// Row 4
	for (int i = 50; i < 60; i++)
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
	CBlock* Recipe1 = new CBlock(_textureMaster->m_Planks, CBlock::BLOCKTYPE::PLANKS);
	Recipe1->GetShape().setScale(0.43f, 0.43f);
	Recipe1->GetShape().setOrigin(Recipe1->GetShape().getGlobalBounds().width / 2, Recipe1->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe1);
	CBlock* Recipe2 = new CBlock(_textureMaster->m_Chest, CBlock::BLOCKTYPE::CHEST);
	Recipe2->GetShape().setScale(0.43f, 0.43f);
	Recipe2->GetShape().setOrigin(Recipe2->GetShape().getGlobalBounds().width / 2, Recipe2->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe2);
	CBlock* Recipe3 = new CBlock(_textureMaster->m_DoorLeft, CBlock::BLOCKTYPE::DOOR);
	Recipe3->GetShape().setScale(0.23f, 0.23f);
	m_CraftList.push_back(*Recipe3);
	Bow* Recipe4 = new Bow(CBlock::BOWTYPE::BASIC);
	Recipe4->GetShape().setScale(0.43f, 0.43f);
	Recipe4->GetShape().setOrigin(Recipe4->GetShape().getGlobalBounds().width / 2, Recipe4->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe4);
	Bow* Recipe5 = new Bow(CBlock::BOWTYPE::IRON);
	Recipe5->GetShape().setScale(0.43f, 0.43f);
	Recipe5->GetShape().setOrigin(Recipe5->GetShape().getGlobalBounds().width / 2, Recipe5->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe5);
	CBlock* Recipe6 = new CBlock(_textureMaster->m_Tourch, CBlock::BLOCKTYPE::TOURCH);
	Recipe6->GetShape().setScale(0.43f, 0.43f);
	Recipe6->GetShape().setOrigin(Recipe6->GetShape().getGlobalBounds().width / 2, Recipe6->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe6);
	CBlock* Recipe7 = new CBlock(_textureMaster->m_Furnace, CBlock::BLOCKTYPE::FURNACE);
	Recipe7->GetShape().setScale(0.43f, 0.43f);
	Recipe7->GetShape().setOrigin(Recipe7->GetShape().getGlobalBounds().width / 2, Recipe7->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe7);
	CBlock* Recipe8 = new CBlock(_textureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
	Recipe8->GetShape().setScale(0.43f, 0.43f);
	Recipe8->GetShape().setOrigin(Recipe8->GetShape().getGlobalBounds().width / 2, Recipe8->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe8);
	CBlock* Recipe9 = new CBlock(_textureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
	Recipe9->GetShape().setScale(0.43f, 0.43f);
	Recipe9->GetShape().setOrigin(Recipe9->GetShape().getGlobalBounds().width / 2, Recipe9->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe9);
	CBlock* Recipe10 = new CBlock(_textureMaster->m_DiamondIngot, CBlock::BLOCKTYPE::DIAMOND);
	Recipe10->GetShape().setScale(0.43f, 0.43f);
	Recipe10->GetShape().setOrigin(Recipe10->GetShape().getGlobalBounds().width / 2, Recipe10->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe10);
	CBlock* Recipe11 = new CBlock(_textureMaster->m_PurpleIngot, CBlock::BLOCKTYPE::PURPLEINGOT);
	Recipe11->GetShape().setScale(0.43f, 0.43f);
	Recipe11->GetShape().setOrigin(Recipe11->GetShape().getGlobalBounds().width / 2, Recipe11->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe11);
	CBlock* Recipe12 = new CBlock(_textureMaster->m_GoldenIngot, CBlock::BLOCKTYPE::GOLDENINGOT);
	Recipe12->GetShape().setScale(0.43f, 0.43f);
	Recipe12->GetShape().setOrigin(Recipe12->GetShape().getGlobalBounds().width / 2, Recipe12->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe12);
	CBlock* Recipe13 = new CBlock(_textureMaster->m_Anvil, CBlock::BLOCKTYPE::ANVIL);
	Recipe13->GetShape().setScale(0.43f, 0.43f);
	Recipe13->GetShape().setOrigin(Recipe13->GetShape().getGlobalBounds().width / 2, Recipe13->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe13);
	CBlock* Recipe14 = new CBlock(_textureMaster->m_WorkBench, CBlock::BLOCKTYPE::WORKBENCH);
	Recipe14->GetShape().setScale(0.43f, 0.43f);
	Recipe14->GetShape().setOrigin(Recipe14->GetShape().getGlobalBounds().width / 2, Recipe14->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe14);
	CPickaxe* Recipe15 = new CPickaxe(CPickaxe::PICKAXETYPE::IRON);
	Recipe15->GetShape().setScale(0.43f, 0.43f);
	Recipe15->GetShape().setOrigin(Recipe15->GetShape().getGlobalBounds().width / 2, Recipe15->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe15);
	CPickaxe* Recipe16 = new CPickaxe(CPickaxe::PICKAXETYPE::GOLD);
	Recipe16->GetShape().setScale(0.43f, 0.43f);
	Recipe16->GetShape().setOrigin(Recipe16->GetShape().getGlobalBounds().width / 2, Recipe16->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe16);
	CPickaxe* Recipe17 = new CPickaxe(CPickaxe::PICKAXETYPE::DIAMOND);
	Recipe17->GetShape().setScale(0.43f, 0.43f);
	Recipe17->GetShape().setOrigin(Recipe17->GetShape().getGlobalBounds().width / 2, Recipe17->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe17);
	CPickaxe* Recipe18 = new CPickaxe(CPickaxe::PICKAXETYPE::PURPLE);
	Recipe18->GetShape().setScale(0.43f, 0.43f);
	Recipe18->GetShape().setOrigin(Recipe18->GetShape().getGlobalBounds().width / 2, Recipe18->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe18);
	CPickaxe* Recipe19 = new CPickaxe(CPickaxe::PICKAXETYPE::GOLDEN);
	Recipe19->GetShape().setScale(0.43f, 0.43f);
	Recipe19->GetShape().setOrigin(Recipe19->GetShape().getGlobalBounds().width / 2, Recipe19->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe19);

	CProjectile* Recipe20 = new CProjectile(CBlock::PROJECTILETYPE::ARROW);
	Recipe20->GetShape().setScale(0.43f, 0.43f);
	Recipe20->GetShape().setOrigin(Recipe20->GetShape().getGlobalBounds().width / 2, Recipe20->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe20);
	CProjectile* Recipe21 = new CProjectile(CBlock::PROJECTILETYPE::FIREARROW);
	Recipe21->GetShape().setScale(0.43f, 0.43f);
	Recipe21->GetShape().setOrigin(Recipe21->GetShape().getGlobalBounds().width / 2, Recipe21->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe21);
	CProjectile* Recipe22 = new CProjectile(CBlock::PROJECTILETYPE::CURSEDARROW);
	Recipe22->GetShape().setScale(0.43f, 0.43f);
	Recipe22->GetShape().setOrigin(Recipe22->GetShape().getGlobalBounds().width / 2, Recipe22->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe22);
	CProjectile* Recipe23 = new CProjectile(CBlock::PROJECTILETYPE::POISONARROW);
	Recipe23->GetShape().setScale(0.43f, 0.43f);
	Recipe23->GetShape().setOrigin(Recipe23->GetShape().getGlobalBounds().width / 2, Recipe23->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe23);

	CPotion* Recipe24 = new CPotion(CBlock::POTIONTYPE::HPSMALL);
	Recipe24->GetShape().setScale(0.43f, 0.43f);
	Recipe24->GetShape().setOrigin(Recipe24->GetShape().getGlobalBounds().width / 2, Recipe24->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe24);
	CPotion* Recipe25 = new CPotion(CBlock::POTIONTYPE::HPLARGE);
	Recipe25->GetShape().setScale(0.43f, 0.43f);
	Recipe25->GetShape().setOrigin(Recipe25->GetShape().getGlobalBounds().width / 2, Recipe25->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe25);

	Bow* Recipe26 = new Bow(CBlock::BOWTYPE::PURPLE);
	Recipe26->GetShape().setScale(0.43f, 0.43f);
	Recipe26->GetShape().setOrigin(Recipe26->GetShape().getGlobalBounds().width / 2, Recipe26->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe26);
	Bow* Recipe27 = new Bow(CBlock::BOWTYPE::GOLDEN);
	Recipe27->GetShape().setScale(0.43f, 0.43f);
	Recipe27->GetShape().setOrigin(Recipe27->GetShape().getGlobalBounds().width / 2, Recipe27->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe27);

	Bow* Recipe28 = new Bow(CBlock::BOWTYPE::IRONGUN);
	Recipe28->GetShape().setScale(0.43f, 0.43f);
	Recipe28->GetShape().setOrigin(Recipe28->GetShape().getGlobalBounds().width / 2, Recipe28->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe28);
	Bow* Recipe29 = new Bow(CBlock::BOWTYPE::GOLDGUN);
	Recipe29->GetShape().setScale(0.43f, 0.43f);
	Recipe29->GetShape().setOrigin(Recipe29->GetShape().getGlobalBounds().width / 2, Recipe29->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe29);
	Bow* Recipe30 = new Bow(CBlock::BOWTYPE::PURPLEGUN);
	Recipe30->GetShape().setScale(0.43f, 0.43f);
	Recipe30->GetShape().setOrigin(Recipe30->GetShape().getGlobalBounds().width / 2, Recipe30->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe30);
	Bow* Recipe31 = new Bow(CBlock::BOWTYPE::GOLDENGUN);
	Recipe31->GetShape().setScale(0.43f, 0.43f);
	Recipe31->GetShape().setOrigin(Recipe31->GetShape().getGlobalBounds().width / 2, Recipe31->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe31);

	CProjectile* Recipe32 = new CProjectile(CBlock::PROJECTILETYPE::IRONBULLET);
	Recipe32->GetShape().setScale(0.43f, 0.43f);
	Recipe32->GetShape().setOrigin(Recipe32->GetShape().getGlobalBounds().width / 2, Recipe32->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe32);
	CProjectile* Recipe33 = new CProjectile(CBlock::PROJECTILETYPE::GOLDBULLET);
	Recipe33->GetShape().setScale(0.43f, 0.43f);
	Recipe33->GetShape().setOrigin(Recipe33->GetShape().getGlobalBounds().width / 2, Recipe33->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe33);
	CProjectile* Recipe34 = new CProjectile(CBlock::PROJECTILETYPE::PURPLEBULLET);
	Recipe34->GetShape().setScale(0.43f, 0.43f);
	Recipe34->GetShape().setOrigin(Recipe34->GetShape().getGlobalBounds().width / 2, Recipe34->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe34);
	CProjectile* Recipe35 = new CProjectile(CBlock::PROJECTILETYPE::GOLDENBULLET);
	Recipe35->GetShape().setScale(0.43f, 0.43f);
	Recipe35->GetShape().setOrigin(Recipe35->GetShape().getGlobalBounds().width / 2, Recipe35->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe35);


	CArmour* Recipe36 = new CArmour(CBlock::BLOCKTYPE::HELMET, CBlock::ARMOURTYPE::CACTUS);
	Recipe36->GetShape().setScale(0.43f, 0.43f);
	Recipe36->GetShape().setOrigin(Recipe36->GetShape().getGlobalBounds().width / 2, Recipe36->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe36);
	CArmour* Recipe37 = new CArmour(CBlock::BLOCKTYPE::CHESTPLATE, CBlock::ARMOURTYPE::CACTUS);
	Recipe37->GetShape().setScale(0.43f, 0.43f);
	Recipe37->GetShape().setOrigin(Recipe37->GetShape().getGlobalBounds().width / 2, Recipe37->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe37);
	CArmour* Recipe38 = new CArmour(CBlock::BLOCKTYPE::LEGGINGS, CBlock::ARMOURTYPE::CACTUS);
	Recipe38->GetShape().setScale(0.43f, 0.43f);
	Recipe38->GetShape().setOrigin(Recipe38->GetShape().getGlobalBounds().width / 2, Recipe38->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe38);

	CArmour* Recipe39 = new CArmour(CBlock::BLOCKTYPE::HELMET, CBlock::ARMOURTYPE::IRON);
	Recipe39->GetShape().setScale(0.43f, 0.43f);
	Recipe39->GetShape().setOrigin(Recipe39->GetShape().getGlobalBounds().width / 2, Recipe39->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe39);
	CArmour* Recipe40 = new CArmour(CBlock::BLOCKTYPE::CHESTPLATE, CBlock::ARMOURTYPE::IRON);
	Recipe40->GetShape().setScale(0.43f, 0.43f);
	Recipe40->GetShape().setOrigin(Recipe40->GetShape().getGlobalBounds().width / 2, Recipe40->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe40);
	CArmour* Recipe41 = new CArmour(CBlock::BLOCKTYPE::LEGGINGS, CBlock::ARMOURTYPE::IRON);
	Recipe41->GetShape().setScale(0.43f, 0.43f);
	Recipe41->GetShape().setOrigin(Recipe41->GetShape().getGlobalBounds().width / 2, Recipe41->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe41);

	CArmour* Recipe42 = new CArmour(CBlock::BLOCKTYPE::HELMET, CBlock::ARMOURTYPE::GOLD);
	Recipe42->GetShape().setScale(0.43f, 0.43f);
	Recipe42->GetShape().setOrigin(Recipe42->GetShape().getGlobalBounds().width / 2, Recipe42->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe42);
	CArmour* Recipe43 = new CArmour(CBlock::BLOCKTYPE::CHESTPLATE, CBlock::ARMOURTYPE::GOLD);
	Recipe43->GetShape().setScale(0.43f, 0.43f);
	Recipe43->GetShape().setOrigin(Recipe43->GetShape().getGlobalBounds().width / 2, Recipe43->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe43);
	CArmour* Recipe44 = new CArmour(CBlock::BLOCKTYPE::LEGGINGS, CBlock::ARMOURTYPE::GOLD);
	Recipe44->GetShape().setScale(0.43f, 0.43f);
	Recipe44->GetShape().setOrigin(Recipe44->GetShape().getGlobalBounds().width / 2, Recipe44->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe44);

	CArmour* Recipe45 = new CArmour(CBlock::BLOCKTYPE::HELMET, CBlock::ARMOURTYPE::DIAMOND);
	Recipe45->GetShape().setScale(0.43f, 0.43f);
	Recipe45->GetShape().setOrigin(Recipe45->GetShape().getGlobalBounds().width / 2, Recipe45->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe45);
	CArmour* Recipe46 = new CArmour(CBlock::BLOCKTYPE::CHESTPLATE, CBlock::ARMOURTYPE::DIAMOND);
	Recipe46->GetShape().setScale(0.43f, 0.43f);
	Recipe46->GetShape().setOrigin(Recipe46->GetShape().getGlobalBounds().width / 2, Recipe46->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe46);
	CArmour* Recipe47 = new CArmour(CBlock::BLOCKTYPE::LEGGINGS, CBlock::ARMOURTYPE::DIAMOND);
	Recipe47->GetShape().setScale(0.43f, 0.43f);
	Recipe47->GetShape().setOrigin(Recipe47->GetShape().getGlobalBounds().width / 2, Recipe47->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe47);

	CArmour* Recipe48 = new CArmour(CBlock::BLOCKTYPE::HELMET, CBlock::ARMOURTYPE::PURPLE);
	Recipe48->GetShape().setScale(0.43f, 0.43f);
	Recipe48->GetShape().setOrigin(Recipe48->GetShape().getGlobalBounds().width / 2, Recipe48->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe48);
	CArmour* Recipe49 = new CArmour(CBlock::BLOCKTYPE::CHESTPLATE, CBlock::ARMOURTYPE::PURPLE);
	Recipe49->GetShape().setScale(0.43f, 0.43f);
	Recipe49->GetShape().setOrigin(Recipe49->GetShape().getGlobalBounds().width / 2, Recipe49->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe49);
	CArmour* Recipe50 = new CArmour(CBlock::BLOCKTYPE::LEGGINGS, CBlock::ARMOURTYPE::PURPLE);
	Recipe50->GetShape().setScale(0.43f, 0.43f);
	Recipe50->GetShape().setOrigin(Recipe50->GetShape().getGlobalBounds().width / 2, Recipe50->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe50);

	CArmour* Recipe51 = new CArmour(CBlock::BLOCKTYPE::HELMET, CBlock::ARMOURTYPE::GOLDEN);
	Recipe51->GetShape().setScale(0.43f, 0.43f);
	Recipe51->GetShape().setOrigin(Recipe51->GetShape().getGlobalBounds().width / 2, Recipe51->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe51); 
	CArmour* Recipe52 = new CArmour(CBlock::BLOCKTYPE::CHESTPLATE, CBlock::ARMOURTYPE::GOLDEN);
	Recipe52->GetShape().setScale(0.43f, 0.43f);
	Recipe52->GetShape().setOrigin(Recipe52->GetShape().getGlobalBounds().width / 2, Recipe52->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe52);
	CArmour* Recipe53 = new CArmour(CBlock::BLOCKTYPE::LEGGINGS, CBlock::ARMOURTYPE::GOLDEN);
	Recipe53->GetShape().setScale(0.43f, 0.43f);
	Recipe53->GetShape().setOrigin(Recipe53->GetShape().getGlobalBounds().width / 2, Recipe53->GetShape().getGlobalBounds().height / 2);
	m_CraftList.push_back(*Recipe53);


	Recipe1 = nullptr;
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
	Recipe12 = nullptr;
	Recipe13 = nullptr;
	Recipe14 = nullptr;
	Recipe15 = nullptr;
	Recipe16 = nullptr;
	Recipe17 = nullptr;
	Recipe18 = nullptr;
	Recipe19 = nullptr;
	Recipe20 = nullptr;
	Recipe21 = nullptr;
	Recipe22 = nullptr;
	Recipe23 = nullptr;
	Recipe24 = nullptr;
	Recipe25 = nullptr;
	Recipe26 = nullptr;
	Recipe27 = nullptr;
	Recipe28 = nullptr;
	Recipe29 = nullptr;
	Recipe30 = nullptr;
	Recipe31 = nullptr;
	Recipe32 = nullptr;
	Recipe33 = nullptr;
	Recipe34 = nullptr;
	Recipe35 = nullptr;
	Recipe36 = nullptr;
	Recipe37 = nullptr;
	Recipe38 = nullptr;
	Recipe39 = nullptr;
	Recipe41 = nullptr;
	Recipe42 = nullptr;
	Recipe43 = nullptr;
	Recipe44 = nullptr;
	Recipe45 = nullptr;
	Recipe46 = nullptr;
	Recipe47 = nullptr;
	Recipe48 = nullptr;
	Recipe49 = nullptr;
	Recipe50 = nullptr;
	Recipe51 = nullptr;
	Recipe52 = nullptr;
	Recipe53 = nullptr;
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

	// Row 6
	for (int i = 50; i < 60; i++)
	{
		_renderWindow->mapCoordsToPixel(m_ChestSlots[i].getPosition(), _uiView);
		m_ChestSlots[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 900 + ((i - 50) * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + 65 + 65);

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

	// Row 6
	for (int i = 50; i < 60; i++)
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

void GUI::UtilityUI(sf::RenderWindow* _renderWindow, CPlayer* _player, sf::View& _uiView, sf::View& _worldView, sf::Event& _event, CTextureMaster* _textureMaster, std::list<CChest>& _chests)
{
	_renderWindow->setView(_uiView);

	sf::Vector2f MousePos = _renderWindow->mapPixelToCoords((sf::Mouse::getPosition(*_renderWindow)), _uiView);

	if (m_FirstEmpyChestSlotTimer.getElapsedTime().asSeconds() >= 0.1f)
	{
		FindFirstEmptyChestSlot(_chests);
		m_FirstEmpyChestSlotTimer.restart();
	}

	// Row 1
	for (int i = 50; i < 60; i++)
	{
		_renderWindow->mapCoordsToPixel(m_InventorySlotMap[i].getPosition(), _uiView);
		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + (60 * 31), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 265 + ((i-50) * 65) + 65);

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


		if (i == 50)
		{
			m_InventorySlotMap[i].setTexture(*_textureMaster->m_EquipmentSpacerHelmet, true);
		}
		else if (i == 51)
		{
			m_InventorySlotMap[i].setTexture(*_textureMaster->m_EquipmentSpacerChestPlate, true);
		}
		else if (i == 52)
		{
			m_InventorySlotMap[i].setTexture(*_textureMaster->m_EquipmentSpacerLeggings, true);
		}
		else if (i == 57)
		{
			m_InventorySlotMap[i].setTexture(*_textureMaster->m_EquipmentSpacerTourch, true);
		}
		else if (i == 58)
		{
			m_InventorySlotMap[i].setTexture(*_textureMaster->m_EquipmentSpacerArrow, true);
		}
		else if (i == 59)
		{
			m_InventorySlotMap[i].setTexture(*_textureMaster->m_EquipmentSpacerBullet, true);
		}
		else
		{
			m_InventorySlotMap[i].setTexture(*_textureMaster->m_EquipmentSpacer, true);
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
}

void GUI::InitUtilityUI(CPlayer* _player, sf::RenderWindow* _renderWindow, CTextureMaster* _textureMaster)
{
	// Row 1
	for (int i = 50; i < 60; i++)
	{
		sf::Sprite test = sf::Sprite();

		if (i == 50)
		{
			test.setTexture(*_textureMaster->m_EquipmentSpacerHelmet, true);
		}
		else if (i == 51)
		{
			test.setTexture(*_textureMaster->m_EquipmentSpacerChestPlate, true);
		}
		else if (i == 52)
		{
			test.setTexture(*_textureMaster->m_EquipmentSpacerLeggings, true);
		}
		else if (i == 58)
		{
			test.setTexture(*_textureMaster->m_EquipmentSpacerTourch, true);
		}
		else if (i == 59)
		{
			test.setTexture(*_textureMaster->m_EquipmentSpacerArrow, true);
		}
		else if (i == 60)
		{
			test.setTexture(*_textureMaster->m_EquipmentSpacerBullet, true);
		}
		else
		{
			test.setTexture(*_textureMaster->m_EquipmentSpacer, true);
		}
		
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
		m_InventoryItemStackCounters.insert({ i, stackcounter });
		m_InventoryItemStackCounters[i].setOrigin(m_InventoryItemStackCounters[i].getGlobalBounds().width / 2, m_InventoryItemStackCounters[i].getGlobalBounds().height / 2);

		_player->m_InventoryStackValues.emplace(i, 0);
	}

	for (int i = 50; i < 60; i++)
	{
		std::cout << i << std::endl;
		m_InventoryItemStackCounters[i];
	}
}

void GUI::InitArmourOnPlayer(CPlayer* _player)
{
	_player->SetArmour(_player->GetArmour() + _player->m_InventoryMap[50].m_ArmourValue);
	std::cout << "Armour : " << _player->GetArmour() << std::endl;
	_player->m_HelmetShape.setTexture(_player->m_InventoryMap[50].m_ArmourHeadLeft, true);

	_player->SetArmour(_player->GetArmour() + _player->m_InventoryMap[51].m_ArmourValue);
	std::cout << "Armour : " << _player->GetArmour() << std::endl;
	_player->m_ChesPlateShape.setTexture(_player->m_InventoryMap[51].m_ArmourChestLeft, true);

	_player->SetArmour(_player->GetArmour() + _player->m_InventoryMap[52].m_ArmourValue);
	std::cout << "Armour : " << _player->GetArmour() << std::endl;
	_player->m_LegsShape.setTexture(_player->m_InventoryMap[52].m_ArmourLegsLeft, true);
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

void GUI::StatusEffectUI(sf::RenderWindow* _renderWindow, CPlayer* _player)
{
	if (10 - _player->m_HPPotionTimer.getElapsedTime().asSeconds() > 0)
	{
		m_StatusString = "Potion Cooldown : " + ToString(10 - _player->m_HPPotionTimer.getElapsedTime().asSeconds());
	}
	else
	{
		m_StatusString = "";
	}
	
	m_StatusText.setString(m_StatusString);
	m_StatusText.setPosition(_renderWindow->getView().getCenter().x + (_renderWindow->getView().getSize().x / 2) - 100, _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 275);
	_renderWindow->draw(m_StatusText);
}

void GUI::InitStatusEffectUI(CPlayer* _player)
{
	// Health Text
	m_StatusText = sf::Text();
	m_StatusString = "Potion Cooldown : " + ToString(_player->m_HPPotionTimer.getElapsedTime().asSeconds());
	m_StatusText.setString(m_StatusString);
	m_StatusText.setFont(m_Font);
	m_StatusText.setFillColor(sf::Color::White);
	m_StatusText.setOutlineThickness(0.75f);
	m_StatusText.setOutlineColor(sf::Color::Black);
	m_StatusText.setOrigin(m_StatusText.getGlobalBounds().width / 2, m_StatusText.getGlobalBounds().height / 2);
	m_StatusText.setCharacterSize(20);
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

