#include "Spawner.h"

Spawner::Spawner()
{
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
}

Spawner::~Spawner()
{
}

void Spawner::Init()
{
}

GameObject* Spawner::FetchGO(vector<GameObject *>* Gameobject_List)
{
	for (std::vector<GameObject *>::iterator it = Gameobject_List->begin(); it != Gameobject_List->end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		Gameobject_List->push_back(new GameObject(GameObject::GO_BALL));
	}
	Gameobject_List->at(Gameobject_List->size() - 1)->active = true;
	return Gameobject_List->at(Gameobject_List->size() - 1);
}

void Spawner::Spawn(int value, int Width_Space, int Height_Space, vector<GameObject *>* Gameobject_List)
{
	if (value == 0)
	{
		spawner = FetchGO(Gameobject_List);
		spawner->type = GameObject::GO_BALL;
		spawner->active = true;
		spawner->dir.Set(0, 1, 0);
		spawner->pos = Vector3(m_worldWidth - Width_Space, m_worldHeight - Height_Space, 0);
		spawner->vel.Set(0, 0, 0);
		spawner->scale.Set(2, 2, 2);
		spawner->mass = 1;
		std::cout << "spawned" << std::endl;
		std::cout << spawner->pos.x << ", " << spawner->pos.y << std::endl;
	}
	else if (value == 1)
	{

	}
	else
	{

	}
}
