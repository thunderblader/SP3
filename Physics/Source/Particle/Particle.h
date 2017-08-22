#pragma once
#include "Vector3.h"

enum ParticleObject_TYPE
{
	P_SPARK = 0,
	P_RAIN,
	P_EXPLOSION,
	P_TOTAL
};

class ParticleObject
{
public:
	ParticleObject(ParticleObject_TYPE = P_SPARK);
	~ParticleObject();

	ParticleObject_TYPE type;

	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	float rotation;
	float rotationSpeed;
	bool  isActive;
private:
};