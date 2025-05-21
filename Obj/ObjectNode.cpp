
#include "pch.h"
#include "ObjectNode.h"

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include "Obj/lCam.h"
#include "Obj/lLight.h"

NODE_OBJECT::NODE_OBJECT()
{
	Initialization();
}

NODE_OBJECT::~NODE_OBJECT()
{
	Close();
}

void NODE_OBJECT::AddTail(void* dt)
{
	Child.AddTail(dt);
}

NODE_OBJECT* NODE_OBJECT::Search(int i)
{
	return (NODE_OBJECT*)Child.Search(i);
}

void NODE_OBJECT::Initialization()
{
	this->parents = NULL;
	strName.Initialization();
	this->Child.Initialization();
	this->Child.DeleteModeOff();
}

void NODE_OBJECT::Close()
{
	Child.RemoveAll();
	strName.Delete();
}

void NODE_OBJECT::Draw(dhMat h0p){
	dhMat hpo;
	int size = Child.GetSize(),i;
	hpo = h0p *GetH();
	if (size <= 0)return;
	for (i = 0; i < size; i++) {
		NODE_OBJECT* obj = (NODE_OBJECT*)Child.Search(i);
		obj->Draw(hpo);
	}
}

dhMat NODE_OBJECT::GetH()
{
	dhMat h;
	return h;
}

void NODE_OBJECT::SetShader(const char*, const char*)
{
}

void NODE_OBJECT::SetCamAndLight(void* c, void* l)
{
}

_3DS::_3DS()
{
	this->Initialization();
}

_3DS::~_3DS()
{
	Close();
}

void _3DS::Initialization()
{
	NODE_OBJECT::Initialization();
	mesh.Initialization();
	c = NULL;
	l = NULL;
}

void _3DS::Close()
{
	NODE_OBJECT::Close();
	mesh.Close();
}

void _3DS::Draw(dhMat hpp)
{
	dhMat hpo = hpp * GetH();
	NODE_OBJECT::Draw(hpp);
	GLUpdate(hpo);
}

dhMat _3DS::GetH()
{
	dhMat h;
	//return h;
	return this->h*this->LCS*this->t;
}

void _3DS::GLUpdate(dhMat hpo)
{
	lCam* c = (lCam*)this->c;
	lLight* l = (lLight*)this->l;
	unsigned int pMat, cMat, hMat, sMat;
	unsigned int pPivot;
	unsigned int lp, vp;
	unsigned int ac, dc, sc;
	unsigned int agFloat, dfFloat, scFloat;
	if (c == NULL || l == NULL)return;
	if (this->mesh.vtn == 0 && this->mesh.pln == 0) return;
	glUseProgram(mesh.GetShaderID());
	pMat = glGetUniformLocation(mesh.GetShaderID(), "projection");
	cMat = glGetUniformLocation(mesh.GetShaderID(), "cam");
	hMat = glGetUniformLocation(mesh.GetShaderID(), "model");
	sMat = glGetUniformLocation(mesh.GetShaderID(), "scail");
	glUniformMatrix4fv(pMat, 1, GL_FALSE, c->Projection.v);
	glUniformMatrix4fv(cMat, 1, GL_FALSE, c->Cam.v);
	glUniformMatrix4fv(hMat, 1, GL_FALSE, hpo.v);
	glUniformMatrix4fv(sMat, 1, GL_FALSE, S.v);



	pPivot = glGetUniformLocation(mesh.GetShaderID(), "pivot");
	glUniform3fv(pPivot, 1, (float*)&mesh.pivot);


	ac = glGetUniformLocation(mesh.GetShaderID(), "AmbientColor");
	glUniform4fv(ac, 1, (float*)&Ambient);
	dc = glGetUniformLocation(mesh.GetShaderID(), "DiffuseColor");
	glUniform4fv(dc, 1, (float*)&Diffues);
	sc = glGetUniformLocation(mesh.GetShaderID(), "SpecularColor");
	glUniform4fv(sc, 1, (float*)&Specular);


	agFloat = glGetUniformLocation(mesh.GetShaderID(), "AmbientRatio");
	dfFloat = glGetUniformLocation(mesh.GetShaderID(), "DiffuseRatio");
	scFloat = glGetUniformLocation(mesh.GetShaderID(), "SpecularRatio");
	glUniform1fv(agFloat, 1, &ColorRatio.x);
	glUniform1fv(dfFloat, 1, &ColorRatio.y);
	glUniform1fv(agFloat, 1, &ColorRatio.z);

	mesh.Draw();
}

