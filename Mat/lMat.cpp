#include "pch.h"
#include "3d/dhMat.h"
#include "Mat/lMat.h"
#include "lRand.h"
#ifdef __cplusplus
#define LAPACK_ROW_MAJOR               101
#define LAPACK_COL_MAJOR               102
extern "C" {
	int LAPACKE_sgesvd_work(int matrix_order, char jobu, char jobvt,
		int m, int n, float* a,
		int lda, float* s, float* u,
		int ldu, float* vt, int ldvt,
		float* work,int lwork);
}
#endif
lMat::lMat()
{
	Initialization();
}

lMat::lMat(int r, int c)
{
	Initialization();
	Init(r, c);
}

lMat::lMat(lMat& v)
{
	Initialization();
	Init((const lMat&)v);
	v.TempRemove();
}
lMat::lMat(const lMat& v) { 
	Initialization();
	Init(v);
}

lMat::~lMat()
{
	if (temp)return;
	Delete();
}

void lMat::Initialization()
{
	v = NULL;
	r = 0;
	c = 0;
	NoTemp();
}

void lMat::Init(int r, int c)
{
	Delete();
	if (r == 0 || c == 0)return;
	v = new float[r*c];
	memset(v, 0,sizeof(float) *r * c);
	this->r = r;
	this->c = c;
}

void lMat::Init(const lMat& v)
{
	Delete();
	if (v.temp) {
		this->v = v.v;
		this->r = v.r;
		this->c = v.c;
		Temp();
		return;
	}
	else {
		Init(v.r, v.c);
		memmove(this->v, v.v, sizeof(float) * r * c);
		NoTemp();
	}
}

void lMat::Delete()
{
	if (v)delete[](v);
	v = NULL;
}

void lMat::E()
{
	int i,n;
	if (r > c) n = c;
	else n = r;
	for (i = 0; i < n; i++) {
		v[i * i] = 1;
	}
}

void lMat::transpose()
{
	int i, j, cnt = 0;
	int r = this->c,c  = this->r;
	float* v = new float[r * c];
	for (i = 0; i < c; i++) {
		for (j = 0; j < r; j++) {
			v[j * c + i] = this->v[cnt++];
		}
	}
	Delete();
	this->v = v;
	this->r = r;
	this->c = c;
}

void lMat::diag(float*v, int n)
{
	int i;
	Init(n, n);
	for (i = 0; i < n; i++) {
		this->v[i * n + i] = v[i];
	}
}

lMat lMat::T()
{
	lMat out;
	int i, j, cnt = 0;
	int r = this->c, c = this->r;
	float* v = new float[r * c];
	for (i = 0; i < c; i++) {
		for (j = 0; j < r; j++) {
			v[j * c + i] = this->v[cnt++];
		}
	}
	out.v = v;
	out.r = r;
	out.c = c;
	out.Temp();
	NoTemp();
	return out;
}

#ifdef __cplusplus
void lMat::SVD(lMat& U, lMat& S, lMat& Vt) {
	
	float* s;
	int p,lda;
	int lwork = -1,info;
	float work_query;
	float* work;
	if (r < c) { p = r;  lda = c;}
	else { p = c; lda = r; }
	s = new float[p];
	
	U.Init(c, c);
	Vt.Init(r, r);
	
	info = LAPACKE_sgesvd_work(LAPACK_COL_MAJOR,'A','A',r,c,v,lda,s,U.v,r,Vt.v,c,&work_query,lwork);
	if (info != 0) { 
		U.Delete();
		Vt.Delete();
		S.Delete();
		return; 
	}
	lwork = (int)work_query;
	work = new float[lwork];
	info = LAPACKE_sgesvd_work(LAPACK_COL_MAJOR, 
		'A', 'A', r, c, v, lda, s, 
		U.v, r, Vt.v, c, work, lwork);
	S.diag(s, p);
	delete[](s);
	delete[](work);
	NoTemp();
}

#else
void lMat::SVD(lMat& U, lMat& S, lMat& Vt)
{
	lMat A, u, vt;
	int n, i, j, k, cnt = 0;
	float e, ramda;
	if (this->v == NULL)return;
	if (r < c) { n = r; }
	else { n = c; }
	U.Init(n, c);
	S.Init(n, n);
	Vt.Init(r, n);
	A = *this;
	for (i = 0; i < n; i++) {
		if (A.r >= A.c) {
			u = this->UVector(A);
			vt = SVTMake(A, &ramda, u);
			vt.transpose();
		}
		else {
			vt = this->UVector(A);
			//break;
			u = SVTMake(A, &ramda, vt);
			vt.transpose();
		}
		float a;
		for (j = 0; j < c; j++) {
			a = u.v[j];
			U.In(i, j) = a;
		}
		for (j = 0; j < r; j++) {
			a = vt.v[j];
			Vt.In(j, i) = vt.v[j];
		}
		S.In(i, i) = ramda;
		A = SVD_Reconstruction(A, ramda, u, vt);
	}
	NoTemp();
}

#endif

lMat lMat::UVector(lMat& A)
{
	lMat At,A2,U,old;
	int i,j;
	float e,ramda;
	At = A.T();
	if (A.r >= A.c) { A2 = A * At; }
	else { A2 = At * A; }
	U = this->Rand_Column_Vector(A2.r);
	old = U;
	for (i = 0; i < 100; i++) {
		U= A2 * U;
		U = U / Ramda(U);
		e = 0;
		for (j = 0; j < U.r * U.c; j++) { 
			float e1 = U.v[j] * old.v[j];
			e += e1;
		}
		if (e >= 1.0-1e-6f)break;
		old = U;
	}
	U.Temp();
	NoTemp();
	return U;
}

