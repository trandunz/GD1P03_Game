#include "WorldManager.h"

CWorldManager::CWorldManager(sf::RenderWindow* _renderWindow, CPlayer* _player, b2World& _world, GUI* _gui, sf::Shader* _shader, sf::Shader* _surfaceShader, sf::Shader* _tourchShader)
{
    m_RenderWindow = _renderWindow;
    m_Player = _player;
    m_World = &_world;
    m_GUI = _gui;
    m_Block = nullptr;
    m_Shader = _shader;
    m_SurfaceShader = _surfaceShader;
    m_TourchShader = _tourchShader;

    _gui->m_CurrentSeed = time(NULL);
    //_gui->m_CurrentSeed = 1631416783;
    srand(_gui->m_CurrentSeed);
    std::cout << _gui->m_CurrentSeed << std::endl;
    m_Chunk = std::list<CBlock>{};
}

CWorldManager::~CWorldManager()
{
    OutPutWorldToFiles();

    m_Chunk.clear();
    m_SkyChunk.clear();
    m_Chests.clear();
    m_Doors.clear();
    m_Furnaces.clear();

    m_TourchShader = nullptr;
    m_SurfaceShader = nullptr;
    m_Shader = nullptr;
    m_Block = nullptr;
    m_GUI = nullptr;
    m_Player = nullptr;
    m_RenderWindow = nullptr;
    m_World = nullptr;
}

void CWorldManager::Start(CTextureMaster* _textureMaster)
{
    // World BackGround (Surface)
    m_BGPlainsSurface = sf::Sprite();
    m_BGPlainsSurface.setTexture(*_textureMaster->m_Sky, true);
    m_BGPlainsSurface.setOrigin(m_BGPlainsSurface.getGlobalBounds().width / 2, m_BGPlainsSurface.getGlobalBounds().height / 2);
    m_BGPlainsSurface.setScale(4.7, 4.7);
    m_BGPlainsSurface.setColor(sf::Color::White);

    // (Under Ground)
    m_BGPlainsUnderGr = sf::Sprite();
    m_BGPlainsUnderGr.setTexture(*_textureMaster->m_UnderGroundPlains, true);
    m_BGPlainsUnderGr.setTextureRect(sf::IntRect(0, 0, 80000, 40000));
    m_BGPlainsUnderGr.setOrigin(m_BGPlainsUnderGr.getGlobalBounds().width / 2, m_BGPlainsUnderGr.getGlobalBounds().height / 2);
    m_BGPlainsUnderGr.setColor(sf::Color::Transparent);
    m_BGPlainsUnderGr.setPosition(0, 0);

    // World
    CreateSkyChunk(_textureMaster);
    CreateClouds(_textureMaster);
    //CreateSurfaceLayerPart1(_textureMaster);
    //CreateSurfaceLayerPart2(_textureMaster);
    CreateWorldBoundary(_textureMaster);

    //
    // Reading From File??
    //
    /*std::string line;
    std::ifstream myfile("example.txt");
    if (myfile.is_open())
    {
        std::getline(myfile, line);

        std::list<CBlock>::iterator it = m_Chunk.begin();
        while (it != m_Chunk.end())
        {
            it->m_
        }
    }*/

    // Noise
    CreateNoiseWorld(_textureMaster);
}

