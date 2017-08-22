#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "SingletonTemplate.h"
#include "Vector3.h"
#include "GameObject.h"
#include "Physics\Collision.h"
#include "Tree.h"
#include "Shop.h"

#include <vector>

using std::vector;

class Player : public Singleton<Player>, public Collider
{
	friend Singleton<Player>;

public:
	void Init(GameObject* _playerObj,
		GameObject* _playerBomb,
		GameObject::GAMEOBJECT_TYPE _type,
		Vector3 _pos = Vector3(0.f, 0.f, 0.f),
		Vector3 _scale = Vector3(1.f, 1.f, 1.f),
		float _mass = 1.f, float _spd = 1.f);
	void Update(double dt);
	void Reset();

	GameObject GetPlayerObj() const;
	GameObject GetPlayerBomb() const;
	Vector3 GetPlayerPos();
	Vector3 GetVel();
	void SetExploded(bool In);
	bool GetExploded();
	void SetHeightmap(vector<unsigned char>* _heightmap, float _worldWidth, float _worldHeight);

	virtual void CollisionResponse();

	void Move_LeftRight(const double dt, const bool dLeft);
	void Jump(const double dt);
	void Upgrade(Tree::avl_node &node);

private:
	Player();
	virtual ~Player();

	GameObject* playerObj, *playerBomb;
	Vector3 defaultPos;
	vector<unsigned char>* m_heightmap;
	float m_TerrainWidth, m_TerrainHeight;
//	Tree* item_node;
//	int id;

	float m_speed;
	bool launched;
	bool exploded;
};

#endif // !PLAYER_H
