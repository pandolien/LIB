#ifndef __LMOTOR_H__
#define __LMOTOR_H__
#include "lMotor.h"
#include "dhMat.h"
class lMotor
{
public:
	lMotor();
	~lMotor();
public:
	void Initialzation();
	void Close();
public:
	float AngularVelocity;//Motor Wheel Velocity
	float AngularAcceleration;//Motor Wheel Acceleration;
	float Angle;// Motor Wheel Angle//rad
	float AccumulateAngle;
private:
	float I;//Inertia
	float I_motor;//MotorInertia;
	float Kp, Ki, Kd;// PID Gain
	float oe,e_integral;//Old Error
	float Tv;//Target Velocity
	float Decelerator;//Decelerator
	float dt;
public:
	void SetDt(float);
	void SetPIDControl(float, float, float);
	void SetInertia(float);
	void SetWheelInertia(float,float);
	void SetBarInertia(float,float);
	void SetCenterBarInertia(float,float);
	void SetMotorInertia(float, float);
	void SetMotorInertia(float);
	void SetTargetVelocity(float);
	void SetDecelerator(float);
public:
	float GetInertia();
public:
	void Update();
};
#endif