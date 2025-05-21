#ifndef __lAxis_H__
#define __lAxis_H__
#include "Obj/lObj.h"
#include "3d/dhMat.h"
class lAxis {
public:
	lAxis();
	~lAxis();
public:
	void Initialization();
	void Close();
public:
	lObj x, y, z;
	dhMat h;
public:
	void SetCamAndLight(void *c,void *l);
	void SetShader(const char*,const char*);
	void T(dhMat h);
	void S(float s);
	void Draw();
};
#endif