void _3DS::SetShader(const char* vertex, const char* Fragment)
{
	lString Ver,Frag;
	Ver.Read(vertex);
	Frag.Read(Fragment);
	mesh.SetShaderProgramID(Ver, Frag);
	Ver.Delete();
	Frag.Delete();
}

void _3DS::SetCamAndLight(void* c, void* l)
{
	this->c = c;
	this->l = l;
}

void _3DS::SetLCS(dhVector u, dhVector v, dhVector w, dhVector origin)
{
	u = u.Unit();
	v = v.Unit();
	w = w.Unit();
	LCS.v[12] = origin.x;
	LCS.v[13] = origin.y;
	LCS.v[14] = origin.z;

	LCS.v[0] = u.x;
	LCS.v[1] = u.y;
	LCS.v[2] = u.z;

	LCS.v[4] = v.x;
	LCS.v[5] = v.y;
	LCS.v[6] = v.z;

	LCS.v[8] = w.x;
	LCS.v[9] = w.y;
	LCS.v[10] = w.z;
}

void _3DS::SetT(dhVector v)
{
	this->h.v[12] = v.x;
	this->h.v[13] = v.y;
	this->h.v[14] = v.z;
}

void _3DS::SetQuat(float q, dhVector v){
	dhMat Q;
	int i;
	v = v.Unit();
	if (v.x == 0 && v.y && v.z == 0) return;
	Q = h.Quat(q, v);
	for (i = 0; i < 3; i++) {
		h.v[i] = Q.v[i];
		h.v[i+4] = Q.v[i+4];
		h.v[i+8] = Q.v[i+8];
	}
}

void _3DS::SetScail(float x, float y, float z)
{
	S = S.Scale(x, y, z);
}

void _3DS::SetVertexAndIndex(dhVertex* vtx, int vn, lPoly* ply, int pn)
{
	mesh.SetBuffer(vtx, vn, ply, pn);
}

void _3DS::SetAmbientColor(lRGB color)
{
	Ambient = color;
}

void _3DS::SetDiffuesColor(lRGB color)
{
	Diffues = color;
}

void _3DS::SetSpecularColor(lRGB color)
{
	Specular = color;
}

void _3DS::SetColorRatio(float x, float y, float z)
{
	ColorRatio.x = x;
	ColorRatio.y = y;
	ColorRatio.z = z;
}

void _3DS::T(dhMat& T)
{
	this->t = T;
}

int _3DS::GetSize()
{
	return Child.GetSize();
}

_GLB::_GLB()
{
	Initialization();
}

_GLB::~_GLB()
{
	Close();
}

void _GLB::Initialization()
{
	NODE_OBJECT::Initialization();
	mesh = NULL;
	msSize = 0;
}

void _GLB::Close()
{
	int i;
	NODE_OBJECT::Close();
	if (mesh) {
		if (msSize == 0) {
			mesh->Close();
			delete(mesh); 
		}
		else { 
			if (msSize == 1) {
				mesh->Close();
				delete(mesh);
			}
			else {
				for (i = 0; i < msSize; i++)
					mesh[i].Close();
				delete[](mesh);
			}
		}
	}
}

void _GLB::Draw(dhMat hpp)
{
	dhMat hpo = hpp * GetH();
	NODE_OBJECT::Draw(hpp);
	GLUpdate(hpo);
}

dhMat _GLB::GetH()
{
	dhMat h;
	return this->h;
}

