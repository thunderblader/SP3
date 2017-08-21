#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H
#include "Vector3.h"

template<class T>
class Physics
{
public:
	static T K1(T _initialVel, T _acceleration, float _time);
	static void K1(T _initialVel, T _acceleration, float _time, T& _finalVel);
	static T K1_CalcInitialVel(T _finalVel, T _acceleration, float _time);
	static void K1_CalcInitialVel(T _finalVel, T _acceleration, float _time, T& _initialVel);
	static T K1_CalcAcceleration(T _initialVel, T _finalVel, float _time);
	static void K1_CalcAcceleration(T _initialVel, T _finalVel, float _time, T& _acceleration);
	static T K1_CalcTime(T _initialVel, T _finalVel, T _acceleration);
	static void K1_CalcTime(T _initialVel, T _finalVel, T _acceleration, float& _time);

	static T K2(T _initialVel, T _finalVel, float _time);
	static void K2(T _initialVel, T _finalVel, float _time, T& _distance);
	static T K2_CalcInitialVel();
	static void K2_CalcInitialVel();
	static T K2_CalcFinalVel();
	static void K2_CalcFinalVel();
	static T K2_CalcTime();
	static void K2_CalcTime();

	static T K3(T _initialVel, T _acceleration, float _time);
	static void K3(T _initialVel, T _acceleration, float _time, T& _distance);
	static T K3_CalcInitialVel();
	static void K3_CalcInitialVel();
	static T K3_CalcAcceleration();
	static void K3_CalcAcceleration();

	static T K4(T _initialVel, T _acceleration, T _distance);
	static void K4(T _initialVel, T _acceleration, T _distance, T& _finalVel);

	static T CalcAcceleration(T _velocity, float _time);
	static void CalcAcceleration(T _velocity, float _time, T& _acceleration);

private:

};
//#include "Physics.cpp"

#endif // !PHYSICS_H
