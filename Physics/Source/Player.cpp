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

	// Player Physics can be done here
}

void Player::Reset()
{
	// Resets player values
}

GameObject Player::GetPlayerObj() const
{
	return *playerObj;
}

Vector3 Player::GetPlayerPos()
{
	return playerObj->pos;
}

void Player::SetHeightmap(vector<unsigned char>* _heightmap, float _worldWidth, float _worldHeight)
{
	m_heightmap = _heightmap;
	m_TerrainWidth = _worldWidth;
	m_TerrainHeight = _worldHeight;
}

void Player::Move_LeftRight(const double dt, const bool dLeft)
{
	if (!m_heightmap || !playerObj || playerObj->vel.Length() > 10 ||
		playerObj->pos.y > (m_TerrainHeight * ReadHeightMap(*m_heightmap, (playerObj->pos.x + m_TerrainWidth*0.5) / m_TerrainWidth, 0)) + playerObj->scale.y *0.5f)
		return;

	playerObj->vel += Vector3(dLeft ? -m_speed : m_speed, 0.f, 0.f) * (float)dt * (1 / playerObj->mass);
}

void Player::Jump(const double dt)
{
	if (!m_heightmap || !playerObj ||
		playerObj->pos.y > (m_TerrainHeight * ReadHeightMap(*m_heightmap, (playerObj->pos.x + m_TerrainWidth*0.5) / m_TerrainWidth, 0)) + playerObj->scale.y * 0.5f)
		return;

	playerObj->vel += Vector3(0.f, 590.f, 0.f) * (float)dt * (1 / playerObj->mass);
}

Player::Player()
	: playerObj(nullptr)
	, m_heightmap(nullptr)
	, m_TerrainWidth(0)
	, m_TerrainHeight(0)
{
}

Player::~Player()
{
}
