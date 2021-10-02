#include "WorldManager.h"

CWorldManager::CWorldManager(sf::RenderWindow* _renderWindow, CPlayer* _player, b2World& _world, GUI* _gui, sf::Shader* _shader, sf::Shader* _surfaceShader, sf::Shader* _tourchShader)
{
    m_RenderWindow = _renderWindow;
    m_Player = _player;
    m_World = &_world;
    m_GUI = _gui;
    m_Shader = _shader;
    m_SurfaceShader = _surfaceShader;
    m_TourchShader = _tourchShader;
    m_Block = nullptr;
    m_Chest = nullptr;
    m_Potion = nullptr;

    // Set World Seed (Change Time To A Value For Custom)
    _gui->m_CurrentSeed = _CURRENT_TIME_;
    srand(_gui->m_CurrentSeed);

    // Debug
    std::cout << _gui->m_CurrentSeed << std::endl;
}

CWorldManager::~CWorldManager()
{
    OutPutWorldToFiles();

    m_Chunk.clear();
    m_SkyChunk.clear();
    m_Chests.clear();
    m_Doors.clear();
    m_Furnaces.clear();
    m_WorkBenches.clear();
    m_Tourches.clear();

    m_TourchShader = nullptr;
    m_SurfaceShader = nullptr;
    m_Shader = nullptr;
    m_Block = nullptr;
    m_GUI = nullptr;
    m_Player = nullptr;
    m_RenderWindow = nullptr;
    m_World = nullptr;
}

void CWorldManager::Start(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners, WORLDTYPE _worldtype)
{
    m_WorldType = _worldtype;

    switch (m_WorldType)
    {
    case CWorldManager::PLAINS:
        // World BackGround (Surface)
        m_BGPlainsSurface = sf::Sprite();
        m_BGPlainsSurface.setTexture(*_textureMaster->m_Sky, true);
        m_BGPlainsSurface.setOrigin(m_BGPlainsSurface.getGlobalBounds().width / 2, m_BGPlainsSurface.getGlobalBounds().height / 2);
        m_BGPlainsSurface.setScale(4.7, 4.7);
        m_BGPlainsSurface.setColor(sf::Color::White);

        // World BackGround (Under Ground)
        m_BGPlainsUnderGr = sf::Sprite();
        m_BGPlainsUnderGr.setTexture(*_textureMaster->m_UnderGroundPlains, true);
        m_BGPlainsUnderGr.setTextureRect(sf::IntRect(0, 0, 80000, 40000));
        m_BGPlainsUnderGr.setOrigin(m_BGPlainsUnderGr.getGlobalBounds().width / 2, m_BGPlainsUnderGr.getGlobalBounds().height / 2);
        m_BGPlainsUnderGr.setColor(sf::Color::Transparent);
        m_BGPlainsUnderGr.setPosition(0, 0);

        CreateNoiseWorld(_textureMaster, _audioManager, _spawners);
        CreateClouds(_textureMaster);
        break;
    case CWorldManager::SAND:
        // World BackGround (Surface)
        m_BGPlainsSurface = sf::Sprite();
        m_BGPlainsSurface.setTexture(*_textureMaster->m_Sky, true);
        m_BGPlainsSurface.setOrigin(m_BGPlainsSurface.getGlobalBounds().width / 2, m_BGPlainsSurface.getGlobalBounds().height / 2);
        m_BGPlainsSurface.setScale(4.7, 4.7);
        m_BGPlainsSurface.setColor(sf::Color::White);

        // World BackGround Sand (Surface)
        m_BGSandSurface = sf::Sprite();
        m_BGSandSurface.setTexture(*_textureMaster->m_SandBG, true);
        m_BGSandSurface.setOrigin(m_BGSandSurface.getGlobalBounds().width / 2, m_BGSandSurface.getGlobalBounds().height / 2);
        m_BGSandSurface.setScale(2.8, 2.8);
        m_BGSandSurface.setColor(sf::Color::White);

        // World BackGround Sand (Under Ground)
        m_BGPlainsUnderGr = sf::Sprite();
        m_BGPlainsUnderGr.setTexture(*_textureMaster->m_UnderGroundSand, true);
        m_BGPlainsUnderGr.setTextureRect(sf::IntRect(0, 0, 80000, 40000));
        m_BGPlainsUnderGr.setOrigin(m_BGPlainsUnderGr.getGlobalBounds().width / 2, m_BGPlainsUnderGr.getGlobalBounds().height / 2);
        m_BGPlainsUnderGr.setColor(sf::Color::Transparent);
        m_BGPlainsUnderGr.setPosition(0, 0);

        CreateSandNoiseWorld(_textureMaster, _audioManager, _spawners);
        break;
    case CWorldManager::JUNGLE:
        break;
    case CWorldManager::HELL:
        break;
    default:
        // Plains
        // World BackGround (Surface)
        m_BGPlainsSurface = sf::Sprite();
        m_BGPlainsSurface.setTexture(*_textureMaster->m_Sky, true);
        m_BGPlainsSurface.setOrigin(m_BGPlainsSurface.getGlobalBounds().width / 2, m_BGPlainsSurface.getGlobalBounds().height / 2);
        m_BGPlainsSurface.setScale(4.7, 4.7);
        m_BGPlainsSurface.setColor(sf::Color::White);

        // World BackGround (Under Ground)
        m_BGPlainsUnderGr = sf::Sprite();
        m_BGPlainsUnderGr.setTexture(*_textureMaster->m_UnderGroundPlains, true);
        m_BGPlainsUnderGr.setTextureRect(sf::IntRect(0, 0, 80000, 40000));
        m_BGPlainsUnderGr.setOrigin(m_BGPlainsUnderGr.getGlobalBounds().width / 2, m_BGPlainsUnderGr.getGlobalBounds().height / 2);
        m_BGPlainsUnderGr.setColor(sf::Color::Transparent);
        m_BGPlainsUnderGr.setPosition(0, 0);

        CreateNoiseWorld(_textureMaster, _audioManager, _spawners);
        CreateClouds(_textureMaster);
        break;
    }

    

    
    // World
    CreateSkyChunk(_textureMaster);
    CreateWorldBoundary(_textureMaster);
    CreateDungeon(_textureMaster, _audioManager, _spawners);
    CreateSurfaceSpawners(_textureMaster, _audioManager, _spawners);
}

