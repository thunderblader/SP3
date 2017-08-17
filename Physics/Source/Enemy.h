#pragma once
#ifndef ENEMY_H
#define ENEMY_H
#include "GameObject.h"

#include <vector>

using std::vector;

class Enemy
{
public:
	Enemy();
	~Enemy();

	void Init(GameObject* _enemyObj, GameObject::GAMEOBJECT_TYPE _type, Vector3 _pos, Vector3 _scale);
	void Update(double dt);
	void Exit();
	void Reset();

	void PushProjectile(GameObject* _projObj, Vector3 _target, Vector3 _scale, float _spd);

protected:
	enum ENEMY_STATE
	{
		IDLE,
		ATTACK,
		NUM_ENEMY_STATE
	};

private:
	GameObject* enemyObj;
	Vector3 defaultPos;
	ENEMY_STATE curState;

	vector<GameObject*> projList;
};

#endif // !ENEMY_H
