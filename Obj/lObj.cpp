#include "pch.h"
#include "Obj/lObj.h"
#include "lString.h"
#include "lToken.h"
#include "Obj/lCam.h"
#include "Obj/lLight.h"
lObj::lObj()
{
	Initialization();
}

lObj::~lObj()
{
}

void lObj::Initialization()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	sdrProgram = 0;
	AmbientColor = lRGB(255, 255, 255, 255);
	DiffuseColor = lRGB(255, 255, 255, 255);
	SpecularColor = lRGB(255, 255, 255, 255);
	this->Ambient = 1;
	this->Diffuse = 1;
	this->Specular = 1;
	BoneMode = TRUE;
}



void lObj::GLDelete()
{
	if(VAO != 0)glDeleteVertexArrays(1, &VAO);
	if (VBO != 0)glDeleteBuffers(1, &VBO);
	if (EBO != 0)glDeleteBuffers(1, &EBO);
	if(sdrProgram != 0)glDeleteProgram(sdrProgram);
	sdrProgram = 0;
	VAO = 0;
	VBO = 0;
	EBO = 0;
}

void lObj::SetGLBuffer(dhVertex* pVt, int vtn, lPoly* pPl, int pln)
{
	if (BoneMode == true)return;
	if (pVt != NULL || pPl != NULL) {
		SetNotBone();
	}
	else return;

	this->vtn = vtn;
	this->pln = pln;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dhVertex)*vtn, pVt, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lPoly)*pln, pPl , GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void lObj::SetNorm(dhVertex* pVt, int vtn, lPoly* pPl,int pln)
{
	int* N;
	dhVector n;
	dhVector v0, v1, v2,v01,v02;
	int i;
	if (pVt != NULL || pPl != NULL) {
		SetNotBone();
	}
	else return;
	if (BoneMode == true)return;
	N = new int[vtn];
	memset(N, 0, sizeof(int) * vtn);
	for (i = 0; i < pln; i++) {
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
		pVt[i].SetNomalVector(n.x,n.y,n.z);
	}
	delete[](N);
}

void lObj::SetPivot(dhVector v)
{
	Pivot = v;
}

void lObj::SetNotBone()
{
	BoneMode = FALSE;
}

void lObj::Triangle()
{
	SetNotBone();
	dhVertex* pVt;
	lPoly* pPl;
	
	this->GLDelete();
	vtn = 3; pln = 1;
	pVt = new dhVertex[vtn];
	pPl = new lPoly[pln];
	pVt[0].SetPoint(0, 0, 0);
	pVt[1].SetPoint(1, 0, 0);
	pVt[2].SetPoint(0, 1, 0);
	pPl[0].SetPoly(0, 1, 2);
	SetNorm(pVt,vtn,pPl,pln);
	SetGLBuffer(pVt, vtn, pPl, pln);
	delete[](pVt);
	delete[](pPl);

	//
}

void lObj::Floor(float x, float y)
{
	SetNotBone();
	dhVertex* pVt;
	lPoly* pPl;
	int i, j, cnt = 0;
	float X = x / 6, Y = y / 6,nx = x/2,ny = y/2;
	float ;
	vtn = 7 * 7; pln = 6 * 6 * 2;
	pVt = new dhVertex[vtn];
	pPl = new lPoly[pln];
	for (i = 0; i < 7; i++) 
		for (j = 0; j < 7; j++) 
			pVt[cnt++].SetPoint(X * j - nx, Y * i - ny, 0);
	cnt = 0;
	for (i = 0; i < 6; i++) 
		for (j = 0; j < 6; j++) {
			pPl[cnt++].SetPoly(i * 7 + j, i * 7 + j + 1, (i + 1) * 7 + (j + 1));
			pPl[cnt++].SetPoly(i * 7 + j, (i + 1) * 7 + (j + 1), (i + 1) * 7 + j);
		}
	SetNorm(pVt, vtn, pPl, pln);
	SetGLBuffer(pVt, vtn, pPl, pln);
	delete[](pVt);
	delete[](pPl);
}

void lObj::Box(float x, float y, float z)
{
	SetNotBone();
	dhVertex* pVt;
	lPoly* pPl;
	vtn = 8; pln = 12;
	pVt = new dhVertex[8];
	pPl = new lPoly[12];
	pVt[0].SetPoint(0, 0, 0);
	pVt[1].SetPoint(x, 0, 0);
	pVt[2].SetPoint(x, y, 0);
	pVt[3].SetPoint(0, y, 0);
	pVt[4].SetPoint(0, 0, z);
	pVt[5].SetPoint(x, 0, z);
	pVt[6].SetPoint(x, y, z);
	pVt[7].SetPoint(0, y, z);

	pPl[0].SetPoly(0, 3, 2);
	pPl[1].SetPoly(0, 2, 1);
	pPl[2].SetPoly(0, 3, 7);
	pPl[3].SetPoly(0, 7, 4);
	pPl[4].SetPoly(0, 1, 5);
	pPl[5].SetPoly(0, 5, 4);
	pPl[6].SetPoly(6, 7, 4);
	pPl[7].SetPoly(6, 4, 5);
	pPl[8].SetPoly(6, 2, 1);
	pPl[9].SetPoly(6, 1, 5);
	pPl[10].SetPoly(6, 3, 7);
	pPl[11].SetPoly(6, 2, 3);

	SetNorm(pVt, vtn, pPl, pln);
	SetGLBuffer(pVt, vtn, pPl, pln);
	delete[](pVt);
	delete[](pPl);
}