void CWorldManager::Update(sf::Vector2f _mousePos)
{
    m_GUI->m_MousePos.setPosition(m_SkyChunk.front().getPosition());

    float Mag1 = 0;
    for (CBlock& block : m_Chunk)
    {
        Mag1 = sqrt(((block.GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x) * (block.GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x)) + ((block.GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y) * (block.GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y)));

        if (Mag1 < 1920 * 1.f)
        {
            block.Update();
            if (block.GetShape().getGlobalBounds().contains(_mousePos))
            {
                m_GUI->m_MousePos.setPosition(block.GetPosition());
            }
        }
    }

    for (CDoor& door : m_Doors)
    {
        if (door.GetShape().getGlobalBounds().contains(_mousePos))
        {
            m_GUI->m_MousePos.setPosition(door.GetShape().getPosition());
        }
    }

    for (CChest& chest : m_Chests)
    {
        if (chest.GetShape().getGlobalBounds().contains(_mousePos))
        {
            m_GUI->m_MousePos.setPosition(chest.GetShape().getPosition());
        }
    }

    for (CFurnace& furnace : m_Furnaces)
    {
        Mag1 = sqrt(((m_RenderWindow->getView().getCenter().x - furnace.GetShape().getPosition().x) * (m_RenderWindow->getView().getCenter().x - furnace.GetShape().getPosition().x)) + ((m_RenderWindow->getView().getCenter().y - furnace.GetShape().getPosition().y) * (m_RenderWindow->getView().getCenter().y - furnace.GetShape().getPosition().y)));
        
        if (Mag1 < 200 && m_Player->m_bInventoryOpen)
        {
            m_GUI->m_bCanSmelt = true;
            break;
        }
        else
        {
            if (furnace.GetShape().getGlobalBounds().contains(_mousePos))
            {
                m_GUI->m_MousePos.setPosition(furnace.GetShape().getPosition());
            }

            m_GUI->m_bCanSmelt = false;
        }
    }
}

void CWorldManager::Render(sf::Shader* _defaultShader)
{
    // World BackGround
    if (m_Player != nullptr)
    {
        m_BGPlainsSurface.setPosition(m_Player->GetShape().getPosition());
        if (m_Player->GetShape().getPosition().y > 1500 && m_Player->GetShape().getPosition().y < 1550)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 255));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 0));
        }
        else if (m_Player->GetShape().getPosition().y > 1550 && m_Player->GetShape().getPosition().y < 1600)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 200));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 50));
        }
        else if (m_Player->GetShape().getPosition().y > 1600 && m_Player->GetShape().getPosition().y < 1650)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 150));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 100));
        }
        else if (m_Player->GetShape().getPosition().y > 1650 && m_Player->GetShape().getPosition().y < 1700)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 100));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 150));
        }
        else if (m_Player->GetShape().getPosition().y > 1700 && m_Player->GetShape().getPosition().y < 1750)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 50));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 200));
        }
        else if (m_Player->GetShape().getPosition().y > 1750)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 0));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 255));
        }
        else if (m_RenderWindow->getView().getCenter().y > 1500)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 0));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 255));
        }
        else
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 255));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 0));
        }
    }
    else if (m_RenderWindow->getView().getCenter().y > 1500)
    {
        m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 0));
        m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 255));
    }
    else
    {
        m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 255));
        m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 0));
    }
    
    m_RenderWindow->draw(m_BGPlainsSurface, _defaultShader);
    m_RenderWindow->draw(m_BGPlainsUnderGr, _defaultShader);

    // Surface Shader
    m_SurfaceShader->setUniform("hasTexture", true);
    m_SurfaceShader->setUniform("lightPos", sf::Vector2f(0, -500));

    if (m_Player != nullptr)
    {
        // Player Light
        m_Shader->setUniform("hasTexture", true);
        m_Shader->setUniform("lightPos", m_Player->GetShape().getPosition());
    }
    else
    {
        // Player Light
        m_Shader->setUniform("hasTexture", true);
        m_Shader->setUniform("lightPos", -1000);
    }

    // Block It
    std::list<CBlock>::iterator it;
    // Draw Blocks In Range (With Player Shader)
    for (it = m_Chunk.begin(); it != m_Chunk.end(); it++)
    {
        float Mag1 = sqrt(((it->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x) * (it->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x)) + ((it->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y) * (it->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y)));

        if (Mag1 < 1920 * 1.8f && it->GetShape().getPosition().y >= 1300)
        {
            if (bIsBlockInRangeOfLightSource(it))
            {

                m_RenderWindow->draw(it->GetShape(), m_TourchShader);
            }
            else
            {
                if (it->GetShape().getPosition().y > 1400)
                {
                    m_RenderWindow->draw(it->GetShape(), m_Shader);
                }
                else
                {
                    if (Mag1 <= 200 && m_RenderWindow->getView().getCenter().y >= 1200)
                    {
                        m_RenderWindow->draw(it->GetShape(), m_Shader);
                    }
                    else
                    {
                        m_RenderWindow->draw(it->GetShape(), m_SurfaceShader);
                    }
                }
            }
        }
        else if (Mag1 < 1920 * 1.8f && it->GetShape().getPosition().y <= 1400 && it->GetShape().getPosition().y >= 400)
        {
            if (m_RenderWindow->getView().getCenter().y >= 1200)
            {
                if (Mag1 <= 200)
                {
                    m_RenderWindow->draw(it->GetShape(), m_Shader);
                }
                else
                {
                    m_RenderWindow->draw(it->GetShape(), m_SurfaceShader);
                }
            }
            else
            {
                if (bIsBlockInRangeOfLightSource(it) && it->GetShape().getPosition().y >= 1300)
                {
                    m_RenderWindow->draw(it->GetShape(), m_TourchShader);
                }
                else
                {
                    m_RenderWindow->draw(it->GetShape(), m_SurfaceShader);
                }
            }

        }
        else if (Mag1 < 1920 * 1.8f && it->GetShape().getPosition().y < 400 && it->GetShape().getPosition().y > -400)
        {
            m_RenderWindow->draw(it->GetShape(), m_SurfaceShader);
        }
        else if (Mag1 < 1920 * 1.8f && it->GetShape().getPosition().y <= -400)
        {
            m_RenderWindow->draw(it->GetShape(), _defaultShader);
        }
        else
        {
            continue;
        }
    }

    // Doors E.t.c
    for (CDoor& door : m_Doors)
    {
        door.Render(_defaultShader);
    }
    for (CChest& chest : m_Chests)
    {
        chest.Render(_defaultShader);
    }
    for (CFurnace& furnace : m_Furnaces)
    {
        furnace.Render(_defaultShader);
    }
}

