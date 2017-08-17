#pragma once
#pragma once
#ifndef SCENE01_H
#define SCENE01_H

#include "GameObject.h"
#include "SceneBase.h"
#include "Player.h"
#include "Controller\Controller.h"
#include "Load_File.h"
#include "Enemy.h"

#include <vector>

using std::vector;
using std::string;

class Scene01 : public SceneBase
{
public:
	Scene01();
	~Scene01();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	bool CheckCollision(GameObject *go1, GameObject *go2, float dt);
	void CollisionResponse(GameObject *go1, GameObject *go2);
	void Camera_Control(double dt);

protected:
	vector<GameObject *> m_goList;
	vector<Enemy*> enemyList;
	Player* m_player;
	Controller* m_control;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	float m_TerrainWidth;
	float m_TerrainHeight;
	GameObject *m_ghost;
	GameObject *test;
	GameObject *spawner;
	int m_ballCount;
	int m_objectCount;

	float m1, m2;
	Vector3 u1, u2, v1, v2, u1N, u2N;
	Vector3 initialMomentum, finalMomentum;
	float initialKE, finalKE;

	bool free_look;

	Load_File file;

	int Unit_Height_Space;
	int Unit_Width_Space;
	int Level;
	int Score;
	int Gold;

	// Physics Parameters
	float m_gravity;
	float m_airfriction;
	float m_terrainfriction;
	float m_cartmass;
	float m_acceleration;
	float m_speedlimit;
};

#endif