#pragma once
#pragma once
#ifndef SCENE01_H
#define SCENE01_H

#include "GameObject.h"
#include "SceneBase.h"
#include "Player.h"
#include "Controller\Controller.h"

#include <vector>
#include "Load_File.h"

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
	bool Load(const string mapName = "Image//Test_Level.csv");
	void Process(string content);
	void Spawn(int value,int Width_Space,int Height_Space);
	bool Load_Data(const string saveFileName = "Image//test.sav");
	bool Save_Data(const string saveFileName = "Image//test.sav");

protected:

	//Physics
	std::vector<GameObject *> m_goList;
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

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2, u1N, u2N;
	Vector3 initialMomentum, finalMomentum;
	float initialKE, finalKE;

	bool free_look;

	int Unit_Height_Space;
	int Unit_Width_Space;
	int Level;
	int Score;
	int Gold;
};

#endif