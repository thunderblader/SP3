#pragma once
#pragma once
#ifndef SCENE01_H
#define SCENE01_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

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

	//Physics
	std::vector<GameObject *> m_goList;
	GameObject* m_player;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ghost;
	GameObject *test;
	int m_ballCount;
	int m_objectCount;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2, u1N, u2N;
	Vector3 initialMomentum, finalMomentum;
	float initialKE, finalKE;

	bool free_look;
};

#endif