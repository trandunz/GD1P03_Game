#include "Enemy.h"

CEnemy::CEnemy()
{
	m_Body = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
}

CEnemy::~CEnemy()
{
	if (m_World != nullptr && m_Body != nullptr)
	{
		m_World->DestroyBody(m_Body);
		//std::cout << "Destructing Block" << std::endl;
	}
	m_Body = nullptr;
	m_World = nullptr;
	m_RenderWindow = nullptr;
	m_TextureMaster = nullptr;
}

void CEnemy::Start()
{
}

void CEnemy::Update()
{
	// Set SFML Shape Transform To Box 2D Body Transform
	m_Shape.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Shape.setRotation(m_Body->GetAngle() * 180 / b2_pi);
}

void CEnemy::Render()
{
	m_RenderWindow->draw(m_Shape);
}

void CEnemy::Movement()
{
}