void lObj::Cylinder(float r, float l)
{
	SetNotBone();
	dhVertex* pVt;
	lPoly* pPl;
	int i;
	float s,c;
	vtn = 36*2+2; pln = 36*4;
	pVt = new dhVertex[vtn];
	pPl = new lPoly[pln];
	for (i = 0; i < 36; i++) {
		s = sin(RAD(i * 10));
		c = cos(RAD(i * 10));
		pVt[i].SetPoint(s * r, c * r, 0);
		pVt[36+i].SetPoint(s * r, c * r, l);
	}
	pVt[72].SetPoint(0, 0, 0);
	pVt[73].SetPoint(0, 0, l);
	for (i = 0; i < 36; i++) {
		if (i == 35) {
			pPl[i].SetPoly(72, i,0);
			pPl[36 + i].SetPoly(73, 36+i, 36);
			pPl[72 + i].SetPoly(i, 0, 36);
			pPl[108 + i].SetPoly(i, 36, 36 + i);
		}
		else {
			pPl[i].SetPoly(72, i, i+1);
			pPl[36 + i].SetPoly(73, 36+i+1, 36 + i );
			pPl[72 + i].SetPoly(i,  i + 1, 36 + i + 1);
			pPl[108 + i].SetPoly(i, 36 + i + 1, 36 + i);
		}
	}
	SetNorm(pVt, vtn, pPl, pln);
	SetGLBuffer(pVt, vtn, pPl, pln);
	delete[](pVt);
	delete[](pPl);
}

void lObj::SetCam(void* c)
{
	this->c = c;
}

void lObj::SetLight(void* l)
{
	L = l;
}

