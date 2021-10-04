#pragma once
#include "Enemy.h"

class CWizard : public CEnemy
{
public:
	CWizard(sf::RenderWindow* _renderWindow, b2World& _world, CTextureMaster* _textureMaster, const float& _scale, float _posX, float _posY, CAudioManager& _audioManager);
	virtual ~CWizard();

	virtual void Update();
	virtual void Movement();
	virtual void Attack();

	virtual void TakeDamage(float _damage, bool _projectile = false);

	virtual void CreateBody(float _posX, float _posY, b2BodyType _type, bool _sensor = false);


private:

};

