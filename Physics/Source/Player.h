#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "SingletonTemplate.h"
#include "Vector3.h"
#include "GameObject.h"

class Player : public Singleton<Player>
{
	friend Singleton<Player>;

public:
	void Init(GameObject* _playerObj,
		GameObject::GAMEOBJECT_TYPE _type,
		Vector3 _pos = Vector3(0.f, 0.f, 0.f),
		Vector3 _scale = Vector3(1.f, 1.f, 1.f),
		float _mass = 1.f);
	void Update(double dt);
	void Reset();

	GameObject GetPlayerObj() const;

private:
	Player();
	virtual ~Player();

	GameObject* playerObj;
};

#endif // !PLAYER_H
