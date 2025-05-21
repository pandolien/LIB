#include "pch.h"
#include "3d/dhVertex.h"

dhVertex::dhVertex()
{
	Initialization();
}

dhVertex::~dhVertex()
{
	Close();
}

void dhVertex::Initialization()
{
	x = 0; y = 0; z = 0;
	nx = 0; ny = 0; nz = 0;
}

void dhVertex::Close()
{
	x = 0; y = 0; z = 0;
	nx = 0; ny = 0; nz = 0;
}

void dhVertex::SetPoint(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void dhVertex::SetPoint(dhVector v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

void dhVertex::SetNomalVector(float nx, float ny, float nz)
{
	this->nx = nx;
	this->ny = ny;
	this->nz = nz;
}

void dhVertex::SetNomalVector(dhVector v)
{
	nx = v.x;
	ny = v.y;
	nz = v.z;
}

dhVector dhVertex::GetPoint()
{
	dhVector v = dhVector(x, y, z);
	return v;
}

dhVector dhVertex::GetNormalVector()
{
	return dhVector(nx,ny,nz);
}

void dhVertex::AddNrom(dhVector v)
{
	nx += v.x;
	ny += v.y;
	nz += v.z;

}
