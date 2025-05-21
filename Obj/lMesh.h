#ifndef __LMESH_H__
#define __LMESH_H__

#include "3d/dhVertex.h"
#include "3d/dhVector.h"
#include "Obj/lPoly.h"
#include "lString.h"
class lMesh
{
public:
	lMesh();
	~lMesh();
public:
	int vtn, pln;
	unsigned int VBO, VAO, EBO;
	unsigned int sdrProgram;
	dhVector pivot;
public:
	void Initialization();
	void Close();
public:
	void SetPivot(dhVector);
	int SetShaderProgramID(lString&,lString&);
	unsigned int GetShaderID();
	void SetBuffer(dhVertex*, int, lPoly*, int);
protected:
	virtual void SetNorm(dhVertex*, int, lPoly*, int);
public:
	void Draw();
};
#endif