void CWorldManager::CreateSkyChunk(CTextureMaster* _textureMaster)
{
    m_SkyChunk.clear();

    // Round To Nearest 100 / Block Size
    int tempx = 100 * std::roundf(m_Player->m_MousePos.x / 100.0);
    int tempy = 100 * std::roundf(m_Player->m_MousePos.y / 100.0);

    sf::RectangleShape sky = sf::RectangleShape();
    sky.setSize(sf::Vector2f(100, 100));
    sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
    sky.setPosition(sf::Vector2f(tempx + 10, tempy));
    m_SkyChunk.push_front(sky);
}

void CWorldManager::CreateWorldBoundary(CTextureMaster* _textureMaster)
{
    GlobalMutex.lock();
    for (int i = -90; i > -69770 / 1.9f; i -= 100)
    {
        // Grass
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, i, -6000, false, CBlock::BLOCKTYPE::OBSIDIAN);

        //m_Block->SetSize(100, 100);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;
    }
    for (int i = -90; i > -69770 / 1.9f; i -= 100)
    {
        // Grass
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, i, 11300, false, CBlock::BLOCKTYPE::OBSIDIAN);

        //m_Block->SetSize(100, 100);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;
    }
    for (int j = 500 - 6500; j < 21400 / 1.9f; j += 100)
    {

        // Grass
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, -69770 / 1.9f, j, false, CBlock::BLOCKTYPE::OBSIDIAN);

        //m_Block->SetSize(100, 100);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;
    }

    for (int i = 10; i < 69770 / 1.9f; i += 100)
    {
        // Grass
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, i, -6000, false, CBlock::BLOCKTYPE::OBSIDIAN);

        //m_Block->SetSize(100, 100);
        m_Chunk.push_back(*m_Block);
        m_Block->m_ArrayIndex = i;
        m_Block = nullptr;
    }
    for (int i = 10; i < 69770 / 1.9f; i += 100)
    {
        // Grass
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, i, 11300, false, CBlock::BLOCKTYPE::OBSIDIAN);

        //m_Block->SetSize(100, 100);
        m_Chunk.push_back(*m_Block);
        m_Block->m_ArrayIndex = i;
        m_Block = nullptr;
    }
    for (int j = 500 - 6500; j < 21400 / 1.9f; j += 100)
    {
        // Grass
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, +69570 / 1.9f, j, false, CBlock::BLOCKTYPE::OBSIDIAN);

        //m_Block->SetSize(100, 100);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;
    }
    GlobalMutex.unlock();
}

