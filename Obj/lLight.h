#ifndef __LLIGHT_H__
#define __LLIGHT_H__
#include "3d/dhVector.h"
#include "lRGB.h"
class lLight
{
public:
	lLight();
	~lLight();
public:
	dhVector p;
	lRGB lColor;
public:
	void SetPoint(float, float, float);
	void SetLightColor(BYTE, BYTE, BYTE, BYTE);
};
#endif
