#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Physics\Collision.h"

class GameObject
{
public:
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_PLAYER,
		GO_ENEMY,
		GO_ENEMY_SNOWYETI,
		GO_WALL,
		GO_BLOCK,
		GO_TEMP,
		GO_BRICK,
		GO_BOMB,
		GO_SCREEN,
		GO_TOTAL //must be last
	};
	GAMEOBJECT_TYPE type;
	ColliderType::COLLIDER_TYPE colType;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 dir;
	Vector3 normal;
	bool active;
	bool boom;
	float mass;
	float momentOfInertia;
	float angularVelocity;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif