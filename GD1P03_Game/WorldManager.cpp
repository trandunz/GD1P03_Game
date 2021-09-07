#include "WorldManager.h"

CWorldManager::CWorldManager(sf::RenderWindow* _renderWindow, CPlayer* _player, b2World& _world, GUI* _gui)
{
    m_RenderWindow = _renderWindow;
    m_Player = _player;
    m_World = &_world;
    m_GUI = _gui;
    m_Block = nullptr;

    m_Chunk = std::list<CBlock>{};
}

CWorldManager::~CWorldManager()
{
    m_Chunk.clear();
    m_SkyChunk.clear();
    m_Chests.clear();
    m_Doors.clear();

    m_Block = nullptr;
    m_GUI = nullptr;
    m_Player = nullptr;
    m_RenderWindow = nullptr;
    m_World = nullptr;
}

void CWorldManager::Start()
{
    // World BackGround
    m_WorldBackGround = sf::Sprite();
    m_WorldBackGround.setTexture(*m_GUI->m_Sky, true);
    m_WorldBackGround.setOrigin(m_WorldBackGround.getGlobalBounds().width / 2, m_WorldBackGround.getGlobalBounds().height / 2);
    m_WorldBackGround.setScale(4.7, 4.7);

    CreateSkyChunk();
    CreateSurfaceLayerPart1();
    CreateSurfaceLayerPart2();
}

void CWorldManager::Update(sf::Event& _event, sf::Vector2f _mousePos)
{
    // Shapes
    //m_Player->Update(_mousePos, _event);
   

    for (sf::RectangleShape& sky : m_SkyChunk)
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

    // World Step
    m_World->Step(1 / 60.0f, 60, 60);
}

void CWorldManager::Render()
{
    // World BackGround
    m_WorldBackGround.setPosition(m_RenderWindow->getView().getCenter());
    m_RenderWindow->draw(m_WorldBackGround);

    std::list<CBlock>::iterator it;


    for (CBlock& block : m_Chunk)
    {
        block.Render();
    }
    for (CDoor& door : m_Doors)
    {
        door.Render();
    }
    for (CChest& chest : m_Chests)
    {
        chest.Render();
    }
   
}

void CWorldManager::CreateSkyChunk()
{
    sf::Vector2f playerPos = sf::Vector2f(m_Player->GetShape().getPosition().x, m_Player->GetShape().getPosition().y);
    m_SkyChunk.clear();
    for (int i = 0; i < 20000; i += 100)
    {
        for (int j = 0; j < 20000; j += 100)
        {
            float Mag2 = sqrt(((sf::Vector2f(i, j).x - playerPos.x) * (sf::Vector2f(i, j).x - playerPos.x)) + ((sf::Vector2f(i, j).y - playerPos.y) * (sf::Vector2f(i, j).y - playerPos.y)));
            if (Mag2 < m_RenderWindow->getSize().x * 1.80f)
            {
                sf::RectangleShape sky = sf::RectangleShape();
                sky.setFillColor(sf::Color::Transparent);
                sky.setSize(sf::Vector2f(100, 100));
                sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
                sky.setPosition(sf::Vector2f(i + 10, j));
                m_SkyChunk.push_front(sky);
                m_RenderWindow->draw(m_SkyChunk.front());
            }
        }
        for (int j = 0; j > -20000; j -= 100)
        {
            float Mag2 = sqrt(((sf::Vector2f(i, j).x - playerPos.x) * (sf::Vector2f(i, j).x - playerPos.x)) + ((sf::Vector2f(i, j).y - playerPos.y) * (sf::Vector2f(i, j).y - playerPos.y)));
            if (Mag2 < m_RenderWindow->getSize().x * 1.80f)
            {
                sf::RectangleShape sky = sf::RectangleShape();
                sky.setFillColor(sf::Color::Transparent);
                sky.setSize(sf::Vector2f(100, 100));
                sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
                sky.setPosition(sf::Vector2f(i + 10, j));
                m_SkyChunk.push_front(sky);
                m_RenderWindow->draw(m_SkyChunk.front());
            }

        }
    }
    for (int i = 0; i > -20000; i -= 100)
    {
        for (int j = 0; j > -20000; j -= 100)
        {
            float Mag2 = sqrt(((sf::Vector2f(i, j).x - playerPos.x) * (sf::Vector2f(i, j).x - playerPos.x)) + ((sf::Vector2f(i, j).y - playerPos.y) * (sf::Vector2f(i, j).y - playerPos.y)));
            if (Mag2 < m_RenderWindow->getSize().x * 1.80f)
            {
                sf::RectangleShape sky = sf::RectangleShape();
                sky.setFillColor(sf::Color::Transparent);
                sky.setSize(sf::Vector2f(100, 100));
                sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
                sky.setPosition(sf::Vector2f(i + 10, j));
                m_SkyChunk.push_front(sky);
                m_RenderWindow->draw(m_SkyChunk.front());
            }
        }
        for (int j = 0; j < 20000; j += 100)
        {
            float Mag2 = sqrt(((sf::Vector2f(i, j).x - playerPos.x) * (sf::Vector2f(i, j).x - playerPos.x)) + ((sf::Vector2f(i, j).y - playerPos.y) * (sf::Vector2f(i, j).y - playerPos.y)));
            if (Mag2 < m_RenderWindow->getSize().x * 1.80f)
            {
                sf::RectangleShape sky = sf::RectangleShape();
                sky.setFillColor(sf::Color::Transparent);
                sky.setSize(sf::Vector2f(100, 100));
                sky.setOrigin(sky.getGlobalBounds().width / 2, sky.getGlobalBounds().height / 2);
                sky.setPosition(sf::Vector2f(i + 10, j));
                m_SkyChunk.push_front(sky);
                m_RenderWindow->draw(m_SkyChunk.front());
            }
        }
    }
}

void CWorldManager::CreateSurfaceLayerPart1()
{
    GlobalMutex.lock();
    for (int i = 10; i < 21400 / 1.9f; i += 100)
    {
        m_Block = new CBlock(m_RenderWindow, *m_World, m_GUI->m_Grass, Utils::m_Scale, i, 400);
        //m_Block->SetSize(100, 100);
        m_Chunk.push_back(*m_Block);
        m_Block->m_ArrayIndex = i;
        m_Block = nullptr;

        for (int j = 500; j < 21400 / 1.9f; j += 100)
        {
            m_Block = new CBlock(m_RenderWindow, *m_World, m_GUI->m_Dirt, Utils::m_Scale, i, j);
            //m_Block->SetSize(100, 100);
            m_Chunk. push_back(*m_Block);
            m_Block->m_ArrayIndex = i;
            m_Block = nullptr;
        }
    }
    GlobalMutex.unlock();
}

void CWorldManager::CreateSurfaceLayerPart2()
{
    GlobalMutex.lock();
    for (int i = -90; i > -21100 / 1.9f; i -= 100)
    {
        m_Block = new CBlock(m_RenderWindow, *m_World, m_GUI->m_Grass, Utils::m_Scale, i, 400);
        //m_Block->SetSize(100, 100);
        m_Chunk.push_back(*m_Block);
        m_Block->m_ArrayIndex = i;
        m_Block = nullptr;

        for (int j = 500; j < 21400 / 1.9f; j += 100)
        {
            m_Block = new CBlock(m_RenderWindow, *m_World, m_GUI->m_Dirt, Utils::m_Scale, i, j);
            //m_Block->SetSize(100, 100);
            m_Chunk.push_back(*m_Block);
            m_Block->m_ArrayIndex = i;
            m_Block = nullptr;
        }
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
