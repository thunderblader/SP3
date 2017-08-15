#include "Player.h"

void Player::Init(GameObject * _playerObj, Vector3 _pos, Vector3 _scale, float _mass)
{
	playerObj = _playerObj;
	position = _pos;
	scale = _scale;
	mass = _mass;
}

void Player::Update(double dt)
{
}

void Player::Reset()
{
}

Player::Player()
	: playerObj(nullptr)
	, position(Vector3(0.f, 0.f, 0.f))
	, scale(Vector3(1.f, 1.f, 1.f))
	, mass(1.f)
{
}

Player::~Player()
{
}
