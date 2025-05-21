#ifndef __LCAM_H__
#define __LCAM_H__
#include "3d/dhMat.h"
class lCam
{
public:
	lCam();
	~lCam();
public:
	void Initialization();
public:
	dhMat Cam;
	dhMat Projection;
public:
	void SETAspectRatioAndFOV(float a, float w, float h);
	void T(dhMat h);
private:
	dhVector p,r,u,f;
public:
	void SetFreeMovement(float,float);
	void SetFreeMovement(dhVector&, float, float);
	void SetInitFreeMovement();
	void FreeMovement();
};

#endif