void CWorldManager::Update(sf::Vector2f _mousePos, CTextureMaster* _textureMaster)
{

    // Set Mouse / Block Indicator To Rounded Sky Position
    m_GUI->m_MousePos.setPosition(m_SkyChunk.front().getPosition());

    // Update All Blocks In Range Of Player && Set Mouse Pos To Blocks
    float Mag1 = 0;
    float x = 0;
    float y = 0;
    for (CBlock& block : m_Chunk)
    {
        x = block.GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
        x *= x;
        y = block.GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
        y *= y;
        Mag1 = sqrt(x + y);

        if (Mag1 < 1920 * 1.f)
        {
            if (block.GetShape().getGlobalBounds().contains(_mousePos))
            {
                m_GUI->m_MousePos.setPosition(block.GetPosition());
            }
        }
        else
        {
            continue;
        }
    }

    // Mouse Pos To Door
    for (CDoor& door : m_Doors)
    {
        if (door.GetShape().getGlobalBounds().contains(_mousePos))
        {
            m_GUI->m_MousePos.setPosition(door.GetShape().getPosition());
        }
    }

    // Mouse Pos To Chest
    for (CChest& chest : m_Chests)
    {
        if (chest.GetShape().getGlobalBounds().contains(_mousePos))
        {
            m_GUI->m_MousePos.setPosition(chest.GetShape().getPosition());
        }
    }

    // Mouse Pos To Furnace
    for (CFurnace& furnace : m_Furnaces)
    {
        Mag1 = 400;

        if (m_Player != nullptr)
        {
            Mag1 = sqrt(((m_Player->GetShape().getPosition().x - furnace.GetShape().getPosition().x) * (m_Player->GetShape().getPosition().x - furnace.GetShape().getPosition().x)) + ((m_Player->GetShape().getPosition().y - furnace.GetShape().getPosition().y) * (m_Player->GetShape().getPosition().y - furnace.GetShape().getPosition().y)));
        }
        
        // SETTING CAN WORKBENCH
        if (Mag1 < 300)
        {
            if (furnace.GetShape().getGlobalBounds().contains(_mousePos))
            {
                m_GUI->m_MousePos.setPosition(furnace.GetShape().getPosition());
            }

            m_GUI->m_bCanSmelt = true;
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

    // Mouse Pos To Workbench
    for (CWorkBench& workbench : m_WorkBenches)
    {
        Mag1 = 400;

        if (m_Player != nullptr)
        {
            Mag1 = sqrt(((m_Player->GetShape().getPosition().x - workbench.GetShape().getPosition().x) * (m_Player->GetShape().getPosition().x - workbench.GetShape().getPosition().x)) + ((m_Player->GetShape().getPosition().y - workbench.GetShape().getPosition().y) * (m_Player->GetShape().getPosition().y - workbench.GetShape().getPosition().y)));
        }

        // SETTING CAN WORKBENCH
        if (Mag1 < 300)
        {
            if (workbench.GetShape().getGlobalBounds().contains(_mousePos))
            {
                m_GUI->m_MousePos.setPosition(workbench.GetShape().getPosition());
            }

            m_GUI->m_bCanWorkBench = true;
        }
        else
        {
            if (workbench.GetShape().getGlobalBounds().contains(_mousePos))
            {
                m_GUI->m_MousePos.setPosition(workbench.GetShape().getPosition());
            }

            m_GUI->m_bCanWorkBench = false;
        }

    }

    if (m_WorkBenches.size() <= 0)
    {
        m_GUI->m_bCanWorkBench = false;
    }
    if (m_Furnaces.size() <= 0)
    {
        m_GUI->m_bCanSmelt = false;
    }
}

void CWorldManager::Render(sf::Shader* _defaultShader)
{
    //
    // CALL EVERY FRAME
    //

    WorldBackGroundColourGradient();
    
    DrawBackGround();

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
        // Fake Player Light
        m_Shader->setUniform("hasTexture", true);
        m_Shader->setUniform("lightPos", -1000);
    }

    // Block Iterator
    std::list<CBlock>::iterator it;
    float Mag1 = 0;
    float x = 0;
    float y = 0;

    // Draw Blocks In Range (With Shaders : Player, Tourch, Surface)
    for (it = m_Chunk.begin(); it != m_Chunk.end(); it++)
    {
        x = it->GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
        y = it->GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
        Mag1 = sqrt((x * x) + (y * y));

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
                else if (Mag1 < 1920 * 1.8f)
                {
                    m_RenderWindow->draw(it->GetShape(), m_SurfaceShader);
                }
            }
            else
            {
                if (bIsBlockInRangeOfLightSource(it) && it->GetShape().getPosition().y >= 1300 && Mag1 < 1920 * 1.8f)
                {
                    m_RenderWindow->draw(it->GetShape(), m_TourchShader);
                }
                else if (Mag1 < 1920 * 1.8f)
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

    // Doors, Chests, Funaces
    for (CDoor& door : m_Doors)
    {
        x = door.GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
        y = door.GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
        Mag1 = sqrt((x * x) + (y * y));

        if (Mag1 < 1920 * 1.8f)
        {
            door.Render(_defaultShader);
        }
    }
    for (CChest& chest : m_Chests)
    {
        x = chest.GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
        y = chest.GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
        Mag1 = sqrt((x * x) + (y * y));

        if (Mag1 < 1920 * 1.8f)
        {
            chest.Render(_defaultShader);
        }
    }
    for (CFurnace& furnace : m_Furnaces)
    {
        x = furnace.GetShape().getPosition().x - m_RenderWindow->getView().getCenter().x;
        y = furnace.GetShape().getPosition().y - m_RenderWindow->getView().getCenter().y;
        Mag1 = sqrt((x * x) + (y * y));

        if (Mag1 < 1920 * 1.8f)
        {
            furnace.Render(_defaultShader);
        }
    }
    for (CWorkBench& workbench : m_WorkBenches)
    {
        workbench.Render(_defaultShader);
    }
    for (CBlock& tourch : m_Tourches)
    {
        tourch.Render(_defaultShader);
    }
}

void CWorldManager::CreateSkyChunk(CTextureMaster* _textureMaster)
{
    //
    // CALL EVERY FRAME
    //
    m_SkyChunk.clear();

    int tempx = 0;
    int tempy = 0;

    if (m_Player != nullptr)
    {
        // Round To Nearest 100 / Block Size
        tempx = 100 * std::roundf(m_Player->m_MousePos.x / 100.0);
        tempy = 100 * std::roundf(m_Player->m_MousePos.y / 100.0);
    }

    sf::RectangleShape sky = sf::RectangleShape();
    sky.setSize(sf::Vector2f(100, 100));
    sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
    sky.setPosition(sf::Vector2f(tempx + 10, tempy));
    m_SkyChunk.push_back(sky);
}

void CWorldManager::CreateWorldBoundary(CTextureMaster* _textureMaster)
{
    m_GlobalMutex.lock();
    for (int i = -90; i > -69770 / 1.9f; i -= 100)
    {
        // OBSIDIAN
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, i, -6000, false, CBlock::BLOCKTYPE::OBSIDIAN);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;
    }
    for (int i = -90; i > -69770 / 1.9f; i -= 100)
    {
        // OBSIDIAN
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, i, 11300, false, CBlock::BLOCKTYPE::OBSIDIAN);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;
    }
    for (int j = 500 - 6500; j < 21400 / 1.9f; j += 100)
    {
        // OBSIDIAN
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, -69770 / 1.9f, j, false, CBlock::BLOCKTYPE::OBSIDIAN);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;
    }
    for (int i = 10; i < 69770 / 1.9f - 14000; i += 100)
    {
        // OBSIDIAN
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, i, -6000, false, CBlock::BLOCKTYPE::OBSIDIAN);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;
    }
    for (int i = 10; i < 69770 / 1.9f - 14000; i += 100)
    {
        // OBSIDIAN
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, i, 11300, false, CBlock::BLOCKTYPE::OBSIDIAN);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;
    }
    for (int j = 500 - 6500; j < 21400 / 1.9f; j += 100)
    {
        // OBSIDIAN
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, +69570 / 1.9f - 14000, j, false, CBlock::BLOCKTYPE::OBSIDIAN);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;
    }
    m_GlobalMutex.unlock();
}

