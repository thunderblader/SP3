#include "Player.h"
#include "Terrain\LoadHmap.h"
#include "Physics\Physics.h"

void Player::Init(GameObject * _playerObj, GameObject * _playerBomb
	, GameObject::GAMEOBJECT_TYPE _type
	, Vector3 _pos, Vector3 _scale, float _mass, float _spd, float _speedlimit, float _jump_boost, float _blast)
{
	playerObj = _playerObj;
	playerObj->type = _type;
	playerObj->pos = _pos;
	playerObj->scale = _scale;
	playerObj->mass = _mass;
	playerObj->vel.Set(0.f, 0.f, 0.f);
	playerObj->dir.Set(1.f, 0.f, 0.f);
	playerObj->normal.Set(0.f, 1.f, 0.f);

	playerBomb = _playerBomb;
	playerBomb->type = GameObject::GO_BOMB;
	playerBomb->pos = _pos;
	playerBomb->scale = Vector3(5,5,1);
	playerBomb->mass = 5;
	playerBomb->SetActive(false);

	pu_stats.power = 1.f;
	pu_stats.speed = 1.f;
	defaultPos = _pos;
	m_speed = _spd;
	jump_boost = _jump_boost;
	blast_strength = _blast;
	exploded = false;
	bombspin = 0;
	wait = 0;
	speedlimit = _speedlimit;
}

void Player::Update(double dt)
{
	if (!playerObj)
		return;

	if (playerObj->pos.x - playerObj->scale.x *0.6f< -m_TerrainWidth)
	{
		playerObj->pos.x = -m_TerrainWidth + playerObj->scale.x*0.6f+0.1f;
		playerObj->vel.x = -playerObj->vel.x * 0.1f;
	}

	// Player Physics can be done here
	if (playerObj->pos.x >= -playerObj->scale.x && !launched)
	{
		playerBomb->SetActive(true);
		playerBomb->type = GameObject::GO_BOMB;
		playerBomb->vel.x = playerObj->vel.x * 5.0f;
		playerBomb->vel.y = 200;
		playerBomb->vel.y = abs(playerObj->vel.y);
		playerBomb->vel.x = playerObj->vel.x;
		playerBomb->vel = playerBomb->vel.Normalized() * playerObj->vel.Length() * 5;
		playerBomb->pos = GetPlayerPos();
		playerBomb->scale.Set(5, 5, 1);
		playerBomb->mass = 5;
		exploded = false;
		launched = true;
	}

	if (playerBomb->GetActive())
	{
		//Physics::K1(playerBomb->vel, Vector3(0.f, -9.8f, 0.f), (float)dt, playerBomb->vel);
		//playerBomb->pos += playerBomb->vel * (float)dt * 40.f;
		if (launched && !exploded)
			bombspin -= 50*(float)dt;
		if (playerBomb->pos.y < 0)
		{
			//playerBomb->SetActive(false);
			//Reset();
			wait += 1 * (float)dt;
		}
		else if (exploded)
		{
			wait += 1 * (float)dt;
		}
		if (exploded && playerBomb->scale.x < 10) // if bomb scale lesser than 10 increase its scale
		{
			playerBomb->scale = Vector3(playerBomb->scale.x + 10 * (float)dt, playerBomb->scale.y + 10 * (float)dt,1);
			if (playerBomb->scale.x > 10)
			{
				//playerObj->SetActive(true);
				//playerBomb->SetActive(false);
				//launched = false;
				//Reset();
			}
		}
	}
	if (wait > 5)
		Reset();
	//if (!playerBomb->GetActive())
	//{
	//	playerBomb->pos.Set(0, playerObj->pos.y, 0);
	//	if (launched && !exploded)
	//	{
	//		playerObj->SetActive(true);
	//		launched = false;
	//		Reset();
	//	}
	//}
		
}

void Player::Reset()
{
	// Resets player values
	playerObj->SetActive(true);
	playerBomb->SetActive(false);
	playerObj->pos = defaultPos;
	exploded = false;
	launched = false;
	bombspin = 0;
	wait = 0;
	playerObj->vel.SetZero();
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
	
		
	if (launched && wait < 5)
		return playerBomb->pos;
	return playerObj->pos;
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

bool Player::GetLaunched()
{
	return launched;
}

float Player::GetBombspin()
{
	return bombspin;
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
	if ((!m_heightmap || !playerObj || playerObj->vel.Length() > speedlimit ||
		playerObj->pos.y > (m_TerrainHeight * ReadHeightMap(*m_heightmap,
		(playerObj->pos.x + m_TerrainWidth * 0.5f) / m_TerrainWidth, 0.f)) + playerObj->scale.y * 0.5f))
		return;

	playerObj->vel += Vector3(dLeft ? -m_speed : m_speed, 0.f, 0.f) * (float)dt * (1 / playerObj->mass);
}

void Player::Jump(const double dt)
{
	if (!m_heightmap || !playerObj || launched ||
		playerObj->pos.y > (m_TerrainHeight * ReadHeightMap(*m_heightmap, (playerObj->pos.x + m_TerrainWidth * 0.5f) / m_TerrainWidth, 0.f)) + playerObj->scale.y * 0.5f)
		return;

	playerObj->vel += Vector3(0.f, 50.f + jump_boost, 0.f);
}

void Player::Upgrade(Tree::avl_node &node)
{
	if (node.id == 1)
	{
		playerObj->scale.Set(playerObj->scale.x + (node.item_count * 1), playerObj->scale.y + (node.item_count * 1), 1);
		playerObj->mass += (node.item_count * 0.1f);
	}
	else if (node.id == 2)
	{
		m_speed += (float)(5 + (node.item_count * 1));
	}
	else if (node.id == 3)
	{
		jump_boost = (float)(node.item_count * 5);
	}
	else if (node.id == 4)
	{
		playerObj->vel.x += 100;
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
