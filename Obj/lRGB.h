#pragma once
class lRGB
{
public:
	lRGB();
	lRGB(BYTE R,BYTE G,BYTE B,BYTE D);
	~lRGB();
public:
	float r, g, b,d;
public:
	void SetRGBD(BYTE R, BYTE G, BYTE B, BYTE D);

};

