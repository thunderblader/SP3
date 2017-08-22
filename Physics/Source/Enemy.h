#pragma once
#ifndef ENEMY_H
#define ENEMY_H
#include "GameObject.h"
#include "SpriteAnimation.h"

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

	int GetCurAnimFrame() const;

	void SetPlayerObj(GameObject* _playerObj);
	void SetSpriteAnim(Mesh* _sprite);

	void PushProjectile(GameObject* _projObj, Vector3 _scale, float _spd);

protected:
	enum ENEMY_STATE
	{
		IDLE,
		ATTACK,
		NUM_ENEMY_STATE
	};

	void RunYeti(double dt);
	void RunKing(double dt);
	void RunKnight(double dt);

private:
	static GameObject* playerObj;
	GameObject* enemyObj;
	Vector3 defaultPos;
	ENEMY_STATE curState;
	SpriteAnimation* spriteAnim;

	vector<GameObject*> projList;
	float projSpd;
};

#endif // !ENEMY_H
