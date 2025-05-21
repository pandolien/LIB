#include "pch.h"
#include "lSpline.h"
#include "Mat/lMat.h"
lSpline::lSpline()
{
	Initialzation();
}

lSpline::~lSpline()
{
	Close();
}

void lSpline::Initialzation()
{
	line.Initialization();
	Point.Initialization();
	Time.Initialization();
	memset(&sc_x, 0, sizeof(SpConstant));
	memset(&sc_y, 0, sizeof(SpConstant));
	memset(&sc_z, 0, sizeof(SpConstant));
}

void lSpline::Close()
{
	line.RemoveAll();
	Point.RemoveAll();
	Time.RemoveAll();
	if (sc_x.a) delete[](sc_x.a);
	if (sc_y.a) delete[](sc_y.a);
	if (sc_z.a) delete[](sc_z.a);

	if (sc_x.b) delete[](sc_x.b);
	if (sc_y.b) delete[](sc_y.b);
	if (sc_z.b) delete[](sc_z.b);

	if (sc_x.c) delete[](sc_x.c);
	if (sc_y.c) delete[](sc_y.c);
	if (sc_z.c) delete[](sc_z.c);

	if (sc_x.d) delete[](sc_x.d);
	if (sc_y.d) delete[](sc_y.d);
	if (sc_z.d) delete[](sc_z.d);
	memset(&sc_x, 0, sizeof(SpConstant));
	memset(&sc_y, 0, sizeof(SpConstant));
	memset(&sc_z, 0, sizeof(SpConstant));
}

int lSpline::GetSize()
{
	return line.GetSize();
}

dhVector lSpline::GetLine(int i)
{
	dhVector* v = (dhVector*)line.Search(i);
	if (v == NULL)return dhVector();
	return dhVector(v->x,v->y,v->z);
}

void lSpline::SetPoint(dhVector sp)
{
	dhVector* pp = new dhVector();
	*pp = sp;
	Point.AddTail(pp);
}

void lSpline::SetPoint(float x, float y , float z)
{
	dhVector* pP = new dhVector();
	pP->x = x;
	pP->y = y;
	pP->z = z;
	Point.AddTail(pP);
}

void lSpline::SetTime(float time)
{
	float* t = new float;
	*t = time;
	Time.AddTail(t);
}



void lSpline::SetDt(float dt)
{
	this->dt = dt;
}

void lSpline::SetComput()
{
	float* t0,* t1;
	int i;
	for (i = 0; i < 3; i++) {
		t0 = (float*)Time.Search(i);
		t1 = (float*)Time.Search(i+1);
		h[i] = *t1 - *t0;
	}
	GetA();
	GetC();
	GetB();
	GetD();

}

void lSpline::SetSpline(float dt)
{
	int i,n;
	float* st = (float*)Time.Search(0),*et = (float*)Time.Search(Time.GetSize()-1),t;
	t = *st;
	n = (int)((*et - *st) / dt);
	for (i = 0; i < n - 1; i++)
	{
		dhVector* p = new dhVector();
		*p = OperationSpline(t);
		
		line.AddTail(p);
		t += dt;
	}
}

void lSpline::GetA()
{
	int i;

	sc_x.a = new float[4];
	sc_y.a = new float[4];
	sc_z.a = new float[4];
	memset(sc_x.a, 0, sizeof(float) * 4);
	memset(sc_y.a, 0, sizeof(float) * 4);
	memset(sc_z.a, 0, sizeof(float) * 4);
	for (i = 0; i < 4; i++) {
		dhVector* vt = (dhVector*)Point.Search(i);
		sc_x.a[i] = vt->x;
		sc_y.a[i] = vt->y;
		sc_z.a[i] = vt->z;
	}
	return;
}

void lSpline::GetB()
{
	int i;
	sc_x.b = new float[4];
	sc_y.b = new float[4];
	sc_z.b = new float[4];

	memset(sc_x.b, 0, sizeof(float) * 4);
	memset(sc_y.b, 0, sizeof(float) * 4);
	memset(sc_z.b, 0, sizeof(float) * 4);

	for (i = 0; i < 3; i++) {
		sc_x.b[i] = (sc_x.a[i + 1] - sc_x.a[i]) / h[i] - (2 * sc_x.c[i] + sc_x.c[i + 1]) * h[i] / 3;
		sc_y.b[i] = (sc_y.a[i + 1] - sc_y.a[i]) / h[i] - (2 * sc_y.c[i] + sc_y.c[i + 1]) * h[i] / 3;
		sc_z.b[i] = (sc_z.a[i + 1] - sc_z.a[i]) / h[i] - (2 * sc_z.c[i] + sc_z.c[i + 1]) * h[i] / 3;
	}

}

