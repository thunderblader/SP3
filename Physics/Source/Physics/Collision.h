#pragma once
#ifndef COLLISION_H
#define COLLISION_H

class GameObject;

struct ColliderType
{
	enum COLLIDER_TYPE
	{
		BOX,
		SPHERE,
		TOTAL
	};

	COLLIDER_TYPE colType;
};

class Collider
{
public:
	virtual void CollisionResponse() = 0;

	bool CheckCollision(GameObject* go, GameObject* go2);

protected:
	bool ColliBoxDetection(GameObject* go, GameObject* go2);
	bool ColliSphereDetection(GameObject* go, GameObject* go2);
	bool ColliBoxSphereDetection(GameObject* go, GameObject* go2);

private:
};


#endif