#ifndef __LSPLINE_H__
#define __LSPLINE_H__
#include "lPTRList.h"
#include "3d/dhVector.h"
typedef struct {
	float *a, *b, *c, *d;
}SpConstant;
class lSpline
{
public:
	lSpline();
	~lSpline();
public:
	void Initialzation();
	void Close();
public:
	lPTRList Point;
	lPTRList line;
	lPTRList Time;
	SpConstant sc_x,sc_y,sc_z;
	float dt;
private:
	float h[3];
public:
	int GetSize();
	dhVector GetLine(int);
	void SetPoint(dhVector);
	void SetPoint(float,float,float);
	void SetTime(float);
	void SetDt(float);
	void SetComput();
	void SetSpline(float dt);
private:
	void GetA();
	void GetB();
	void GetC();
	void GetD();
	dhVector OperationSpline(float);

};
#endif