void lSpline::GetC()
{
	lMat A,x,y,z,c;
	
	A.Init(3, 3);
	x.Init(1, 3);

	A.In(0, 0) = 2 * (h[0] + h[1]);
	A.In(1, 1) = 2 * (h[1] + h[2]);
	A.In(2, 2) = 2 * (h[2]);

	A.In(0, 1) = h[1];
	A.In(1, 0) = h[1];
	A.In(1, 2) = h[2];
	A.In(2, 1) = h[2];

	A.Inverse();


	sc_x.c = new float[4];
	sc_y.c = new float[4];
	sc_z.c = new float[4];

	memset(sc_x.c, 0, sizeof(float) * 4);
	memset(sc_y.c, 0, sizeof(float) * 4);
	memset(sc_z.c, 0, sizeof(float) * 4);

	x.In(0, 0) = ((sc_x.a[2] - sc_x.a[1]) / h[1] - (sc_x.a[1] - sc_x.a[0]) / h[0])* 3;
	x.In(0, 1) = ((sc_x.a[3] - sc_x.a[2]) / h[2] - (sc_x.a[2] - sc_x.a[1]) / h[0]) * 3;
	x.In(0, 2) = 0;
	c = A * x;

	sc_x.c[0] = c.Out(0, 0);
	sc_x.c[1] = c.Out(0, 1);
	sc_x.c[2] = c.Out(0, 2);

	x.In(0, 0) = ((sc_y.a[2] - sc_y.a[1]) / h[1] - (sc_y.a[1] - sc_y.a[0]) / h[0]) * 3;
	x.In(0, 1) = ((sc_y.a[3] - sc_y.a[2]) / h[2] - (sc_y.a[2] - sc_y.a[1]) / h[0]) * 3;
	x.In(0, 2) = 0;
	float a = x.Out(0, 0);
	float b = x.Out(0, 1);
	c = A * x;

	sc_y.c[0] = c.Out(0, 0);
	sc_y.c[1] = c.Out(0, 1);
	sc_y.c[2] = c.Out(0, 2);

	x.In(0, 0) = ((sc_z.a[2] - sc_z.a[1]) / h[1] - (sc_z.a[1] - sc_z.a[0]) / h[0]) * 3;
	x.In(0, 1) = ((sc_z.a[3] - sc_z.a[2]) / h[2] - (sc_z.a[2] - sc_z.a[1]) / h[0]) * 3;
	x.In(0, 2) = 0;
	c = A * x;
	sc_z.c[0] = c.Out(0, 0);
	sc_z.c[1] = c.Out(0, 1);
	sc_z.c[2] = c.Out(0, 2);

	return;
}

void lSpline::GetD()
{
	int i;
	sc_x.d = new float[4];
	sc_y.d = new float[4];
	sc_z.d = new float[4];

	memset(sc_x.d, 0, sizeof(float) * 4);
	memset(sc_y.d, 0, sizeof(float) * 4);
	memset(sc_z.d, 0, sizeof(float) * 4);
	for (i = 0; i < 3; i++) {
		sc_x.d[i] = (sc_x.c[i + 1] - sc_x.c[i]) / (3 * h[i]);
		sc_y.d[i] = (sc_y.c[i + 1] - sc_y.c[i]) / (3 * h[i]);
		sc_z.d[i] = (sc_z.c[i + 1] - sc_z.c[i]) / (3 * h[i]);
	}
}

dhVector lSpline::OperationSpline(float t)
{
	int i;
	float* st,*et, dt;
	dhVector v;
	for (i = 0; i < Time.GetSize()-1; i++){
		st = (float*)Time.Search(i);
		et = (float*)Time.Search(i+1);
		if (t >= *st&&t < *et) {
			dt = t - *st;
			v.x = sc_x.a[i] + sc_x.b[i] * dt + sc_x.c[i] * dt * dt + sc_x.d[i] * dt * dt * dt;
			v.y = sc_y.a[i] + sc_y.b[i] * dt + sc_y.c[i] * dt * dt + sc_y.d[i] * dt * dt * dt;
			v.z = sc_z.a[i] + sc_z.b[i] * dt + sc_z.c[i] * dt * dt + sc_z.d[i] * dt * dt * dt;
			break;
		}
	}
	return v;
}