void CWorldManager::CreateNoiseWorld(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners)
{
    m_GlobalMutex.lock();
    GenerateNoise();
    for (int y = 0; y < m_GenerateOffsetY; y++)
    {
        for (int x =  -m_GenerateOffsetX; x <  +m_GenerateOffsetX - 70; x++)
        {
            double xyValue = x * m_XPeriod / _NOISEWIDTH_ + y * m_YPeriod / _NOISEHEIGHT_ + m_TurbPower * Turbulence(x, y, m_TurbSize) / 256.0;
            double sineValue = 256 * fabs(sin(xyValue * _PI_));

            if (sineValue <= 15 && x % 15 == 0 && y % 15 == 0)
            {
                // Spawners
                Spawner* m_SlimeSpawner = new Spawner(_audioManager, m_RenderWindow, *m_World, _textureMaster, Utils::m_Scale, x * 100 + 10, y * 100, m_Player, CEnemy::ENEMYTYPE::SLIME, m_Shader, m_TourchShader,false);
                m_SlimeSpawner->ToggleSpawning();
                m_SlimeSpawner->SetSpawnCount(1 + rand() % 3);
                _spawners.push_back(*m_SlimeSpawner);
                m_SlimeSpawner = nullptr;
            }
            else if (sineValue <= 70)
            {
                // Caves
            }
            else if (sineValue > 95 && sineValue <= 100 && y > 10)
            {
                // Iron
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_IronOre, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::IRONORE);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            else if (sineValue > 82 && sineValue <= 85 && y > 6)
            {
                // coal
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Coal, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::COALORE);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            else if (sineValue > 76 && sineValue <= 79 && y > 60)
            {
                // Diamond
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_DiamondOre, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::DIAMONDORE);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            else if (sineValue > 74 && sineValue <= 76 && y > 40)
            {
                // gold
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_GoldOre, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::GOLDORE);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            else
            {
                if (y > 12 || y > 11 || y > 10)
                {
                    if (_DIRT_RARITY_SURFACE_)
                    {
                        // Dirt
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Dirt, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::DIRT);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = x + y;
                        m_Block = nullptr;
                    }
                    else if (_DIRT_RARITY_UNDERGOUND_)
                    {
                        // Dirt
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Dirt, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::DIRT);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = x + y;
                        m_Block = nullptr;
                    }
                    else if (rand() % 30 == 0 && y < 80 && y >= 50)
                    {
                        if (_MOSSYBRICK_RARITY_UNDERGOUND_)
                        {
                            // MossyBrick
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_MossyBrick, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::MOSSYBRICK);
                            //m_Block->SetSize(100, 100);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                        else
                        {
                            // Dirt
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Dirt, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::DIRT);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                    }
                    else
                    {
                        if (_MOSSYBRICK_RARITY_SURFACE_)
                        {
                            // Mossy Brick
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_MossyBrick, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::MOSSYBRICK);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                        else
                        {
                            // Stone
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::STONE);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                    }
                }
                else if (y == 0)
                {
                    // Trees
                    if (_TREE_RARITY_)
                    {
                        // Trunk
                        if (true)
                        {
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, x * 100 + 10, y * 100 - 100, true, CBlock::BLOCKTYPE::WOOD);
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
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, x * 100 - 90, y * 100 - 700, true, CBlock::BLOCKTYPE::LEAVES);
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

                    // Chests
                    else if (_CHEST_RARITY_)
                    {
                        m_Chest = new CChest(m_RenderWindow, *m_World, 50.0f, x * 100 + 10, y * 100 - 100);
                        m_Chest->SetSizeAndPos(x * 100 + 10, y * 100 - 100, 100, 100);

                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
                            for (int i = 0; i < 0 + rand() % 4; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 0, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_Cloud, CBlock::BLOCKTYPE::CLOUD);
                            for (int i = 0; i < 0 + rand() % 8; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 1, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Potion = new CPotion(CBlock::POTIONTYPE::HPSMALL);
                            for (int i = 0; i < 0 + rand() % 3; i++)
                            {
                                m_Chest->AddItemToInventory(m_Potion, 2, true);
                            }
                            m_Potion = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_Arrow, CBlock::BLOCKTYPE::PROJECTILE);
                            for (int i = 0; i < 0 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 3, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_FireArrow, CBlock::BLOCKTYPE::PROJECTILE);
                            for (int i = 0; i < 0 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 4, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_PoisonArrow, CBlock::BLOCKTYPE::PROJECTILE);
                            for (int i = 0; i < 0 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 5, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_Tourch, CBlock::BLOCKTYPE::TOURCH);
                            for (int i = 0; i < 0 + rand() % 14; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 6, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_Stone, CBlock::BLOCKTYPE::STONE);
                            for (int i = 0; i < 0 + rand() % 14; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 7, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 2 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_GoldOre, CBlock::BLOCKTYPE::GOLDORE);
                            for (int i = 0; i < 0 + rand() % 2; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 8, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_RedSlime, CBlock::BLOCKTYPE::REDSLIME);
                            for (int i = 0; i < 0 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 9, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_GlassBeaker, CBlock::BLOCKTYPE::EMPTYBEAKER);
                            for (int i = 0; i < 0 + rand() % 3; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 10, true);
                            }
                            m_Block = nullptr;
                        }

                        m_Chests.push_back(*m_Chest);
                        m_Chest = nullptr;
                    }

                    // Grass
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Grass, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::GRASS);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = x + y;
                    m_Block = nullptr;
                }
                else
                {
                    if (_STONE_RARITY_)
                    {
                        // Stone
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::STONE);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = x + y;
                        m_Block = nullptr;
                    }
                    else
                    {
                        // Dirt
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Dirt, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::DIRT);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = x + y;
                        m_Block = nullptr;
                    }

                }
            }
        }
    }
    m_GlobalMutex.unlock();
}

