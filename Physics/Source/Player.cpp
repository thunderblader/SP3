#include "Player.h"
#include "Terrain\LoadHmap.h"
#include "Physics\Physics.h"

void Player::Init(GameObject * _playerObj, GameObject * _playerBomb
	, GameObject::GAMEOBJECT_TYPE _type
	, Vector3 _pos, Vector3 _scale, float _mass, float _spd)
{
	playerObj = _playerObj;
	playerObj->type = _type;
	playerObj->pos = _pos;
	playerObj->scale = _scale;
	playerObj->mass = _mass;
	playerObj->vel.Set(0.f, 0.f, 0.f);
	playerObj->dir.Set(1.f, 0.f, 0.f);
	playerObj->normal.Set(0.f, 1.f, 0.f);
//	item_node = Tree::getInstance();
	playerBomb = _playerBomb;
	playerBomb->type = GameObject::GO_BOMB;
	playerBomb->pos = _pos;
	playerBomb->scale = _scale;
	playerBomb->mass = _mass;
	playerBomb->active = false;

	defaultPos = _pos;
	m_speed = _spd;
}

void Player::Update(double dt)
{
	if (!playerObj)
		return;

	// Player Physics can be done here
	if (playerObj->pos.x >= -playerObj->scale.x / 2 && !launched)
	{		
		playerBomb->vel = playerObj->vel;
		//if (playerObj->vel.y < 0)
		//{
		//	playerBomb->vel.y = -playerObj->vel.y;
		//}
		playerBomb->active = true;
		playerBomb->pos = GetPlayerPos();
		playerObj->vel.SetZero();
		playerObj->active = false;
		launched = true;
	}

	if (playerBomb->active)
	{
		Physics::K1(playerBomb->vel, Vector3(0.f, -9.8f, 0.f), (float)dt, playerBomb->vel);
		playerBomb->pos += playerBomb->vel * (float)dt * 40.f;

		if (playerBomb->pos.y < 0)
		{
			playerBomb->active = false;
		}
	}
	if (!playerBomb->active)
	{
		playerBomb->pos.Set(0, playerObj->pos.y, 0);
		playerBomb->scale.Set(5, 5, 1);
		if (launched)
		{
			playerObj->active = true;
			launched = false;
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
	if ((!m_heightmap || !playerObj || playerObj->vel.Length() > 10 ||
		playerObj->pos.y > (m_TerrainHeight * ReadHeightMap(*m_heightmap,
		(playerObj->pos.x + m_TerrainWidth * 0.5f) / m_TerrainWidth, 0.f)) + playerObj->scale.y * 0.5f))
		return;

	playerObj->vel += Vector3(dLeft ? -m_speed : m_speed, 0.f, 0.f) * (float)dt * (1 / playerObj->mass);
}

void Player::Jump(const double dt)
{
	if (!m_heightmap || !playerObj ||
		playerObj->pos.y > (m_TerrainHeight * ReadHeightMap(*m_heightmap, (playerObj->pos.x + m_TerrainWidth * 0.5f) / m_TerrainWidth, 0.f)) + playerObj->scale.y * 0.5f)
		return;

	playerObj->vel.y += 300.f * (float)dt * (1.f / playerObj->mass);
}

void Player::Upgrade(int id)
{	//size=2, mass=1.5
	if (id == 1)
	{
		playerObj->scale.Set(10, 10, 1);
		playerObj->mass = 1.5f;
		//m_speed = 20;
	}
	else if (id == 2)
	{
		playerObj->scale.Set(7.5, 7.5, 1);
		playerObj->mass = 1.5f;
	}
	else if (id == 3)
	{
		playerObj->scale.Set(2.5, 2.5, 1);
		playerObj->mass = 0.5f;
	}
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
