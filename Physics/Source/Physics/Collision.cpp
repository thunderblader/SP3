#include "Collision.h"
#include "../GameObject.h"

bool Collider::CheckCollision(GameObject * go, GameObject * go2)
{
	if (go->colType == TYPE::BOX && go2->colType == TYPE::BOX)
		return ColliBoxDetection(go, go2);
	else if (go->colType == TYPE::SPHERE && go2->colType == TYPE::SPHERE)
		return ColliSphereDetection(go, go2);
	else if ((go->colType == TYPE::BOX && go2->colType == TYPE::SPHERE)
		|| (go->colType == TYPE::SPHERE && go2->colType == TYPE::BOX))
		return ColliBoxSphereDetection(go, go2);

	return false;
}

bool Collider::ColliBoxDetection(GameObject * go, GameObject * go2)
{
	return false;
}

bool Collider::ColliSphereDetection(GameObject* go, GameObject* go2)
{
	if ((go2->pos - go->pos).LengthSquared()
		<= (go->scale.x + go2->scale.x) * (go->scale.x + go2->scale.x))
		return true;

	return false;
}

bool Collider::ColliBoxSphereDetection(GameObject * go, GameObject * go2)
{
	return false;
}