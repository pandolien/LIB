#ifndef __LOBJ_H__
#define __LOBJ_H__
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include "3d/dhVertex.h"
#include "3d/dhMat.h"
#include "Obj/lPoly.h"
#include "Obj/lRGB.h"
class lObj
{
public:
	lObj();
	~lObj();
public:
	void Initialization();
	void GLDelete();
	void SetCam(void* c);
	void SetLight(void* l);
	BOOL SetShader(const char*, const char*);
	void SetAmbientColor(lRGB&);
	void SetDiffuseColor(lRGB&);
	void SetSpecularColor(lRGB&);
	void SetOptical(float, float, float);
	void SetScale(dhVector);
	dhMat GetH();
public:
	void SetGLBuffer(dhVertex*, int, lPoly*, int);
	void SetNorm(dhVertex*, int, lPoly*, int);
private://Object Moving And Camera Moveing
	void *c,*L;
	dhMat h, s;
	lRGB AmbientColor, DiffuseColor, SpecularColor;
	float Ambient, Diffuse, Specular;
	int vtn, pln;
	unsigned int VBO, VAO, EBO;
	unsigned int sdrProgram;
	BOOL BoneMode;
	dhVector Pivot;
public:
	void SetPivot(dhVector);
	void SetNotBone();
	void Triangle();
	void Floor(float, float);
	void Box(float, float, float);
	void Cylinder(float, float);
public://Out Setting
	void Draw();
	void Draw(dhMat t);
	void T(dhMat h);
public:
	float GetAmbient();
	float GetDiffuse();
	float GetSpecular();
	
};
#endif
