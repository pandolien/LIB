#ifndef __LOPENGL_H__
#define __LOPENGL_H__
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include "Obj/lAxis.h"
#include "Obj/lCam.h"
#include "Obj/lLight.h"
// lOpenGL

class lOpenGL : public CWnd
{
	DECLARE_DYNAMIC(lOpenGL)

public:
	lOpenGL();
	virtual ~lOpenGL();
protected:
	lCam Cam;
	lLight Light;
	CPoint oldCPoint;
	lAxis o;
	bool LButtonClick, RButtonClick;
	float anglex,angley,cT, cTx, cTy;
	BOOL keyStatus[6];
private:
	HDC m_hDC;  // Device context
	HGLRC m_hRC; // OpenGL rendering context
protected:
	DECLARE_MESSAGE_MAP()
private:
	BOOL CreateOpenGLContext();
	void GLResize(int cx, int cy);
	void GLDraw(CDC* pDC);
protected:
	virtual void GLObjectCreate();
	virtual void GLRenderScene();
	virtual void GLObjectDelete();
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


#endif