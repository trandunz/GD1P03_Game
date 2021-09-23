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
    m_BGPlainsUnderGr.setTextureRect(sf::IntRect(0, 0, 40000, 40000));
    m_BGPlainsUnderGr.setOrigin(m_BGPlainsUnderGr.getGlobalBounds().width / 2, m_BGPlainsUnderGr.getGlobalBounds().height / 2);
    m_BGPlainsUnderGr.setColor(sf::Color::Transparent);
    m_BGPlainsUnderGr.setPosition(0,0);
    

    // World
    CreateSkyChunk(_textureMaster);
    CreateSurfaceLayerPart1(_textureMaster);
    CreateSurfaceLayerPart2(_textureMaster);
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
}

void CWorldManager::Update(sf::Event& _event, sf::Vector2f _mousePos)
{

    for (sf::Sprite& sky : m_SkyChunk)
    {
        if (sky.getGlobalBounds().contains(_mousePos))
        {
            m_GUI->m_MousePos.setPosition(sky.getPosition());
        }
        if (sky.getGlobalBounds().contains(m_GUI->m_MousePos.getPosition()))
        {
            m_GUI->m_MousePos.setPosition(sky.getPosition());
        }
    }

    for (CBlock& block : m_Chunk)
    {
        block.Update();
        if (block.GetShape().getGlobalBounds().contains(_mousePos))
        {
            m_GUI->m_MousePos.setPosition(block.GetShape().getPosition());
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
        if (furnace.GetShape().getGlobalBounds().contains(_mousePos))
        {
            m_GUI->m_MousePos.setPosition(furnace.GetShape().getPosition());
        }

        // Distance To Player
        float Mag = 0;
        if (m_Player != nullptr)
        {
            Mag = sqrt(((m_Player->GetShape().getPosition().x - furnace.GetShape().getPosition().x) * (m_Player->GetShape().getPosition().x - furnace.GetShape().getPosition().x)) + ((m_Player->GetShape().getPosition().y - furnace.GetShape().getPosition().y) * (m_Player->GetShape().getPosition().y - furnace.GetShape().getPosition().y)));
        }
        else
        {
            Mag = 200;
        }
        
        if (Mag < 200 && m_Player->m_bInventoryOpen)
        {
            m_GUI->m_bCanSmelt = true;
            break;
        }
        else
        {
            m_GUI->m_bCanSmelt = false;
        }
    }
}

void CWorldManager::Render(sf::Shader* _defaultShader)
{
    // World BackGround
    m_BGPlainsSurface.setPosition(m_RenderWindow->getView().getCenter());
    if (m_Player != nullptr)
    {
        if (m_Player->GetShape().getPosition().y > 1500 && m_Player->GetShape().getPosition().y < 1550)
        {
            m_BGPlainsSurface.setColor(sf::Color(255,255,255,255));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 0));
        }
        else if (m_Player->GetShape().getPosition().y > 1550 && m_Player->GetShape().getPosition().y < 1600)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 200));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50,50));
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
        else if (m_Player->GetShape().getPosition().y > 1700 && m_Player->GetShape().getPosition().y< 1750)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 50));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 200));
        }
        else if (m_Player->GetShape().getPosition().y > 1750)
        {
            m_BGPlainsSurface.setColor(sf::Color(255, 255, 255, 0));
            m_BGPlainsUnderGr.setColor(sf::Color(50, 50, 50, 255));
        }
    }
    m_RenderWindow->draw(m_BGPlainsSurface);
    m_RenderWindow->draw(m_BGPlainsUnderGr);

    // Surface Shader
    m_SurfaceShader->setUniform("hasTexture", true);
    m_SurfaceShader->setUniform("lightPos", sf::Vector2f(0, -500));

    if (m_Player != nullptr)
    {
        // Player Light
        m_Shader->setUniform("hasTexture", true);
        m_Shader->setUniform("lightPos", m_Player->GetShape().getPosition());

        // Block It
        std::list<CBlock>::iterator it;
        // Draw Blocks In Range (With Player Shader)
        for (it = m_Chunk.begin(); it != m_Chunk.end(); it++)
        {
            float Mag1 = sqrt(((it->GetShape().getPosition().x - m_Player->GetShape().getPosition().x) * (it->GetShape().getPosition().x - m_Player->GetShape().getPosition().x)) + ((it->GetShape().getPosition().y - m_Player->GetShape().getPosition().y) * (it->GetShape().getPosition().y - m_Player->GetShape().getPosition().y)));
            if (Mag1 < 1920 * 1.8f && it->GetShape().getPosition().y >= 1300)
            {

                if (bIsBlockInRangeOfLightSource(it))
                {
                    
                    m_RenderWindow->draw(it->GetShape(), m_TourchShader);
                }
                else
                {
                    m_RenderWindow->draw(it->GetShape(), m_Shader);
                }
            }
            else if (Mag1 < 1920 * 1.8f && it->GetShape().getPosition().y <= 1450 && it->GetShape().getPosition().y >= 400)
            {
                if (m_Player->GetShape().getPosition().y >= 1200)
                {
                    m_RenderWindow->draw(it->GetShape(), m_Shader);
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
        }

        // Doors E.t.c
        for (CDoor& door : m_Doors)
        {
            door.Render();
        }
        for (CChest& chest : m_Chests)
        {
            chest.Render();
        }
        for (CFurnace& furnace : m_Furnaces)
        {
            furnace.Render();
        }
    }
    else
    {
        // Block It
        std::list<CBlock>::iterator it;
        // Blocks In Range
        for (it = m_Chunk.begin(); it != m_Chunk.end(); it++)
        {
            float Mag1 = 0;
            if (Mag1 < 1920 * 1.8f && it->GetShape().getPosition().y > 1300)
            {
                m_RenderWindow->draw(it->GetShape(), m_Shader);
            }
            else if (Mag1 < 1920 * 1.8f && it->GetShape().getPosition().y <= 1300 && it->GetShape().getPosition().y >= 400)
            {
                if (m_RenderWindow->getView().getCenter().y > 1250)
                {
                    m_RenderWindow->draw(it->GetShape(), m_Shader);
                }
                else
                {
                    m_RenderWindow->draw(it->GetShape(), m_SurfaceShader);
                }
                
            }
            else if (Mag1 < 1920 * 1.8f && it->GetShape().getPosition().y < 400 && it->GetShape().getPosition().y > -400)
            {
                m_RenderWindow->draw(it->GetShape(), m_SurfaceShader);
            }
            else if (Mag1 < 1920 * 1.8f)
            {
                m_RenderWindow->draw(it->GetShape(), _defaultShader);
            }
        }

        // Doors E.t.c
        for (CDoor& door : m_Doors)
        {
            door.Render();
        }
        for (CChest& chest : m_Chests)
        {
            chest.Render();
        }
        for (CFurnace& furnace : m_Furnaces)
        {
            furnace.Render();
        }
    }

    
}

void CWorldManager::CreateSkyChunk(CTextureMaster* _textureMaster)
{
    sf::Vector2f playerPos;
    if (m_Player != nullptr)
    {
        playerPos = sf::Vector2f(m_Player->GetShape().getPosition().x, m_Player->GetShape().getPosition().y);
    }
    else
    {
        playerPos = sf::Vector2f(0, 0);
    }
    
    m_SkyChunk.clear();

    if (m_Player != nullptr)
    {
        for (int i = 0; i > -20000; i -= 100)
        {
            for (int j = 0; j > -20000; j -= 100)
            {
                float Mag2 = sqrt(((sf::Vector2f(i, j).x - playerPos.x) * (sf::Vector2f(i, j).x - playerPos.x)) + ((sf::Vector2f(i, j).y - playerPos.y) * (sf::Vector2f(i, j).y - playerPos.y)));
                if (Mag2 < m_RenderWindow->getSize().x * 1.5)
                {
                    sf::Sprite sky = sf::Sprite();
                    sky.setColor(sf::Color::Transparent);
                    sky.setTexture(*_textureMaster->m_Stone);
                    sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
                    sky.setPosition(sf::Vector2f(i + 10, j));
                    m_SkyChunk.push_front(sky);
                }
            }
            for (int j = 0; j < 20000; j += 100)
            {
                float Mag2 = sqrt(((sf::Vector2f(i, j).x - playerPos.x) * (sf::Vector2f(i, j).x - playerPos.x)) + ((sf::Vector2f(i, j).y - playerPos.y) * (sf::Vector2f(i, j).y - playerPos.y)));
                if (Mag2 < m_RenderWindow->getSize().x * 1.5)
                {
                    sf::Sprite sky = sf::Sprite();
                    if (j > 1450)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255));
                    }
                    else if (j > 1475)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255 - (31.875f * 2)));
                    }
                    else if (j > 1500)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255 - (31.875f * 3)));
                    }
                    else if (j > 1625)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255 - (31.875f * 4)));
                    }
                    else if (j > 1650)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255 - (31.875f * 5)));
                    }
                    else if (j > 1675)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255 - (31.875f * 6)));
                    }
                    else if (j > 1700)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255 - (31.875f * 7)));
                    }
                    else if (j > 1725)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 0));
                    }
                    else
                    {
                        sky.setColor(sf::Color::Transparent);
                    }
                    sky.setTexture(*_textureMaster->m_Stone);
                    sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
                    sky.setPosition(sf::Vector2f(i + 10, j));
                    m_SkyChunk.push_front(sky);
                }
            }
        }

        for (int i = 0; i < 20000; i += 100)
        {
            for (int j = 0; j > -20000; j -= 100)
            {
                float Mag2 = sqrt(((sf::Vector2f(i, j).x - playerPos.x) * (sf::Vector2f(i, j).x - playerPos.x)) + ((sf::Vector2f(i, j).y - playerPos.y) * (sf::Vector2f(i, j).y - playerPos.y)));
                if (Mag2 < m_RenderWindow->getSize().x * 1.5)
                {
                    sf::Sprite sky = sf::Sprite();
                    sky.setColor(sf::Color::Transparent);
                    sky.setTexture(*_textureMaster->m_Stone);
                    sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
                    sky.setPosition(sf::Vector2f(i + 10, j));
                    m_SkyChunk.push_front(sky);
                }
            }
            for (int j = 0; j < 20000; j += 100)
            {
                float Mag2 = sqrt(((sf::Vector2f(i, j).x - playerPos.x) * (sf::Vector2f(i, j).x - playerPos.x)) + ((sf::Vector2f(i, j).y - playerPos.y) * (sf::Vector2f(i, j).y - playerPos.y)));
                if (Mag2 < m_RenderWindow->getSize().x * 1.5)
                {
                    sf::Sprite sky = sf::Sprite();
                    if (j > 1450)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255));
                    }
                    else if (j > 1475)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255 - (31.875f * 2)));
                    }
                    else if (j > 1500)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255 - (31.875f * 3)));
                    }
                    else if (j > 1625)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255 - (31.875f * 4)));
                    }
                    else if (j > 1650)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255 - (31.875f * 5)));
                    }
                    else if (j > 1675)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255 - (31.875f * 6)));
                    }
                    else if (j > 1700)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 255 - (31.875f * 7)));
                    }
                    else if (j > 1725)
                    {
                        sky.setColor(sf::Color(40, 40, 40, 0));
                    }
                    else
                    {
                        sky.setColor(sf::Color::Transparent);
                    }
                    sky.setTexture(*_textureMaster->m_Stone);
                    sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
                    sky.setPosition(sf::Vector2f(i + 10, j));
                    m_SkyChunk.push_front(sky);
                }
            }
        }
    }
    
}

