#ifndef __OBJECT_NODE_DATA_H__
#define __OBJECT_NODE_DATA_H__
#include "3d/dhVertex.h"
#include "3d/dhVector.h"
#include "3d/dhMat.h"

#include "Obj/lMesh.h"
#include "Obj/lRGB.h"

#include "lPTRList.h"
#include "lString.h"
class NODE_OBJECT{
public:
    NODE_OBJECT();
    ~NODE_OBJECT();
public:
    lString strName;
    NODE_OBJECT* parents;
    lPTRList Child;
public://List Part
    void AddTail(void*);
    NODE_OBJECT* Search(int i);
public://virtual Function
    virtual void Initialization();
    virtual void Close();
    virtual void Draw(dhMat);
    virtual dhMat GetH();
    virtual void SetShader(const char*, const char*);
   virtual void SetCamAndLight(void* c, void* l);
};

class _3DS :public NODE_OBJECT{
public:
    _3DS();
    ~_3DS();
public:
    dhMat LCS,t,h,S;
    lMesh mesh;
    lRGB Ambient, Diffues, Specular;
    dhVector ColorRatio;
    void* c, * l;
public:
    void Initialization() override;
    void Close() override;
    void Draw(dhMat) override;
    dhMat GetH();
    void GLUpdate(dhMat hpo);
    void SetShader(const char*, const char*);
    void SetCamAndLight(void* c, void* l);
public://Set Part
    void SetLCS(dhVector u, dhVector v, dhVector w, dhVector origin);
    void SetT(dhVector v);
    void SetQuat(float, dhVector);
    void SetScail(float, float, float);
    void SetVertexAndIndex(dhVertex*, int, lPoly*, int);
    void SetAmbientColor(lRGB color);
    void SetDiffuesColor(lRGB color);
    void SetSpecularColor(lRGB color);
    void SetColorRatio(float, float, float);
    void T(dhMat&);
public://Get Part
    int GetSize();
};
class _GLB : public NODE_OBJECT {
public:
    _GLB();
    ~_GLB();
public:
    lMesh *mesh;
    int msSize;
    dhMat h,t;
    lRGB Ambient, Diffues, Specular;
    void* c, * l;
public:
    void Initialization() override;
    void Close() override;
    void Draw(dhMat) override;
    dhMat GetH();
    void SetShader(const char*, const char*);
    void SetCamAndLight(void* c, void* l);
public:
    void Init(int);
    void SetTranslation(float*);
    void SetQuat(float*);
    void SetScale(float*);
    void GLUpdate(dhMat h);
};
#endif