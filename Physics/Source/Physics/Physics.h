#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H
#include "Vector3.h"

#include <cmath>

class Physics
{
public:
	template<typename T>
	static T K1(T _initialVel, T _acceleration, float _time);
	template<typename T>
	static void K1(T _initialVel, T _acceleration, float _time, T& _finalVel);
	template<typename T>
	static T K1_CalcInitialVel(T _finalVel, T _acceleration, float _time);
	template<typename T>
	static void K1_CalcInitialVel(T _finalVel, T _acceleration, float _time, T& _initialVel);
	template<typename T>
	static T K1_CalcAcceleration(T _initialVel, T _finalVel, float _time);
	template<typename T>
	static void K1_CalcAcceleration(T _initialVel, T _finalVel, float _time, T& _acceleration);
	template<typename T>
	static T K1_CalcTime(T _initialVel, T _finalVel, T _acceleration);
	template<typename T>
	static void K1_CalcTime(T _initialVel, T _finalVel, T _acceleration, float& _time);

	template<typename T>
	static T K2(T _initialVel, T _finalVel, float _time);
	template<typename T>
	static void K2(T _initialVel, T _finalVel, float _time, T& _distance);
	template<typename T>
	static T K2_CalcInitialVel();
	static void K2_CalcInitialVel();
	template<typename T>
	static T K2_CalcFinalVel();
	static void K2_CalcFinalVel();
	template<typename T>
	static T K2_CalcTime();
	static void K2_CalcTime();

	template<typename T>
	static T K3(T _initialVel, T _acceleration, float _time);
	template<typename T>
	static void K3(T _initialVel, T _acceleration, float _time, T& _distance);
	template<typename T>
	static T K3_CalcInitialVel();
	static void K3_CalcInitialVel();
	template<typename T>
	static T K3_CalcAcceleration();
	static void K3_CalcAcceleration();

	template<typename T>
	static T K4(T _initialVel, T _acceleration, T _distance);
	template<typename T>
	static void K4(T _initialVel, T _acceleration, T _distance, T& _finalVel);

	template<typename T>
	static T CalcAcceleration(T _velocity, float _time);
	template<typename T>
	static void CalcAcceleration(T _velocity, float _time, T& _acceleration);

private:

};

#endif // !PHYSICS_H

template<typename T>
inline T Physics::K1(T _initialVel, T _acceleration, float _time)
{
	return T(_initialVel + _acceleration * _time);
}

template<typename T>
inline void Physics::K1(T _initialVel, T _acceleration, float _time, T & _finalVel)
{
	_finalVel = _initialVel + _acceleration * _time;
}

template<typename T>
inline T Physics::K1_CalcInitialVel(T _finalVel, T _acceleration, float _time)
{
	return T(_finalVel - _acceleration * _time);
}

template<typename T>
inline void Physics::K1_CalcInitialVel(T _finalVel, T _acceleration, float _time, T & _initialVel)
{
	_initialVel = _finalVel - _acceleration * _time;
}

template<typename T>
inline T Physics::K1_CalcAcceleration(T _initialVel, T _finalVel, float _time)
{
	return T((_finalVel - _initialVel) / _time);
}

template<typename T>
inline void Physics::K1_CalcAcceleration(T _initialVel, T _finalVel, float _time, T & _acceleration)
{
	_acceleration = (_finalVel - _initialVel) / _time;
}

template<typename T>
inline T Physics::K1_CalcTime(T _initialVel, T _finalVel, T _acceleration)
{
	return T((_finalVel - _initialVel) / _acceleration);
}

//Vector3 Physics::K1_CalcTime(Vector3 _initialVel, Vector3 _finalVel, Vector3 _acceleration)
//{
//	// TODO: Calculate Time for Vector variables
//	return Vector3();
//}

template<typename T>
inline void Physics::K1_CalcTime(T _initialVel, T _finalVel, T _acceleration, float & _time)
{
	_time = (_finalVel - _initialVel) / _acceleration;
}

template<typename T>
inline T Physics::K2(T _initialVel, T _finalVel, float _time)
{
	return T(0.5f * (_initialVel + _finalVel) * _time);
}

template<typename T>
inline void Physics::K2(T _initialVel, T _finalVel, float _time, T & _distance)
{
	_distance = 0.5f * (_initialVel + _finalVel) * _time;
}

template<typename T>
inline T Physics::K3(T _initialVel, T _acceleration, float _time)
{
	return T((_initialVel * _time) + (0.5f * _acceleration * (_time * _time)));
}

template<typename T>
inline void Physics::K3(T _initialVel, T _acceleration, float _time, T & _distance)
{
	_distance = (_initialVel * _time) + (0.5f * _acceleration * (_time * _time));
}

template<typename T>
inline T Physics::K4(T _initialVel, T _acceleration, T _distance)
{
	return T((_initialVel * _initialVel) + (2.f * _acceleration * _distance));
}

template<typename T>
inline void Physics::K4(T _initialVel, T _acceleration, T _distance, T & _finalVel)
{
	_finalVel = (_initialVel * _initialVel) + (2.f * _acceleration * _distance);
}

template<typename T>
inline T Physics::CalcAcceleration(T _velocity, float _time)
{
	return T(_velocity / _time);
}

template<typename T>
inline void Physics::CalcAcceleration(T _velocity, float _time, T & _acceleration)
{
	_acceleration = _velocity / _time;
}
