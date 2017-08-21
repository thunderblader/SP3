#include "Player.h"
#include "Terrain\LoadHmap.h"

void Player::Init(GameObject * _playerObj, GameObject * _playerBomb
	, GameObject::GAMEOBJECT_TYPE _type
	, Vector3 _pos, Vector3 _scale, float _mass, float _spd)
{
	playerObj = _playerObj;
	playerBomb = _playerBomb;
	playerObj->type = _type;
	playerObj->pos = _pos;
	playerObj->scale = _scale;
	playerObj->mass = _mass;
	playerObj->vel = Vector3(0.f, 0.f, 0.f);
	playerObj->dir = Vector3(1.f, 0.f, 0.f);
	playerObj->normal = Vector3(0.f, 1.f, 0.f);

	defaultPos = _pos;
	m_speed = _spd;
}

void Player::Update(double dt)
{
	if (!playerObj)
		return;

	// Player Physics can be done here
	if (playerObj->pos.x >= -playerObj->scale.x/2 && !launched)
	{
		if (playerObj->vel.y < 0)
		{
			playerObj->vel = playerObj->vel.Cross(Vector3(0, 0, -1));
			playerObj->vel.x *= 5;
		}
		playerBomb->active = true;
		playerBomb->type = GameObject::GO_BOMB;
		playerBomb->vel = playerObj->vel * 5;
		playerBomb->pos = GetPlayerPos();
		playerBomb->scale.Set(2, 2, 1);
		playerObj->active = false;
		launched = true;
	}

	if (playerBomb->active)
	{
		playerBomb->pos += playerBomb->vel * dt;

		if (playerBomb->pos.y < 0)
		{
			playerBomb->active = false;
		}
	}
	std::cout << playerObj->pos.y << std::endl;
	if (!playerBomb->active)
	{
		playerBomb->pos.Set(0, playerObj->pos.y, 0);
		if (launched)
		{
			playerObj->active = true;
			launched = false;
			playerObj->vel.SetZero();
			Reset();
		}
	}
		
}

void Player::Reset()
{
	// Resets player values
	playerObj->pos = defaultPos;
}

GameObject Player::GetPlayerObj() const
{
	return *playerObj;
}

GameObject Player::GetPlayerBomb() const
{
	return *playerBomb;
}	

Vector3 Player::GetPlayerPos()
{
	if(!launched)
		return playerObj->pos;

	if (launched)
		return playerBomb->pos;
}

Vector3 Player::GetVel()
{
	return playerObj->vel;
}

void Player::SetHeightmap(vector<unsigned char>* _heightmap, float _worldWidth, float _worldHeight)
{
	m_heightmap = _heightmap;
	m_TerrainWidth = _worldWidth;
	m_TerrainHeight = _worldHeight;
}

void Player::CollisionResponse()
{
}

void Player::Move_LeftRight(const double dt, const bool dLeft)
{
	if (!m_heightmap || !playerObj || playerObj->vel.Length() > 10 ||
		playerObj->pos.y > (m_TerrainHeight * ReadHeightMap(*m_heightmap,
		(playerObj->pos.x + m_TerrainWidth * 0.5f) / m_TerrainWidth, 0.f)) + playerObj->scale.y * 0.5f)
		return;

	playerObj->vel += Vector3(dLeft ? -m_speed : m_speed, 0.f, 0.f) * (float)dt * (1 / playerObj->mass);
}

void Player::Jump(const double dt)
{
	if (!m_heightmap || !playerObj ||
		playerObj->pos.y > (m_TerrainHeight * ReadHeightMap(*m_heightmap, (playerObj->pos.x + m_TerrainWidth * 0.5f) / m_TerrainWidth, 0.f)) + playerObj->scale.y * 0.5f)
		return;

	playerObj->vel += Vector3(0.f, m_speed * 4.f, 0.f) * (float)dt * (1.f / playerObj->mass);
}

Player::Player()
	: playerObj(nullptr)
	, playerBomb(nullptr)
	, defaultPos(0.f, 0.f, 0.f)
	, m_heightmap(nullptr)
	, m_TerrainWidth(0)
	, m_TerrainHeight(0)
	, launched(false)
{
}

Player::~Player()
{
}
