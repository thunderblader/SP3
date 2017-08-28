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

	void Init(GameObject* _enemyObj, GameObject::GAMEOBJECT_TYPE _type, Vector3 _pos, Vector3 _scale, float _mass = 1.f);
	void Update(double dt);
	void Exit();
	void Reset();

	Vector3 GetPos() const;
	Vector3 GetScale() const;
	SpriteAnimation * GetSprite();
	int GetCurAnimFrame() const;
	bool GetActive() const;
	bool GetProjFired() const;
	bool GetProjActive() const;
	GameObject::GAMEOBJECT_TYPE GetType() const;

	void SetPlayerObj(GameObject* _playerObj, GameObject* _bombObj);
	void SetHeightMap(vector<unsigned char>* _heightMap, float _terrainWidth, float _terrainHeight);
	void SetSpriteAnim(Mesh* _sprite, int _startFrame, int _endFrame, int _repeat, float _time, bool _active);
	void SetProjFired(bool _projFired);

	void PushProjectile(GameObject* _projObj, Vector3 _scale, float _spd);
	void ClearProjectile();

protected:
	enum ENEMY_STATE
	{
		IDLE,
		ATTACK,
		NUM_ENEMY_STATE
	};

	void RunYeti(double dt);
	void RunSledYeti(double dt);

private:
	static GameObject* playerObj;
	static GameObject* bombObj;
	static vector<unsigned char>* m_heightMap;
	static float m_terrainWidth;
	static float m_terrainHeight;
	GameObject* enemyObj;
	Vector3 defaultPos;
	ENEMY_STATE curState;
	SpriteAnimation* spriteAnim;

	GameObject* snowBall;
	float projSpd;
	bool projFired;
};

#endif // !ENEMY_H
