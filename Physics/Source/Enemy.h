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
	bool GetActive() const;
	bool GetProjFired() const;

	void SetPlayerObj(GameObject* _playerObj);
	void SetBombObj(GameObject* _bombObj);
	void SetSpriteAnim(Mesh* _sprite);
	void SetProjFired(bool _projFired);

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
	static GameObject* bombObj;
	GameObject* enemyObj;
	Vector3 defaultPos;
	ENEMY_STATE curState;
	SpriteAnimation* spriteAnim;

	vector<GameObject*> projList;
	float projSpd;
	bool projFired;
};

#endif // !ENEMY_H
