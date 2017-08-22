#pragma once
#ifndef COLLISION_H
#define COLLISION_H

class GameObject;

class Collider
{
public:
	enum TYPE
	{
		BOX,
		SPHERE,
		TOTAL
	};

	virtual void CollisionResponse() = 0;

	static bool CheckCollision(GameObject* go, GameObject* go2);

protected:
	static bool ColliBoxDetection(GameObject* go, GameObject* go2);
	static bool ColliSphereDetection(GameObject* go, GameObject* go2);
	static bool ColliBoxSphereDetection(GameObject* go, GameObject* go2);

private:
};


#endif