void CWorldManager::CreateSandNoiseWorld(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners)
{
    m_GlobalMutex.lock();
    GenerateNoise();
    for (int y = 0; y < m_GenerateOffsetY; y++)
    {
        for (int x = -m_GenerateOffsetX; x < +m_GenerateOffsetX - 70; x++)
        {
            double xyValue = x * m_XPeriod / _NOISEWIDTH_ + y * m_YPeriod / _NOISEHEIGHT_ + m_TurbPower * Turbulence(x, y, m_TurbSize) / 256.0;
            double sineValue = 256 * fabs(sin(xyValue * _PI_));

            if (sineValue <= 15 && x % 15 == 0 && y % 15 == 0)
            {
                // Spawners
                Spawner* m_SlimeSpawner = new Spawner(_audioManager, m_RenderWindow, *m_World, _textureMaster, Utils::m_Scale, x * 100 + 10, y * 100, m_Player, CEnemy::ENEMYTYPE::SLIME, m_Shader, m_TourchShader, false);
                m_SlimeSpawner->ToggleSpawning();
                m_SlimeSpawner->SetSpawnCount(1 + rand() % 3);
                _spawners.push_back(*m_SlimeSpawner);
                m_SlimeSpawner = nullptr;
            }
            else if (sineValue <= 70)
            {
                // Caves
            }
            else if (sineValue > 95 && sineValue <= 100 && y > 10)
            {
                // Iron
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_IronOre, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::IRONORE);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            else if (sineValue > 82 && sineValue <= 85 && y > 6)
            {
                // coal
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Coal, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::COALORE);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            else if (sineValue > 76 && sineValue <= 79 && y > 60)
            {
                // Purple
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_PurpleOre, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::PURPLEORE);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            else if (sineValue > 74 && sineValue <= 76 && y > 40)
            {
                // gold
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_GoldOre, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::GOLDORE);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            else if (sineValue > 70 && sineValue <= 72 && y > 95)
            {
                // red
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_GoldenOre, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::GOLDENORE);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            else
            {
                if (y > 12 || y > 11 || y > 10)
                {
                    if (_DIRT_RARITY_SURFACE_)
                    {
                        // Sand
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Sand, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::SAND);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = x + y;
                        m_Block = nullptr;
                    }
                    else if (_DIRT_RARITY_UNDERGOUND_)
                    {
                        // Sand
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Sand, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::SAND);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = x + y;
                        m_Block = nullptr;
                    }
                    else if (rand() % 30 == 0 && y < 80 && y >= 50)
                    {
                        if (_MOSSYBRICK_RARITY_UNDERGOUND_)
                        {
                            // Hard SandStone
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_HardSandStone, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::HARDSANDSTONE);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                        else
                        {
                            // Sand
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Sand, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::SAND);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                    }
                    else
                    {
                        if (_MOSSYBRICK_RARITY_SURFACE_)
                        {
                            // Hard SandStone
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_HardSandStone, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::HARDSANDSTONE);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                        else
                        {
                            // SandStone
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_SandStone, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::SANDSTONE);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                        }
                    }
                }
                else if (y == 0)
                {
                    // Trees
                    if (_TREE_RARITY_)
                    {
                        // Trunk
                        if (true)
                        {
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Cactas, Utils::m_Scale, x * 100 + 10, y * 100 - 100, true, CBlock::BLOCKTYPE::CACTUS);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Cactas, Utils::m_Scale, x * 100 + 10, y * 100 - 200, true, CBlock::BLOCKTYPE::CACTUS);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Cactas, Utils::m_Scale, x * 100 + 10, y * 100 - 300, true, CBlock::BLOCKTYPE::CACTUS);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = x + y;
                            m_Block = nullptr;

                            srand(x);
                            if (rand() % 3 == 0)
                            {
                                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Cactas, Utils::m_Scale, x * 100 + 10, y * 100 - 400, true, CBlock::BLOCKTYPE::CACTUS);
                                m_Chunk.push_back(*m_Block);
                                m_Block->m_ArrayIndex = x + y;
                                m_Block = nullptr;
                            }
                            else if (rand() % 3 == 1)
                            {
                                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Cactas, Utils::m_Scale, x * 100 + 10, y * 100 - 400, true, CBlock::BLOCKTYPE::CACTUS);
                                m_Chunk.push_back(*m_Block);
                                m_Block->m_ArrayIndex = x + y;
                                m_Block = nullptr;
                                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Cactas, Utils::m_Scale, x * 100 + 10, y * 100 - 500, true, CBlock::BLOCKTYPE::CACTUS);
                                m_Chunk.push_back(*m_Block);
                                m_Block->m_ArrayIndex = x + y;
                                m_Block = nullptr;
                            }
                            else if (rand() % 3 == 1)
                            {
                                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Cactas, Utils::m_Scale, x * 100 + 10, y * 100 - 400, true, CBlock::BLOCKTYPE::CACTUS);
                                m_Chunk.push_back(*m_Block);
                                m_Block->m_ArrayIndex = x + y;
                                m_Block = nullptr;
                                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Cactas, Utils::m_Scale, x * 100 + 10, y * 100 - 500, true, CBlock::BLOCKTYPE::CACTUS);
                                m_Chunk.push_back(*m_Block);
                                m_Block->m_ArrayIndex = x + y;
                                m_Block = nullptr;
                                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Cactas, Utils::m_Scale, x * 100 + 10, y * 100 - 600, true, CBlock::BLOCKTYPE::CACTUS);
                                m_Chunk.push_back(*m_Block);
                                m_Block->m_ArrayIndex = x + y;
                                m_Block = nullptr;
                            }
                        }
                    }

                    // Chests
                    else if (_CHEST_RARITY_)
                    {
                        m_Chest = new CChest(m_RenderWindow, *m_World, 50.0f, x * 100 + 10, y * 100 - 100);
                        m_Chest->SetSizeAndPos(x * 100 + 10, y * 100 - 100, 100, 100);

                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
                            for (int i = 0; i < 0 + rand() % 4; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 0, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_Cloud, CBlock::BLOCKTYPE::CLOUD);
                            for (int i = 0; i < 0 + rand() % 8; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 1, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Potion = new CPotion(CBlock::POTIONTYPE::HPSMALL);
                            for (int i = 0; i < 0 + rand() % 3; i++)
                            {
                                m_Chest->AddItemToInventory(m_Potion, 2, true);
                            }
                            m_Potion = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_Arrow, CBlock::BLOCKTYPE::PROJECTILE);
                            for (int i = 0; i < 0 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 3, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_FireArrow, CBlock::BLOCKTYPE::PROJECTILE);
                            for (int i = 0; i < 0 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 4, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_PoisonArrow, CBlock::BLOCKTYPE::PROJECTILE);
                            for (int i = 0; i < 0 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 5, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_Tourch, CBlock::BLOCKTYPE::TOURCH);
                            for (int i = 0; i < 0 + rand() % 14; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 6, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_Stone, CBlock::BLOCKTYPE::STONE);
                            for (int i = 0; i < 0 + rand() % 14; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 7, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 2 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_GoldOre, CBlock::BLOCKTYPE::GOLDORE);
                            for (int i = 0; i < 0 + rand() % 2; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 8, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_RedSlime, CBlock::BLOCKTYPE::REDSLIME);
                            for (int i = 0; i < 0 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 9, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_GlassBeaker, CBlock::BLOCKTYPE::EMPTYBEAKER);
                            for (int i = 0; i < 0 + rand() % 3; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 10, true);
                            }
                            m_Block = nullptr;
                        }

                        m_Chests.push_back(*m_Chest);
                        m_Chest = nullptr;
                    }

                    // Grass
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Sand, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::SAND);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = x + y;
                    m_Block = nullptr;
                }
                else
                {
                    if (_STONE_RARITY_)
                    {
                        // SandStone
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_SandStone, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::SANDSTONE);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = x + y;
                        m_Block = nullptr;
                    }
                    else
                    {
                        // Sand
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Sand, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::SAND);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = x + y;
                        m_Block = nullptr;
                    }

                }
            }
        }
    }
    m_GlobalMutex.unlock();
}