void CWorldManager::CreateSurfaceLayerPart1(CTextureMaster* _textureMaster)
{
    GlobalMutex.lock();
    for (int i = 10; i < 31400 / 1.9f; i += 100)
    {
        // Gap Edges Right
        if (i > 4800 && i < 5000)
        {
            for (int j = 500; j < 21400 / 1.9f; j += 100)
            {
                // Cave
                if (j > 9200 && j < 10100)
                {
                }
                else
                {
                    if (rand() % i <= 3000)
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::STONE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                    else if (rand() % i <= 1000)
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Coal, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::COALORE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                    else if (rand() % i <= 800)
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_IronOre, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::IRONORE
                        );
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                }
            }
        }
        // Gap Edges Left
        else if (i > 4000 && i < 4200)
        {
            for (int j = 500; j < 21400 / 1.9f; j += 100)
            {
                // Cave
                if (j > 9200 && j < 10100)
                {
                }
                else
                {
                    if (rand() % i <= 3000)
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::STONE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                    else if (rand() % i <= 1000)
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Coal, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::COALORE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                    else if (rand() % i <= 800)
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_IronOre, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::IRONORE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                }
                
            }
        }
        // Gap
        else if (i > 4200 && i < 4800)
        {
        }
        else
        {
            // Trees
            if (rand() % 5 == 0)
            {
                // Trunk
                if (true)
                {
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, i, 300, true, CBlock::BLOCKTYPE::WOOD);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, i, 200, true, CBlock::BLOCKTYPE::WOOD);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, i, 100, true, CBlock::BLOCKTYPE::WOOD);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, i, 000, true, CBlock::BLOCKTYPE::WOOD);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, i, -100, true, CBlock::BLOCKTYPE::WOOD);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, i, -200, true, CBlock::BLOCKTYPE::WOOD);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                }
                // Leaves
                if (true)
                {
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i, -300, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i, -400, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i, -500, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i, -600, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;

                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 100, -300, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 200, -300, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i - 100, -300, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i - 200, -300, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 300, -300, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i - 300, -300, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;

                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 100, -400, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 200, -400, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i - 100, -400, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i - 200, -400, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;

                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 100, -500, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 200, -500, true, CBlock::BLOCKTYPE::LEAVES);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                }
            }

            // Grass
            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Grass, Utils::m_Scale, i, 400, false, CBlock::BLOCKTYPE::GRASS);

            //m_Block->SetSize(100, 100);
            m_Chunk.push_back(*m_Block);
            m_Block->m_ArrayIndex = i;
            m_Block = nullptr;
            
            // Underground And Ores
            for (int j = 500; j < 21400 / 1.9f; j += 100)
            {
                // Cave
                if (j > 9300 && j < 10100 && i > 4200)
                {
                    if (rand() % i <= 3000 && (j == 9400 || j == 10000))
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::STONE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                    else if (rand() % i <= 1000 && (j == 9400 || j == 10000))
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Coal, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::COALORE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                    else if (rand() % i <= 800 && (j == 9400 || j == 10000))
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_IronOre, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::IRONORE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                }
                // World
                else
                {
                    if ((rand() % j < 1000))
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Dirt, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::DIRT);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                    else
                    {
                        if (rand() % 64 == 0)
                        {
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Coal, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::COALORE);
                            //m_Block->SetSize(100, 100);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = i;
                            m_Block = nullptr;
                        }
                        else if ((rand() % j < 6000) && (rand() % j > 3000))
                        {
                            if (rand() % 16 == 0)
                            {
                                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_IronOre, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::IRONORE);
                                //m_Block->SetSize(100, 100);
                                m_Chunk.push_back(*m_Block);
                                m_Block->m_ArrayIndex = i;
                                m_Block = nullptr;
                            }
                            else
                            {
                                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::STONE);
                                //m_Block->SetSize(100, 100);
                                m_Chunk.push_back(*m_Block);
                                m_Block->m_ArrayIndex = i;
                                m_Block = nullptr;
                            }

                        }
                        else if ((rand() % j < 8000) && (rand() % j > 7000))
                        {

                            if (rand() % 45 == 0)
                            {
                                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_DiamondOre, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::DIAMONDORE);
                                //m_Block->SetSize(100, 100);
                                m_Chunk.push_back(*m_Block);
                                m_Block->m_ArrayIndex = i;
                                m_Block = nullptr;

                            }
                            else if (rand() % 28 == 0)
                            {
                                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_GoldOre, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::GOLDORE);
                                //m_Block->SetSize(100, 100);
                                m_Chunk.push_back(*m_Block);
                                m_Block->m_ArrayIndex = i;
                                m_Block = nullptr;
                            }
                            else
                            {
                                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::STONE);
                                //m_Block->SetSize(100, 100);
                                m_Chunk.push_back(*m_Block);
                                m_Block->m_ArrayIndex = i;
                                m_Block = nullptr;
                            }
                        }
                        else
                        {
                            m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::STONE);
                            //m_Block->SetSize(100, 100);
                            m_Chunk.push_back(*m_Block);
                            m_Block->m_ArrayIndex = i;
                            m_Block = nullptr;

                        }
                    }
                }
            }
        }
    }
    GlobalMutex.unlock();
}

