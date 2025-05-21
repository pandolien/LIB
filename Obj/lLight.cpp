#include "pch.h"
#include "lLight.h"

lLight::lLight()
{
}

lLight::~lLight()
{
}

void lLight::SetPoint(float x, float y, float z)
{
	p = dhVector(x, y, z);
}

void lLight::SetLightColor(BYTE r, BYTE g, BYTE b, BYTE d)
{
	lColor.SetRGBD(r, g, b, d);
}
