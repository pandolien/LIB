#include "pch.h"
#include "lMesh.h"

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
lMesh::lMesh()
{
	this->Initialization();
}

lMesh::~lMesh()
{
	//Close();
}

void lMesh::Initialization()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	sdrProgram = 0;
}

void lMesh::Close()
{
	if (VAO != 0)glDeleteVertexArrays(1, &VAO);
	if (VBO != 0)glDeleteBuffers(1, &VBO);
	if (EBO != 0)glDeleteBuffers(1, &EBO);
	if (sdrProgram != 0)glDeleteProgram(sdrProgram);
	sdrProgram = 0;
	VAO = 0;
	VBO = 0;
	EBO = 0;
}

void lMesh::SetPivot(dhVector v)
{
	pivot = v;
}

int lMesh::SetShaderProgramID(lString&strVertexShader, lString&strFragmentShader)
{
	unsigned versdr,frgsdr;
	char infoLog[512] = { 0, };
	char* p;
	int success;
	if (this->vtn == 0 || this->pln == 0)return 0;
	versdr = glCreateShader(GL_VERTEX_SHADER);
	p = strVertexShader.GetC();
	glShaderSource(versdr, 1, &p, NULL);
	glCompileShader(versdr);
	glGetShaderiv(versdr, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(versdr, 512, NULL, infoLog);
		return 0;
	}
	frgsdr = glCreateShader(GL_FRAGMENT_SHADER);
	p = strFragmentShader.GetC();
	glShaderSource(frgsdr, 1, &p, NULL);
	glCompileShader(frgsdr);
	glGetShaderiv(frgsdr, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(frgsdr, 512, NULL, infoLog);
		infoLog[0] = infoLog[0];
		return 0;
	}
	sdrProgram = glCreateProgram();
	glAttachShader(sdrProgram, versdr);
	glAttachShader(sdrProgram, frgsdr);
	glLinkProgram(sdrProgram);
	glGetProgramiv(sdrProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(sdrProgram, 512, NULL, infoLog);
		infoLog[0] = infoLog[0];
		return 0;
	}
	glDeleteShader(versdr);
	glDeleteShader(frgsdr);
	return 1;
}

unsigned int lMesh::GetShaderID()
{
	return sdrProgram;
}

void lMesh::SetBuffer(dhVertex* pVt, int vtn, lPoly* pPl, int pln){
	this->vtn = vtn;
	this->pln = pln;
	if (this->vtn == 0 || this->pln == 0)return;
	SetNorm(pVt, vtn, pPl, pln);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dhVertex) * vtn, pVt, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lPoly) * pln, pPl, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void lMesh::SetNorm(dhVertex* pVt, int vtn, lPoly* pPl, int pln)
{
	int* N;
	dhVector n;
	dhVector v0, v1, v2, v01, v02;
	int i;
	N = new int[vtn];
	memset(N, 0, sizeof(int) * vtn);
	for (i = 0; i < pln; i++) {
		if (i == 125) {
			i = i;
		}
		pPl[i-1];
		v0 = pVt[pPl[i].f].GetPoint();
		v1 = pVt[pPl[i].s].GetPoint();
		v2 = pVt[pPl[i].t].GetPoint();
		v01 = v0 - v1;
		v02 = v0 - v2;
		n = v01 * v02;
		N[pPl[i].f] += 1;
		pVt[pPl[i].f].SetNomalVector(pVt[pPl[i].f].GetNormalVector() + n);
		N[pPl[i].s] += 1;
		pVt[pPl[i].s].SetNomalVector(pVt[pPl[i].s].GetNormalVector() + n);
		N[pPl[i].t] += 1;
		pVt[pPl[i].t].SetNomalVector(pVt[pPl[i].t].GetNormalVector() + n);
	}
	for (i = 0; i < vtn; i++) {
		n = pVt[i].GetNormalVector() / N[i];
		n = n.Unit();
		pVt[i].SetNomalVector(n.x, n.y, n.z);
	}
	delete[](N);
}


void lMesh::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, pln * 3, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}