void CWorldManager::UpdateWorldTexture(sf::View& _view)
{
    // Assigning Render Texture View and Zooming
    m_WorldTexture.setView(_view);

    // Draw All Blocks In Radius 1.8f

    // Blocks
    for (CBlock& block : m_Chunk)
    {
        float Mag1 = sqrt(((block.GetShape().getPosition().x - m_Player->GetShape().getPosition().x) * (block.GetShape().getPosition().x - m_Player->GetShape().getPosition().x)) + ((block.GetShape().getPosition().y - m_Player->GetShape().getPosition().y) * (block.GetShape().getPosition().y - m_Player->GetShape().getPosition().y)));
        if (Mag1 < m_RenderWindow->getSize().x * 1.80f)
        {
            m_WorldTexture.draw(block.GetShape());
        }
    }

    // Doors
    for (CDoor& door : m_Doors)
    {
        float Mag1 = sqrt(((door.GetShape().getPosition().x - m_Player->GetShape().getPosition().x) * (door.GetShape().getPosition().x - m_Player->GetShape().getPosition().x)) + ((door.GetShape().getPosition().y - m_Player->GetShape().getPosition().y) * (door.GetShape().getPosition().y - m_Player->GetShape().getPosition().y)));
        if (Mag1 < m_RenderWindow->getSize().x * 1.80f)
        {
            m_WorldTexture.draw(door.GetShape());
        }
    }

    // Update RendTexture
    m_WorldTexture.display();

    // Assigning Sprite Texture And Drawing (MiniMap)
    m_WorldSprite = sf::RectangleShape();
    m_WorldSprite.setTexture(&m_WorldTexture.getTexture());
    m_WorldSprite.setSize(sf::Vector2f(200, 200));
    //test.setFillColor(sf::Color::White);
    m_WorldSprite.setOrigin(m_WorldSprite.getGlobalBounds().width / 2, m_WorldSprite.getGlobalBounds().height / 2);
    m_RenderWindow->mapCoordsToPixel(m_WorldSprite.getPosition());

    // Draw Shape With Applied RenderText To Main Window
    m_RenderWindow->draw(m_WorldSprite);
}

