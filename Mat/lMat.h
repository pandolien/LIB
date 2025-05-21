#ifndef __LMAT_H__
#define __LMAT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
class lMat
{
public:
	lMat();
	lMat(int, int);
	lMat(lMat&);
	lMat(const lMat&);
	~lMat();
public:
	float* v;
	int r,c;
	bool temp;
public:
	void Initialization();
	void Init(int r, int c);
	void Init(const lMat&);
	void Delete();
public:
	bool isTemp() {
		return temp;
	}
	void Temp() {
		temp = true;
	}
	void NoTemp() {
		temp = false;
	}
	void TempRemove() {
		if (temp) {
			Delete();
		}
		NoTemp();
	}
public:
	void E();
	void transpose();
	void diag(float*,int);
	lMat T();
	lMat Inv();
	void Inverse();
public:
	void SVD(lMat&, lMat&, lMat&);
private:
	lMat UVector(lMat &);
	lMat SVTMake(lMat&, float *, lMat&);
	float Ramda(lMat&);
	lMat SVD_Reconstruction(lMat&,float , lMat&, lMat&);
	lMat Rand_Row_Vector(int);
	lMat Rand_Column_Vector(int);
public:
	lMat operator*(lMat);
	lMat operator*(float);
	lMat operator/(float);
	lMat operator+(lMat);
	lMat operator-(lMat);
	void operator=(lMat);
	float& In(int, int);
	float Out(int, int);
};
#endif