lMat lMat::SVTMake(lMat& A, float* s, lMat&u)
{
	lMat out,At;
	At = A.T();
	if (A.r >= A.c)out = At*u;
	else out = A * u;
	*s = Ramda(out);
	out = out / *s;
	out.Temp();
	NoTemp();
	return out;
}

float lMat::Ramda(lMat& v)
{
	int i;
	float out = 0;
	for (i = 0; i < v.r * v.c; i++) { 
		out += v.v[i] * v.v[i]; 
		out = out;
	}
	out = sqrt(out);
	NoTemp();
	return out;
}

lMat lMat::SVD_Reconstruction(lMat& A,float r, lMat& u, lMat&vt)
{
	lMat out,ut,v,h;
	int i,j;
	ut = u.T();
	v = vt.T();
	if (A.r >= A.c)h = u * vt;
	else h = u * vt;

	h = h * r;
	out = A - h;

	out.Temp();
	NoTemp();
	return out;
}

lMat lMat::Rand_Row_Vector(int r)
{
	lMat out;
	lRand rd;
	int i;
	float dt, Unit = 0;
	out.Init(r, 1);
	out.Temp();
	for (i = 0; i < r; i++) {	
		dt =  rd.Gaussian();
		out.v[i] = dt;
		Unit += dt*dt;
	}
	Unit = sqrt(Unit);
	out= out/Unit;
	Unit = 0;
	for (i = 0; i < c; i++) {
		Unit += out.v[i] * out.v[i];
	}
	Unit = sqrt(Unit);
	NoTemp();
	return out;
}

lMat lMat::Rand_Column_Vector(int c)
{
	lMat out;
	lRand rd;
	int i;
	float dt, Unit = 0;
	out.Init(1,c);
	out.Temp();
	for (i = 0; i < c; i++) {	
		dt = rd.Gaussian();
		out.v[i] = dt;
		Unit += dt*dt;
	}
	Unit = sqrt(Unit);
	out = out/Unit;
	Unit = 0;
	for (i = 0; i < c; i++) {
		Unit += out.v[i] * out.v[i];
	}
	Unit = sqrt(Unit);
	NoTemp();
	return out;
}

lMat lMat::Inv()
{
	lMat out,U, S, Vt;
	int i;
	SVD(U, S, Vt);
	Vt.transpose();
	U.transpose();
	for (i = 0; i < S.r; i++) {
		S.In(i, i) = 1 / S.Out(i, i);
	}
	out = Vt * S * U;
	out.Temp();
	NoTemp();
	return out;
}

void lMat::Inverse()
{
	lMat out, U, S, Vt;
	int i;
	SVD(U, S, Vt);
	Vt.transpose();
	U.transpose();
	for (i = 0; i < S.r; i++) {
		S.In(i, i) = 1 / S.Out(i, i);
	}
	out = Vt * S * U;
	*this = out;
	NoTemp();
}

lMat lMat::operator*(lMat v)
{
	lMat out;
	int i, j, k, cnt = 0;
	float sum;
	if (this->r != v.c) {
		v.TempRemove();
		NoTemp();
		return out;
	}
	out.Init(v.r, this->c);
	for (i = 0; i < v.r; i++) {
		for (j = 0; j < this->c; j++) {
			sum = 0.0;
			for (k = 0; k < this->r; k++) {
				float a = this->v[k * this->c + j];
				float b = v.v[i * v.c + k];
				sum += a * b;
			}
			out.v[cnt++] = sum;
		}
	}
	out.Temp();
	v.TempRemove();
	NoTemp();
	return out;
}

lMat lMat::operator*(float other)
{
	lMat out;
	int i;
	out.Init(r, c);
	for (i = 0; i < r * c; i++)
		out.v[i] = v[i] * other;
	out.Temp();
	NoTemp();
	return out;
}

lMat lMat::operator/(float other)
{
	lMat out;
	int i;
	out.Init(r, c);
	for (i = 0; i < r * c; i++)
		out.v[i] = v[i] / other;
	out.Temp();
	NoTemp();
	return out;
}

lMat lMat::operator+(lMat v)
{
	lMat out;
	int i;
	if (this->r != v.r || this->c != v.c)return out;
	out.Init(r, c);
	for (i = 0; i < r * c; i++)
		out.v[i] = this->v[i] + v.v[i];
	out.Temp();
	NoTemp();
	return out;
}

lMat lMat::operator-(lMat v)
{
	lMat out;
	int i;
	if (this->r != v.r || this->c != v.c)return out;
	out.Init(r, c);
	for (i = 0; i < r * c; i++)
		out.v[i] = this->v[i] - v.v[i];
	out.Temp();
	NoTemp();
	return out;
}

void lMat::operator=(lMat v)
{
	Init(v.r, v.c);
	memmove(this->v, v.v, sizeof(float) * r * c);
	NoTemp();
	v.TempRemove();
}

float& lMat::In(int r, int c)
{
	float err;
	int a = r * this->c + c;
	if (this->r <= r) return err;
	if (this->c <= c)return err;
	NoTemp();
	return v[a];
	// TODO: 여기에 return 문을 삽입합니다.
}

float lMat::Out(int r, int c)
{
	float err;
	if (this->r <= r) return 0.0;
	if (this->c <= c)return 0.0;
	NoTemp();
	return v[r * this->c + c];
}