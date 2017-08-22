#include "Enemy.h"
#include "Physics\Physics.h"
#include "Mesh.h"
#include "Physics\Collision.h"

GameObject* Enemy::playerObj = 0;
GameObject* Enemy::bombObj = 0;

Enemy::Enemy()
	: enemyObj(nullptr)
	, defaultPos(0.f, 0.f, 0.f)
	, spriteAnim(nullptr)
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
	defaultPos = _pos;
	enemyObj->scale = _scale;
	enemyObj->mass = 1.f;
	enemyObj->vel.Set(0.f, 0.f, 0.f);
	enemyObj->colType = Collider::SPHERE;
	curState = IDLE;
	projSpd = 0.f;
}

void Enemy::Update(double dt)
{
	if (!enemyObj || !playerObj)
		return;

	if (spriteAnim)
	{
		spriteAnim->Update(dt);
		spriteAnim->m_anim->animActive = true;
	}

	switch (enemyObj->type)
	{
	case GameObject::GO_ENEMY_SNOWYETI: RunYeti(dt); break;
	case GameObject::GO_ENEMY_KING: RunKing(dt); break;
	case GameObject::GO_ENEMY_KNIGHT: RunKnight(dt); break;
	default: break;
	}
}

void Enemy::Exit()
{
}

void Enemy::Reset()
{
	enemyObj->pos = defaultPos;
}

int Enemy::GetCurAnimFrame() const
{
	return spriteAnim->m_currentFrame;
}

bool Enemy::GetActive() const
{
	return enemyObj->active;
}

void Enemy::SetPlayerObj(GameObject * _playerObj)
{
	if (_playerObj->type == GameObject::GO_BLOCK)
		playerObj = _playerObj;
}

void Enemy::SetBombObj(GameObject * _bombObj)
{
	if (_bombObj->type == GameObject::GO_BOMB)
		bombObj = _bombObj;
}

void Enemy::SetSpriteAnim(Mesh * _sprite)
{
	if (spriteAnim)
		return;

	spriteAnim = dynamic_cast<SpriteAnimation*>(_sprite);
	spriteAnim->m_anim = new Animation();
	spriteAnim->m_anim->Set(0, 15, 0, 1.f, true);
}

void Enemy::PushProjectile(GameObject * _projObj, Vector3 _scale, float _spd)
{
	_projObj->type = GameObject::GO_PROJ_SNOWBALL;	// Set type to projectile
	_projObj->pos = enemyObj->pos;
	_projObj->scale = _scale;
	_projObj->dir = (playerObj->pos - _projObj->pos).Normalized() + Vector3(0.f, 1.f, 0.f);
	_projObj->normal.Set(0.f, 1.f, 0.f);
	_projObj->mass = .1f;
	_projObj->vel = _projObj->dir/* * (playerObj->pos - _projObj->pos).Length()*/;
	projSpd = _spd;

	projList.push_back(_projObj);
}

void Enemy::RunYeti(double dt)
{
	for (unsigned i = 0; i < projList.size(); ++i)
	{
		Physics::K1(projList[i]->vel, Vector3(0.f, -9.8f * projList[i]->mass, 0.f), (float)dt, projList[i]->vel);
		//Physics::K3_CalcInitialVel(playerObj->pos - projList[i]->pos, Vector3(0.f, -9.8f * projList[i]->mass, 0.f), (float)dt, projList[i]->vel);
		projList[i]->pos += projList[i]->vel * projSpd * (float)dt;

		if ((playerObj->pos - projList[i]->pos).LengthSquared()
			<= (playerObj->scale.x + projList[i]->scale.x) * (playerObj->scale.x + projList[i]->scale.x))
		{
			playerObj->vel *= 0.5f;
			projList[i]->active = false;
		}

		if (projList[i]->pos.y < 0)
			projList[i]->active = false;

		if (!projList[i]->active)
			projList.erase(projList.begin() + i);
	}
}

void Enemy::RunKing(double dt)
{
	if (Collider::CheckCollision(enemyObj, bombObj))
		enemyObj->active = false;
}

void Enemy::RunKnight(double dt)
{
	if (Collider::CheckCollision(enemyObj, bombObj))
		enemyObj->active = false;
}