void CWorldManager::CreateClouds(CTextureMaster* _textureMaster)
{
    m_GlobalMutex.lock();
    m_XPeriod = 0;
    m_YPeriod = 0;
    m_TurbSize = 10;

    GenerateNoise();

    for (int y = -_WORLDSTART_HEIGHT_; y < _NOISEHEIGHT_; y++)
    {
        for (int x = -_NOISEWIDTH_; x < _NOISEWIDTH_; x++)
        {
            sf::Uint8 L = 192 + sf::Uint8(Turbulence(x, y, 22)) / 4;

            if (y * 100 < -3100)
            {
                // Light Cloud
                if (L < 208)
                {
                    // Cloud
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Cloud, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::CLOUD);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = (x + y) * 1337;
                    m_Block = nullptr;

                    // Chests
                    if (_CHEST_RARITY_CLOUDS_)
                    {
                        m_Chest = new CChest(m_RenderWindow, *m_World, 50.0f, x * 100 + 10, y * 100 - 100);
                        m_Chest->SetSizeAndPos(x * 100 + 10, y * 100 - 100, 100, 100);

                        srand(1);
                        // ITEM RARITIES
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
                            for (int i = 0; i < 1 + rand() % 2; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 0, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(10);
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
                            for (int i = 0; i < 1 + rand() % 2; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 1, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(100);
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_Cloud, CBlock::BLOCKTYPE::CLOUD);
                            for (int i = 0; i < 2 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 2, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(1000);
                        if (rand() % 2 == 0)
                        {
                           Bow* tempbow = new Bow(CBlock::BOWTYPE::ICE);
                            for (int i = 0; i < 1; i++)
                            {
                                m_Chest->AddItemToInventory(tempbow, 3, false);
                            }
                            tempbow = nullptr;
                        }
                        srand(2000);
                        if (rand() % 2 == 0)
                        {
                            m_Potion = new CPotion(CBlock::POTIONTYPE::HPSMALL);
                            for (int i = 0; i < 2 + rand() % 4; i++)
                            {
                                m_Chest->AddItemToInventory(m_Potion, 3, true);
                            }
                            m_Potion = nullptr;
                        }
                        else if (rand() % 2 == 1)
                        {
                            m_Potion = new CPotion(CBlock::POTIONTYPE::HPLARGE);
                            for (int i = 0; i < 2 + rand() % 3; i++)
                            {
                                m_Chest->AddItemToInventory(m_Potion, 4, true);
                            }
                            m_Potion = nullptr;
                        }
                        srand(6);
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_Arrow, CBlock::BLOCKTYPE::PROJECTILE);
                            for (int i = 0; i < 2 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 5, true);
                            }
                            m_Block = nullptr;
                        }

                        m_Chests.push_back(*m_Chest);
                        m_Chest = nullptr;
                    }
                }
                // Darker Cloud
                else if (L < 209 + 3)
                {
                    // Chests
                    if (_CHEST_RARITY_CLOUDS_)
                    {
                        m_Chest = new CChest(m_RenderWindow, *m_World, 50.0f, x * 100 + 10, y * 100 - 100);
                        m_Chest->SetSizeAndPos(x * 100 + 10, y * 100 - 100, 100, 100);

                        srand(x + m_Chest->m_Inventory.size());
                        // ITEM RARITIES
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
                            for (int i = 0; i < 1 + rand() % 2; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 0, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
                            for (int i = 0; i < 0 + rand() % 2; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 1, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_Cloud, CBlock::BLOCKTYPE::CLOUD);
                            for (int i = 0; i < 2 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 2, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 3 == 0)
                        {
                            Bow* tempbow = new Bow(CBlock::BOWTYPE::ICE);
                            for (int i = 0; i < 1; i++)
                            {
                                m_Chest->AddItemToInventory(tempbow, 12, false);
                            }
                            tempbow = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Potion = new CPotion(CBlock::POTIONTYPE::HPSMALL);
                            for (int i = 0; i < 0 + rand() % 4; i++)
                            {
                                m_Chest->AddItemToInventory(m_Potion, 3, true);
                            }
                            m_Potion = nullptr;
                        }
                        else if (rand() % 2 == 0)
                        {
                            m_Potion = new CPotion(CBlock::POTIONTYPE::HPLARGE);
                            for (int i = 0; i < 0 + rand() % 3; i++)
                            {
                                m_Chest->AddItemToInventory(m_Potion, 4, true);
                            }
                            m_Potion = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_PoisonArrow, CBlock::BLOCKTYPE::PROJECTILE);
                            for (int i = 0; i < 0 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 5, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_CursedArrow, CBlock::BLOCKTYPE::PROJECTILE);
                            for (int i = 0; i < 0 + rand() % 6; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 7, true);
                            }
                            m_Block = nullptr;
                        }
                        srand(x + m_Chest->m_Inventory.size());
                        if (rand() % 1 == 0)
                        {
                            m_Block = new CBlock(_textureMaster->m_GlassBeaker, CBlock::BLOCKTYPE::EMPTYBEAKER);
                            for (int i = 0; i < 0 + rand() % 3; i++)
                            {
                                m_Chest->AddItemToInventory(m_Block, 8, true);
                            }
                            m_Block = nullptr;
                        }

                        m_Chests.push_back(*m_Chest);
                        m_Chest = nullptr;
                    }

                    // Cloud
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_CloudDark, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::CLOUD);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = (x + y) * 1337;
                    m_Block = nullptr;
                }
            }
        }
    }
    m_GlobalMutex.unlock();
}

bool CWorldManager::bIsBlockInRangeOfLightSource(std::list<CBlock>::iterator _it)
{
    float Mag1 = 0;
    float x = 0;
    float y = 0;

    for (std::list<CFurnace>::iterator fit = m_Furnaces.begin(); fit != m_Furnaces.end(); fit++)
    {
        x = _it->GetShape().getPosition().x - fit->GetShape().getPosition().x;
        x *= x;
        y = _it->GetShape().getPosition().y - fit->GetShape().getPosition().y;
        y *= y;
        Mag1 = sqrt(x + y);
        
        if (Mag1 < 600)
        {
            // Surface Shader
            if (m_TourchShader != nullptr) 
            {
                m_TourchShader->setUniform("hasTexture", true);
                m_TourchShader->setUniform("lightPos", fit->GetShape().getPosition());

                return true;
                break;
            }
        }
    }

    for (std::list<CBlock>::iterator touit = m_Tourches.begin(); touit != m_Tourches.end(); touit++)
    {
        x = _it->GetShape().getPosition().x - touit->GetShape().getPosition().x;
        x *= x;
        y = _it->GetShape().getPosition().y - touit->GetShape().getPosition().y;
        y *= y;
        Mag1 = sqrt(x + y);

        if (Mag1 < 600)
        {
            // Surface Shader
            if (m_TourchShader != nullptr)
            {
                m_TourchShader->setUniform("hasTexture", true);
                m_TourchShader->setUniform("lightPos", touit->GetShape().getPosition());

                return true;
                break;
            }
        }
    }

    return false;
}

bool CWorldManager::bIsItemInRangeOfLightSource(sf::Sprite _shape)
{
    float Mag1 = 0;
    float x = 0;
    float y = 0;

    for (std::list<CFurnace>::iterator fit = m_Furnaces.begin(); fit != m_Furnaces.end(); fit++)
    {
        x = _shape.getPosition().x - fit->GetShape().getPosition().x;
        x *= x;
        y = _shape.getPosition().y - fit->GetShape().getPosition().y;
        y *= y;
        Mag1 = sqrt(x + y);

        if (Mag1 < 600)
        {
            // Surface Shader
            if (m_TourchShader != nullptr)
            {
                m_TourchShader->setUniform("hasTexture", true);
                m_TourchShader->setUniform("lightPos", fit->GetShape().getPosition());
                return true;
                break;
            }
        }
    }
    for (std::list<CBlock>::iterator touit = m_Tourches.begin(); touit != m_Tourches.end(); touit++)
    {
        x = _shape.getPosition().x - touit->GetShape().getPosition().x;
        x *= x;
        y = _shape.getPosition().y - touit->GetShape().getPosition().y;
        y *= y;
        Mag1 = sqrt(x + y);

        if (Mag1 < 600)
        {
            // Surface Shader
            if (m_TourchShader != nullptr)
            {
                m_TourchShader->setUniform("hasTexture", true);
                m_TourchShader->setUniform("lightPos", touit->GetShape().getPosition());
                return true;
                break;
            }
        }
    }

    return false;
}

void CWorldManager::InitPointer(CPlayer* _player)
{
    m_Player = _player;

    // Reset BG
    m_BGPlainsUnderGr.setColor(sf::Color::Transparent);
    m_BGPlainsUnderGr.setPosition(0, 0);
    m_BGPlainsSurface.setColor(sf::Color::White);

    std::cout << "Player Initialized On World" << std::endl;
}

void CWorldManager::OutPutWorldToFiles(std::string _xPositions, std::string _yPositions)
{
    // Ofstream
    std::ofstream out_file;

    // X Positions
    out_file.open(_xPositions);
    out_file.clear();
    for (std::list<CBlock>::iterator it = m_Chunk.begin(); it != m_Chunk.end(); it++)
    {
        out_file << it->GetShape().getPosition().x << std::endl;
    }
    out_file.close();

    // Y Positions
    out_file.open(_yPositions);
    out_file.clear();
    for (std::list<CBlock>::iterator it = m_Chunk.begin(); it != m_Chunk.end(); it++)
    {
        out_file << it->GetShape().getPosition().y << std::endl;
    }
    out_file.close();
}