void _GLB::SetShader(const char*ver, const char*frag)
{
	int i;
	if (mesh == NULL) return;
	else {
		lString vertex, Fragment;
		vertex.Read(ver);
		Fragment.Read(frag);
		for (i = 0; i < msSize;  i++) {
			mesh[i].SetShaderProgramID(vertex,Fragment);
		}
	}
}

void _GLB::SetCamAndLight(void* c, void* l)
{
	this->c = c;
	this->l = l;
}

void _GLB::Init(int size)
{
	int i;
	if (size <= 1) { 
		mesh = new lMesh();
		mesh->Initialization();
	}
	else {
		mesh = new lMesh[size];
		for (i = 0; i < size; i++) {
			mesh[i].Initialization();
		}
	}
	msSize = size;
}

void _GLB::SetTranslation(float* v)
{
	h.v[12] = v[0];
	h.v[13] = v[1];
	h.v[14] = v[2];
}

void _GLB::SetQuat(float* v)
{
	dhQuat q;
	dhMat h;
	int i;
	memmove(&q, v, sizeof(dhQuat));
	h = h.Quat(q);
	for (i = 0; i < 3; i++) {
		this->h.v[i] = h.v[i];
		this->h.v[i+4] = h.v[i+4];
		this->h.v[i+8] = h.v[i+8];
	}
}

void _GLB::SetScale(float* v)
{
	h.v[0] *= v[0];
	h.v[5] *= v[1];
	h.v[10] *= v[2];
}

void _GLB::GLUpdate(dhMat h)
{
	lCam* c = (lCam*)this->c;
	lLight* l = (lLight*)this->l;
	unsigned int pMat, cMat, hMat, sMat;
	unsigned int pPivot;
	unsigned int lp, vp;
	unsigned int ac, dc, sc;
	unsigned int agFloat, dfFloat, scFloat;
	dhMat hpo,S;
	int i;
	for (i = 0; i < msSize; i++) {
		if (c == NULL || l == NULL)return;
		if (mesh[i].vtn == 0 && this->mesh[i].pln == 0) return;
		glUseProgram(mesh[i].GetShaderID());
		pMat = glGetUniformLocation(mesh[i].GetShaderID(), "projection");
		cMat = glGetUniformLocation(mesh[i].GetShaderID(), "cam");
		hMat = glGetUniformLocation(mesh[i].GetShaderID(), "model");
		sMat = glGetUniformLocation(mesh[i].GetShaderID(), "scail");
		glUniformMatrix4fv(pMat, 1, GL_FALSE, c->Projection.v);
		glUniformMatrix4fv(cMat, 1, GL_FALSE, c->Cam.v);
		glUniformMatrix4fv(hMat, 1, GL_FALSE, hpo.v);
		glUniformMatrix4fv(sMat, 1, GL_FALSE, S.v);



		pPivot = glGetUniformLocation(mesh[i].GetShaderID(), "pivot");
		glUniform3fv(pPivot, 1, (float*)&mesh[i].pivot);


		ac = glGetUniformLocation(mesh[i].GetShaderID(), "AmbientColor");
		glUniform4fv(ac, 1, (float*)&Ambient);
		dc = glGetUniformLocation(mesh[i].GetShaderID(), "DiffuseColor");
		glUniform4fv(dc, 1, (float*)&Diffues);
		sc = glGetUniformLocation(mesh[i].GetShaderID(), "SpecularColor");
		glUniform4fv(sc, 1, (float*)&Specular);

		float a = 1;
		agFloat = glGetUniformLocation(mesh[i].GetShaderID(), "AmbientRatio");
		dfFloat = glGetUniformLocation(mesh[i].GetShaderID(), "DiffuseRatio");
		scFloat = glGetUniformLocation(mesh[i].GetShaderID(), "SpecularRatio");
		glUniform1fv(agFloat, 1, &a);
		glUniform1fv(dfFloat, 1, &a);
		glUniform1fv(agFloat, 1, &a);
		mesh[i].Draw();
	}
}
	