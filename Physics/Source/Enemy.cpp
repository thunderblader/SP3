#include "Enemy.h"
#include "Physics\Physics.h"
#include "Mesh.h"
#include "Physics\Collision.h"
#include "Terrain\LoadHmap.h"

GameObject* Enemy::playerObj = 0;
GameObject* Enemy::bombObj = 0;
vector<unsigned char>* Enemy::m_heightMap = 0;
float Enemy::m_terrainWidth = 0;
float Enemy::m_terrainHeight = 0;

Enemy::Enemy()
	: enemyObj(nullptr)
	, defaultPos(0.f, 0.f, 0.f)
	, spriteAnim(nullptr)
	, snowBall(nullptr)
	, projFired(false)
{
}

Enemy::~Enemy()
{
	if (spriteAnim)
	{
		delete spriteAnim;
	}
}

void Enemy::Init(GameObject* _enemyObj, GameObject::GAMEOBJECT_TYPE _type, Vector3 _pos, Vector3 _scale, float _mass)
{
	enemyObj = _enemyObj;
	enemyObj->type = _type;
	enemyObj->pos = _pos;
	defaultPos = _pos;
	enemyObj->scale = _scale;
	enemyObj->mass = _mass;
	enemyObj->vel.Set(0.f, 0.f, 0.f);
	enemyObj->colType = Collider::SPHERE;
	curState = IDLE;
	projSpd = 0.f;
	projFired = false;
}

void Enemy::Update(double dt)
{
	if (!enemyObj || !playerObj)
		return;

	if (spriteAnim)
	{
		spriteAnim->Update(dt);
		spriteAnim->SetActive(true);
	}

	switch (enemyObj->type)
	{
	case GameObject::GO_ENEMY_SNOWYETI: RunYeti(dt); break;
	case GameObject::GO_SLEDYETI: RunSledYeti(dt); break;
	default: break;
	}
}

void Enemy::Exit()
{
	if (snowBall)
	{
		snowBall->SetActive(false);
		snowBall = nullptr;
	}

	if (spriteAnim)
	{
		delete spriteAnim;
		spriteAnim = nullptr;
	}

	if (enemyObj)
	{
		enemyObj->type = GameObject::GO_NONE;
		enemyObj->SetActive(false);
	}
}

void Enemy::Reset()
{
	enemyObj->pos = defaultPos;
}

Vector3 Enemy::GetPos() const
{
	return enemyObj->pos;
}

Vector3 Enemy::GetScale() const
{
	return enemyObj->scale;
}

SpriteAnimation * Enemy::GetSprite()
{
	return spriteAnim;
}

int Enemy::GetCurAnimFrame() const
{
	return spriteAnim->GetCurFrame();
}

bool Enemy::GetActive() const
{
	return enemyObj->GetActive();
}

bool Enemy::GetProjFired() const
{
	return projFired;
}

bool Enemy::GetProjActive() const
{
	if (!snowBall) return false;

	return snowBall->GetActive();
}

GameObject::GAMEOBJECT_TYPE Enemy::GetType() const
{
	return enemyObj->type;
}

void Enemy::SetPlayerObj(GameObject * _playerObj, GameObject* _bombObj)
{
	if (_playerObj->type == GameObject::GO_PLAYER)
		playerObj = _playerObj;

	if (_bombObj->type == GameObject::GO_BOMB)
		bombObj = _bombObj;
}

void Enemy::SetHeightMap(vector<unsigned char>* _heightMap, float _terrainWidth, float _terrainHeight)
{
	m_heightMap = _heightMap;
	m_terrainWidth = _terrainWidth;
	m_terrainHeight = _terrainHeight;
}

void Enemy::SetSpriteAnim(Mesh * _sprite, int _startFrame, int _endFrame, int _repeat, float _time, bool _active)
{
	if (spriteAnim)
		return;

	spriteAnim = new SpriteAnimation();
	spriteAnim->Set(dynamic_cast<SpriteMesh*>(_sprite), _startFrame, _endFrame, _repeat, _time, _active);
}

void Enemy::SetProjFired(bool _projFired)
{
	projFired = _projFired;
}

void Enemy::PushProjectile(GameObject * _projObj, Vector3 _scale, float _spd)
{
	_projObj->type = GameObject::GO_PROJ_SNOWBALL;	// Set type to projectile
	_projObj->pos = enemyObj->pos;
	_projObj->scale = _scale;
	//_projObj->dir = (playerObj->pos - _projObj->pos).Normalized() + Vector3(0.f, 1.f, 0.f);
	_projObj->dir = Vector3(0.f, -9.8f, 0.f);
	_projObj->normal.Set(0.f, 1.f, 0.f);
	_projObj->mass = .1f;
	_projObj->vel = _projObj->dir;
	projSpd = _spd;

	snowBall = _projObj;
}

void Enemy::ClearProjectile()
{
	if (snowBall && snowBall->GetActive())
	{
		snowBall->SetActive(false);
		snowBall->type = GameObject::GO_NONE;
	}
}

void Enemy::RunYeti(double dt)
{
	switch (curState)
	{
	case IDLE:
		if (spriteAnim->GetCurFrame() == 0)
			spriteAnim->SetActive(false);

		if (!GetProjActive() && abs(playerObj->pos.x - enemyObj->pos.x) <= 100.f)
			curState = ATTACK;
		break;

	case ATTACK:
		spriteAnim->SetActive(true);

		if (GetProjFired())
			curState = IDLE;
		break;

	default:
		break;
	}

	if (!snowBall)
		return;

	float tHeight = m_terrainHeight * ReadHeightMap(*m_heightMap, (snowBall->pos.x + m_terrainWidth * 0.5f) / m_terrainWidth, 0.f);
	if (tHeight <= snowBall->pos.y - snowBall->scale.y)
	{
		//Physics::K1(snowBall->vel, Vector3(0.f, -9.8f * snowBall->mass, 0.f), (float)dt, snowBall->vel);
		snowBall->pos += snowBall->vel * projSpd * (float)dt;
	}
	else
	{
		snowBall->pos.y = tHeight + snowBall->scale.y * 0.5f;
	}

	if ((playerObj->pos - snowBall->pos).LengthSquared()
		<= (playerObj->scale.x * 0.5f + snowBall->scale.x) * (playerObj->scale.x * 0.5f + snowBall->scale.x))
	{
		playerObj->vel *= 0.5f;
		snowBall->SetActive(false);
	}

	if (snowBall->pos.y < 0)
		snowBall->SetActive(false);

	if (!snowBall->GetActive())
		snowBall = nullptr;
}

void Enemy::RunSledYeti(double dt)
{
	// Physics can be done here


}