void CWorldManager::CreateNoiseWorld(CTextureMaster* _textureMaster)
{
    generateNoise();

    for (int y = 0; y < noiseHeight; y++)
        for (int x = -366; x < noiseWidth; x++)
        {
            double xyValue = x * xPeriod / noiseWidth + y * yPeriod / noiseHeight + turbPower * turbulence(x, y, turbSize) / 256.0;
            double sineValue = 256 * fabs(sin(xyValue * 3.14159));
            if (sineValue <= 50)
            {

            }
            else if (sineValue > 80 && sineValue <=  85 && y > 10)
            {
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_IronOre, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::IRONORE);
                //m_Block->SetSize(100, 100);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            else if (sineValue > 75 && sineValue <= 80 && y > 50)
            {
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_GoldOre, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::GOLDORE);
                //m_Block->SetSize(100, 100);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            else if (sineValue > 70 && sineValue <= 75 && y > 80)
            {
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_DiamondOre, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::DIAMONDORE);
                //m_Block->SetSize(100, 100);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            else
            {
                if (y > 12 || y > 11 || y > 10)
                {
                    if (rand() % 10 == 0 && y < 30)
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Dirt, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::DIRT);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = x + y;
                        m_Block = nullptr;
                    }
                    else if (rand() % 15 == 0 && y < 50 && y >= 30)
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Dirt, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::DIRT);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = x + y;
                        m_Block = nullptr;
                    }
                    else if (rand() % 30 == 0 && y < 80 && y >= 50)
                    {
                        if (rand() % 2 == 0)
                        {
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_MossyBrick, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::MOSSYBRICK);
                            //m_Block->SetSize(100, 100);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                        else
                        {
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Dirt, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::DIRT);
                            //m_Block->SetSize(100, 100);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                        
                    }
                    else
                    {
                        if (rand() % 30 == 0)
                        {
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_MossyBrick, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::MOSSYBRICK);
                            //m_Block->SetSize(100, 100);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                        else
                        {
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::STONE);
                            //m_Block->SetSize(100, 100);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                        
                    } 
                    
                }
                else if (y == 0)
                {
                    // Trees
                    if (rand() % 6 <= 3 && x % 6 == 0)
                    {
                        // Trunk
                        if (true)
                        {
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, x * 100 + 10, y * 100 -100, true, CBlock::BLOCKTYPE::WOOD);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, x * 100 + 10, y * 100 - 200, true, CBlock::BLOCKTYPE::WOOD);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, x * 100 + 10, y * 100 - 300, true, CBlock::BLOCKTYPE::WOOD);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, x * 100 + 10, y * 100 - 400, true, CBlock::BLOCKTYPE::WOOD);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, x * 100 + 10, y * 100 - 500, true, CBlock::BLOCKTYPE::WOOD);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, x * 100 + 10, y * 100 - 600, true, CBlock::BLOCKTYPE::WOOD);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                        // Leaves
                        if (true)
                        {
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 + 10, y * 100 - 700, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 + 10, y * 100 - 800, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 + 10, y * 100 - 900, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 + 10, y * 100 - 1000, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;

                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 + 110, y * 100 - 700, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 + 210, y * 100 - 700, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);                                                             
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;                                                                       
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 -90, y * 100 - 700, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);                                                             
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;                                                                       
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 - -190, y * 100 - 700, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);                                                             
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;                                                                       
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 + 310, y * 100 - 700, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);                                                             
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;                                                                       
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 - -290, y * 100 - 700, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);                                                             
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;                                                                       
                                                                                                                     
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 + 110, y * 100 - 800, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);                                                             
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;                                                                       
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 + 210, y * 100 - 800, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);                                                             
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;                                                                       
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 - 90, y * 100 - 800, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);                                                             
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;                                                                       
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 - 190, y * 100 - 800, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);                                                             
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;                                                                       
                                                                                                                     
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 + 110, y * 100 - 900, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);                                                             
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;                                                                       
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 + 210, y * 100 - 900, true, CBlock::BLOCKTYPE::LEAVES);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                    }


                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Grass, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::GRASS);
                    //m_Block->SetSize(100, 100);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = x + y;
                    m_Block = nullptr;
                }
                else
                {
                if (rand() % 2 == 0 && y > 4)
                {
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::STONE);
                    //m_Block->SetSize(100, 100);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = x + y;
                    m_Block = nullptr;
                }
                else
                {
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Dirt, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::DIRT);
                    //m_Block->SetSize(100, 100);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = x + y;
                    m_Block = nullptr;
                }
                   
                }
            }
            
            
        }
}

bool CWorldManager::CleanUpBlocks()
{
    while (m_Chunk.size() > 0)
    {
        m_Chunk.pop_front();
    }
    return true;
}

bool CWorldManager::CleanUpSky()
{
    while (m_SkyChunk.size() > 0)
    {
        m_SkyChunk.pop_front();
    }
    return true;
}

