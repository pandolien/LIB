#ifndef __dhMat_h__
#define __dhMat_h__

#include "dhVector.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define PI 3.141592
#define RAD(angle)(angle/(180/PI))
#define DEG(angle)(angle/(PI/180))
#define MIN(a,b)(a <b? a : b)
#define MAX(a,b)(a <b? b : a)
#define ABS(a)(a < 0? -a : a)
typedef struct {
	float a;
	float b;
	float r;
}_RPY;
typedef struct {
	float w,x,y,z;
}dhQuat;
class dhMat
{
public:
	dhMat();
	~dhMat();
public:
	float v[16];
public:
	dhMat Trans(float, float, float);
	dhMat RotX(float);
	dhMat RotY(float);
	dhMat RotZ(float);
	dhMat Scale(float);
	dhMat Scale(float, float);
	dhMat Scale(float, float,float);
	dhMat Scale(dhVector);
	dhMat Inv();
	dhMat Quat(float, dhVector);
	dhMat Quat(dhQuat&);
	dhQuat GetQuat();
	_RPY RPY();
	void Hmatrix(float, float, float, float, float, float);
	dhVector O();
public:
	dhMat operator*(dhMat);
	dhVector operator*(dhVector);
};
#endif
