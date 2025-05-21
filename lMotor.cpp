#include "pch.h"
#include "lMotor.h"

lMotor::lMotor()
{
	Initialzation();
}

lMotor::~lMotor()
{
	Close();
}

void lMotor::Initialzation()
{
	AngularVelocity = 0;//Motor Wheel Velocity
	AngularAcceleration = 0; //Motor Wheel Acceleration;
	AccumulateAngle = 0;
	Angle = 0;
	I = 0;//Inertia
	I_motor = 0;//MotorInertia;
	Kp = 0;
	Ki = 0;
	Kd = 0;// PID Gain
	oe = 0;
	e_integral = 0;//Old Error
	Tv = 0;//Target Velocity
	Decelerator = 1;
	dt = 0;
}

void lMotor::Close()
{
}

void lMotor::SetDt(float Dt)
{
	dt = Dt;
}

void lMotor::SetPIDControl(float p, float i, float d)
{
	Kp = p;
	Ki = i; 
	Kd = d;
}

void lMotor::SetInertia(float I)
{
	this->I = I;
}

void lMotor::SetWheelInertia(float M, float R)
{
	I = (M * R * R) / 2;
}

void lMotor::SetBarInertia(float M, float L)
{
	I = (M * L * L) / 3;
}

void lMotor::SetCenterBarInertia(float M, float L)
{
	I = (M * L * L) / 12;
}

void lMotor::SetMotorInertia(float M, float R)
{
	I_motor = (M * R * R) / 2;
}

void lMotor::SetMotorInertia(float I)
{
	I_motor = I;
}

void lMotor::SetTargetVelocity(float tv)
{
	Tv = tv;
}

void lMotor::SetDecelerator(float R)
{
	Decelerator = R * R;
}

float lMotor::GetInertia()
{
	return I+I_motor* Decelerator* Decelerator;
}

void lMotor::Update()
{
	float I,e,p,i,d,PID;
	float Ang;
	float vel;
	I = GetInertia();
	e = Tv - AngularVelocity;
	e_integral += (e * dt);
	p = e * Kp;
	i = Ki * e_integral;
	d = Kd * ((e - oe) / dt);
	oe = e;
	PID = p + i + d;
	AngularAcceleration = (PID / I ) * dt;
	AngularVelocity += (AngularAcceleration*dt);
	vel = (AngularVelocity * dt);
	Angle += vel;
	AccumulateAngle += vel;
	Ang = DEG(Angle);
	if (Ang > 180) Ang = -360 + Ang;
	if (Ang < -180) Ang = 360 + Ang;
	Angle = RAD(Ang);
}
