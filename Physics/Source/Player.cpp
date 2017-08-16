#include "Player.h"

void Player::Init(GameObject * _playerObj
	, GameObject::GAMEOBJECT_TYPE _type
	, Vector3 _pos, Vector3 _scale, float _mass)
{
	playerObj = _playerObj;
	playerObj->type = _type;
	playerObj->pos = _pos;
	playerObj->scale = _scale;
	playerObj->mass = _mass;
	playerObj->vel = Vector3(0.f, 0.f, 0.f);
	playerObj->normal = Vector3(0.f, 1.f, 0.f);
	playerObj->active = true;
}

void Player::Update(double dt)
{
}

void Player::Reset()
{
}

GameObject Player::GetPlayerObj() const
{
	return *playerObj;
}

Player::Player()
	: playerObj(nullptr)
{
}

Player::~Player()
{
}