void CWorldManager::CreateSurfaceLayerPart2(CTextureMaster* _textureMaster)
{
    GlobalMutex.lock();
    for (int i = -90; i > -31400 / 1.9f; i -= 100)
    {
        // Trees
        if (rand() % 5 == 0)
        {
            // Trunk
            if (true)
            {
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, i, 300, true, CBlock::BLOCKTYPE::WOOD);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, i, 200, true, CBlock::BLOCKTYPE::WOOD);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, i, 100, true, CBlock::BLOCKTYPE::WOOD);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, i, 000, true, CBlock::BLOCKTYPE::WOOD);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, i, -100, true, CBlock::BLOCKTYPE::WOOD);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Wood, Utils::m_Scale, i, -200, true, CBlock::BLOCKTYPE::WOOD);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
            }
            // Leaves
            if (true)
            {
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i, -300, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i, -400, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i, -500, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i, -600, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;

                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 100, -300, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 200, -300, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i - 100, -300, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i - 200, -300, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 300, -300, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i - 300, -300, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;

                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 100, -400, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 200, -400, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i - 100, -400, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i - 200, -400, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;

                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 100, -500, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Leaves, Utils::m_Scale, i + 200, -500, true, CBlock::BLOCKTYPE::LEAVES);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
            }
        }

        // Grass
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Grass, Utils::m_Scale, i, 400, false, CBlock::BLOCKTYPE::GRASS);

        //m_Block->SetSize(100, 100);
        m_Chunk.push_back(*m_Block);
        m_Block->m_ArrayIndex = i;
        m_Block = nullptr;

        // Underground And Ores
        for (int j = 500; j < 21400 / 1.9f; j += 100)
        {

            if ((rand() % j < 1000))
            {
                m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Dirt, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::DIRT);
                //m_Block->SetSize(100, 100);
                m_Chunk.push_back(*m_Block);
                m_Block->m_ArrayIndex = i;
                m_Block = nullptr;
            }
            else
            {
                if (rand() % 64 == 0)
                {
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Coal, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::COALORE);
                    //m_Block->SetSize(100, 100);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                }
                else if ((rand() % j < 6000) && (rand() % j > 3000))
                {
                    if (rand() % 16 == 0)
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_IronOre, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::IRONORE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                    else
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::STONE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }

                }
                else if ((rand() % j < 8000) && (rand() % j > 7000))
                {

                    if (rand() % 45 == 0)
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_DiamondOre, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::DIAMONDORE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;

                    }
                    else if (rand() % 28 == 0)
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_GoldOre, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::GOLDORE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                    else
                    {
                        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::STONE);
                        //m_Block->SetSize(100, 100);
                        m_Chunk.push_back(*m_Block);
                        m_Block->m_ArrayIndex = i;
                        m_Block = nullptr;
                    }
                }
                else
                {
                    m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Stone, Utils::m_Scale, i, j, false, CBlock::BLOCKTYPE::STONE);
                    //m_Block->SetSize(100, 100);
                    m_Chunk.push_back(*m_Block);
                    m_Block->m_ArrayIndex = i;
                    m_Block = nullptr;
                    
                }
            }
            
        }
    }
    GlobalMutex.unlock();
}