void lObj::T(dhMat t)
{
	this->h = t;
}
#include "3d/dhVector.h"
void lObj::Draw()
{ 
	lCam* c = (lCam*)this->c;
	lLight* L = (lLight*)this->L;
	dhVector cv;
	if (BoneMode == TRUE)return;
	glUseProgram(sdrProgram);
	unsigned int pMat, cMat, hMat, sMat;
	unsigned int pPivot;
	unsigned int lp, vp;
	unsigned int ac, dc, sc;
	unsigned int agFloat, dfFloat, scFloat;

	pMat = glGetUniformLocation(sdrProgram, "projection");
	glUniformMatrix4fv(pMat, 1, GL_FALSE, c->Projection.v);
	cMat = glGetUniformLocation(sdrProgram, "cam");
	glUniformMatrix4fv(cMat, 1, GL_FALSE, c->Cam.v);
	hMat = glGetUniformLocation(sdrProgram, "model");
	glUniformMatrix4fv(hMat, 1, GL_FALSE, h.v);
	sMat = glGetUniformLocation(sdrProgram, "scail");
	glUniformMatrix4fv(sMat, 1, GL_FALSE, s.v);


	pPivot = glGetUniformLocation(sdrProgram, "pivot");
	glUniform3fv(pPivot, 1, (float*)&Pivot);


	lp = glGetUniformLocation(sdrProgram, "lightPos");
	glUniform3fv(lp, 1, (float*)&L->p);
	dhVector v;
	vp = glGetUniformLocation(sdrProgram, "viewPos");
	glUniform3fv(vp, 1, (float*)&v);


	ac = glGetUniformLocation(sdrProgram, "AmbientColor");
	glUniform4fv(ac, 1, (float*)&AmbientColor);
	dc = glGetUniformLocation(sdrProgram, "DiffuseColor");
	glUniform4fv(dc, 1, (float*)&DiffuseColor);
	sc = glGetUniformLocation(sdrProgram, "SpecularColor");
	glUniform4fv(sc, 1, (float*)&SpecularColor);


	agFloat = glGetUniformLocation(sdrProgram, "AmbientRatio");
	dfFloat = glGetUniformLocation(sdrProgram, "DiffuseRatio");
	scFloat = glGetUniformLocation(sdrProgram, "SpecularRatio");
	glUniform1fv(agFloat, 1, &Ambient);
	glUniform1fv(dfFloat, 1, &Diffuse);
	glUniform1fv(agFloat, 1, &Specular);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, pln * 3, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

void lObj::Draw(dhMat t)
{
	lCam* c = (lCam*)this->c;
	lLight* L = (lLight*)this->L;
	dhVector cv;
	dhMat h0o = t* h;
	if (BoneMode == TRUE)return;
	glUseProgram(sdrProgram);
	unsigned int pMat, cMat, hMat, sMat;
	unsigned int pPivot;
	unsigned int lp, vp;
	unsigned int ac, dc, sc;
	unsigned int agFloat, dfFloat, scFloat;

	pMat = glGetUniformLocation(sdrProgram, "projection");
	glUniformMatrix4fv(pMat, 1, GL_FALSE, c->Projection.v);
	cMat = glGetUniformLocation(sdrProgram, "cam");
	glUniformMatrix4fv(cMat, 1, GL_FALSE, c->Cam.v);
	hMat = glGetUniformLocation(sdrProgram, "model");
	glUniformMatrix4fv(hMat, 1, GL_FALSE, h0o.v);
	sMat = glGetUniformLocation(sdrProgram, "scail");
	glUniformMatrix4fv(sMat, 1, GL_FALSE, s.v);

	
	pPivot = glGetUniformLocation(sdrProgram, "pivot");
	glUniform3fv(pPivot, 1, (float*)&Pivot);

	
	lp = glGetUniformLocation(sdrProgram, "lightPos");
	glUniform3fv(lp, 1, (float*)&L->p);
	dhVector v;
	vp = glGetUniformLocation(sdrProgram, "viewPos");
	glUniform3fv(vp, 1, (float*)&v);


	ac = glGetUniformLocation(sdrProgram, "AmbientColor");
	glUniform4fv(ac, 1, (float*)&AmbientColor);
	dc = glGetUniformLocation(sdrProgram, "DiffuseColor");
	glUniform4fv(dc, 1, (float*)&DiffuseColor);
	sc = glGetUniformLocation(sdrProgram, "SpecularColor");
	glUniform4fv(sc, 1, (float*)&SpecularColor);


	agFloat = glGetUniformLocation(sdrProgram, "AmbientRatio");
	dfFloat = glGetUniformLocation(sdrProgram, "DiffuseRatio");
	scFloat = glGetUniformLocation(sdrProgram, "SpecularRatio");
	glUniform1fv(agFloat, 1, &Ambient);
	glUniform1fv(dfFloat, 1, &Diffuse);
	glUniform1fv(agFloat, 1, &Specular);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, pln * 3, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

BOOL lObj::SetShader(const char* vs, const char*fs)
{
	lString vShader, fShader;
	unsigned int sdrVertex, sdrFragment;
	char* p;
	int success;
	char infoLog[512] = {0,};
	if (BoneMode == TRUE)return FALSE;
	vShader.Read(vs);
	fShader.Read(fs);
	//MAKE GL SHADER CODE
	sdrVertex = glCreateShader(GL_VERTEX_SHADER);
	p = vShader.GetC();
	glShaderSource(sdrVertex, 1, &p, NULL);
	glCompileShader(sdrVertex);
	//CHECK SHADER CODE
	
	glGetShaderiv(sdrVertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(sdrVertex, 512, NULL, infoLog);
		infoLog[0] = infoLog[0];
		return FALSE;
	}


	//MAKE GL SHADER CODE
	sdrFragment = glCreateShader(GL_FRAGMENT_SHADER);
	p = fShader.GetC();
	glShaderSource(sdrFragment, 1, &p, NULL);
	glCompileShader(sdrFragment);
	//CHECK SHADER CODE
	glGetShaderiv(sdrFragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(sdrFragment, 512, NULL, infoLog);
		infoLog[0] = infoLog[0];
		return FALSE;
	}
	
	//UPLODE SHADER CODE

	sdrProgram = glCreateProgram();
	glAttachShader(sdrProgram, sdrVertex);
	glAttachShader(sdrProgram, sdrFragment);
	glLinkProgram(sdrProgram);
	glGetProgramiv(sdrProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(sdrProgram, 512, NULL, infoLog);
		infoLog[0] = infoLog[0];
		return FALSE;
	}
	glDeleteShader(sdrVertex);
	glDeleteShader(sdrFragment);
	return TRUE;
}

void lObj::SetAmbientColor(lRGB& c)
{
	this->AmbientColor.r = c.r;
	this->AmbientColor.g = c.g;
	this->AmbientColor.b = c.b;
	this->AmbientColor.d = c.d;
}

void lObj::SetDiffuseColor(lRGB&c)
{
	DiffuseColor.r = c.r;
	DiffuseColor.g = c.g;
	DiffuseColor.b = c.b;
	DiffuseColor.d = c.d;
}

void lObj::SetSpecularColor(lRGB& c)
{
	SpecularColor.r = c.r;
	SpecularColor.g = c.g;
	SpecularColor.b = c.b;
	SpecularColor.d = c.d;
}


void lObj::SetOptical(float a, float d, float s)
{
	this->Ambient = a;
	this->Diffuse = d;
	this->Specular = s;
}

void lObj::SetScale(dhVector v)
{
	s = h.Scale(v.x, v.y, v.z);
}

dhMat lObj::GetH()
{
	return this->h;
}

float lObj::GetAmbient()
{
	return Ambient;
}

float lObj::GetDiffuse()
{
	return Diffuse;
}

float lObj::GetSpecular()
{
	return Specular;
}
