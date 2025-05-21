#ifndef __DHVERTEX_H__
#define __DHVERTEX_H__
#include "3d/dhVector.h"
class dhVertex
{
public:
	dhVertex();
	~dhVertex();
public:
	void Initialization();
	void Close();
public:
	float x, y, z;
	float nx, ny, nz;
public:
	void SetPoint(float, float, float);
	void SetPoint(dhVector v);
	void SetNomalVector(float, float, float);
	void SetNomalVector(dhVector v);
	dhVector GetPoint();
	dhVector GetNormalVector();
	void AddNrom(dhVector );
};
#endif