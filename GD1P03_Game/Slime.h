#pragma once
#ifndef _SLIME_H__
#define _SLIME_H__

#include "Enemy.h"
class Slime : public CEnemy
{
public:
	enum class SLIMETYPE
	{
		GREEN,
		BLUE,
		RED,
		PURPLE,
		YELLOW,
		BOSSGREEN,
		BOSSBLUE,
		BOSSRED,
		BOSSPURPLE,
		BOSSYELLOW,
	};

	Slime(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CAudioManager& _audioManager);
	virtual ~Slime();

	virtual void Start();
	virtual void Update();
	void Render(sf::Shader* _shader = NULL);

	void SetPlayer(CPlayer* _player);
	void LoosePlayer();
	bool bHasPlayer();

	virtual void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);

	virtual void Movement();
	virtual void Attack();

	void TakeDamage(float _damage, bool _projectile = false);

	bool bIsSlimeInRangeOfLightSource(std::list<CFurnace>& _furnaces, sf::Shader* m_TourchShader)
	{
		float Mag1 = 0;

		for (std::list<CFurnace>::iterator fit = _furnaces.begin(); fit != _furnaces.end(); fit++)
		{
			Mag1 = sqrt(((m_Shape.getPosition().x - fit->GetShape().getPosition().x) * (m_Shape.getPosition().x - fit->GetShape().getPosition().x)) + ((m_Shape.getPosition().y - fit->GetShape().getPosition().y) * (m_Shape.getPosition().y - fit->GetShape().getPosition().y)));

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

	SLIMETYPE m_SlimeType = SLIMETYPE::GREEN;

	bool m_bIsBoss = false;
private:
	



	CPlayer* m_Player;

	bool m_bCanFallDamage = true;

	sf::Clock* m_DamageTimer;
};
#endif
