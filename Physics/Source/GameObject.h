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
		GO_ENEMY_KING,
		GO_ENEMY_KNIGHT,
		GO_PROJ_SNOWBALL,
		GO_WALL,
		GO_BLOCK,
		GO_TEMP,
		GO_BRICK,
		GO_BOMB,
		GO_SCREEN,
		GO_TOTAL //must be last
	};

	static int m_totalGameObjects;

	GAMEOBJECT_TYPE type;
	Collider::TYPE colType;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 dir;
	Vector3 normal;
	bool boom;
	float mass;
	float momentOfInertia;
	float angularVelocity;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();

	void SetActive(bool _input);
	bool GetActive() const;

private:
	bool active;
};

#endif