void CWorldManager::OutPutWorldToFiles()
{
    // OFstream
    std::ofstream out_file;

    //
    // X Positions
    out_file.open("Output/output_x.txt");
    out_file.clear();
    for (std::list<CBlock>::iterator it = m_Chunk.begin(); it != m_Chunk.end(); it++)
    {
        
        out_file << it->GetShape().getPosition().x << std::endl;
        //std::cout << it->GetShape().getPosition().x << std::endl;
    }
    out_file.close();

    //
    // Y Positions
    out_file.open("Output/output_y.txt");
    out_file.clear();
    for (std::list<CBlock>::iterator it = m_Chunk.begin(); it != m_Chunk.end(); it++)
    {
        out_file << it->GetShape().getPosition().y << std::endl;
    }
    out_file.close();
}

void CWorldManager::WorldBackGroundColourGradient()
{
    //
    // CALLED EVERY FRAME
    //

    if (m_Player != nullptr)
    {
        // Positioning Planes Surface
        m_BGPlainsSurface.setPosition(m_RenderWindow->getView().getCenter());
        m_BGSandSurface.setPosition(m_RenderWindow->getView().getCenter());

        // Colour
        if (m_Player->GetShape().getPosition().y > 1200 && m_Player->GetShape().getPosition().y < 1250)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 255));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 0));

            m_BGSandSurface.setColor(sf::Color(255, 255, 255, 255));
        }
        else if (m_Player->GetShape().getPosition().y > 1250 && m_Player->GetShape().getPosition().y < 1300)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 200));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 50));

            m_BGSandSurface.setColor(sf::Color(255, 255, 255, 200));
        }
        else if (m_Player->GetShape().getPosition().y > 1300 && m_Player->GetShape().getPosition().y < 1350)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 150));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 100));

            m_BGSandSurface.setColor(sf::Color(255, 255, 255, 150));
        }
        else if (m_Player->GetShape().getPosition().y > 1350 && m_Player->GetShape().getPosition().y < 1400)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 100));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 150));

            m_BGSandSurface.setColor(sf::Color(255, 255, 255, 100));
        }
        else if (m_Player->GetShape().getPosition().y > 1400 && m_Player->GetShape().getPosition().y < 1450)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 50));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 200));

            m_BGSandSurface.setColor(sf::Color(255, 255, 255, 50));
        }
        else if (m_Player->GetShape().getPosition().y > 1450)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 0));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 255));

            m_BGSandSurface.setColor(sf::Color(255, 255, 255, 0));
        }
        else if (m_RenderWindow->getView().getCenter().y > 1500)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 0));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 255));

            m_BGSandSurface.setColor(sf::Color(255, 255, 255, 0));
        }
        else
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 255));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 0));

            m_BGSandSurface.setColor(sf::Color(255, 255, 255, 255));
        }
    }
    else if (m_RenderWindow->getView().getCenter().y > 1500)
    {
        m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 0));
        m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 255));

        m_BGSandSurface.setColor(sf::Color(255, 255, 255, 0));
    }
    else
    {
        m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 255));
        m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 0));

        m_BGSandSurface.setColor(sf::Color(255, 255, 255, 255));
    }
}

void CWorldManager::InputWorldFromFiles(CTextureMaster* _textureMaster)
{
    m_GlobalMutex.lock();

    //
    // Reading From File??
    //
    std::ifstream xoutputs("Output/output_x.txt");
    int* xPositions = new int[15000]{};
    if (xoutputs.is_open())
    {
        for (int i = 0; i < 5000; i++)
        {
            xoutputs >> xPositions[i];
        }
        for (int i = 5000; i < 10000; i++)
        {
            xoutputs >> xPositions[i];
        }
        for (int i = 10000; i < 15000; i++)
        {
            xoutputs >> xPositions[i];
        }
        xoutputs.close();
    }

    std::ifstream youtputs("Output/output_y.txt");
    int* yPositions = new int[15000]{};
    if (youtputs.is_open())
    {
        for (int i = 0; i < 5000; i++)
        {
            youtputs >> yPositions[i];
        }
        for (int i = 5000; i < 10000; i++)
        {
            youtputs >> yPositions[i];
        }
        for (int i = 10000; i < 15000; i++)
        {
            youtputs >> yPositions[i];
        }
        youtputs.close();
    }

    for (int i = 0; i < 15000; i++)
    {
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Dirt, 50.0f, xPositions[i], yPositions[i], false, CBlock::BLOCKTYPE::DIRT);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;

        m_LoadTimer.restart();
    }
    
    std::cout << m_Chunk.size();
    
    m_GlobalMutex.unlock();
}

void CWorldManager::DrawBackGround(sf::Shader* _defaultShader)
{
    m_RenderWindow->draw(m_BGPlainsSurface, _defaultShader);
    m_RenderWindow->draw(m_BGPlainsUnderGr, _defaultShader);
    m_RenderWindow->draw(m_BGSandSurface, _defaultShader);
}

bool CWorldManager::IsObjectInBlock(sf::Sprite _shape)
{
    for (CBlock& block : m_Chunk)
    {
        if (block.GetShape().getGlobalBounds().intersects(_shape.getGlobalBounds()))
        {
            return true;
            break;
        }
    }
    return false;
}

bool CWorldManager::PositionIsBlock(sf::Vector2f _pos)
{
    for (CBlock& block : m_Chunk)
    {
        if (block.GetShape().getGlobalBounds().contains(_pos) || block.GetShape().getGlobalBounds().contains(sf::Vector2f(_pos.x - 50, _pos.y - 50)) || block.GetShape().getGlobalBounds().contains(sf::Vector2f(_pos.x + 50, _pos.y + 50)) || block.GetShape().getGlobalBounds().contains(sf::Vector2f(_pos.x - 50, _pos.y + 50)) || block.GetShape().getGlobalBounds().contains(sf::Vector2f(_pos.x + 50, _pos.y - 50)))
        {
            return true;
        }
    }
    return false;
}

void CWorldManager::CreateDungeon(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners)
{
    for (int y = 0; y < m_GenerateOffsetY; y ++)
    {
        for (int x = -m_GenerateOffsetX - 70; x < -m_GenerateOffsetX; x++)
        {
            if (x < -m_GenerateOffsetX - 50 && x > -m_GenerateOffsetX - 56 && y < 20)
            {
                if (y % 5 == 0 && (x == -m_GenerateOffsetX - 51 || x == -m_GenerateOffsetX - 55))
                {
                    // Obsidan "Steps"
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::OBSIDIAN);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = x + y;
                    m_Block = nullptr;
                }
            }
            // Hallway
            else if (x < -m_GenerateOffsetX - 16 && x > -m_GenerateOffsetX - 56 && y >= 20 && y < 26)
            {
                if (y == 22 && (x == -m_GenerateOffsetX - 55))
                {
                    // Obsidan "Steps"
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::OBSIDIAN);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = x + y;
                    m_Block = nullptr;
                }

                if (y == 25 && (x * 100) % 1500 == 0)
                {
                    m_Chest = new CChest(m_RenderWindow, *m_World, 50.0f, x * 100 + 10, y * 100);
                    m_Chest->SetSizeAndPos(x * 100 + 10, y * 100, 100, 100);

                    srand(x + m_Chest->m_Inventory.size());
                    // ITEM RARITIES
                    if (rand() % 1 == 0)
                    {
                        m_Block = new CBlock(_textureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
                        for (int i = 0; i < 0 + rand() % 3; i++)
                        {
                            m_Chest->AddItemToInventory(m_Block, 0, true);
                        }
                        m_Block = nullptr;
                    }

                    srand(x + m_Chest->m_Inventory.size());
                    if (rand() % 1 == 0)
                    {
                        m_Block = new CBlock(_textureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
                        for (int i = 0; i < 0 + rand() % 3; i++)
                        {
                            m_Chest->AddItemToInventory(m_Block, 1, true);
                        }
                        m_Block = nullptr;
                    }

                    srand(x + m_Chest->m_Inventory.size());
                    if (rand() % 1 == 0)
                    {
                        m_Potion = new CPotion(CBlock::POTIONTYPE::HPLARGE);
                        for (int i = 0; i < 0 + rand() % 3; i++)
                        {
                            m_Chest->AddItemToInventory(m_Potion, 2, true);
                        }
                        m_Potion = nullptr;
                    }

                    srand(x + m_Chest->m_Inventory.size());
                    if (rand() % 1 == 0)
                    {
                        CProjectile* m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::CURSEDARROW);
                        for (int i = 0; i < 0 + rand() % 4; i++)
                        {
                            m_Chest->AddItemToInventory(m_Projectile, 4, true);
                        }
                        m_Projectile = nullptr;
                    }

                    srand(x + m_Chest->m_Inventory.size());
                    if (rand() % 1 == 0)
                    {
                        CProjectile* m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::FIREARROW);
                        for (int i = 0; i < 0 + rand() % 4; i++)
                        {
                            m_Chest->AddItemToInventory(m_Projectile, 5, true);
                        }
                        m_Projectile = nullptr;
                    }

                    srand(x + m_Chest->m_Inventory.size());
                    if (rand() % 1 == 0)
                    {
                        CProjectile* m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::IRONBULLET);
                        for (int i = 0; i < 0 + rand() % 7; i++)
                        {
                            m_Chest->AddItemToInventory(m_Projectile, 12, true);
                        }
                        m_Projectile = nullptr;
                    }

                    srand(x + m_Chest->m_Inventory.size());
                    if (rand() % 3 == 0)
                    {
                        Bow* m_Bow = new Bow(CBlock::BOWTYPE::IRONGUN);
                        for (int i = 0; i < 1; i++)
                        {
                            m_Chest->AddItemToInventory(m_Bow, 14, true);
                        }
                        m_Bow = nullptr;
                    }

                    m_Chests.push_back(*m_Chest);
                    m_Chest = nullptr;
                }

                if (y == 22 && (x * 100) % 1700 == 0)
                {
                    Spawner* m_SlimeSpawner = new Spawner(_audioManager, m_RenderWindow, *m_World, _textureMaster, Utils::m_Scale, x * 100 + 10, y * 100, m_Player, CEnemy::ENEMYTYPE::SLIME, m_Shader, m_TourchShader, false);
                    m_SlimeSpawner->ToggleSpawning();
                    m_SlimeSpawner->SetSpawnCount(2);
                    _spawners.push_front(*m_SlimeSpawner);
                    m_SlimeSpawner = nullptr;
                }
            }
            else if (x < -m_GenerateOffsetX - 16 && x > -m_GenerateOffsetX - 22 && y >= 26 && y < 46)
            {
                if (y % 5 == 0 && (x == -m_GenerateOffsetX - 17 || x == -m_GenerateOffsetX - 21))
                {
                    // Obsidan "Steps"
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::OBSIDIAN);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = x + y;
                    m_Block = nullptr;
                }
            }
            // Hallway
            else if (x < -m_GenerateOffsetX - 16 && x > -m_GenerateOffsetX - 56 && y >= 46 && y < 52)
            {
                if (y == 47 && (x == -m_GenerateOffsetX - 55))
                {
                    // Obsidan "Steps"
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::OBSIDIAN);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = x + y;
                    m_Block = nullptr;
                }

                if (y == 51 && (x * 100) % 1500 == 0)
                {
                    m_Chest = new CChest(m_RenderWindow, *m_World, 50.0f, x * 100 + 10, y * 100);
                    m_Chest->SetSizeAndPos(x * 100 + 10, y * 100, 100, 100);

                    srand(x + m_Chest->m_Inventory.size());
                    // ITEM RARITIES
                    if (rand() % 1 == 0)
                    {
                        m_Block = new CBlock(_textureMaster->m_IronIngot, CBlock::BLOCKTYPE::IRONINGOT);
                        for (int i = 0; i < 0 + rand() % 3; i++)
                        {
                            m_Chest->AddItemToInventory(m_Block, 0, true);
                        }
                        m_Block = nullptr;
                    }

                    srand(x + m_Chest->m_Inventory.size());
                    if (rand() % 1 == 0)
                    {
                        m_Block = new CBlock(_textureMaster->m_GoldIngot, CBlock::BLOCKTYPE::GOLDINGOT);
                        for (int i = 0; i < 0 + rand() % 3; i++)
                        {
                            m_Chest->AddItemToInventory(m_Block, 1, true);
                        }
                        m_Block = nullptr;
                    }

                    srand(x + m_Chest->m_Inventory.size());
                    if (rand() % 1 == 0)
                    {
                        m_Potion = new CPotion(CBlock::POTIONTYPE::HPLARGE);
                        for (int i = 0; i < 0 + rand() % 3; i++)
                        {
                            m_Chest->AddItemToInventory(m_Potion, 2, true);
                        }
                        m_Potion = nullptr;
                    }

                    srand(x + m_Chest->m_Inventory.size());
                    if (rand() % 1 == 0)
                    {
                        CProjectile* m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::CURSEDARROW);
                        for (int i = 0; i < 0 + rand() % 4; i++)
                        {
                            m_Chest->AddItemToInventory(m_Projectile, 4, true);
                        }
                        m_Projectile = nullptr;
                    }

                    srand(x + m_Chest->m_Inventory.size());
                    if (rand() % 1 == 0)
                    {
                        CProjectile* m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::FIREARROW);
                        for (int i = 0; i < 0 + rand() % 4; i++)
                        {
                            m_Chest->AddItemToInventory(m_Projectile, 5, true);
                        }
                        m_Projectile = nullptr;
                    }

                    srand(x + m_Chest->m_Inventory.size());
                    if (rand() % 1 == 0)
                    {
                        CProjectile* m_Projectile = new CProjectile(CBlock::PROJECTILETYPE::IRONBULLET);
                        for (int i = 0; i < 0 + rand() % 7; i++)
                        {
                            m_Chest->AddItemToInventory(m_Projectile, 12, true);
                        }
                        m_Projectile = nullptr;
                    }

                    srand(x + m_Chest->m_Inventory.size());
                    if (rand() % 2 == 0)
                    {
                        Bow* m_Bow = new Bow(CBlock::BOWTYPE::IRONGUN);
                        for (int i = 0; i < 0 + rand() % 2; i++)
                        {
                            m_Chest->AddItemToInventory(m_Bow, 14, true);
                        }
                        m_Bow = nullptr;
                    }

                    m_Chests.push_back(*m_Chest);
                    m_Chest = nullptr;
                }

                if (y == 48 && (x * 100) % 1700 == 0)
                {
                    Spawner* m_SlimeSpawner = new Spawner(_audioManager, m_RenderWindow, *m_World, _textureMaster, Utils::m_Scale, x * 100 + 10, y * 100, m_Player, CEnemy::ENEMYTYPE::SLIME, m_Shader, m_TourchShader, false);
                    m_SlimeSpawner->ToggleSpawning();
                    m_SlimeSpawner->SetSpawnCount(2);
                    _spawners.push_front(*m_SlimeSpawner);
                    m_SlimeSpawner = nullptr;
                }
            }
            else if (x < -m_GenerateOffsetX - 50 && x > -m_GenerateOffsetX - 56 && y >= 52 && y < 92)
            {
                if (y % 5 == 0 && (x == -m_GenerateOffsetX - 51 || x == -m_GenerateOffsetX - 55))
                {
                    // Obsidan "Steps"
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::OBSIDIAN);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = x + y;
                    m_Block = nullptr;
                }
            }
            else if (x < -m_GenerateOffsetX - 46 && x > -m_GenerateOffsetX - 56 && y >= 92 && y < 98)
            {
                if (y == 94 && (x == -m_GenerateOffsetX - 55))
                {
                    // Obsidan "Steps"
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::OBSIDIAN);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = x + y;
                    m_Block = nullptr;
                }

                
            }
            else if (x < -m_GenerateOffsetX - 6 && x >= -m_GenerateOffsetX - 46 && y >= 62 && y < 98)
            {
                if (x == -m_GenerateOffsetX - 26 && y == 72)
                {
                    Spawner* m_SlimeSpawner = new Spawner(_audioManager, m_RenderWindow, *m_World, _textureMaster, Utils::m_Scale, x * 100 + 10, y * 100, m_Player, CEnemy::ENEMYTYPE::SLIME, m_Shader, m_TourchShader, true);
                    m_SlimeSpawner->ToggleSpawning();
                    m_SlimeSpawner->SetSpawnCount(1);
                    m_SlimeSpawner->m_bCanSpawnBoss = true;
                    _spawners.push_front(*m_SlimeSpawner);
                    m_SlimeSpawner = nullptr;

                    std::cout << " Created Boss Spawner " << _spawners.front().m_Shape.getPosition().x << " " << _spawners.front().m_Shape.getPosition().y;
                }


            }
            else
            {
                // Obsidan "Steps"
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, x * 100 + 10, y * 100, false, CBlock::BLOCKTYPE::OBSIDIAN);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = x + y;
                m_Block = nullptr;
            }
            

        }
    }   
}

