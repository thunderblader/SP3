#include "Enemy.h"

Enemy::Enemy()
	: enemyObj(nullptr)
	, defaultPos(0.f, 0.f, 0.f)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(GameObject* _enemyObj, GameObject::GAMEOBJECT_TYPE _type, Vector3 _pos, Vector3 _scale)
{
	enemyObj = _enemyObj;
	enemyObj->type = _type;
	enemyObj->pos = _pos;
	enemyObj->scale = _scale;
	enemyObj->mass = 1.f;
	enemyObj->vel.Set(0.f, 0.f, 0.f);
}

void Enemy::Update(double dt)
{
	if (!enemyObj)
		return;

	vector<GameObject*>::iterator it, end;
	end = projList.end();
	for (it = projList.begin(); it != end; ++it)
	{
		if (!(*it)->active)
			projList.erase(it);
	}
}

void Enemy::Exit()
{
}

void Enemy::Reset()
{
	enemyObj->pos = defaultPos;
}

void Enemy::PushProjectile(GameObject * _projObj, Vector3 _target, Vector3 _scale, float _spd)
{
	_projObj->type = GameObject::GO_BALL;	// Set type to projectile
	_projObj->pos = enemyObj->pos;
	_projObj->scale = _scale;
	_projObj->dir = (_target - _projObj->pos).Normalized() + Vector3(0.f, 1.f, 0.f);
	_projObj->normal.Set(0.f, 1.f, 0.f);
	_projObj->mass = 4.f;
	_projObj->vel = _projObj->dir * _spd;

	projList.push_back(_projObj);
}
