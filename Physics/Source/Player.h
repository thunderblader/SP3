#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "SingletonTemplate.h"
#include "Vector3.h"
#include "GameObject.h"

#include <vector>

using std::vector;

class Player : public Singleton<Player>
{
	friend Singleton<Player>;

public:
	void Init(GameObject* _playerObj,
		GameObject::GAMEOBJECT_TYPE _type,
		Vector3 _pos = Vector3(0.f, 0.f, 0.f),
		Vector3 _scale = Vector3(1.f, 1.f, 1.f),
		float _mass = 1.f, float _spd = 1.f);
	void Update(double dt);
	void Reset();

	GameObject GetPlayerObj() const;
	void SetHeightmap(vector<unsigned char>* _heightmap, float _worldWidth, float _worldHeight);

	void Move_LeftRight(const double dt, const bool dLeft);
	void Jump(const double dt);
	Vector3 Get_PlayerPos();

private:
	Player();
	virtual ~Player();

	GameObject* playerObj;
	vector<unsigned char>* m_heightmap;
	float m_TerrainWidth, m_TerrainHeight;

	float m_speed;
};

#endif // !PLAYER_H
