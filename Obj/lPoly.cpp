#include "pch.h"
#include "Obj/lPoly.h"

lPoly::lPoly()
{
	Initialization();
}

lPoly::~lPoly()
{
	Close();
}

void lPoly::Initialization()
{
	f = 0;
	s = 0;
	t = 0;
}

void lPoly::Close()
{
	f = 0;
	s = 0;
	t = 0;
}

void lPoly::SetPoly(short f, short s , short t)
{
	this->f = f;
	this->s = s;
	this->t = t;
}
