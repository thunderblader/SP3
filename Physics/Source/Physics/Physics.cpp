//#include "Physics.h"

//#include <cmath>
/*
template<class T>
T Physics<T>::K1(T _initialVel, T _acceleration, float _time)
{
	return T(_initialVel + _acceleration * _time);
}

template<class T>
void Physics<T>::K1(T _initialVel, T _acceleration, float _time, T & _finalVel)
{
	_finalVel = _initialVel + _acceleration * _time;
}

template<class T>
T Physics<T>::K1_CalcInitialVel(T _finalVel, T _acceleration, float _time)
{
	return T(_finalVel - _acceleration * _time);
}

template<class T>
void Physics<T>::K1_CalcInitialVel(T _finalVel, T _acceleration, float _time, T & _initialVel)
{
	_initialVel = _finalVel - _acceleration * _time;
}

template<class T>
T Physics<T>::K1_CalcAcceleration(T _initialVel, T _finalVel, float _time)
{
	return T((_finalVel - _initialVel) / _time);
}

template<class T>
void Physics<T>::K1_CalcAcceleration(T _initialVel, T _finalVel, float _time, T & _acceleration)
{
	_acceleration = (_finalVel - _initialVel) / _time;
}

template<class T>
T Physics<T>::K1_CalcTime(T _initialVel, T _finalVel, T _acceleration)
{
	return T((_finalVel - _initialVel) / _acceleration);
}

template<class T>
void Physics<T>::K1_CalcTime(T _initialVel, T _finalVel, T _acceleration, float & _time)
{
	_time = (_finalVel - _initialVel) / _acceleration;
}

Vector3 Physics<Vector3>::K1_CalcTime(Vector3 _initialVel, Vector3 _finalVel, Vector3 _acceleration)
{
	// TODO: Calculate Time for Vector variables
	return Vector3();
}

template<class T>
T Physics<T>::K2(T _initialVel, T _finalVel, float _time)
{
	return T(0.5f * (_initialVel + _finalVel) * _time);
}

template<class T>
void Physics<T>::K2(T _initialVel, T _finalVel, float _time, T & _distance)
{
	_distance = 0.5f * (_initialVel + _finalVel) * _time;
}

template<class T>
T Physics<T>::K3(T _initialVel, T _acceleration, float _time)
{
	return T((_initialVel * _time) + (0.5f * _acceleration * (_time * _time)));
}

template<class T>
void Physics<T>::K3(T _initialVel, T _acceleration, float _time, T & _distance)
{
	_distance = (_initialVel * _time) + (0.5f * _acceleration * (_time * _time));
}

template<class T>
T Physics<T>::K4(T _initialVel, T _acceleration, T _distance)
{
	return T((_initialVel * _initialVel) + (2.f * _acceleration * _distance));
}

template<class T>
void Physics<T>::K4(T _initialVel, T _acceleration, T _distance, T & _finalVel)
{
	_finalVel = (_initialVel * _initialVel) + (2.f * _acceleration * _distance);
}

template<class T>
T Physics<T>::CalcAcceleration(T _velocity, float _time)
{
	return T(_velocity / _time);
}

template<class T>
void Physics<T>::CalcAcceleration(T _velocity, float _time, T & _acceleration)
{
	_acceleration = _velocity / _time;
}
*/