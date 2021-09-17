#include "GUI.h"


GUI::GUI()
{
	m_Font = sf::Font();
	m_Font.loadFromFile("Fonts/ANDYB.TTF");
}

GUI::~GUI()
{
	m_InventoryItemStackCounters.clear();
	m_InventorySlotMap.clear();
	
	delete m_BlackTexture;
	m_BlackTexture = nullptr;
	delete m_miniMap;
	m_miniMap = nullptr;
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

void GUI::MiniMapUI(sf::RenderWindow* _renderWindow, std::list<CBlock>& _chunk, std::list<sf::RectangleShape>& _skyChunk, CPlayer* _player, sf::Shader* _shader, sf::Shader* _shaderBlack)
{
	/*_shaderBlack->setUniform("hasTexture", true);
	_shaderBlack->setUniform("lightPos", 0);*/

	// Assigning Render Texture View and Zooming
	sf::View MiniMapView = sf::View(_player->GetShape().getPosition(), sf::Vector2f(200.0f, 200.0f));
	MiniMapView.zoom(40);
	m_miniMap->setView(MiniMapView);

	m_MiniMapWorldBackGround.setPosition(_renderWindow->getView().getCenter());
	m_miniMap->draw(m_MiniMapWorldBackGround);

	// Draw All Blocks In Radius 4.0f
	std::list<CBlock>::iterator it;
	// Blocks
	for (it = _chunk.begin(); it != _chunk.end(); it++)
	{
		float Mag1 = sqrt(((it->GetShape().getPosition().x - _player->GetShape().getPosition().x) * (it->GetShape().getPosition().x - _player->GetShape().getPosition().x)) + ((it->GetShape().getPosition().y - _player->GetShape().getPosition().y) * (it->GetShape().getPosition().y - _player->GetShape().getPosition().y)));
		if (Mag1 < 1920 * 2.9f)
		{
			//m_miniMap->draw(it->GetShape());
			if (it->m_bHasBeenSpotted)
			{
				m_miniMap->draw(it->GetShape());
			}
			else if (Mag1 > 500)
			{
				m_miniMap->draw(it->GetShape());
			}
			else if (Mag1 <= 500)
			{
				m_miniMap->draw(it->GetShape());
			}
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
	_renderWindow->draw(m_MiniMapShape, _shader);
}

void GUI::InitMiniMap(sf::RenderWindow* _renderWindow, CTextureMaster* _textureMaster)
{
	m_BlackTexture = new sf::Texture();
	if (!m_BlackTexture->loadFromFile("Images/Black.png"))
	{
		std::cout << "Black Texture Failed!" << std::endl;
	}
	
	

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

void GUI::InventoryUI(sf::RenderWindow* _renderWindow, CPlayer* _player, sf::View& _uiView, sf::View& _worldView, sf::Event& _event, CTextureMaster* _textureMaster)
{
	_renderWindow->setView(_uiView);

	sf::Vector2f MousePos = _renderWindow->mapPixelToCoords((sf::Mouse::getPosition(*_renderWindow)), _uiView);

	// Row 1
	for (int i = 0; i < 10; i++)
	{
		_renderWindow->mapCoordsToPixel(m_InventorySlotMap[i].getPosition(), _uiView);
		m_InventorySlotMap[i].setPosition(_renderWindow->getView().getCenter().x - (_renderWindow->getView().getSize().x / 2) + 60 + (i * 65), _renderWindow->getView().getCenter().y - (_renderWindow->getView().getSize().y / 2) + 70);

		LetGoOfItemInInventory(_renderWindow, _uiView, _worldView, _event, _player, i);

		m_MousePointer.setPosition(MousePos);

		//ClickedItemInInventory(_event, _player, i);
		if (_player->bInventoryOpen() && sf::Mouse::isButtonPressed(sf::Mouse::Left) && _player->m_InventoryMap[i].GetShape().getGlobalBounds().contains(m_MousePointer.getPosition()))
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(MousePos.x - 11, MousePos.y - 11);
		}
		else
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(m_InventorySlotMap[i].getPosition().x, m_InventorySlotMap[i].getPosition().y);
			
		}
		//std::cout << _player->m_InventoryMap[0].GetPosition().x << _player->m_InventoryMap[0].GetPosition().y << std::endl;

		
		m_InventorySlotMap[i].setTexture(*_textureMaster->m_ItemSpacer);

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
		if (_player->bInventoryOpen() && sf::Mouse::isButtonPressed(sf::Mouse::Left) && _player->m_InventoryMap[i].GetShape().getGlobalBounds().contains(m_MousePointer.getPosition()))
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(MousePos.x - 11, MousePos.y - 11);
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
		if (_player->bInventoryOpen() && sf::Mouse::isButtonPressed(sf::Mouse::Left) && _player->m_InventoryMap[i].GetShape().getGlobalBounds().contains(m_MousePointer.getPosition()))
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(MousePos.x - 11, MousePos.y - 11);
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
		if (_player->bInventoryOpen() && sf::Mouse::isButtonPressed(sf::Mouse::Left) && _player->m_InventoryMap[i].GetShape().getGlobalBounds().contains(m_MousePointer.getPosition()))
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(MousePos.x - 11, MousePos.y - 11);
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
		if (_player->bInventoryOpen() && sf::Mouse::isButtonPressed(sf::Mouse::Left) && _player->m_InventoryMap[i].GetShape().getGlobalBounds().contains(m_MousePointer.getPosition()))
		{
			_renderWindow->mapCoordsToPixel(_player->m_InventoryMap[i].GetPosition(), _uiView);
			_player->m_InventoryMap[i].SetPosition(MousePos.x - 11, MousePos.y - 11);
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

void GUI::InitHotBarScrolling(sf::Event& _event, CPlayer* _player)
{
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

void GUI::LetGoOfItemInInventory(sf::RenderWindow* _renderWindow, sf::View& _uiView, sf::View& _worldView, sf::Event& _event, CPlayer* _player, int _iterator)
{

	_renderWindow->setView(_uiView);
	if (m_MousePointer.getGlobalBounds().intersects(_player->m_InventoryMap[_iterator].GetShape().getGlobalBounds()) && _player->bInventoryOpen())
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

					// Moved Item Into Currently Selected Slot?
					for (std::map<int, CBlock>::iterator iit = _player->m_InventoryMap.begin(); iit != _player->m_InventoryMap.end(); iit++)
					{
						if (_player->m_CurrentItemIndex == iit->first && iit->second.m_Type == iit->second.PICKAXE && iit->second.m_bIsItemAndSelected == false)
						{
							std::cout << "Pickaxe Selected!" << std::endl;
							iit->second.m_bIsItemAndSelected = true;
						}
					}
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
	if (_player->bInventoryOpen() && _event.type == sf::Event::MouseButtonPressed && _player->m_InventoryMap[_iterator].GetShape().getGlobalBounds().intersects(m_MousePointer.getGlobalBounds()) && _player->bInventoryOpen())
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

void GUI::CraftingUI(sf::RenderWindow* _renderWindow, CPlayer* _player, CTextureMaster* _textureMaster)
{
	if (_player->bInventoryOpen())
	{
		sf::Color color = sf::Color();
		
		// Row 1
		for (int i = 50; i < 55; i++)
		{
			sf::Sprite test = sf::Sprite();
			test.setTexture(*_textureMaster->m_ItemSpacer, true);
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
	if (_player->bInventoryOpen())
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

	if (_player->bInventoryOpen())
	{
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
