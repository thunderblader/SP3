
#include "GameObject.h"

int GameObject::m_totalGameObjects = 0;

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	momentOfInertia(1.f),
	angularVelocity(0.f)
{
}

GameObject::~GameObject()
{
}

void GameObject::SetActive(bool _input)
{
	if (active != _input)
	{
		active = _input;
		(_input) ? ++m_totalGameObjects : --m_totalGameObjects;
	}
}

bool GameObject::GetActive() const
{
	return active;
}
