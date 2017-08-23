#include "Player.h"
#include "Terrain\LoadHmap.h"
#include "Physics\Physics.h"

void Player::Init(GameObject * _playerObj, GameObject * _playerBomb
	, GameObject::GAMEOBJECT_TYPE _type
	, Vector3 _pos, Vector3 _scale, float _mass, float _spd, float _jump_boost, float _blast)
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
	playerBomb->scale = Vector3(5,5,1);
	playerBomb->mass = _mass;
	playerBomb->SetActive(false);

	defaultPos = _pos;
	m_speed = _spd;
	jump_boost = _jump_boost;
	blast_strength = _blast;
	exploded = false;

}

void Player::Update(double dt)
{
	if (!playerObj)
		return;

	// Player Physics can be done here
	if (playerObj->pos.x >= -playerObj->scale.x * 2 && !launched)
	{
		playerBomb->SetActive(true);
		playerBomb->type = GameObject::GO_BOMB;
		playerBomb->vel = playerObj->vel;
		playerBomb->vel *= 3.5;
		playerBomb->vel.y = 5;
		playerBomb->pos = GetPlayerPos();
		playerBomb->scale.Set(5, 5, 1);
		exploded = false;
		launched = true;
	}

	if (playerBomb->GetActive())
	{
		//Physics::K1(playerBomb->vel, Vector3(0.f, -9.8f, 0.f), (float)dt, playerBomb->vel);
		//playerBomb->pos += playerBomb->vel * (float)dt * 40.f;
		if (playerBomb->pos.y < 0)
		{
			playerBomb->SetActive(false);
			Reset();
		}
		else if (exploded && playerBomb->scale.x < 10)
		{
			playerBomb->scale = Vector3(playerBomb->scale.x + 10 * dt, playerBomb->scale.y + 10 * dt,1);
			if (playerBomb->scale.x > 10)
			{
				playerBomb->SetActive(false);
				launched = false;
				playerObj->vel.SetZero();
				Reset();
			}
		}
	}
	if (!playerBomb->GetActive())
	{
		playerBomb->pos.Set(0, playerObj->pos.y, 0);
		if (launched && !exploded)
		{
			playerObj->SetActive(true);
			launched = false;
			Reset();
		}
	}
		
}

void Player::Reset()
{
	// Resets player values
	playerObj->pos = defaultPos;
	exploded = false;
	launched = false;
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
	else
		return playerBomb->pos;
}

Vector3 Player::GetVel()
{
	return playerObj->vel;
}

void Player::SetExploded(bool In)
{
	exploded = In;
}

bool Player::GetExploded()
{
	return exploded;
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

	playerObj->vel += Vector3(0.f, 300.f + jump_boost, 0.f) * (float)dt * (1.f / playerObj->mass);
}

void Player::Upgrade(Tree::avl_node &node)
{	//size=2, mass=1.5
	if (node.id == 1)
	{
//		playerObj->scale.Set(10, 10, 1);
//		playerObj->mass = 1.5f;
		playerObj->scale.Set(playerObj->scale.x + (node.item_count * 1), playerObj->scale.y + (node.item_count * 1), 1);
		playerObj->mass = 1 + (node.item_count * 0.1);
		//m_speed = 20;
	}
	else if (node.id == 2)
	{
		m_speed = 10 + (node.item_count * 1);
	}
	else if (node.id == 3)
	{
		jump_boost = (node.item_count * 30);
	}
	else if (node.id == 4)
	{
		playerObj->vel.x += 5;
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
