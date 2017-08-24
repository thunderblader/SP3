#pragma once
#ifndef SCENE01_H
#define SCENE01_H

#include "GameObject.h"
#include "SceneBase.h"
#include "Player.h"
#include "Controller\Controller.h"
#include "Enemy.h"
#include "Load_File.h"
#include "Shop.h"
#include "ParameterPhysics.h"
#include "Tree.h"

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
	void UpdateParticles(double dt);
	void Shop_Update(double dt);
	void Menu(double dt);
	void RenderMenu();
	void RenderHUD();

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
	unsigned int m_tries;
	int m_ballCount;
	int* m_objectCount;

	float m1, m2;
	Vector3 u1, u2, v1, v2, u1N, u2N;
	//Vector3 initialMomentum, finalMomentum;
	//float initialKE, finalKE;

	bool free_look;
	bool in_shop;
	bool buy_item;
	bool purchased;

	Load_File file;
	Shop shop;
	Tree* item_node;
	float time_limit;
	int Unit_Height_Space;
	int Unit_Width_Space;
	int newlevel, currlevel;
	int Score;
	int Gold;
	int item_id;
	float wind;

	enum MENU {
		MENU = 1,
		MENU2,
		MENU3,
		MENU4,
		MENU5,

		START,
		START2,

		LS,
		LS2,
		LS3,

		OPTIONS,
		OPTIONS2,

		INSTRUCTIONS
	} menustate;
	float menuBounce;
	bool display;

	// Physics Parameters
	ParamPhysics param_physics;
};

#endif