#ifndef SPAWNER_H
#define SPAWNER_H
#include "Vector3.h"
#include "GameObject.h"
#include "Application.h"
#include <vector>

using std::vector;

class Spawner
{
public:
	Spawner();
	~Spawner();

	void Init();
	GameObject* FetchGO(vector<GameObject *>* Gameobject_List);
	void Spawn(int value, int Width_Space, int Height_Space, vector<GameObject *>* Gameobject_List);

private:
	GameObject *spawner;
	int Unit_Height_Space;
	int Unit_Width_Space;
	float m_worldWidth;
	float m_worldHeight;

};

#endif