void CWorldManager::CreateWorldBoundary(CTextureMaster* _textureMaster)
{
    GlobalMutex.lock();
    for (int i = -90; i > -31100 / 1.9f; i -= 100)
    {
        // Grass
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, i, -6000, false, CBlock::BLOCKTYPE::OBSIDIAN);

        //m_Block->SetSize(100, 100);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;
    }
    for (int i = -90; i > -31100 / 1.9f; i -= 100)
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
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, -31390 / 1.9f, j, false, CBlock::BLOCKTYPE::OBSIDIAN);

        //m_Block->SetSize(100, 100);
        m_Chunk.push_back(*m_Block);
        m_Block = nullptr;
    }

    for (int i = 10; i < 31400 / 1.9f; i += 100)
    {
        // Grass
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, i, -6000, false, CBlock::BLOCKTYPE::OBSIDIAN);

        //m_Block->SetSize(100, 100);
        m_Chunk.push_back(*m_Block);
        m_Block->m_ArrayIndex = i;
        m_Block = nullptr;
    }
    for (int i = 10; i < 31400 / 1.9f; i += 100)
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
        m_Block = new CBlock(m_RenderWindow, *m_World, _textureMaster->m_Obsidian, Utils::m_Scale, +31410 / 1.9f, j, false, CBlock::BLOCKTYPE::OBSIDIAN);

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

    for (sf::Sprite& sky : m_SkyChunk)
    {
        m_WorldTexture.draw(sky);
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

bool CWorldManager::bIsBlockInRangeOfLightSource(std::list<CBlock>::iterator _it)
{
    for (std::list<CFurnace>::iterator fit = m_Furnaces.begin(); fit != m_Furnaces.end(); fit++)
    {
        float Mag1 = 0;
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
    m_Player = _player;
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
