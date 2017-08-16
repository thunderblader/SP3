#include "Player.h"
#include "Terrain\LoadHmap.h"

void Player::Init(GameObject * _playerObj
	, GameObject::GAMEOBJECT_TYPE _type
	, Vector3 _pos, Vector3 _scale, float _mass, float _spd)
{
	playerObj = _playerObj;
	playerObj->type = _type;
	playerObj->pos = _pos;
	playerObj->scale = _scale;
	playerObj->mass = _mass;
	playerObj->vel = Vector3(0.f, 0.f, 0.f);
	playerObj->dir = Vector3(1.f, 0.f, 0.f);
	playerObj->normal = Vector3(0.f, 1.f, 0.f);
	playerObj->active = true;

	m_speed = _spd;
}

void Player::Update(double dt)
{
	if (!playerObj)
		return;
}

void Player::Reset()
{
}

GameObject Player::GetPlayerObj() const
{
	return *playerObj;
}

void Player::SetHeightmap(vector<unsigned char>* _heightmap, float _worldWidth, float _worldHeight)
{
	m_heightmap = _heightmap;
	m_worldWidth = _worldWidth;
	m_worldHeight = _worldHeight;
}

void Player::Move_LeftRight(const double dt, const bool dLeft)
{
	if (!m_heightmap || !playerObj ||
		playerObj->pos.y > (m_worldHeight * ReadHeightMap(*m_heightmap, playerObj->pos.x / (m_worldWidth * 2), 0)) + playerObj->scale.x)
		return;

	playerObj->vel += Vector3(dLeft ? -m_speed : m_speed, 0.f, 0.f) * dt * (1 / playerObj->mass);
}

Player::Player()
	: playerObj(nullptr)
{
}

Player::~Player()
{
}