void CWorldManager::CreateSurfaceSpawners(CTextureMaster* _textureMaster, CAudioManager* _audioManager, std::list<Spawner>& _spawners)
{
    Spawner* m_SlimeSpawner = new Spawner(_audioManager, m_RenderWindow, *m_World, _textureMaster, Utils::m_Scale, 0, -2000, m_Player, CEnemy::ENEMYTYPE::SLIME, m_Shader, m_TourchShader, false);
    m_SlimeSpawner->ToggleSpawning();
    m_SlimeSpawner->SetSpawnCount(1 + rand() % 3);
    _spawners.push_back(*m_SlimeSpawner);
    m_SlimeSpawner = nullptr;

    m_SlimeSpawner = new Spawner(_audioManager, m_RenderWindow, *m_World, _textureMaster, Utils::m_Scale, 8000, -2000, m_Player, CEnemy::ENEMYTYPE::SLIME, m_Shader, m_TourchShader, false);
    m_SlimeSpawner->ToggleSpawning();
    m_SlimeSpawner->SetSpawnCount(1 + rand() % 3);
    _spawners.push_back(*m_SlimeSpawner);
    m_SlimeSpawner = nullptr;

    m_SlimeSpawner = new Spawner(_audioManager, m_RenderWindow, *m_World, _textureMaster, Utils::m_Scale, 16000, -2000, m_Player, CEnemy::ENEMYTYPE::SLIME, m_Shader, m_TourchShader, false);
    m_SlimeSpawner->ToggleSpawning();
    m_SlimeSpawner->SetSpawnCount(1 + rand() % 3);
    _spawners.push_back(*m_SlimeSpawner);
    m_SlimeSpawner = nullptr;

    m_SlimeSpawner = new Spawner(_audioManager, m_RenderWindow, *m_World, _textureMaster, Utils::m_Scale, 24000, -2000, m_Player, CEnemy::ENEMYTYPE::SLIME, m_Shader, m_TourchShader, false);
    m_SlimeSpawner->ToggleSpawning();
    m_SlimeSpawner->SetSpawnCount(1 + rand() % 3);
    _spawners.push_back(*m_SlimeSpawner);
    m_SlimeSpawner = nullptr;

    m_SlimeSpawner = new Spawner(_audioManager, m_RenderWindow, *m_World, _textureMaster, Utils::m_Scale, -8000, -2000, m_Player, CEnemy::ENEMYTYPE::SLIME, m_Shader, m_TourchShader, false);
    m_SlimeSpawner->ToggleSpawning();
    m_SlimeSpawner->SetSpawnCount(1 + rand() % 3);
    _spawners.push_back(*m_SlimeSpawner);
    m_SlimeSpawner = nullptr;

    m_SlimeSpawner = new Spawner(_audioManager, m_RenderWindow, *m_World, _textureMaster, Utils::m_Scale, -16000, -2000, m_Player, CEnemy::ENEMYTYPE::SLIME, m_Shader, m_TourchShader, false);
    m_SlimeSpawner->ToggleSpawning();
    m_SlimeSpawner->SetSpawnCount(1 + rand() % 3);
    _spawners.push_back(*m_SlimeSpawner);
    m_SlimeSpawner = nullptr;

    m_SlimeSpawner = new Spawner(_audioManager, m_RenderWindow, *m_World, _textureMaster, Utils::m_Scale, -24000, -2000, m_Player, CEnemy::ENEMYTYPE::SLIME, m_Shader, m_TourchShader, false);
    m_SlimeSpawner->ToggleSpawning();
    m_SlimeSpawner->SetSpawnCount(1 + rand() % 3);
    _spawners.push_back(*m_SlimeSpawner);
    m_SlimeSpawner = nullptr;
}

void CWorldManager::GenerateNoise()
{
    for (int y = 0; y < _NOISEHEIGHT_; y++)
    {
        for (int x = 0; x < _NOISEWIDTH_; x++)
        {
            m_Noise[y][x] = (rand() % 32768) / 32768.0; // Random 0-1
        }
    } 
}

double CWorldManager::SmoothNoise(double _x, double _y)
{
    // Get Fractional Part Of X And Y
    double fractX = _x - int(_x);
    double fractY = _y - int(_y);

    // Wrap Around
    int x1 = (int(_x) + _NOISEWIDTH_) % _NOISEWIDTH_;
    int y1 = (int(_y) + _NOISEHEIGHT_) % _NOISEHEIGHT_;

    // Neighbors
    int x2 = (x1 + _NOISEWIDTH_ - 1) % _NOISEWIDTH_;
    int y2 = (y1 + _NOISEHEIGHT_ - 1) % _NOISEHEIGHT_;

    // Smooth The Noise Eith Bilinear Interpolation
    double value = 0.0;
    value += fractX * fractY * m_Noise[y1][x1];
    value += (1 - fractX) * fractY * m_Noise[y1][x2];
    value += fractX * (1 - fractY) * m_Noise[y2][x1];
    value += (1 - fractX) * (1 - fractY) * m_Noise[y2][x2];

    // Return Noise Value
    return value;
}

double CWorldManager::Turbulence(double _x, double _y, double _size)
{
    double value = 0.0f;
    double initialSize = _size;

    while (_size >= 1)
    {
        value += SmoothNoise(_x / _size, _y / _size) * _size;
        _size /= 2.0;
    }

    // Return Noise Value
    return(128.0 * value / initialSize);
}
