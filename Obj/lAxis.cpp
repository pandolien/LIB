#include "pch.h"
#include "lAxis.h"

lAxis::lAxis()
{
}

lAxis::~lAxis()
{
}

void lAxis::Initialization()
{
	x.Cylinder(0.1, 1);
	y.Cylinder(0.1, 1);
	z.Cylinder(0.1, 1);
	x.SetAmbientColor(lRGB(255, 0, 0, 255));
	y.SetAmbientColor(lRGB(0, 255, 0, 255));
	z.SetAmbientColor(lRGB(0, 0, 255, 255));

	x.SetDiffuseColor(lRGB(255, 0, 0, 255));
	y.SetDiffuseColor(lRGB(0, 255, 0, 255));
	z.SetDiffuseColor(lRGB(0, 0, 255, 255));

	x.SetSpecularColor(lRGB(255, 0, 0, 255));
	y.SetSpecularColor(lRGB(0, 255, 0, 255));
	z.SetSpecularColor(lRGB(0, 0, 255, 255));

	x.T(h.RotY(RAD(-90)));
	y.T(h.RotX(RAD(90)));
	z.T(h.RotX(RAD(180)));
}

void lAxis::Close()
{
	x.GLDelete();
	y.GLDelete();
	z.GLDelete();
}

void lAxis::SetCamAndLight(void* c, void* l)
{
	x.SetCam(c);
	y.SetCam(c);
	z.SetCam(c);
	x.SetLight(l);
	y.SetLight(l);
	z.SetLight(l);
}

void lAxis::SetShader(const char* v, const char* c)
{
	x.SetShader(v, c);
	y.SetShader(v, c);
	z.SetShader(v, c);
}

void lAxis::T(dhMat h)
{
	this->h = h;
}

void lAxis::S(float s)
{
	dhVector v = dhVector(s, s, s);
	x.SetScale(v);
	y.SetScale(v);
	z.SetScale(v);
}

void lAxis::Draw()
{
	x.Draw(h);
	y.Draw(h);
	z.Draw(h);
}
