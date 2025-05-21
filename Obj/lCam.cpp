#include "pch.h"
#include "lCam.h"

lCam::lCam()
{
	Initialization();
}

lCam::~lCam()
{
}

void lCam::Initialization()
{
	int n, f;
	n = 1;
	f = 65535;
	Projection.v[10] = (n + f )/ (n - f);
	Projection.v[11] = -1;
	Projection.v[14] = 2*n*f/(n-f);
	Projection.v[15] = 0;

}

void lCam::SETAspectRatioAndFOV(float a, float w, float h)
{
	float a_ = a / 2;
	float cot = cos(a_) / sin(a_);
	float ratio = w / h;
	Projection.v[0] = cot / ratio;
	Projection.v[5] = cot ;
}

void lCam::T(dhMat h)
{
	Cam = h;
}

void lCam::SetFreeMovement(float q,float phi)
{
	dhVector u_vt;
	f.x = cos(q) * cos(phi);
	f.y = sin(phi);
	f.z = sin(q) * cos(phi);
	u_vt = dhVector(0, 0, 1);
	r = f * u_vt;
	r = r.Unit();
	u = f * r;
	u = u.Unit();
}

void lCam::SetFreeMovement(dhVector& v,float velocity, float dt)
{
	p = p + (v * velocity * dt);
}

void lCam::SetInitFreeMovement()
{
	p = dhVector(0, 0, 10);
	f = dhVector(0, 0, -1);
	r= dhVector(1, 0, 0);
	u = dhVector(0, 1, 0);
}

void lCam::FreeMovement()
{
	Cam.v[0] = r.x;
	Cam.v[1] = r.y;
	Cam.v[2] = r.z;

	Cam.v[4] = u.x;
	Cam.v[5] = u.y;
	Cam.v[6] = u.z;

	Cam.v[8] = -f.x;
	Cam.v[9] = -f.y;
	Cam.v[10] = -f.z;

	Cam.v[3] = (p * -1).Dot(r);
	Cam.v[7] = (p * -1).Dot(u);
	Cam.v[11] = (p * -1).Dot(f);

}

