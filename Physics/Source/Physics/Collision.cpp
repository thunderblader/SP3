#include "Collision.h"
#include "../GameObject.h"

void Collider::CollisionResponse()
{
}

bool Collider::CheckCollision(GameObject * go, GameObject * go2)
{
	if (go->colType == ColliderType::BOX && go2->colType == ColliderType::BOX)
		return ColliBoxDetection(go, go2);
	else if (go->colType == ColliderType::SPHERE && go2->colType == ColliderType::SPHERE)
		return ColliSphereDetection(go, go2);
	else if ((go->colType == ColliderType::BOX && go2->colType == ColliderType::SPHERE)
		|| (go->colType == ColliderType::SPHERE && go2->colType == ColliderType::BOX))
		return ColliBoxSphereDetection(go, go2);

	return false;
}

bool Collider::ColliBoxDetection(GameObject * go, GameObject * go2)
{
	return false;
}

bool Collider::ColliSphereDetection(GameObject* go, GameObject* go2)
{
	return false;
}

bool Collider::ColliBoxSphereDetection(GameObject * go, GameObject * go2)
{
	return false;
}