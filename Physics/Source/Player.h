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
		Vector3 _pos = Vector3(0.f, 0.f, 0.f),
		Vector3 _scale = Vector3(1.f, 1.f, 1.f),
		float _mass = 1.f);
	void Update(double dt);
	void Reset();

private:
	Player();
	virtual ~Player();

	GameObject* playerObj;
	Vector3 position;
	Vector3 scale;
	float mass;
};

#endif // !PLAYER_H