void CWorldManager::CreateClouds(CTextureMaster* _textureMaster)
{
    generateNoise();

    for (int y = -42; y < noiseHeight; y++)
        for (int x = -noiseWidth; x < noiseWidth; x++)
        {
            sf::Uint8 L = 192 + sf::Uint8(turbulence(x, y, 15)) / 4;

            if (y * 100 < -3100)
            {
                if (L < 205)
                {
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Cloud, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::CLOUD);
                    //m_Block->SetSize(100, 100);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = (x + y) * 1337;
                    m_Block = nullptr;

                    if (rand() % 3 == 0 && x % 19 == 0 && y == -42)
                    {
                        m_Chest = new CChest(m_RenderWindow, *m_World, 50.0f, x * 100 + 10, y * 100 - 100);
                        m_Chests.push_back(*m_Chest);
                        m_Chest->m_ArrayIndex = (x + y) * 1337;
                        m_Chest = nullptr;
                    }
                }
                else if (L < 209)
                {
                    if (rand() % 3 == 0 && x % 19 == 0 && y == -42)
                    {
                        m_Chest = new CChest(m_RenderWindow, *m_World, 50.0f, x * 100 + 10, y * 100 - 100);
                        m_Chests.push_back(*m_Chest);
                        m_Chest->m_ArrayIndex = (x + y) * 1337;
                        m_Chest = nullptr;
                    }
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_CloudDark, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::CLOUD);
                    //m_Block->SetSize(100, 100);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = (x + y) * 1337;
                    m_Block = nullptr;
                }
            }
            

        }
    
}

bool CWorldManager::bIsBlockInRangeOfLightSource(std::list<CBlock>::iterator _it)
{
    float Mag1 = 0;
    for (std::list<CFurnace>::iterator fit = m_Furnaces.begin(); fit != m_Furnaces.end(); fit++)
    {
        Mag1 = sqrt(((_it->GetShape().getPosition().x - fit->GetShape().getPosition().x) * (_it->GetShape().getPosition().x - fit->GetShape().getPosition().x)) + ((_it->GetShape().getPosition().y - fit->GetShape().getPosition().y) * (_it->GetShape().getPosition().y - fit->GetShape().getPosition().y)));
        
        if (Mag1 < 600)
        {
            // Surface Shader
            if (m_TourchShader != nullptr) 
            {
                m_TourchShader->setUniform("hasTexture", true);
                m_TourchShader->setUniform("lightPos", fit->GetShape().getPosition());

                return true;
            }
        }
    }

    return false;
}

void CWorldManager::InitPointer(CPlayer* _player)
{
    std::cout << "Player Initialized On World" << std::endl;
    m_Player = _player;
    m_BGPlainsUnderGr.setColor(sf::Color::Transparent);
    m_BGPlainsUnderGr.setPosition(0, 0);
    m_BGPlainsSurface.setColor(sf::Color::White);
}

void CWorldManager::OutPutWorldToFiles(std::string _xPositions, std::string _yPositions)
{
    //
    // ofstream
    // x Pos
    std::ofstream out_file;

    out_file.open(_xPositions);
    
    out_file.clear();

    for (std::list<CBlock>::iterator it = m_Chunk.begin(); it != m_Chunk.end(); it++)
    {
        out_file << it->GetShape().getPosition().x << std::endl;
    }
    out_file.close();

    out_file.open(_yPositions);
    
    out_file.clear();

    for (std::list<CBlock>::iterator it = m_Chunk.begin(); it != m_Chunk.end(); it++)
    {
        out_file << it->GetShape().getPosition().y << std::endl;
    }
    out_file.close();
    //
    //
}

void CWorldManager::OutPutWorldToFiles()
{
    //
    // ofstream
    // x Pos
    std::ofstream out_file;

    out_file.open("Output/output_x.txt");

    out_file.clear();

    for (std::list<CBlock>::iterator it = m_Chunk.begin(); it != m_Chunk.end(); it++)
    {
        
        out_file << it->GetShape().getPosition().x << std::endl;
        //std::cout << it->GetShape().getPosition().x << std::endl;
    }
    out_file.close();

    out_file.open("Output/output_y.txt");

    out_file.clear();

    for (std::list<CBlock>::iterator it = m_Chunk.begin(); it != m_Chunk.end(); it++)
    {
        out_file << it->GetShape().getPosition().y << std::endl;
    }
    out_file.close();
    //
    //
}
