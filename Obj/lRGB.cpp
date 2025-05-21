#include "pch.h"
#include "Obj/lRGB.h"

lRGB::lRGB()
{
	r = 0;
	g = 0;
	b = 0;
	d = 0;
}

lRGB::lRGB(BYTE R, BYTE G, BYTE B,BYTE D)
{
	r = (float)R / (float)255;
	g = (float)G / (float)255;
	b = (float)B / (float)255;
	d = (float)D / (float)255;

}

lRGB::~lRGB()
{
}

void lRGB::SetRGBD(BYTE R, BYTE G, BYTE B, BYTE D)
{
	r = (float)R / (float)255;
	g = (float)G / (float)255;
	b = (float)B / (float)255;
	d = (float)D / (float)255;
}
